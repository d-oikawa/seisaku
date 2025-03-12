#pragma once

#include <Windows.h>

#include "Lib/Graphics/Graphics.h"
#include "Lib/Graphics/ConstantBuffer.h"
#include "Lib/Graphics/RootSignature.h"
#include "Lib/Graphics/PipelineState.h"
#include "Lib/Graphics/ResShader.h"

#include "Fwk/Shape/Shape.h"
#include "Fwk/Graphics/Camera.h"
#include "SpriteFont.h"
#include "RenderDef.h"

#include <cstdint>
#include <vector>
#include <map>

class Font;
using namespace Lib::Graphics;

namespace Fwk {

class RenderObject;

class RenderManager
{
	friend class Framework;

private:
	
	struct Layer {
		std::string name;
		int order;
		bool isActive;
		std::vector<RenderObject*> requestBuffer;
	};

private:

	RenderManager(const RenderManager&) = delete;
	void operator = (const RenderManager&) = delete;

	//この辺りはフレームワークにのみ呼ばせる

	RenderManager();
	~RenderManager();

	void Init(
		HWND hwnd,
		uint32_t width,
		uint32_t height,
		uint32_t bufferSize,
		uint32_t DescriptorSize_Resource,
		uint32_t DescriptorSize_Sampler,
		const wchar_t* pFontPath,
		const wchar_t* pVtxShaderPath,
		const wchar_t* pPixShaderPath
	);

	void Term();
	void Render();

public:

	void RequestRender(RenderObject* pDrawable);

	void CreateConstantBuffer(Lib::Graphics::ConstantBuffer& ConstBufferRef);

	void SyncGpuProc();

	Lib::Graphics::IGraphics* GetGraphics();

	template<class T>
	void CreateShape(T* shape) {
		auto pShape = static_cast<Fwk::Shape*>(shape);
		if (pShape == nullptr)
			return;

		pShape->Init(m_Graphics.GetDevice());
	}

	void SetupFont(Font* pFont);

	void SetClearColor(float r, float g, float b);
	void SetClearColor(const float rgb[]);

	void AddLayer(const std::string& name);
	void RemoveLayer(const std::string& name);
	void SetLayerOrder(const std::string& name,int order);
	void SetLayerActive (const std::string& name, bool isActive);

	void SetCamera(const Camera& camera);
	void RemoveCamera(const std::string& name);
	Camera GetCamera();
	Camera GetCamera(const std::string& name);

	//メインカメラの操作
	void SetCameraOrder(int order);
	void SetCameraActive (bool isActive);
	void SetCameraPosition (const Vector2f& position);

	//カメラ名を指定して操作
	void SetCameraOrder(const std::string& name,int order);
	void SetCameraActive (const std::string& name, bool isActive);
	void SetCameraPosition (const std::string& name, const Vector2f& position);

	//カメラの描画対象レイヤーの操作
	void AddCameraRenderLayer(const std::string& cameraName,const std::string& targetLayerName);
	void RemoveCameraRenderLayer(const std::string& cameraName,const std::string& targetLayerName);

private:

	enum class PIPELINE {
		NONE = -1,

		DEFAULT,
		OPAQUE_2D,
		TRANSPARENT_2D,
		ADD_2D,
		SUB_2D,
		INVERSE_2D,

		COUNT
	};
	enum class ROOT_SIGNATURE {
		NONE = -1,

		DEFAULT,
		OPAQUE_2D,
		TRANSPARENT_2D,
		ADD_2D,
		SUB_2D,
		INVERSE_2D,

		COUNT
	};

private:

	void _postPresent();
	void _clearRequest();

	PIPELINE _getPipelineState(RENDER_PASS pass, RENDER_MODE mode);
	ROOT_SIGNATURE _getRootSignature(RENDER_PASS pass, RENDER_MODE mode);
	void _onRenderPassChanged(RENDER_PASS pass, RENDER_PASS prevPass, Lib::Graphics::IGraphics* g);

	Camera* _findCamera(const string& name);
	int _getCameraIndex(const string& name);

	Layer* _findLayer(const string& name);
	int _getLayerIndex(const string& name);

	static bool cmpareDrawReq(RenderObject* p0, RenderObject* p1);
	static bool cmpareCamera(Camera* p0, Camera* p1);
	static bool cmpareLayer(RenderManager::Layer* p0, RenderManager::Layer* p1);

private:

	bool m_IsInitilized;

	Graphics m_Graphics;

	std::vector<Layer*> m_Layers;
	int m_RequestBufferSize;

	SpriteFont m_SpriteFont;

	RootSignature m_RootSignature[(int)ROOT_SIGNATURE::COUNT];
	PipelineState m_PipelineState[(int)PIPELINE::COUNT];

	ResShader m_VtxShader;
	ResShader m_PixShader;

	std::vector<Camera*> m_Cameras;
};

}