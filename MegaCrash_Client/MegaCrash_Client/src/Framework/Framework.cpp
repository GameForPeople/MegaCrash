//-----------------------------------------------------------------------------
// File: CFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene\Main\MainScene.h"
#include "Scene\Title\TitleScene.h"
#include "Scene\Logo\LogoScene.h"
#include "Scene\Lobby\LobbyScene.h"
#include "Framework.h"

CFramework::CFramework()
	: m_nWndClientWidth				{ CLIENT_WIDTH }
	, m_nWndClientHeight			{ CLIENT_HEIGHT }
	, m_pdxgiFactory				{ nullptr }
	, m_pdxgiSwapChain				{ nullptr }
	, m_pd3dDevice					{ nullptr }
	, m_CurrSwapChainBufIndex		{ 0 }
	, m_pd3dCommandAllocator		{ nullptr }
	, m_pd3dCommandQueue			{ nullptr }
	, m_pd3dCommandList				{ nullptr }
	, m_pd3dFence					{ nullptr }
	, m_hFenceEvent					{ nullptr }
	, m_pBufMgr						{ CD3DBufMgr::Instance() }
	, m_pResMgr						{ CResMgr::Instance() }
	, m_CurrSceneTag				{ CScene::Tag::Logo }
	, m_pScene						{ nullptr }
	, m_pPlayer						{ nullptr }
	, m_pDeferredLightingShader		{ nullptr }
	, m_pBlendRTShader				{ nullptr }
{
	::SetWindowText(m_hWnd, TITLESTRING);
	for (int i = 0; i < NUM_SWAP_CHAIN_BUFS; i++) {
		m_nFenceValues[i] = 0;
	}
}
CFramework::~CFramework()
{
}

bool CFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	// 클래스와 윈도우 프로시저 연결
	::SetUserDataPtr(m_hWnd, this);

	CreateD3DDevice();
	CreateCmdQueueAndList();
	CreateRtvDsvDescHeaps();
	CreateSwapChain();

	BuildObjects();

	return(true);
}

//#define _WITH_SWAPCHAIN

void CFramework::CreateSwapChain()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = NUM_SWAP_CHAIN_BUFS;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc = m_pResMgr->GetSampleDesc();
	dxgiSwapChainDesc.Windowed = TRUE;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	m_pdxgiFactory->CreateSwapChain(
		  m_pd3dCommandQueue.Get()
		, &dxgiSwapChainDesc
		, reinterpret_cast<IDXGISwapChain **>(m_pdxgiSwapChain.GetAddressOf()));

	if (!m_pdxgiSwapChain) {
		MessageBox(NULL, L"Swap Chain Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}
	
	m_pdxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
	m_CurrSwapChainBufIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
}

void CFramework::CreateD3DDevice()
{
	DWORD dxgiFactoryFlags{ 0 };
#if defined(_DEBUG)
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_pd3dDebugController)))) {
		m_pd3dDebugController->EnableDebugLayer();
	}
	else {
		MessageBox(NULL, L"Failed EnableDebugLayer()", L"Error", MB_OK);
	}
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(m_pdxgiInfoQueue.GetAddressOf()))))
	{
		dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

		m_pdxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
		m_pdxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
	}
#endif
	::CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_pdxgiFactory));

	ComPtr<IDXGIAdapter1> pd3dAdapter = nullptr;
	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pdxgiFactory->EnumAdapters1(i, &pd3dAdapter); ++i) {
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
#if USE_FEATURE_LEVEL_12
		if (SUCCEEDED(D3D12CreateDevice(
			  pd3dAdapter.Get()
			, D3D_FEATURE_LEVEL_12_0
			, IID_PPV_ARGS(&m_pd3dDevice)))) 
			break;
#else
		if (SUCCEEDED(D3D12CreateDevice(
			  pd3dAdapter.Get()
			, D3D_FEATURE_LEVEL_11_0
			, IID_PPV_ARGS(&m_pd3dDevice)))) 
			break;
#endif
	}

	if (!m_pd3dDevice) {
		m_pdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pd3dAdapter));
		D3D12CreateDevice(pd3dAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pd3dDevice));
	}

	if (!m_pd3dDevice) {
		MessageBox(NULL, L"Direct3D 12 Device Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pd3dFence));
	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	for (UINT i = 0; i < NUM_SWAP_CHAIN_BUFS; i++) 
		m_nFenceValues[i] = 1;

	m_pBufMgr->ConnectDevice(m_pd3dDevice);
	m_pResMgr->ConnectDevice(m_pd3dDevice);
}

void CFramework::CreateCmdQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	HRESULT hResult = m_pd3dDevice->CreateCommandQueue(
		  &d3dCommandQueueDesc
		, IID_PPV_ARGS(&m_pd3dCommandQueue));

	hResult = m_pd3dDevice->CreateCommandAllocator(
		  D3D12_COMMAND_LIST_TYPE_DIRECT
		, IID_PPV_ARGS(&m_pd3dCommandAllocator));

	hResult = m_pd3dDevice->CreateCommandList(
		  0
		, D3D12_COMMAND_LIST_TYPE_DIRECT
		, m_pd3dCommandAllocator.Get()
		, NULL
		, IID_PPV_ARGS(&m_pd3dCommandList));
	hResult = m_pd3dCommandList->Close();
}

void CFramework::CreateRtvDsvDescHeaps()
{
	m_pRTVDescHeap = make_shared<DescriptorHeap>(
		  m_pd3dDevice.Get()
		, D3D12_DESCRIPTOR_HEAP_TYPE_RTV
		, NUM_SWAP_CHAIN_BUFS + NUM_RENDER_TARGET_BUFS
		, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

	m_pDSVDescHeap = make_shared<DescriptorHeap>(
		  m_pd3dDevice.Get()
		, D3D12_DESCRIPTOR_HEAP_TYPE_DSV
		, 1
		, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
}

void CFramework::CreateRenderTargetViews()
{
	for (UINT i = 0; i < NUM_SWAP_CHAIN_BUFS; i++)
	{
		m_vecSwapChainBufs.push_back(RT_DESC{});
		RT_DESC& rt = m_vecSwapChainBufs.back();
		m_pdxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&rt.pResource));

		D3D12_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
		d3dRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice = 0;
		d3dRTVDesc.Texture2D.PlaneSlice = 0;
		rt.view_desc = d3dRTVDesc;
		m_pRTVDescHeap->CreateView(m_pd3dDevice.Get(), rt, i);
	}

	DXGI_FORMAT dxgiRTFormat{ DXGI_FORMAT_R32G32B32A32_FLOAT };
	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = dxgiRTFormat;
	memcpy_s(&d3dClearValue.Color, sizeof(FLOAT) * 4, &BUF_CLEAR_COLOR, sizeof(FLOAT) * 4);
	m_pRTTextureSet = make_shared<CTexture>(2, m_pd3dDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	m_pRTTextureSet->CreateTextureSet(0, NUM_RENDER_TARGET_BUFS, ROOT_PARAMETER_TEXTURE1);
	for (UINT i = 0; i < NUM_RENDER_TARGET_BUFS; i++)
	{
		m_vecRenderTargetBufs.push_back(RT_DESC{});
		RT_DESC& rt = m_vecRenderTargetBufs.back();
		rt.pResource = m_pBufMgr->CreateTexture2DResource(
			  m_nWndClientWidth
			, m_nWndClientHeight
			, dxgiRTFormat
			, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
			, D3D12_RESOURCE_STATE_GENERIC_READ
			, &d3dClearValue);

		D3D12_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
		d3dRTVDesc.Format = dxgiRTFormat;
		d3dRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice = 0;
		d3dRTVDesc.Texture2D.PlaneSlice = 0;
		rt.view_desc = d3dRTVDesc;
		m_pRTVDescHeap->CreateView(m_pd3dDevice.Get(), rt, i + NUM_SWAP_CHAIN_BUFS);
		m_pRTTextureSet->SetTexture(m_pd3dDevice.Get(), 0, i, rt.pResource.Get());
	}
	m_pRTTextureSet->CreateTextureSet(1, 1, ROOT_PARAMETER_RESOURCE2);
	m_pRTTextureSet->SetTexture(m_pd3dDevice.Get(), 1, 0, CResMgr::Instance()->GetShadowMap()->GetDSBuf(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
	m_pDeferredLightingShader->SetRTTexture(m_pRTTextureSet);
	m_pBlendRTShader->SetRTTexture(m_pRTTextureSet);
}

void CFramework::CreateDepthStencilView()
{
	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;

	m_DepthStencilBuf.pResource = m_pBufMgr->CreateTexture2DResource(
		  m_nWndClientWidth
		, m_nWndClientHeight
		, DXGI_FORMAT_D24_UNORM_S8_UINT
		, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		, D3D12_RESOURCE_STATE_DEPTH_WRITE
		, &d3dClearValue
	);

	m_pDSVDescHeap->CreateView(m_pd3dDevice.Get(), m_DepthStencilBuf, 0);
}

void CFramework::OnResizeBackBuffers()
{
	WaitForGpuComplete();

	m_pd3dCommandList->Reset(m_pd3dCommandAllocator.Get(), NULL);

	m_vecSwapChainBufs.clear();
	m_vecRenderTargetBufs.clear();
	m_DepthStencilBuf.pResource.Reset();
	
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(
		  NUM_SWAP_CHAIN_BUFS
		, m_nWndClientWidth
		, m_nWndClientHeight
		, dxgiSwapChainDesc.BufferDesc.Format
		, dxgiSwapChainDesc.Flags);
	m_CurrSwapChainBufIndex = 0;

	CreateRenderTargetViews();
	CreateDepthStencilView();

	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList.Get() };
	m_pd3dCommandList->Close();
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	WaitForGpuComplete();

	m_pBufMgr->ReleaseUploadBuffers();
}

void CFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
			break;
		default:
			break;
	}
}

void CFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
		case WM_KEYUP:
			switch (wParam)
			{
				case VK_ESCAPE:
					::PostQuitMessage(0);
					break;
				case VK_RETURN:
					break;
				case VK_F1:
				case VK_F2:
				case VK_F3:
					m_pCamera = m_pPlayer->ChangeCamera((DWORD)(wParam - VK_F1 + 1));
					break;
				case VK_F9:
				{
					BOOL bFullScreenState = FALSE;
					m_pdxgiSwapChain->GetFullscreenState(&bFullScreenState, NULL);
					m_pdxgiSwapChain->SetFullscreenState(!bFullScreenState, NULL);

					DXGI_MODE_DESC dxgiTargetParameters;
					dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					dxgiTargetParameters.Width = m_nWndClientWidth;
					dxgiTargetParameters.Height = m_nWndClientHeight;
					dxgiTargetParameters.RefreshRate.Numerator = 60;
					dxgiTargetParameters.RefreshRate.Denominator = 1;
					dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
					m_pdxgiSwapChain->ResizeTarget(&dxgiTargetParameters);

					OnResizeBackBuffers();

					break;
				}
				case VK_F10:
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

LRESULT CFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) m_pScene->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID)
	{
		case WM_ACTIVATE:
		{
			//if (LOWORD(wParam) == WA_INACTIVE)
			//	m_GameTimer.Stop();
			//else
			//	m_GameTimer.Start();
			break;
		}
		case WM_SIZE:
		{
			m_nWndClientWidth = LOWORD(lParam);
			m_nWndClientHeight = HIWORD(lParam);

			OnResizeBackBuffers();
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEWHEEL:
			OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
            break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
			break;
	}
	return DefWindowProc(hWnd, nMessageID, wParam, lParam);
}

LRESULT CFramework::WndProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CFramework* self = ::GetUserDataPtr<CFramework*>(hWnd);
	if (!self) return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_DESTROY:
		::SetUserDataPtr(hWnd, nullptr);
		::PostQuitMessage(0);
		break;

	default:
		return self->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);

	}
	return DefWindowProc(hWnd, nMessageID, wParam, lParam);
}

void CFramework::OnDestroy()
{
    ReleaseObjects();

	::CloseHandle(m_hFenceEvent);

	m_pdxgiSwapChain->SetFullscreenState(FALSE, NULL);
}

void CFramework::BuildObjects()
{
	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList.Get() };
	m_pd3dCommandList->Reset(m_pd3dCommandAllocator.Get(), NULL);

	m_pBufMgr->CreateUploadBuffer(m_pd3dCommandList.Get());
	m_pBufMgr->CreateVertexBuffer(m_pd3dCommandList.Get());
	m_pBufMgr->CreateIndexBuffer(m_pd3dCommandList.Get());
	m_pResMgr->BuildDescs();
	m_pResMgr->CreateRootSignature();
	m_pResMgr->LoadResources(m_pd3dCommandList.Get());

	m_pDeferredLightingShader = make_shared<CDeferredLightingShader>(m_pd3dDevice.Get());
	m_pBlendRTShader = make_shared<CBlendRenderTargetShader>(m_pd3dDevice.Get());

	m_pFont = make_shared<CFont>(
		  m_pd3dDevice.Get()
		, m_pd3dCommandList.Get()
		, eTex::Font::Default
		, CFont::DefaultFontPath
		, 128
		, CLIENT_WIDTH
		, CLIENT_HEIGHT);

	m_pParticleManager = make_shared<CParticleMgr>();
	m_pParticleManager->Initialize(m_pd3dDevice.Get(), m_pd3dCommandList.Get());

	m_pObjectManager = make_shared<CObjectMgr>(m_pParticleManager);
	m_pObjectManager->Initialize(m_pd3dDevice.Get(), m_pd3dCommandList.Get());

	ChangePlayerObject(dynamic_cast<CPlayer*>(m_pObjectManager->CreatePlayerObject(
		ID_INVALID, CPlayer::Tag::Ruby, XMFLOAT3(0.0f, 0.0f, 0.0f))));

	// Stage
	m_pObjectManager->CreateStageObjects(XMFLOAT3(16262.f, 16766.f, 26967.f));

	// Cubes
	{
		UINT nObj = 500;
		CGameObject* pCube = nullptr;
		for (int i = 0; i < nObj; ++i) {
			XMFLOAT3 offset{
				  static_cast<float>((rand_int() % 2) ? (-rand_int() % 2500) : rand_int() % 2500)
				, static_cast<float>((rand_int() % 2) ? (-rand_int() % 2500) : rand_int() % 2500)
				, static_cast<float>((rand_int() % 2) ? (-rand_int() % 2500) : rand_int() % 2500)
			};

			pCube = m_pObjectManager->CreateCubeObject(
				  m_pd3dDevice.Get()
				, m_pd3dCommandList.Get()
				, Vector3::Add(m_pCamera->GetPosition(), offset));

			XMFLOAT3 rand_direction{ 0,(float)cos(rand_float() * 12 * i )*i,0 };
			pCube->Rotate(rand_direction.x, rand_direction.y, rand_direction.z);
			pCube->UpdateTransform();
		}
	}

	// Bullets
	{
		CGameObject* pBullet = nullptr;
		for (int i = 0; i < 50; ++i) {
			pBullet = m_pObjectManager->CreateBulletObject(
				  m_pd3dDevice.Get()
				, m_pd3dCommandList.Get()
				, XMFLOAT3(0, 0, 0));
		}
	}

	m_pShaderHelper = make_shared<CShaderHelper>(m_pd3dDevice.Get());

	m_lstScenes[CScene::Tag::Logo] = new CLogoScene();
	m_lstScenes[CScene::Tag::Title] = new CTitleScene();
	m_lstScenes[CScene::Tag::Lobby] = new CLobbyScene();
	m_lstScenes[CScene::Tag::Main] = new CMainScene();
	for (auto p : m_lstScenes) {
		p.second->SetHInstanceAndHWnd(m_hInstance, m_hWnd);
		p.second->SetFramework(this);
		p.second->SetShaderHelper(m_pShaderHelper);
		p.second->SetParticleMgr(m_pParticleManager);
		p.second->SetObjectMgr(m_pObjectManager);
		p.second->BuildObjects(m_pd3dDevice.Get(), m_pd3dCommandList.Get());
	}
	m_pScene = m_lstScenes[m_CurrSceneTag];

	m_pd3dCommandList->Close();
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
	//WaitForGpuComplete();
	//
	//m_pBufMgr->ReleaseUploadBuffers();

	m_GameTimer.Reset();
	float vol;
	CSoundMgr::Instance()->PlaySoundByName(CSoundMgr::SoundName::MainTheme);
	CSoundMgr::Instance()->GetChannel()[0]->setVolume(0.3f);
}

void CFramework::ReleaseObjects()
{
	WaitForGpuComplete();
	m_pObjectManager->Release();
	for (auto p : m_lstScenes) {
		p.second->ReleaseObjects();
		delete p.second;
	}
	m_pResMgr->Release();
	m_pBufMgr->Release();
	m_pd3dDevice.Reset();
	m_pdxgiFactory.Reset();
}

void CFramework::ProcessInput()
{
	static UCHAR pKeysBuffer[256];
	bool bProcessedByScene = false;
	if (GetKeyboardState(pKeysBuffer)) {
		bProcessedByScene = m_pScene->ProcessInput(pKeysBuffer, m_GameTimer.GetTimeElapsed());
	}
	if (!bProcessedByScene) {
		
	}
}

void CFramework::AnimateObjects()
{
	m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
}

void CFramework::WaitForGpuComplete()
{
	const UINT64 nFenceValue{ ++m_nFenceValues[m_CurrSwapChainBufIndex] };
	m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue) {
		m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void CFramework::MoveToNextFrame()
{
	m_CurrSwapChainBufIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	//m_CurrSwapChainBufIndex = (m_CurrSwapChainBufIndex + 1) % NUM_SWAP_CHAIN_BUFS;

	const UINT64 nFenceValue{ ++m_nFenceValues[m_CurrSwapChainBufIndex] };
	m_pd3dCommandQueue->Signal(m_pd3dFence.Get(), nFenceValue);

	if (m_pd3dFence->GetCompletedValue() < nFenceValue) {
		m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}

void CFramework::EnterRenderingToSwapChain(bool bClearDSB)
{
	m_pBufMgr->SynchronizeResourceTransition(
		  m_pd3dCommandList.Get()
		, m_vecSwapChainBufs[m_CurrSwapChainBufIndex].pResource.Get()
		, D3D12_RESOURCE_STATE_PRESENT
		, D3D12_RESOURCE_STATE_RENDER_TARGET);
	if (bClearDSB) {
		m_pd3dCommandList->ClearDepthStencilView(
			  m_DepthStencilBuf.hCPUDescTable
			, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL
			, 1.0f, 0, 0, NULL);
	}
	m_pd3dCommandList->ClearRenderTargetView(
		  m_vecSwapChainBufs[m_CurrSwapChainBufIndex].hCPUDescTable
		, BUF_CLEAR_COLOR, 0, NULL);
	m_pd3dCommandList->OMSetRenderTargets(
		  1
		, &m_vecSwapChainBufs[m_CurrSwapChainBufIndex].hCPUDescTable
		, TRUE, &m_DepthStencilBuf.hCPUDescTable);
}

void CFramework::LeaveRenderingToSwapChain()
{
	m_pBufMgr->SynchronizeResourceTransition(
		  m_pd3dCommandList.Get()
		, m_vecSwapChainBufs[m_CurrSwapChainBufIndex].pResource.Get()
		, D3D12_RESOURCE_STATE_RENDER_TARGET
		, D3D12_RESOURCE_STATE_PRESENT);
}

void CFramework::EnterRenderingToRenderTarget(UINT render_target_idx, bool bClearRT)
{
	m_pBufMgr->SynchronizeResourceTransition(
		  m_pd3dCommandList.Get()
		, m_vecRenderTargetBufs[render_target_idx].pResource.Get()
		, D3D12_RESOURCE_STATE_GENERIC_READ
		, D3D12_RESOURCE_STATE_RENDER_TARGET);
	if (bClearRT) {
		m_pd3dCommandList->ClearDepthStencilView(
			  m_DepthStencilBuf.hCPUDescTable
			, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL
			, 1.0f, 0, 0, NULL);
		m_pd3dCommandList->ClearRenderTargetView(
			  m_vecRenderTargetBufs[render_target_idx].hCPUDescTable
			, BUF_CLEAR_COLOR, 0, NULL);
	}
	m_pd3dCommandList->OMSetRenderTargets(
		  1
		, &m_vecRenderTargetBufs[render_target_idx].hCPUDescTable
		, FALSE, &m_DepthStencilBuf.hCPUDescTable);
}

void CFramework::LeaveRenderingToRenderTarget(UINT render_target_idx)
{
	m_pBufMgr->SynchronizeResourceTransition(
		  m_pd3dCommandList.Get()
		, m_vecRenderTargetBufs[render_target_idx].pResource.Get()
		, D3D12_RESOURCE_STATE_RENDER_TARGET
		, D3D12_RESOURCE_STATE_GENERIC_READ);
}

void CFramework::EnterDeferredLightingToRenderTargets()
{
	D3D12_CPU_DESCRIPTOR_HANDLE hCPUDescs[NUM_DEFERRED_LIGHTING_BUFS];
	D3D12_RESOURCE_BARRIER d3dResourceBarrier[NUM_DEFERRED_LIGHTING_BUFS];
	for (int i = 0; i < NUM_DEFERRED_LIGHTING_BUFS; ++i) {
		hCPUDescs[i] = m_vecRenderTargetBufs[i].hCPUDescTable;
		d3dResourceBarrier[i].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier[i].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier[i].Transition.pResource = m_vecRenderTargetBufs[i].pResource.Get();
		d3dResourceBarrier[i].Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
		d3dResourceBarrier[i].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		d3dResourceBarrier[i].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	}
	m_pd3dCommandList->ResourceBarrier(NUM_DEFERRED_LIGHTING_BUFS, d3dResourceBarrier);
	for (int i = 0; i < NUM_DEFERRED_LIGHTING_BUFS; ++i) {
		m_pd3dCommandList->ClearRenderTargetView(
		m_vecRenderTargetBufs[i].hCPUDescTable, BUF_CLEAR_COLOR, 0, NULL);
	};
	m_pd3dCommandList->ClearDepthStencilView(
		  m_DepthStencilBuf.hCPUDescTable
		, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL
		, 1.0f, 0, 0, NULL);
	m_pd3dCommandList->OMSetRenderTargets(
		  NUM_DEFERRED_LIGHTING_BUFS
		, hCPUDescs
		, FALSE, &m_DepthStencilBuf.hCPUDescTable);
}

void CFramework::LeaveDeferredLightingToRenderTargets()
{
	D3D12_RESOURCE_BARRIER d3dResourceBarrier[NUM_DEFERRED_LIGHTING_BUFS];
	for (int i = 0; i < NUM_DEFERRED_LIGHTING_BUFS; ++i) {
		d3dResourceBarrier[i].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier[i].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier[i].Transition.pResource = m_vecRenderTargetBufs[i].pResource.Get();
		d3dResourceBarrier[i].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		d3dResourceBarrier[i].Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
		d3dResourceBarrier[i].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	}
	m_pd3dCommandList->ResourceBarrier(NUM_DEFERRED_LIGHTING_BUFS, d3dResourceBarrier);
}

void CFramework::FrameAdvance()
{    
#if USE_SERVER_CONNECT
	m_pScene->ProcessMsgs();
#endif
	if (!m_GameTimer.Tick()) return;

	ProcessInput();
	AnimateObjects();

	WaitForGpuComplete();

	if (m_pdxgiSwapChain->Present(0, 0) == 0x887a0005) {
		HRESULT retval = m_pd3dDevice->GetDeviceRemovedReason();
	}
	MoveToNextFrame();

	m_pd3dCommandAllocator->Reset();
	m_pd3dCommandList->Reset(m_pd3dCommandAllocator.Get(), NULL);

	m_pScene->RenderShadowMap(m_pd3dCommandList.Get());

	EnterDeferredLightingToRenderTargets();
	{
		m_pScene->Render(m_pd3dCommandList.Get(), m_pCamera);
	}
	LeaveDeferredLightingToRenderTargets();

	EnterRenderingToSwapChain(false);
	{
		m_pDeferredLightingShader->Render(m_pd3dCommandList.Get(), m_pCamera);
		m_pScene->RenderUI(m_pd3dCommandList.Get(), m_pCamera);
		m_pFont->ResetTextData();
		m_pFont->AppendTextData(
			(std::to_wstring(m_GameTimer.GetFrameRate()) + L" FPS").c_str()
			, XMFLOAT2(0.0f, 0.0f)
			, XMFLOAT2(1.0f, 1.0f)
			, XMFLOAT2(0.5f, 0.0f)
			, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pFont->AppendTextData(
			(std::to_wstring(m_GameTimer.GetFrameRate()) + L" FPS").c_str()
			, XMFLOAT2(0.0f, 0.0f)
			, XMFLOAT2(1.0f, 1.0f)
			, XMFLOAT2(0.5f, 0.0f)
			, XMFLOAT4(0.0f, 1.0f, 0.0f, 0.9f));
		m_pFont->Render(m_pd3dCommandList.Get(), m_pCamera);
	}
	LeaveRenderingToSwapChain();
	m_pd3dCommandList->Close();

	ID3D12CommandList* ppd3dCommandLists[]{ m_pd3dCommandList.Get() };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);
}