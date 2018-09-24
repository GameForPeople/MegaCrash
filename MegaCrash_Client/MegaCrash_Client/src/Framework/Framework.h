#pragma once

#include "Timer/Timer.h"
#include "Object/Player/Player.h"
#include "Object/Font/Font.h"
#include "Scene/Scene.h"
#include "Manager/ObjectMgr/ObjectMgr.h"
#include "Manager/ParticleMgr/ParticleMgr.h"
#include "Shader/Helper/ShaderHelper.h"

class CFont;
class CFramework
{
public:
	CFramework();
	~CFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void CreateSwapChain();
	void CreateD3DDevice();
	void CreateCmdQueueAndList();
	void CreateRtvDsvDescHeaps();
	void CreateRenderTargetViews();
	void CreateDepthStencilView();

	void OnResizeBackBuffers();

	void BuildObjects();
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void EnterRenderingToSwapChain(bool bClearDSB = true);
	void LeaveRenderingToSwapChain();
	void EnterRenderingToRenderTarget(UINT render_target_idx, bool bClearRT = true);
	void LeaveRenderingToRenderTarget(UINT render_target_idx);
	void EnterDeferredLightingToRenderTargets();
	void LeaveDeferredLightingToRenderTargets();

public:
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

public:
	void ChangePlayerObject(CPlayer* pPlayer) { m_pPlayer = pPlayer;  m_pCamera = pPlayer->GetCamera(); }
	void ChangeCurrScene(CScene::Tag tag) { m_CurrSceneTag = tag; m_pScene = m_lstScenes[tag]; m_pScene->ResetObjects(); }

	CPlayer* GetPlayer() const { return m_pPlayer; }

private:
	HINSTANCE								m_hInstance;
	HWND									m_hWnd;

	UINT									m_nWndClientWidth;
	UINT									m_nWndClientHeight;

	ComPtr<IDXGIFactory5>					m_pdxgiFactory;
	ComPtr<IDXGISwapChain4>					m_pdxgiSwapChain;
	ComPtr<ID3D12Device3>					m_pd3dDevice;

	UINT									m_CurrSwapChainBufIndex;
	vector<RT_DESC>							m_vecSwapChainBufs;
	vector<RT_DESC>							m_vecRenderTargetBufs;
	shared_ptr<DescriptorHeap>				m_pRTVDescHeap;
	shared_ptr<CTexture>					m_pRTTextureSet;

	DS_DESC									m_DepthStencilBuf;
	shared_ptr<DescriptorHeap>				m_pDSVDescHeap;

	ComPtr<ID3D12CommandAllocator>			m_pd3dCommandAllocator;
	ComPtr<ID3D12CommandQueue>				m_pd3dCommandQueue;
	ComPtr<ID3D12GraphicsCommandList>		m_pd3dCommandList;

	ComPtr<ID3D12Fence>						m_pd3dFence;
	UINT64									m_nFenceValues[NUM_SWAP_CHAIN_BUFS];
	HANDLE									m_hFenceEvent;

	CD3DBufMgr * const						m_pBufMgr;
	CResMgr * const							m_pResMgr;

	shared_ptr<CObjectMgr>					m_pObjectManager;
	shared_ptr<CParticleMgr>				m_pParticleManager;

	shared_ptr<CShaderHelper>				m_pShaderHelper;

#if defined(_DEBUG)
	ComPtr<ID3D12Debug>						m_pd3dDebugController;
	ComPtr<IDXGIInfoQueue>					m_pdxgiInfoQueue;
#endif

	CTimer									m_GameTimer;

	CScene::Tag								m_CurrSceneTag;
	unordered_map<CScene::Tag, CScene*>		m_lstScenes;
	CScene*									m_pScene;

	CPlayer*								m_pPlayer;
	CCamera*								m_pCamera;
	shared_ptr<CDeferredLightingShader>		m_pDeferredLightingShader;
	shared_ptr<CBlendRenderTargetShader>	m_pBlendRTShader;

	shared_ptr<CFont>						m_pFont;
};
