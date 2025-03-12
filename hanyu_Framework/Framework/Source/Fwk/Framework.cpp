#include "Fwk/Framework.h"

#include <stdlib.h>
#include <time.h>
#include <stdarg.h> 

#include "Lib/Util/Log.h"

#include "Fwk/Shape/Shape.h"
#include "Fwk/Shape/ShapePlane.h"
#include "Fwk/Shape/ShapePlane2D.h"
#include "Fwk/Graphics/Font.h"
#include "Fwk/Version.h"

namespace Fwk {

Framework* Framework::g_Instance = nullptr;

void Framework::GenerateInstance() {
	if (g_Instance != nullptr)
		return;

	g_Instance = new Framework();
}

Framework* Framework::GetInstance() {
	return g_Instance;
}

void Framework::ReleaseInstance() {
	if (g_Instance == nullptr)
		return;

	delete g_Instance;
	g_Instance = nullptr;
}


Framework::Framework()
	:m_UseTextNum(0)
{
    ;
}

Framework::~Framework()
{
    Term();
}

bool Framework::Init(const FrameworkDesc& desc){

	Debug::Logf("実習用フレームワーク version:%d\n", FWK_VERSION);

	//乱数初期化
	srand((unsigned int)time(NULL));

	if (!m_Window.Init(
		desc.width,
		desc.height,
		desc.pClassName,
		desc.pWndTitle,
		desc.WndProc
	)) {
		return false;
	}

	//COMの初期化
	HRESULT result;
	result = CoInitializeEx(NULL,COINIT_MULTITHREADED);

	m_Input.Init();
	m_Time.Init(desc.DeltaTimeMsMax);
	m_FPS.Init(desc.TargetFPS, m_Time.GetTime());

	m_AssetManager.Init(
		desc.AssetRootDir,
		desc.AssetBufferSize
	);

	//フォントファイルのパスを作る
	wchar_t fontfilepath[256] = {};
	wcscpy_s(fontfilepath, 256, m_AssetManager.GetAssetsRoot().c_str());
	wcscat_s(fontfilepath, 256, desc.SpriteFontFile);

	//シェーダーのパスを作る
	wchar_t VtxShaderPath[256] = {};
	wcscpy_s(VtxShaderPath, 256, m_AssetManager.GetAssetsRoot().c_str());
	wcscat_s(VtxShaderPath, 256, desc.VtxShaderFile);

	wchar_t PixShaderPath[256] = {};
	wcscpy_s(PixShaderPath, 256, m_AssetManager.GetAssetsRoot().c_str());
	wcscat_s(PixShaderPath, 256, desc.PixShaderFile);

	m_RenderManager.Init(
		m_Window.GetHWND(),
		desc.width,
		desc.height,
		desc.RenderRequestBufferSize,
		desc.DescriptorSize_Resouce,
		desc.DescriptorSize_Sampler,
		fontfilepath,
		VtxShaderPath,
		PixShaderPath
	);

	m_Audio.Init();

	for (int i = 0; i < TEXT_BUFFER_MAX; ++i) {
		m_Text[i].Init();
	}
	m_UseTextNum = 0;

#if defined(DEBUG) || defined(_DEBUG)
	mDebugLog.Init();
#endif

	//コリジョンマネージャの初期化
	m_CollisionManager.Init(desc.CollisionBufferSize);

	//タイマーの精度を上げる
	timeBeginPeriod(1);

	return true;
}

void Framework::Term()
{
	//タイマーの制度を元に戻す
	timeEndPeriod(1);

	m_CollisionManager.Term();

#if defined(DEBUG) || defined(_DEBUG)
	mDebugLog.Term();
#endif

	for (int i = 0; i < TEXT_BUFFER_MAX; ++i) {
		m_Text[i].Term();
	}

	m_Time.Term();
	m_Input.Term();

	m_Audio.Term();
	m_RenderManager.Term();
	m_AssetManager.Term();

	CoUninitialize();

	m_Window.Term();
}

void Framework::Update()
{
	m_UseTextNum = 0;

	m_Time.Update();
	m_FPS.Update(m_Time.GetTime());
	m_Input.Update();

#if defined(DEBUG) || defined(_DEBUG)
	mDebugLog.Update(m_Time.GetDeltaTime());
#endif
}

void Framework::Render()
{
	for (int i = 0; i < m_UseTextNum; ++i) {
		m_Text[i].Draw();
	}
#if defined(DEBUG) || defined(_DEBUG)
	mDebugLog.Draw();
#endif

	m_FPS.Draw();
	m_RenderManager.Render();
}

void Framework::FrameSync()
{
	int waitTime =m_FPS.CalcWaitTime();
	//指定FPSになるよう待つ
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}

void Framework::ShowFPS() {
	m_FPS.SetVisible(true);
}

void Framework::HideFPS() {
	m_FPS.SetVisible(false);
}

void Framework::SetFPSPosition(float x, float y) {
	m_FPS.SetDrawPosition(x,y);
}

//ウィンドウの中心点を原点とした時のカーソル位置を戻す
//ゲーム内座標ではなくウィンドウの内の位置なので注意
Vector2f Framework::GetMousePointInClient()const {
	return m_Window.TransformToClient(m_Input.GetMousePoint());
}


RenderManager* Framework::GetRenderManager() {
	return &m_RenderManager;
}

Fwk::Collision::CollisionManager* Framework::GetCollisionManager() {
	return &m_CollisionManager;
}

IInput* Framework::GetInput(){
	return &m_Input;
}

Time* Framework::GetTime() {
	return &m_Time;
}

Window* Framework::GetWindow() {
	return &m_Window;
}
Lib::Audio::Audio* Framework::GetAudio() {
	return &m_Audio;
}

AssetManager* Framework::GetAssetManager() {
	return &m_AssetManager;
}

void Framework::_PrintText(const wchar_t* pText, float pos_x, float pos_y)
{
	const float color[3] = { 1.0f,1.0f,1.0f };
	_PrintText(pText, pos_x, pos_y, color, 1.0f);
}
void Framework::_PrintText(const wchar_t* pText, float pos_x, float pos_y, const float rgb[])
{
	_PrintText(pText, pos_x, pos_y, rgb, 1.0f);
}

void Framework::_PrintText(const wchar_t* pText, float pos_x, float pos_y, const float rgb[], float scale)
{
	if (m_UseTextNum >= TEXT_BUFFER_MAX)
		return;

	m_Text[m_UseTextNum].SetText(pText);
	m_Text[m_UseTextNum].SetPosition(pos_x, pos_y);
	m_Text[m_UseTextNum].SetColor(rgb[0], rgb[1], rgb[2]);
	m_Text[m_UseTextNum].SetScale(scale, scale);

	++m_UseTextNum;
}

void Framework::_PrintText(const char* pText, float pos_x, float pos_y)
{
	const float color[3] = { 1.0f,1.0f,1.0f };
	_PrintText(pText, pos_x, pos_y, color, 1.0f);
}
void Framework::_PrintText(const char* pText, float pos_x, float pos_y, const float rgb[])
{
	_PrintText(pText, pos_x, pos_y, rgb, 1.0f);
}
void Framework::_PrintText(const char* pText, float pos_x, float pos_y, const float rgb[], float scale)
{
	if (m_UseTextNum >= TEXT_BUFFER_MAX)
		return;

	m_Text[m_UseTextNum].SetText(pText);
	m_Text[m_UseTextNum].SetPosition(pos_x, pos_y);
	m_Text[m_UseTextNum].SetColor(rgb[0], rgb[1], rgb[2]);
	m_Text[m_UseTextNum].SetScale(scale, scale);

	++m_UseTextNum;
}

void Framework::_PrintText(const string& pText, float pos_x, float pos_y) {
	_PrintText(pText.c_str(), pos_x, pos_y);
}
void Framework::_PrintText(const string& pText, float pos_x, float pos_y, const float rgb[]) {
	_PrintText(pText.c_str(), pos_x, pos_y,rgb);
}
void Framework::_PrintText(const string& pText, float pos_x, float pos_y, const float rgb[], float scale) {
	_PrintText(pText.c_str(), pos_x, pos_y, rgb,scale);
}

void Framework::DebugLogImpl(const char* format, ...)
{
#if defined(DEBUG) || defined(_DEBUG)
	va_list args;
	va_start(args, format);
	mDebugLog.Print(format,args);
	va_end(args);   // (9)
#endif
}

void Framework::DebugLogImpl(const string& format, ...) {
#if defined(DEBUG) || defined(_DEBUG)
	mDebugLog.Print(format.c_str());
#endif

}

}//namespace Fwk