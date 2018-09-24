//-----------------------------------------------------------------------------
// File: LogoScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Framework\Framework.h"
#include "Object/Font/Font.h"
#include "UI/UI.h"
#include "LogoScene.h"

CLogoScene::CLogoScene()
	: CScene()
{
}
CLogoScene::~CLogoScene()
{
}

void CLogoScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
	// UI
	m_pUI = make_shared<CLogoUI>(pd3dDevice, pd3dCmdLst);
	m_pFont = make_shared<CFont>(
		  pd3dDevice
		, pd3dCmdLst
		, eTex::Font::Default
		, CFont::DefaultFontPath
		, 128
		, CLIENT_WIDTH
		, CLIENT_HEIGHT);

	CreateShaderVariables(pd3dDevice, pd3dCmdLst);
}

void CLogoScene::ReleaseObjects()
{
	ReleaseShaderVariables();
}

void CLogoScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCmdLst)
{
}
void CLogoScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCmdLst)
{
	m_pShaderHelper->UpdateShaderVariables(pd3dCmdLst);
}
void CLogoScene::ReleaseShaderVariables()
{
	m_pUI->ReleaseShaderVariables();
}

bool CLogoScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
	return false;
}

bool CLogoScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		break;
	case WM_KEYDOWN:
		break;
	}
	
	return false;
}

bool CLogoScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
		break;
	case WM_SIZE:
		break;
	}
	return false;
}

bool CLogoScene::ProcessInput(UCHAR *pKeysBuffer, float fTimeElapsed)
{
	return false;
}

void CLogoScene::AnimateObjects(float fTimeElapsed)
{
	m_fLogoTimer += fTimeElapsed;
	if (m_fLogoTimer > LOGO_WAIT_TIME) {
		m_fLogoTimer = 0.0f;
		m_pFramework->WaitForGpuComplete();
		CD3DBufMgr::Instance()->ReleaseUploadBuffers();
		m_pFramework->ChangeCurrScene(CScene::Tag::Title);
		return;
	}

	m_pUI->Animate(fTimeElapsed);

	m_pShaderHelper->UpdateTimer(fTimeElapsed);
}

void CLogoScene::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
}

void CLogoScene::Render(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();
	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);
	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);
	pCamera->UpdateShaderVariables(pd3dCmdLst);
	UpdateShaderVariables(pd3dCmdLst);
}

void CLogoScene::RenderUI(ID3D12GraphicsCommandList* pd3dCmdLst, CCamera* pCamera)
{
	static ID3D12RootSignature* pRootSignature = CResMgr::Instance()->GetRootSignature();

	pd3dCmdLst->SetGraphicsRootSignature(pRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCmdLst);
	pCamera->UpdateShaderVariables(pd3dCmdLst);

	m_pUI->RenderUI(pd3dCmdLst, pCamera);
}

void CLogoScene::RenderShadowMap(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
}