#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>

#include "ComPtr.h"
#include "DescriptorHeap.h"
#include "CommandList.h"
#include "RenderTarget.h"
#include "DepthTarget.h"
#include "Fence.h"

namespace Lib {
namespace Graphics {

enum HEAP_TYPE
{
	HEAP_TYPE_RES = 0,     // CBV / SRV / UAV
	HEAP_TYPE_SMP = 1,     // Sampler
	HEAP_TYPE_RTV = 2,     // RTV
	HEAP_TYPE_DSV = 3,     // DSV
	HEAP_COUNT = 4,
};

//グラフィクス情報を取得するためのインターフェース
//必要な情報を渡す際はGraphicsを直接渡さずインターフェースを介して渡す
class IGraphics {
public:
	virtual ID3D12Device* GetDevice()const = 0;
	virtual DescriptorHeap* GetDescriptorHeap(HEAP_TYPE heap)const = 0;
	virtual ID3D12CommandQueue* GetCommandQueue()const = 0;
	virtual CommandList* GetCommandList() = 0;
	virtual uint32_t GetFrameIndex()const = 0;
	virtual const D3D12_VIEWPORT& GetViewPort()=0;
	virtual const D3D12_RECT& GetScissorRect() = 0;
};

class Graphics : public IGraphics
{
public:

	static const uint32_t FrameCount = 2;

	struct InitializeParameters {
		HWND hwnd;
		uint32_t NumDescriptors[HEAP_COUNT];
	};

public:

	Graphics();
	~Graphics();

	bool Init(InitializeParameters params);
	void Term();

	void BeginRender();
	void EndRender();

	ID3D12Device* GetDevice()const override;
	DescriptorHeap* GetDescriptorHeap(HEAP_TYPE heap)const override;
	ID3D12CommandQueue* GetCommandQueue()const override;
	CommandList* GetCommandList() override;
	uint32_t GetFrameIndex()const override;

	const D3D12_VIEWPORT& GetViewPort() override;
	const D3D12_RECT& GetScissorRect()override;

	void SyncGpuProc();

	void SetClearColor(float r, float g, float b);

private:

	HINSTANCE   m_hInst;        // インスタンスハンドルです.
	HWND        m_hWnd;         // ウィンドウハンドルです.
	uint32_t    m_Width;        // ウィンドウの横幅です.
	uint32_t    m_Height;       // ウィンドウの縦幅です.

	ComPtr<ID3D12Device>        m_pDevice;                   // デバイスです.
	ComPtr<ID3D12CommandQueue>  m_pQueue;                    // コマンドキューです.
	ComPtr<IDXGISwapChain3>     m_pSwapChain;                // スワップチェインです.

	CommandList m_CommandList;

	Fence m_Fence;

	D3D12_VIEWPORT m_Viewport; //ビューポート
	D3D12_RECT m_Scissor; //シザー矩形

	ComPtr<ID3D12RootSignature> m_pRootSignature; //ルートシグネチャ
	ComPtr<ID3D12PipelineState> m_pPipelineState; //パイプラインステート
	DescriptorHeap* m_pDescriptorHeaps[HEAP_COUNT];

	RenderTarget m_RenderTarget[FrameCount];
	DepthTarget m_DepthTarget;

	float m_ClearColor[4];

	uint32_t m_FrameIndex;
};

}//namespace Graphics
}//namespace Lib