#include "Lib/Graphics/Graphics.h"

#include <cassert>
#include <windows.h>

#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dxguid.lib" )

using namespace DirectX;

namespace Lib {
namespace Graphics {

Graphics::Graphics()
	: m_hInst(0)
	, m_hWnd(0)
	, m_Width(0)
	, m_Height(0)
	, m_pDevice()
	, m_pQueue()
	, m_pSwapChain()
	, m_CommandList()
	, m_Fence()
	, m_Viewport()
	, m_Scissor()
	, m_pRootSignature()
	, m_pPipelineState()
	, m_pDescriptorHeaps()
	, m_RenderTarget()
	, m_DepthTarget()
	, m_ClearColor()
	, m_FrameIndex(0)
{
	m_ClearColor[0] = 0.25f;
	m_ClearColor[1] = 0.25f;
	m_ClearColor[2] = 0.25f;
	m_ClearColor[3] = 1.0f;
};

Graphics::~Graphics()
{

}

//グラフィクスの初期化処理
bool Graphics::Init(InitializeParameters params)
{
	//デバッグレイヤーの有効化
#if defined(DEBUG) || defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debug;
		auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));
		if (SUCCEEDED(hr)) {
			debug->EnableDebugLayer();
		}
	}
#endif

	//バッファサイズ
	RECT rect;
	GetClientRect(params.hwnd, &rect);
	const int _w = rect.right - rect.left;
	const int _h = rect.bottom - rect.top;

	HRESULT hr;

	//デバイスの生成
	{
		hr = D3D12CreateDevice(
			nullptr,
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(m_pDevice.GetAddressOf())
		);
		if (FAILED(hr))
			return false;
	}

	//コマンドキューの生成
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		hr = m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pQueue.GetAddressOf()));
		if (FAILED(hr))
			return false;
	}

	//スワップチェーンの生成
	{
		//DXGIファクトリーの生成
		ComPtr<IDXGIFactory4> pFactory = nullptr;
		HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
		if (FAILED(hr))
			return false;

		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferDesc.Width = _w;
		desc.BufferDesc.Height = _h;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = FrameCount;
		desc.OutputWindow = params.hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		ComPtr<IDXGISwapChain> pSwapChain = nullptr;
		hr = pFactory->CreateSwapChain(m_pQueue.Get(), &desc, &pSwapChain);
		if (FAILED(hr))
		{
			pFactory.Reset();
			return false;
		}

		hr = pSwapChain->QueryInterface(IID_PPV_ARGS(m_pSwapChain.GetAddressOf()));
		if (FAILED(hr))
		{
			pFactory.Reset();
			pSwapChain.Reset();
			return false;
		}

		m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		//不要になったので削除
		pFactory.Reset();
		pSwapChain.Reset();
	}

	// ディスクリプタプールの生成.
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};

		desc.NodeMask = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = params.NumDescriptors[HEAP_TYPE_RES];
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (!DescriptorHeap::Create(m_pDevice.Get(), &desc, &m_pDescriptorHeaps[HEAP_TYPE_RES]))
		{
			return false;
		}

		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		desc.NumDescriptors = params.NumDescriptors[HEAP_TYPE_SMP];
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (!DescriptorHeap::Create(m_pDevice.Get(), &desc, &m_pDescriptorHeaps[HEAP_TYPE_SMP]))
		{
			return false;
		}

		//レンダーターゲットビュー用のディスクリプタヒープ
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors = params.NumDescriptors[HEAP_TYPE_RTV];
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		if (!DescriptorHeap::Create(m_pDevice.Get(), &desc, &m_pDescriptorHeaps[HEAP_TYPE_RTV]))
		{
			return false;
		}

		//デプスステンシルビューの為のデスクリプタヒープ
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.NumDescriptors = params.NumDescriptors[HEAP_TYPE_DSV];
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		if (!DescriptorHeap::Create(m_pDevice.Get(), &desc, &m_pDescriptorHeaps[HEAP_TYPE_DSV]))
		{
			return false;
		}
	}

	//レンダーターゲットビューの生成
	{
		for (auto i = 0u; i < FrameCount; ++i)
		{
			if (!m_RenderTarget[i].InitFromBackBuffer(
				m_pDevice.Get(),
				m_pDescriptorHeaps[HEAP_TYPE_RTV],
				i,
				m_pSwapChain.Get()))
			{
				return false;
			}
		}
	}

	//深度ステンシルバッファの生成
	m_DepthTarget.Init(
		m_pDevice.Get(),
		m_pDescriptorHeaps[HEAP_TYPE_DSV],
		_w,
		_h
	);

	//コマンドリストの生成
	m_CommandList.Init(
		m_pDevice.Get(),
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		FrameCount
	);

	//フェンスの生成
	m_Fence.Init(m_pDevice.Get());

	//ビューポートとシザー矩形
	{
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = static_cast<float>(_w);
		m_Viewport.Height = static_cast<float>(_h);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		m_Scissor.left = 0;
		m_Scissor.top = 0;
		m_Scissor.right = _w;
		m_Scissor.bottom = _h;
	}

	return true;
}

void Graphics::Term()
{
	// フェンス破棄.
	m_Fence.Term();

	for (int i = 0; i < HEAP_COUNT; ++i)
	{
		if (m_pDescriptorHeaps[i]) {
			m_pDescriptorHeaps[i]->Release();
			m_pDescriptorHeaps[i] = nullptr;
		}
	}

	// 深度ステンシルビューの破棄.
	m_DepthTarget.Term();

	// レンダーターゲットビューの破棄.
	for (auto i = 0u; i < FrameCount; ++i) {
		m_RenderTarget[i].Term();
	}

	//コマンドリストの解放
	m_CommandList.Term();

	m_pSwapChain.Reset();
	m_pQueue.Reset();
	m_pDevice.Reset();
}

void Graphics::BeginRender()
{
	//バックバッファ番号を更新
	m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	//コマンドの記録を開始
	m_CommandList.Reset(m_FrameIndex);
	ID3D12GraphicsCommandList* pCommandList = m_CommandList.Get();

	//リソースバリアの設定
	{
		//コマンド積み始める設定
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_RenderTarget[m_FrameIndex].GetResource();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		//リソースバリア
		pCommandList->ResourceBarrier(1, &barrier);
	}

	//レンダーターゲットの設定
	pCommandList->OMSetRenderTargets(
		1,
		&(m_RenderTarget[m_FrameIndex].GetHandleRTV()->HandleCPU),
		FALSE,
		&m_DepthTarget.GetHandleDSV()->HandleCPU
	);

	//レンダーターゲットビューをクリア
	pCommandList->ClearRenderTargetView(
		m_RenderTarget[m_FrameIndex].GetHandleRTV()->HandleCPU,
		m_ClearColor,
		0,
		nullptr
	);

	//深度ステンシルビューをクリア
	pCommandList->ClearDepthStencilView(
		m_DepthTarget.GetHandleDSV()->HandleCPU,
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr
	);
}

void Graphics::EndRender()
{
	ID3D12GraphicsCommandList* pCommandList = m_CommandList.Get();

	//リソースバリアの設定
	{
		//コマンド積み終わったのでGPUに描画させるよう設定

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_RenderTarget[m_FrameIndex].GetResource();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		//リソースバリア
		pCommandList->ResourceBarrier(1, &barrier);
	}

	//コマンドの記録を終了
	pCommandList->Close();

	//コマンドを実行
	ID3D12CommandList* ppCmdLists[] = { pCommandList };
	m_pQueue->ExecuteCommandLists(1, ppCmdLists);

	//画面に表示
	m_pSwapChain->Present(1, 0);

	// 完了待ち.
	m_Fence.Wait(m_pQueue.Get(), INFINITE);
}

ID3D12Device* Graphics::GetDevice()const
{
	return m_pDevice.Get();
}

DescriptorHeap* Graphics::GetDescriptorHeap(HEAP_TYPE heap)const
{
	return m_pDescriptorHeaps[heap];
}
const D3D12_VIEWPORT& Graphics::GetViewPort()
{
	return m_Viewport;
}
const D3D12_RECT& Graphics::GetScissorRect()
{
	return m_Scissor;
}
ID3D12CommandQueue* Graphics::GetCommandQueue()const
{
	return m_pQueue.Get();
}

uint32_t Graphics::GetFrameIndex()const {
	return m_FrameIndex;
}

CommandList* Graphics::GetCommandList(){
	return &m_CommandList;
}

void Graphics::SyncGpuProc() {
	// GPU処理の完了を待機.
	m_Fence.Sync(m_pQueue.Get());
}

void Graphics::SetClearColor(float r, float g, float b) {
	m_ClearColor[0] = r;
	m_ClearColor[1] = g;
	m_ClearColor[2] = b;

}

}//namespace Graphics
}//namespace Lib