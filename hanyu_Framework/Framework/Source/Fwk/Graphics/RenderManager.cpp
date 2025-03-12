﻿#include "Fwk/Graphics/RenderManager.h"

#include <algorithm>

#include "Lib/Graphics/Graphics.h"
#include "Lib/LibDef.h"
#include "Lib/Util/Log.h"


#include "Fwk/Graphics/RenderObject.h"
#include "Fwk/Graphics/IDrawable.h"
#include "Fwk/Graphics/Font.h"
#include "Fwk/Graphics/Camera.h"

using namespace DirectX;

#define PrintSuccess Debug::Log("　...成功\n");
#define PrintFailed Debug::Log("　...失敗\n");

namespace Fwk {

bool RenderManager::cmpareDrawReq(RenderObject* p0, RenderObject* p1) {

	//順番が正しいならtrue、そうでなければfalseを返せ
	
	//DrawPass→Priorityの順にソート
	//3Dの描画物についてはZ値やマテリアルのソートも入れるべきだがそれはもう後々で。

	int pass0 = static_cast<int>(p0->GetRenderPass());
	int pass1 = static_cast<int>(p1->GetRenderPass());
	if (pass0  != pass1)
		return pass0 < pass1;

	return p0->GetPriority() < p1->GetPriority();
}

bool RenderManager::cmpareCamera(Camera* p0, Camera* p1) {
	return (p0)->GetOrder() < (p1)->GetOrder();
}

bool RenderManager::cmpareLayer(Layer* p0, Layer* p1) {
	return p0->order < p1->order;
}

RenderManager::RenderManager()
	: m_IsInitilized(false)
	, m_Layers()
	, m_RequestBufferSize(0)
	, m_Cameras()
{
	;
}
RenderManager::~RenderManager()
{
	;
}

void RenderManager::Init(
	HWND hwnd,
	uint32_t width, uint32_t height,
	uint32_t bufferSize,
	uint32_t DescriptorSize_Resource,
	uint32_t DescriptorSize_Sampler,
	const wchar_t* pFontPath,
	const wchar_t* pVtxShaderPath,
	const wchar_t* pPixShaderPath
){
	Term();

	if (bufferSize == 0)
		return;

	//初期化成功フラグ
	bool bSucceed = true;

	Debug::Logf(">>>>>>>> RenderManagerの初期化開始\n");

	Debug::Log("グラフィクスシステムの初期化");

	Graphics::InitializeParameters initParams = {};
	{
		initParams.hwnd = hwnd;
		initParams.NumDescriptors[HEAP_TYPE_RES] = DescriptorSize_Resource;
		initParams.NumDescriptors[HEAP_TYPE_SMP] = DescriptorSize_Sampler;
		//基本的にスワップチェインで作るバッファの数だけあればいい
		initParams.NumDescriptors[HEAP_TYPE_RTV] = Graphics::FrameCount;
		//1つしかつかわないので
		initParams.NumDescriptors[HEAP_TYPE_DSV] = 1;
	}

	if( m_Graphics.Init(initParams) ){
		PrintSuccess;
	}else {
		PrintFailed;
		bSucceed = false;
	}

	//カメラ
	{
		Camera::SetDefaultOrtho((float)width, (float)height, 0.0f, 100.0f);

		Camera* pMainCam = new Camera();
		pMainCam->SetPosition(0.0f, 0.0f);
		pMainCam->SetOrtho((float)width, (float)height, 0.0f, 100.0f);
		m_Cameras.push_back(pMainCam);
	}

	//描画レイヤー
	{
		m_RequestBufferSize = bufferSize;
		Layer* pLayer = new Layer;
		pLayer->requestBuffer.reserve(m_RequestBufferSize);
		pLayer->isActive = true;
		pLayer->order = 0;
		pLayer->name = "default";
		m_Layers.push_back(pLayer);
	}

	//シェーダの読み込み
	Debug::Logf("シェーダファイルの読み込み:%ls",pVtxShaderPath);
	if (m_VtxShader.Init(pVtxShaderPath)) {
		PrintSuccess;
	}else {
		PrintFailed;
		bSucceed = false;
	}

	Debug::Logf("シェーダファイルの読み込み:%ls",pPixShaderPath);
	if(m_PixShader.Init(pPixShaderPath)){
		PrintSuccess;
	}else {
		PrintFailed;
		bSucceed = false;
	}

	//ルートシグネチャの作成（これはシェーダと関わっているので
	//シェーダの内容変わったら調整する必要がある）
	m_RootSignature[(int)ROOT_SIGNATURE::DEFAULT].Init(
		m_Graphics.GetDevice()
	);

	m_RootSignature[(int)ROOT_SIGNATURE::TRANSPARENT_2D].Init(
		m_Graphics.GetDevice(),
		D3D12_FILTER_MIN_MAG_MIP_POINT
	);

	m_RootSignature[(int)ROOT_SIGNATURE::ADD_2D].Init(
		m_Graphics.GetDevice(),
		D3D12_FILTER_MIN_MAG_MIP_POINT
	);

	m_RootSignature[(int)ROOT_SIGNATURE::SUB_2D].Init(
		m_Graphics.GetDevice(),
		D3D12_FILTER_MIN_MAG_MIP_POINT
	);

	m_RootSignature[(int)ROOT_SIGNATURE::OPAQUE_2D].Init(
		m_Graphics.GetDevice(),
		D3D12_FILTER_MIN_MAG_MIP_POINT
	);

	m_RootSignature[(int)ROOT_SIGNATURE::INVERSE_2D].Init(
		m_Graphics.GetDevice(),
		D3D12_FILTER_MIN_MAG_MIP_POINT
	);

	//パイプラインステートの作成

	//3D
	{
		//ブレンドステートの設定
		D3D12_BLEND_DESC descBS = {};
		{
			//レンダーターゲットのブレンド設定
			D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
				FALSE,FALSE,
				D3D12_BLEND_ONE,D3D12_BLEND_ZERO,D3D12_BLEND_OP_ADD,
				D3D12_BLEND_ONE,D3D12_BLEND_ZERO,D3D12_BLEND_OP_ADD,
				D3D12_LOGIC_OP_NOOP,
				D3D12_COLOR_WRITE_ENABLE_ALL
			};

			descBS.AlphaToCoverageEnable = FALSE;
			descBS.IndependentBlendEnable = FALSE;
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
				descBS.RenderTarget[i] = descRTBS;
			}
		}

		//深度ステンシルステートの設定
		D3D12_DEPTH_STENCIL_DESC descDSS = {};
		{
			descDSS.DepthEnable = true;
			descDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			descDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			descDSS.StencilEnable = false;
		}

		m_PipelineState[(int)PIPELINE::DEFAULT].Init(
			m_Graphics.GetDevice(),
			m_RootSignature[(int)ROOT_SIGNATURE::DEFAULT].Get(),
			&m_VtxShader,
			&m_PixShader,
			descBS,
			descDSS
		);
	}

	//2D
	
	//深度ステンシルステートの設定
	{
		D3D12_DEPTH_STENCIL_DESC descDSS_2D = {};
		{
			descDSS_2D.DepthEnable = false;
			descDSS_2D.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			descDSS_2D.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
			descDSS_2D.StencilEnable = false;
		};

		//レンダーターゲットのブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC descRTBS_Default = {
			TRUE,
			FALSE,

			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_INV_SRC_ALPHA,
			D3D12_BLEND_OP_ADD,

			D3D12_BLEND_ONE,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,

			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL
		};

		//ブレンドステートの設定
		D3D12_BLEND_DESC descBS_Default = {};
		{
			descBS_Default.AlphaToCoverageEnable = FALSE;
			descBS_Default.IndependentBlendEnable = FALSE;
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
				descBS_Default.RenderTarget[i] = descRTBS_Default;
			}
		}

		//デフォルト2D描画
		m_PipelineState[(int)PIPELINE::TRANSPARENT_2D].Init(
			m_Graphics.GetDevice(),
			m_RootSignature[(int)ROOT_SIGNATURE::TRANSPARENT_2D].Get(),
			&m_VtxShader,
			&m_PixShader,
			descBS_Default,
			descDSS_2D
		);

		//2D_BLEND_ADD
		D3D12_RENDER_TARGET_BLEND_DESC descRTBS_Add = descRTBS_Default;
		{
			descRTBS_Add.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			descRTBS_Add.DestBlend = D3D12_BLEND_ONE;
		}

		//ブレンドステートの設定
		D3D12_BLEND_DESC descBS_2D_Add = descBS_Default;
		{
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
				descBS_2D_Add.RenderTarget[i] = descRTBS_Add;
			}
		}

		//加算ブレンド2D
		m_PipelineState[(int)PIPELINE::ADD_2D].Init(
			m_Graphics.GetDevice(),
			m_RootSignature[(int)PIPELINE::ADD_2D].Get(),
			&m_VtxShader,
			&m_PixShader,
			descBS_2D_Add,
			descDSS_2D
		);

		//2D_BLEND_SUB
		D3D12_RENDER_TARGET_BLEND_DESC descRTBS_Sub = descRTBS_Default;
		{
			descRTBS_Sub.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			descRTBS_Sub.DestBlend = D3D12_BLEND_ONE;
			descRTBS_Sub.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		}

		//ブレンドステートの設定
		D3D12_BLEND_DESC descBS_2D_Sub = descBS_Default;
		{
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
				descBS_2D_Sub.RenderTarget[i] = descRTBS_Sub;
			}
		}

		m_PipelineState[(int)PIPELINE::SUB_2D].Init(
			m_Graphics.GetDevice(),
			m_RootSignature[(int)PIPELINE::SUB_2D].Get(),
			&m_VtxShader,
			&m_PixShader,
			descBS_2D_Sub,
			descDSS_2D
		);

		//2D_BLEND_OPAQUE
		D3D12_RENDER_TARGET_BLEND_DESC descRTBS_Opaque = descRTBS_Default;
		{
			descRTBS_Opaque.BlendEnable = FALSE;
			descRTBS_Opaque.SrcBlend = D3D12_BLEND_ONE;
			descRTBS_Opaque.DestBlend = D3D12_BLEND_ZERO;
		}

		//ブレンドステートの設定
		D3D12_BLEND_DESC descBS_2D_Opaque = descBS_Default;
		{
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
				descBS_2D_Opaque.RenderTarget[i] = descRTBS_Opaque;
			}
		}

		m_PipelineState[(int)PIPELINE::OPAQUE_2D].Init(
			m_Graphics.GetDevice(),
			m_RootSignature[(int)PIPELINE::OPAQUE_2D].Get(),
			&m_VtxShader,
			&m_PixShader,
			descBS_2D_Opaque,
			descDSS_2D
		);

		//2D_BLEND_INVERSE
		D3D12_RENDER_TARGET_BLEND_DESC descRTBS_Inverse = descRTBS_Default;
		{
			descRTBS_Inverse.BlendEnable = TRUE;
			descRTBS_Inverse.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			descRTBS_Inverse.DestBlend = D3D12_BLEND_ZERO;
			descRTBS_Inverse.BlendOp = D3D12_BLEND_OP_ADD;
		}

		//ブレンドステートの設定
		D3D12_BLEND_DESC descBS_2D_Inverse = descBS_Default;
		{
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
				descBS_2D_Inverse.RenderTarget[i] = descRTBS_Inverse;
			}
		}

		m_PipelineState[(int)PIPELINE::INVERSE_2D].Init(
			m_Graphics.GetDevice(),
			m_RootSignature[(int)PIPELINE::INVERSE_2D].Get(),
			&m_VtxShader,
			&m_PixShader,
			descBS_2D_Inverse,
			descDSS_2D
		);
	}

	Debug::Logf("フォントファイルの読み込み:%ls",pFontPath);
	if(m_SpriteFont.Init(&m_Graphics, pFontPath)){
		PrintSuccess;
	}else {
		PrintFailed;
		bSucceed = false;
	}


	Debug::Logf("<<<<<<<< RenderManagerの初期化終了");
	if(bSucceed){
			PrintSuccess;
	}else {
		PrintFailed;
		bSucceed = false;
	}
	Debug::Logf("\n");

	//初期化済フラグ設定
	m_IsInitilized = bSucceed;
}

void RenderManager::Term()
{
	for (int i = 0; i < (int)ROOT_SIGNATURE::COUNT; ++i) {
		m_RootSignature[i].Term();
	}

	for (int i = 0; i < (int)PIPELINE::COUNT; ++i) {
		m_PipelineState[i].Term();
	}
	m_PixShader.Term();
	m_VtxShader.Term();

	m_SpriteFont.Term();

	for (int i = 0; i < m_Layers.size(); ++i) {
		SAFE_DELETE(m_Layers[i]);
	}
	m_Layers.clear();

	for (int i = 0; i < m_Cameras.size(); ++i) {
		SAFE_DELETE(m_Cameras[i]);
	}
	m_Cameras.clear();

	m_Graphics.Term();

	m_IsInitilized = false;
}

void RenderManager::RequestRender(RenderObject* pDrawable)
{
	if (pDrawable == nullptr)
		return;

	Layer* pLayer = _findLayer(pDrawable->GetRenderLayer());

	if (pLayer==nullptr) {
		return;
	}

	if (pLayer->requestBuffer.capacity() <= pLayer->requestBuffer.size())
		return;

	pLayer->requestBuffer.push_back(pDrawable);
}

void RenderManager::Render(){

	if (!m_IsInitilized) {
		Debug::Log("エラー：未初期化のRenderManagerに対して描画命令を要求しました。\n");
		return;
	}

	m_Graphics.BeginRender();

	//各レイヤーの描画リクエスト並べ替え
	for (int i = 0; i < m_Layers.size(); ++i) 
	{
		std::vector<RenderObject*>* reqBuffer = &(m_Layers[i]->requestBuffer);
		std::sort(
			reqBuffer->begin(),
			reqBuffer->end(),
			cmpareDrawReq
		);
	}

	//カメラの順番整列
	std::sort(
		m_Cameras.begin(),
		m_Cameras.end(),
		cmpareCamera
	);

	const uint32_t frameIndex = m_Graphics.GetFrameIndex();
	ID3D12GraphicsCommandList* pCommandList = m_Graphics.GetCommandList()->Get();

	//描画処理
	ID3D12DescriptorHeap* const pHeaps[] = {
		m_Graphics.GetDescriptorHeap(Lib::Graphics::HEAP_TYPE_RES)->Get()
	};
	pCommandList->SetDescriptorHeaps(1, pHeaps);

	pCommandList->RSSetViewports(1, &m_Graphics.GetViewPort());
	pCommandList->RSSetScissorRects(1, &m_Graphics.GetScissorRect());

	RENDER_PASS currentPass = RENDER_PASS::None;
	RENDER_MODE currentMode = RENDER_MODE::None;

	PIPELINE currentPipeline = PIPELINE::NONE;
	ROOT_SIGNATURE currentSignature = ROOT_SIGNATURE::NONE;

	for (auto cam = m_Cameras.begin(); cam != m_Cameras.end(); ++cam) {
	
		if (!(*cam)->IsActive()) {
			continue;
		}

		bool isMainCam = (*cam)->GetName()._Equal("main");

		for (auto layer = m_Layers.begin(); layer != m_Layers.end(); ++layer)
		{
			if (!(*layer)->isActive) {
				continue;
			}

			//メインカメラはデフォルトレイヤーを無条件で描画対象とする
			if (!(isMainCam && (*layer)->name._Equal("default")))
			{
				//それ以外はカメラの描画対象となっているか調べる
				if (!(*cam)->ContainsRenderLayer((*layer)->name)) {
					continue;
				}
			}

			std::vector<RenderObject*>& RequestBuffer = (*layer)->requestBuffer;

			for (auto renderObj = RequestBuffer.begin(); renderObj != RequestBuffer.end(); ++renderObj) {

				RenderObject* pRenderObject = *renderObj;

				if (!pRenderObject->IsVisible())
					continue;

				RENDER_PASS pass = pRenderObject->GetRenderPass();
				RENDER_MODE mode = pRenderObject->GetRenderMode();

				if (pass != currentPass) {
					_onRenderPassChanged(pass, currentPass, &m_Graphics);
				}

				if (pass != currentPass || mode != currentMode) 
				{
					PIPELINE pipeline = _getPipelineState(pass, mode);
					ROOT_SIGNATURE signature = _getRootSignature(pass, mode);

					if (pipeline != currentPipeline)
					{
						if (pipeline != PIPELINE::NONE) {
							pCommandList->SetPipelineState(m_PipelineState[(int)pipeline].Get());
						}
						currentPipeline = pipeline;
					}

					if (signature != currentSignature)
					{
						if (signature != ROOT_SIGNATURE::NONE) {
							pCommandList->SetGraphicsRootSignature(m_RootSignature[(int)signature].Get());
						}
						currentSignature = signature;
					}

					currentPass = pass;
					currentMode = mode;
				}

				pRenderObject->OnDraw(&m_Graphics, *cam);
			}
		}
	}

	_onRenderPassChanged(RENDER_PASS::None, currentPass, &m_Graphics);

	m_Graphics.EndRender();

	_postPresent();	
}

void RenderManager::CreateConstantBuffer(Lib::Graphics::ConstantBuffer& ConstBufferRef) {

	ConstBufferRef.Init(
		m_Graphics.GetDevice(),
		m_Graphics.GetDescriptorHeap(Lib::Graphics::HEAP_TYPE_RES),
		sizeof(CB_Transform)
	);

	//変換行列の設定
	CB_Transform* p = (CB_Transform*)(ConstBufferRef.GetPtr());
	p->World = XMMatrixIdentity();

	//3D用
	/* {
		auto eyePos = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
		auto targetPos = XMVectorZero();
		auto upward = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		auto fovY = XMConvertToRadians(37.5f);
		auto aspect = static_cast<float>(m_Width) / static_cast<float>(m_Height);

		p->View = XMMatrixLookAtLH(eyePos, targetPos, upward);
		p->Proj = XMMatrixPerspectiveFovLH(fovY, aspect, 1.0f, 1000.0f);
	}*/

	//2D用
	{
		Camera* pCam = _findCamera("main");
		p->View = pCam->GetViewMatrix();
		p->Proj = pCam->GetOrthiMatrix();
	}
}

IGraphics* RenderManager::GetGraphics() {
	return &m_Graphics;
}

void RenderManager::SetupFont(Font* pFont)
{
	if (pFont == nullptr)
		return;

	pFont->SetSpriteFont(&m_SpriteFont);
}

void RenderManager::SetClearColor(float r, float g, float b)
{
	m_Graphics.SetClearColor(r, g, b);
}

void RenderManager::SetClearColor(const float rgb[]) {
	m_Graphics.SetClearColor(rgb[0], rgb[1], rgb[2]);
}

void RenderManager::AddLayer(const std::string& name) {

	if (_findLayer(name) != nullptr) {
		return;
	}
	
	Layer* pLayer = new Layer();
	pLayer->requestBuffer.reserve(m_RequestBufferSize);
	pLayer->isActive = true;
	pLayer->name = name;

	int maxOrder = m_Layers[0]->order;
	for (int i = 0; i < m_Layers.size(); ++i) {
		if (maxOrder < m_Layers[i]->order) {
			maxOrder = m_Layers[i]->order;
		}
	}

	pLayer->order = maxOrder + 1;

	m_Layers.push_back(pLayer);
}

void RenderManager::RemoveLayer(const std::string& name){

	//デフォルトレイヤーは消せない
	if (name._Equal("default")) {
		return;
	}

	int index = _getLayerIndex(name);

	if (index < 0) {
		return;
	}

	SAFE_DELETE(m_Layers[index]);
	m_Layers.erase(m_Layers.begin() +index);
}

void RenderManager::SetLayerOrder(const std::string& name, int order) {
	
	Layer* pLayer = _findLayer(name);
	
	if (pLayer == nullptr) {
		return;
	}

	pLayer->order = order;

	//レイヤーの順番整列
	std::sort(
		m_Layers.begin(),
		m_Layers.end(),
		cmpareLayer
	);
}

void RenderManager::SetLayerActive(const std::string& name, bool isActive) {
	
	Layer* pLayer = _findLayer(name);
	
	if (pLayer == nullptr) {
		return;
	}

	pLayer->isActive = isActive;
}

void RenderManager::RemoveCamera(const std::string& name) {

	//メインカメラは消せない
	if (name._Equal("main")) {
		return;
	}

	int index = _getCameraIndex(name);

	if (index < 0) {
		return;
	}

	SAFE_DELETE(m_Cameras[index]);
	m_Cameras.erase(m_Cameras.begin() + index);
}

void RenderManager::SetCamera(const Camera& camera) {

	int index = _getCameraIndex(camera.GetName());

	if (index < 0) {
		Camera* pCam = new Camera();
		m_Cameras.push_back(pCam);
		index = (int)m_Cameras.size() - 1;
	}

	*m_Cameras[index] = camera;
}

Camera RenderManager::GetCamera() {
	Camera* pCam = _findCamera("main");
	return *pCam;
}

Camera RenderManager::GetCamera(const std::string& name) {
	Camera* pCam = _findCamera(name);
	if (pCam == nullptr) {
		Camera cam;
		return cam;
	}
	return *pCam;
}

//メインカメラの操作
void RenderManager::SetCameraOrder(int order) {
	SetCameraOrder("main", order);
}
void RenderManager::SetCameraActive(bool isActive) {
	SetCameraActive("main", isActive);
}
void RenderManager::SetCameraPosition(const Vector2f& position) {
	SetCameraPosition("main", position);
}

void RenderManager::SetCameraOrder(const std::string& name, int order) {
	Camera* pCam = _findCamera(name);
	if (pCam == nullptr) {
		return ;
	}
	pCam->SetOrder(order);
}

void RenderManager::SetCameraActive(const std::string& name, bool isActive) {
	Camera* pCam = _findCamera(name);
	if (pCam == nullptr) {
		return ;
	}
	pCam->SetActive(isActive);
}

void RenderManager::SetCameraPosition(const std::string& name, const Vector2f& position) {
	Camera* pCam = _findCamera(name);
	if (pCam == nullptr) {
		return ;
	}
	pCam->SetPosition(position);
}

//カメラの描画対象レイヤーの操作
void RenderManager::AddCameraRenderLayer(const std::string& cameraName, const std::string& targetLayerName) {
	Camera* pCam = _findCamera(cameraName);
	if (pCam == nullptr) {
		return ;
	}
	pCam->AddRenderLayer(targetLayerName);
}

void RenderManager::RemoveCameraRenderLayer(const std::string& cameraName, const std::string& targetLayerName) {
	Camera* pCam = _findCamera(cameraName);
	if (pCam == nullptr) {
		return ;
	}
	pCam->RemoveRenderLayer(targetLayerName);
}

void RenderManager::_postPresent() {
	m_SpriteFont.PostPresent(&m_Graphics);
	_clearRequest();
}

void RenderManager::_clearRequest() 
{
	for (int i = 0; i < m_Layers.size(); ++i) 
	{
		m_Layers[i]->requestBuffer.clear();
	}
}

void RenderManager::SyncGpuProc() {
	m_Graphics.SyncGpuProc();
}

RenderManager::ROOT_SIGNATURE RenderManager::_getRootSignature(RENDER_PASS pass, RENDER_MODE mode)
{
	switch (pass)
	{
	case RENDER_PASS::Mesh_Opaque:
		return ROOT_SIGNATURE::DEFAULT;
	case RENDER_PASS::Mesh_Transparent:
		return ROOT_SIGNATURE::DEFAULT;
	case RENDER_PASS::Sprite:
	{
		if (mode == RENDER_MODE::Sprite_Blend_Add){
			return ROOT_SIGNATURE::ADD_2D;
		}
		else if (mode == RENDER_MODE::Sprite_Blend_Sub) {
			return ROOT_SIGNATURE::SUB_2D;
		}
		else if (mode == RENDER_MODE::Sprite_Opaque) {
			return ROOT_SIGNATURE::OPAQUE_2D;
		}
		else if (mode == RENDER_MODE::Sprite_Blend_Inverse) {
			return ROOT_SIGNATURE::INVERSE_2D;
		}

		return ROOT_SIGNATURE::TRANSPARENT_2D;
	}
	break;
	case RENDER_PASS::SpriteFont:
		return ROOT_SIGNATURE::NONE;
	}

	return ROOT_SIGNATURE::NONE;
}
RenderManager::PIPELINE RenderManager::_getPipelineState(RENDER_PASS pass, RENDER_MODE mode)
{
	switch (pass)
	{
	case RENDER_PASS::Mesh_Opaque:
		return PIPELINE::DEFAULT;
	case RENDER_PASS::Mesh_Transparent:
		return PIPELINE::DEFAULT;
	case RENDER_PASS::Sprite:
	{
		if (mode == RENDER_MODE::Sprite_Blend_Add){
			return PIPELINE::ADD_2D;
		}
		else if (mode == RENDER_MODE::Sprite_Blend_Sub) {
			return PIPELINE::SUB_2D;
		}
		else if (mode == RENDER_MODE::Sprite_Opaque) {
			return PIPELINE::OPAQUE_2D;
		}
		else if (mode == RENDER_MODE::Sprite_Blend_Inverse) {
			return PIPELINE::INVERSE_2D;
		}
		return PIPELINE::TRANSPARENT_2D;
	}
	case RENDER_PASS::SpriteFont:
		return PIPELINE::NONE;
	}

	return PIPELINE::NONE;
}

void RenderManager::_onRenderPassChanged(RENDER_PASS pass, RENDER_PASS prevPass, Lib::Graphics::IGraphics* g) {

	if (pass == RENDER_PASS::SpriteFont) {
		m_SpriteFont.BeginRender(g);
	}
	if (prevPass == RENDER_PASS::SpriteFont) {
		m_SpriteFont.EndRender();

		ID3D12GraphicsCommandList* pCommandList = m_Graphics.GetCommandList()->Get();

		ID3D12DescriptorHeap* const pHeaps[] = {
			m_Graphics.GetDescriptorHeap(Lib::Graphics::HEAP_TYPE_RES)->Get()
		};
		pCommandList->SetDescriptorHeaps(1, pHeaps);
	}
}

Camera* RenderManager::_findCamera(const string& name) {
	for (int i = 0; i < m_Cameras.size(); ++i) {
		if (name._Equal(m_Cameras[i]->GetName())) {
			return m_Cameras[i];
		}
	}
	return nullptr;
}

int RenderManager::_getCameraIndex(const string& name) {
	for (int i = 0; i < m_Cameras.size(); ++i) {
		if (name._Equal(m_Cameras[i]->GetName())) {
			return i;
		}
	}
	return -1;
}

RenderManager::Layer* RenderManager::_findLayer(const string& name) {
	for (int i = 0; i < m_Layers.size(); ++i) {
		if (name._Equal(m_Layers[i]->name)) {
			return m_Layers[i];
		}
	}
	return nullptr;
}

int RenderManager::_getLayerIndex(const string& name) {
	for (int i = 0; i < m_Layers.size(); ++i) {
		if (name._Equal(m_Layers[i]->name)) {
			return i;
		}
	}
	return -1;
}

}
