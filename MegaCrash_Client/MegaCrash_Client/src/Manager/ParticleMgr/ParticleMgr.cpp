#include "stdafx.h"
#include "ParticleMgr.h"

CParticleMgr::CParticleMgr()
{
}

CParticleMgr::~CParticleMgr()
{
}

void CParticleMgr::Render(ID3D12GraphicsCommandList * pd3dCmdLst, CCamera * pCamera)
{
	if (m_pSparkBillboard) m_pSparkBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pSparkIntenseCenterBillboard) m_pSparkIntenseCenterBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pWhiteBaseMuzzleBillboard) m_pWhiteBaseMuzzleBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pColorMuzzleBillboard) m_pColorMuzzleBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pSmokeBillboard) m_pSmokeBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pRubySmokeBillboard) m_pRubySmokeBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pLegionSmokeBillboard) m_pLegionSmokeBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pEpsilonSmokeBillboard) m_pEpsilonSmokeBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pGravisSmokeBillboard) m_pGravisSmokeBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pBulletTrailBlastBillboard) m_pBulletTrailBlastBillboard->Render(pd3dCmdLst, pCamera);

	if (m_pExplodeSmokeBillboard) m_pExplodeSmokeBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pExplodeIntenseSmokeBillboard) m_pExplodeIntenseSmokeBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pExplodeUpperCloudBillboard) m_pExplodeUpperCloudBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pExplodeFireBlackSmokeBillboard) m_pExplodeFireBlackSmokeBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pExplodeBackDraftCloudBillboard) m_pExplodeBackDraftCloudBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pExplodeSparkBillboard) m_pExplodeSparkBillboard->Render(pd3dCmdLst, pCamera);

	if (m_pHexagonDefaultBillboard) m_pHexagonDefaultBillboard->Render(pd3dCmdLst, pCamera);
	if (m_pHexagonAdditiveBillboard) m_pHexagonAdditiveBillboard->Render(pd3dCmdLst, pCamera);

}

void CParticleMgr::Animate(float fTimeElapsed)
{
	if (m_pSparkBillboard) m_pSparkBillboard->Animate(fTimeElapsed);
	if (m_pSparkIntenseCenterBillboard) m_pSparkIntenseCenterBillboard->Animate(fTimeElapsed);
	if (m_pWhiteBaseMuzzleBillboard) m_pWhiteBaseMuzzleBillboard->Animate(fTimeElapsed);
	if (m_pColorMuzzleBillboard) m_pColorMuzzleBillboard->Animate(fTimeElapsed);
	if (m_pSmokeBillboard) m_pSmokeBillboard->Animate(fTimeElapsed);
	if (m_pRubySmokeBillboard) m_pRubySmokeBillboard->Animate(fTimeElapsed);
	if (m_pLegionSmokeBillboard) m_pLegionSmokeBillboard->Animate(fTimeElapsed);
	if (m_pEpsilonSmokeBillboard) m_pEpsilonSmokeBillboard->Animate(fTimeElapsed);
	if (m_pGravisSmokeBillboard) m_pGravisSmokeBillboard->Animate(fTimeElapsed);
	if (m_pBulletTrailBlastBillboard) m_pBulletTrailBlastBillboard->Animate(fTimeElapsed);

	if (m_pExplodeSmokeBillboard) m_pExplodeSmokeBillboard->Animate(fTimeElapsed);
	if (m_pExplodeIntenseSmokeBillboard) m_pExplodeIntenseSmokeBillboard->Animate(fTimeElapsed);
	if (m_pExplodeUpperCloudBillboard) m_pExplodeUpperCloudBillboard->Animate(fTimeElapsed);
	if (m_pExplodeFireBlackSmokeBillboard) m_pExplodeFireBlackSmokeBillboard->Animate(fTimeElapsed);
	if (m_pExplodeBackDraftCloudBillboard) m_pExplodeBackDraftCloudBillboard->Animate(fTimeElapsed);
	if (m_pExplodeSparkBillboard) m_pExplodeSparkBillboard->Animate(fTimeElapsed);

	if (m_pHexagonDefaultBillboard) m_pHexagonDefaultBillboard->Animate(fTimeElapsed);
	if (m_pHexagonAdditiveBillboard) m_pHexagonAdditiveBillboard->Animate(fTimeElapsed);

}

void CParticleMgr::ParticlesParameterSettingInitialize()
{

	///////////////////<Spark>////////////////////

	m_pSparkBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(10, 10)));
	m_pSparkBillboard->AddSizeParam(CParticleParamManager::SizeParam(500, XMFLOAT2(5, 5)));
	m_pSparkBillboard->AddSizeParam(CParticleParamManager::SizeParam(1000, XMFLOAT2(2, 2)));
	m_pSparkBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(0, 0)));

	m_pSparkBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 1));
	m_pSparkBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0));

	m_pSparkBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(255.f / 255.f, 174.f / 255.f, 43.f / 255.f)));
	m_pSparkBillboard->AddColorParam(CParticleParamManager::ColorParam(500, XMFLOAT3(255.f / 255.f, 246.f / 255.f, 149.f / 255.f)));
	m_pSparkBillboard->AddColorParam(CParticleParamManager::ColorParam(2000, XMFLOAT3(233.f / 255.f, 161.f / 255.f, 133.f / 255.f)));
	m_pSparkBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(30.f / 255.f, 30.f / 255.f, 30.f / 255.f)));
	
	m_pSparkIntenseCenterBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(280, 280)));
	m_pSparkIntenseCenterBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(320, 320)));

	m_pSparkIntenseCenterBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 1));
	m_pSparkIntenseCenterBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(500, 0.2));
	m_pSparkIntenseCenterBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0));

	m_pSparkIntenseCenterBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(255.f / 255.f, 174.f / 255.f, 43.f / 255.f)));
	m_pSparkIntenseCenterBillboard->AddColorParam(CParticleParamManager::ColorParam(500, XMFLOAT3(255.f / 255.f, 246.f / 255.f, 149.f / 255.f)));
	m_pSparkIntenseCenterBillboard->AddColorParam(CParticleParamManager::ColorParam(2000, XMFLOAT3(233.f / 255.f, 118.f / 255.f, 83.f / 255.f)));
	m_pSparkIntenseCenterBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(30.f / 255.f, 30.f / 255.f, 30.f / 255.f)));

	/////////////////////<MuzzleFlash>//////////////////////////////////////
	
	m_pWhiteBaseMuzzleBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(200, 200)));
	m_pWhiteBaseMuzzleBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(160, 160)));
	
	m_pWhiteBaseMuzzleBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 1));
	m_pWhiteBaseMuzzleBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(500, 0.2));
	m_pWhiteBaseMuzzleBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0));

	m_pWhiteBaseMuzzleBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(255.f / 255.f, 255.f / 255.f, 255.f / 255.f)));
	m_pWhiteBaseMuzzleBillboard->AddColorParam(CParticleParamManager::ColorParam(500, XMFLOAT3(255.f / 255.f, 246.f / 255.f, 149.f / 255.f)));
	m_pWhiteBaseMuzzleBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(30.f / 255.f, 30.f / 255.f, 30.f / 255.f)));


	
	m_pColorMuzzleBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(160, 160)));
	m_pColorMuzzleBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(60, 60)));

	m_pColorMuzzleBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 1));
	m_pColorMuzzleBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(500, 0.2));
	m_pColorMuzzleBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0));

	m_pColorMuzzleBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(109.f / 255.f, 172.f / 255.f, 255.f / 255.f)));
	m_pColorMuzzleBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(30.f / 255.f, 30.f / 255.f, 30.f / 255.f)));

	/////////////////////<SmokeSmoke>//////////////////////////////////////


	m_pSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(30, 30)));
	m_pSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(60, 60)));

	m_pSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0));
	m_pSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(1500, 192.f / 255.f));
	m_pSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam-3500, 81.f/ 255.f));
	m_pSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(207.f / 255.f, 246.f / 255.f, 255.f / 255.f)));
	m_pSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam-1000, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));
	m_pSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));

	////////////////////<RubySmoke>//////////////////////////////////////

	m_pRubySmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(60, 60)));
	m_pRubySmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(480, 480)));

	m_pRubySmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0));
	m_pRubySmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(1500, 64.f / 255.f));
	m_pRubySmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam - 3500, 32.f / 255.f));
	m_pRubySmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pRubySmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(255.f / 255.f, 246.f / 255.f, 207.f / 255.f)));
	m_pRubySmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam - 1000, XMFLOAT3(128.f / 255.f, 0.f / 255.f, 0.f / 255.f)));
	m_pRubySmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));

	////////////////////<LegionSmoke>//////////////////////////////////////

	m_pLegionSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(60, 60)));
	m_pLegionSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(480, 480)));

	m_pLegionSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0));
	m_pLegionSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(1500, 64.f / 255.f));
	m_pLegionSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam - 3500, 32.f / 255.f));
	m_pLegionSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pLegionSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(255.f / 255.f, 246.f / 255.f, 207.f / 255.f)));
	m_pLegionSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam - 1000, XMFLOAT3(150.f / 255.f, 243.f / 255.f, 190.f / 255.f)));
	m_pLegionSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));

	////////////////////<EpsilonSmoke>//////////////////////////////////////

	m_pEpsilonSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(60, 60)));
	m_pEpsilonSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(480, 480)));

	m_pEpsilonSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0));
	m_pEpsilonSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(1500, 128.f / 255.f));
	m_pEpsilonSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam - 3500, 64.f / 255.f));
	m_pEpsilonSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pEpsilonSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(181.f / 255.f, 228.f / 255.f, 255.f / 255.f)));
	m_pEpsilonSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam - 1000, XMFLOAT3(159.f / 255.f, 223.f / 255.f, 255.f / 255.f)));
	m_pEpsilonSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(255.f / 255.f, 255.f / 255.f, 255.f / 255.f)));

	////////////////////<GravisSmoke>//////////////////////////////////////

	m_pGravisSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(60, 60)));
	m_pGravisSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(480, 480)));

	m_pGravisSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0));
	m_pGravisSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(1500, 64.f / 255.f));
	m_pGravisSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam - 3500, 32.f / 255.f));
	m_pGravisSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pGravisSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(240.f / 255.f, 234.f / 255.f, 183.f / 255.f)));
	m_pGravisSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam - 1000, XMFLOAT3(128.f / 255.f, 128.f / 255.f, 128.f / 255.f)));
	m_pGravisSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(243.f / 255.f, 249.f / 255.f, 67.f / 255.f)));

	////////////////////<BulletSpark>//////////////////////////////////////

	m_pBulletTrailBlastBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(160, 160)));
	m_pBulletTrailBlastBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam -5000, XMFLOAT2(280, 280)));
	
	m_pBulletTrailBlastBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0));
	m_pBulletTrailBlastBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(1500, 64.f / 255.f));
	m_pBulletTrailBlastBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam - 3500, 0.f / 255.f));
	m_pBulletTrailBlastBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pBulletTrailBlastBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(207.f / 255.f, 207.f / 255.f, 207.f / 255.f)));
	m_pBulletTrailBlastBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));

	////////////////////////<Explode>///////////////////////////////

	//m_pExplodeSmokeBillboard
	m_pExplodeSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(50, 50)));
	m_pExplodeSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(80, 80)));

	m_pExplodeSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0.f / 255.f));
	m_pExplodeSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.05, 255.f / 255.f));
	m_pExplodeSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.45, 240.f / 255.f));
	m_pExplodeSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.7, 0.f / 255.f));
	m_pExplodeSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pExplodeSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(49.f / 255.f, 49.f / 255.f, 53.f / 255.f)));
	m_pExplodeSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));

	//m_pExplodeIntenseSmokeBillboard
	m_pExplodeIntenseSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(20, 20)));
	m_pExplodeIntenseSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.075, XMFLOAT2(70, 70)));
	m_pExplodeIntenseSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.135, XMFLOAT2(80, 80)));
	m_pExplodeIntenseSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.9, XMFLOAT2(90, 90)));
	m_pExplodeIntenseSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(60, 60)));

	m_pExplodeIntenseSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0.f / 255.f));
	m_pExplodeIntenseSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.3, 255.f / 255.f));
	m_pExplodeIntenseSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.5, 120.f / 255.f));
	m_pExplodeIntenseSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.9, 0.f / 255.f));
	m_pExplodeIntenseSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pExplodeIntenseSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(148.f / 255.f, 129.f / 255.f, 255.f / 255.f)));
	m_pExplodeIntenseSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam*0.35, XMFLOAT3(255.f / 255.f, 184.f / 255.f, 109.f / 255.f)));
	m_pExplodeIntenseSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam*0.9, XMFLOAT3(205.f / 255.f, 134.f / 255.f, 59.f / 255.f)));
	m_pExplodeIntenseSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));

	//m_pExplodeUpperCloudBillboard
	m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(30, 30)));
	m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.001, XMFLOAT2(70, 70)));
	m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.003, XMFLOAT2(140, 140)));
	m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.7, XMFLOAT2(142, 142)));
	//m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.75, XMFLOAT2(30, 30)));
	//m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.8, XMFLOAT2(10, 10)));
	//m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.85, XMFLOAT2(0, 0)));
	//m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(0, 0)));
	m_pExplodeUpperCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(143, 143)));

	m_pExplodeUpperCloudBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 255.f / 255.f));
	m_pExplodeUpperCloudBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.1, 255.f / 255.f));
	m_pExplodeUpperCloudBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.5, 32.f / 255.f));
	m_pExplodeUpperCloudBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pExplodeUpperCloudBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(128.f / 255.f, 91.f / 255.f, 55.f / 255.f)));
	m_pExplodeUpperCloudBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));
	//m_pExplodeFireBlackSmokeBillboard
	m_pExplodeFireBlackSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(30, 30)));
	m_pExplodeFireBlackSmokeBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(50, 50)));

	m_pExplodeFireBlackSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0.f / 255.f));
	m_pExplodeFireBlackSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.1, 35.f / 255.f));
	m_pExplodeFireBlackSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.7, 8.f / 255.f));
	m_pExplodeFireBlackSmokeBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pExplodeFireBlackSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(105.f / 255.f, 108.f / 255.f, 112.f / 255.f)));
	m_pExplodeFireBlackSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam*0.25, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));
	m_pExplodeFireBlackSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam*0.45, XMFLOAT3(47.f / 255.f, 48.f / 255.f, 52.f / 255.f)));
	m_pExplodeFireBlackSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam*0.6, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));
	m_pExplodeFireBlackSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam*0.8, XMFLOAT3(54.f / 255.f, 54.f / 255.f, 54.f / 255.f)));
	m_pExplodeFireBlackSmokeBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));

	//m_pExplodeBackDraftCloudBillboard
	m_pExplodeBackDraftCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMinParam, XMFLOAT2(10, 10)));
	m_pExplodeBackDraftCloudBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(1, 1)));

	m_pExplodeBackDraftCloudBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0.f / 255.f));
	m_pExplodeBackDraftCloudBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.1, 255.f / 255.f));
	m_pExplodeBackDraftCloudBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.7, 32.f / 255.f));
	m_pExplodeBackDraftCloudBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pExplodeBackDraftCloudBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(72.f / 255.f, 72.f / 255.f, 81.f / 255.f)));
	m_pExplodeBackDraftCloudBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam*0.4, XMFLOAT3(98.f / 255.f, 84.f / 255.f, 76.f / 255.f)));
	m_pExplodeBackDraftCloudBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(72.f / 255.f, 72.f / 255.f, 81.f / 255.f)));

	//m_pExplodeSparkBillboard
	m_pExplodeSparkBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(30, 30)));
	m_pExplodeSparkBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(0, 0)));

	m_pExplodeSparkBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 255.f / 255.f));
	m_pExplodeSparkBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.1, 255.f / 255.f));
	m_pExplodeSparkBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.5, 32.f / 255.f));
	m_pExplodeSparkBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pExplodeSparkBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(128.f / 255.f, 91.f / 255.f, 55.f / 255.f)));
	m_pExplodeSparkBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));


	///////////////////////////////////////////
	//m_pHexagonDefaultBillboard


	m_pHexagonDefaultBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(20, 20)));
	m_pHexagonDefaultBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.1, XMFLOAT2(32, 32)));
	m_pHexagonDefaultBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.4, XMFLOAT2(37, 37)));
	m_pHexagonDefaultBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.8, XMFLOAT2(40, 40)));
	m_pHexagonDefaultBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(42, 42)));

	m_pHexagonDefaultBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0.f / 255.f));
	m_pHexagonDefaultBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.1, 255.f / 255.f));
	m_pHexagonDefaultBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.5, 128.f / 255.f));
	m_pHexagonDefaultBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.6, 0.f / 255.f));
	m_pHexagonDefaultBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.7, 64.f / 255.f));
	m_pHexagonDefaultBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.8, 0.f / 255.f));
	m_pHexagonDefaultBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.95, 32.f / 255.f));
	m_pHexagonDefaultBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pHexagonDefaultBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(255.f / 255.f, 255.f / 255.f, 255.f / 255.f)));
	m_pHexagonDefaultBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));

	m_pHexagonAdditiveBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(20, 20)));
	m_pHexagonAdditiveBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.1, XMFLOAT2(32, 32)));
	m_pHexagonAdditiveBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.4, XMFLOAT2(37, 37)));
	m_pHexagonAdditiveBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.8, XMFLOAT2(40, 40)));
	m_pHexagonAdditiveBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam*0.9, XMFLOAT2(42, 42)));
	m_pHexagonAdditiveBillboard->AddSizeParam(CParticleParamManager::SizeParam(ParticleMaxParam, XMFLOAT2(0, 0)));

	m_pHexagonAdditiveBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMinParam, 0.f / 255.f));
	m_pHexagonAdditiveBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.1, 255.f / 255.f));
	m_pHexagonAdditiveBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.5, 128.f / 255.f));
	m_pHexagonAdditiveBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.6, 0.f / 255.f));
	m_pHexagonAdditiveBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.7, 64.f / 255.f));
	m_pHexagonAdditiveBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.8, 0.f / 255.f));
	m_pHexagonAdditiveBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam*0.95, 32.f / 255.f));
	m_pHexagonAdditiveBillboard->AddAlphaParam(CParticleParamManager::AlphaParam(ParticleMaxParam, 0.f / 255.f));

	m_pHexagonAdditiveBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMinParam, XMFLOAT3(255.f / 255.f, 255.f / 255.f, 255.f / 255.f)));
	m_pHexagonAdditiveBillboard->AddColorParam(CParticleParamManager::ColorParam(ParticleMaxParam, XMFLOAT3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f)));






}

void CParticleMgr::PopSparkParticle(const XMFLOAT3& pos)
{
	XMFLOAT3 rand3Vector = XMFLOAT3(rand_int() % 100 - 50, rand_int() % 50, rand_int() % 100 - 50);
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;

	// 파삭파삭
	for (int i = 0; i < 500; ++i)
	{
		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5, rand_int() % 10 - 5, rand_int() % 10 - 5);

		float speed = rand_int() % 500;
		float direction_factor = (i > 450) ? 0.1 : 0.05;
		particle_event.time = GetCurrTime();
		particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, speed*(1 + (rand_int() % 5)*0.1));
		particle_event.dir
			= Vector3::Add(
				rand3Vector,
				Vector3::ScalarProduct(
					Vector3::Normalize(
						XMFLOAT3(
							rand_int() % 100 - 50,
							rand_int() % 100,
							rand_int() % 100 - 50))
					, i*direction_factor + 0.5));
		particle_event.life = 12 + rand_int() % 6;;
		particle_event.option = XMFLOAT3(1.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = speed + rand_int() % 50;

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f+ sizemulti, 1.f + sizemulti);

		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		switch (rand_int() % 3)
		{
		case 0:
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
			break;
		case 1:
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.86f, 0.4f, 1.f);
			break;
		case 2:
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 0.f, 1.f);
			break;
		}

		
		m_pSparkBillboard->AddParticleEvent(particle_event);

	}

	// 스파크 원점 빛
	for (int i = 0; i < 12; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);

		particle_event.time = GetCurrTime() + i * 0.016*0.25;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(0, 0, 0);
		particle_event.life = 0.25;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 0;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		particle_event.SizeMultiplier = XMFLOAT2(1.f, 1.f);

		switch (rand_int() % 3)
		{
		case 0:
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
			break;
		case 1:
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.f);
			break;
		case 2:
			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);
			break;
		}

		m_pSparkIntenseCenterBillboard->AddParticleEvent(particle_event);


	}

}

void CParticleMgr::PopMuzzleFlashParticle(const XMFLOAT3& pos,const XMFLOAT4& color)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;

	for (int i = 0; i < 2; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 1 - 2,
				rand_int() % 1 - 2, 
				rand_int() % 1 - 2);

		particle_event.time = GetCurrTime() + i * 0.016;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(0, 0, 0);
		particle_event.life = 0.1;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 0;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		particle_event.SizeMultiplier = XMFLOAT2(1.f, 1.f);

		m_pWhiteBaseMuzzleBillboard->AddParticleEvent(particle_event);

	}

	for (int i = 0; i < 6; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 1 - 2, 
				rand_int() % 1 - 2, 
				rand_int() % 1 - 2);

		particle_event.time = GetCurrTime() + i * 0.016;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(0, 0, 0);
		particle_event.life = 0.1;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 0;
		particle_event.CustomColor = color;
		particle_event.SizeMultiplier = XMFLOAT2(1.f, 1.f);
		m_pColorMuzzleBillboard->AddParticleEvent(particle_event);

	}

}

void CParticleMgr::PopSmokeParticle(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 300; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.016;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360- 180, rand_int() % 360, rand_int() % 360 - 180);
		particle_event.life = 1;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1+rand_int()%100;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pSmokeBillboard->AddParticleEvent(particle_event);

	}

}

void CParticleMgr::PopBasicExplode(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 30; ++i)
	{
		int MainpivotPositionOffset = 200 + i * 7;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// m_pExplodeSmokeBillboard
		for (int j = 0; j < 10; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5,
					rand_int() % 10 - 5,
					rand_int() % 10 - 5);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*8.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSmokeBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeIntenseSmokeBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 1.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeIntenseSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeUpperCloudBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 2.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeUpperCloudBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeFireBlackSmokeBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 3.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 0.1f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeFireBlackSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeBackDraftCloudBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = Vector3::Subtract(Vector3::Add(Mainpivot, randPosOffsetVector), pos);
			particle_event.life = 0.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 30;
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeBackDraftCloudBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeSparkBillboard
		for (int j = 0; j < 215; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay + j * 0.1;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 30 * (1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, rand_int() % 60 - 30, rand_int() % 60 - 30);
			particle_event.life = 5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 60;
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSparkBillboard->AddParticleEvent(particle_event);

		}


	}
}

void CParticleMgr::PopRubyBoostTrail(const XMFLOAT3 & pos)
{

	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 1; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 100;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pRubySmokeBillboard->AddParticleEvent(particle_event);

	}

	for (int i = 0; i < 5; ++i)
	{
		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime();
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 0.032;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 2;
		particle_event.CustomColor = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.0525;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);
	}

}

void CParticleMgr::PopRubyBulletTrail(const XMFLOAT3 & pos)
{


	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 2; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 100;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pRubySmokeBillboard->AddParticleEvent(particle_event);

	}

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime();
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 0.032;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int()%2;
		particle_event.CustomColor = XMFLOAT4(0.35f, 0.35f, 0.225f, 0.55f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*2.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);

	
}

void CParticleMgr::PopRubySpecial(const XMFLOAT3 & pos)
{
}

void CParticleMgr::PopRubyBulletExplosion(const XMFLOAT3 & pos)
{

	CAdvancedParticleBillBoards::PopParticleEvent particle_event;

	
	for (int i = 0; i < 10; ++i)
	{
		int MainpivotPositionOffset = 50 + i * 50;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// 파삭파삭
		for (int j = 0; j < 50; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5, rand_int() % 10 - 5, rand_int() % 10 - 5);

			float speed = rand_int() % 500;
			particle_event.time = GetCurrTime()+ Maindelay;
			particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, speed*(1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);
			particle_event.life = 3 + rand_int() % 6;
			particle_event.option = XMFLOAT3(1.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = speed + rand_int() % 50;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*4.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);

			m_pSparkBillboard->AddParticleEvent(particle_event);

		}

		// 스파크 원점 빛
		for (int j = 0; j < 6; ++j)
		{

			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);

			particle_event.time = GetCurrTime() + j * 0.016*0.25 + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(0, 0, 0);
			particle_event.life = 0.25;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = 0;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.75;

			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);
			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);


			m_pSparkIntenseCenterBillboard->AddParticleEvent(particle_event);


		}

	}
	





}

void CParticleMgr::PopRubyExplode(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 30; ++i)
	{
		int MainpivotPositionOffset = 200 + i*7;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// m_pExplodeSmokeBillboard
		for (int j = 0; j < 10; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3( rand_int() % 10 - 5,
					rand_int() % 10 - 5,
					rand_int() % 10 - 5);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*8.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSmokeBillboard->AddParticleEvent(particle_event);

		}
		

		//m_pExplodeIntenseSmokeBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 1.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeIntenseSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeUpperCloudBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 2.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeUpperCloudBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeFireBlackSmokeBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 3.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 0.1f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeFireBlackSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeBackDraftCloudBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = Vector3::Subtract(Vector3::Add(Mainpivot, randPosOffsetVector), pos);
			particle_event.life = 0.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 30;
			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeBackDraftCloudBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeSparkBillboard
		for (int j = 0; j < 215; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay + j*0.1;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 30*(1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, rand_int() % 60 - 30, rand_int() % 60 - 30);
			particle_event.life = 5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 60;
			particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSparkBillboard->AddParticleEvent(particle_event);

		}


	}


}

void CParticleMgr::PopRubySpecialExplosion(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;

	for (int i = 0; i < 360; ++i)
	{




		particle_event.time = GetCurrTime() + i * 2.0;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = (i % 2 == 0) ?
			XMFLOAT3(cos(i*fPI / 180.f * 2) * 180, 0, sin(i*fPI / 180.f * 2) * 180) :
			XMFLOAT3(cos(i*fPI / 180.f) * 180, 0, sin(i*fPI / 180.f) * 180);
		particle_event.life = 3;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = pos;


		particle_event.speed = 1800;
		particle_event.CustomColor = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*2.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);


		m_pRubySmokeBillboard->AddParticleEvent(particle_event);

		XMFLOAT3 new_position = pos;
		new_position = Vector3::Add(new_position, Vector3::ScalarProduct(particle_event.dir, i*0.1));

		for (int j = 0; j < 50; ++j)
		{

			particle_event.dir = Vector3::Add(particle_event.dir, XMFLOAT3(rand() % 30 - 15, rand() % 60 - 30, rand() % 30 - 15));
			particle_event.speed = 30 + rand_int() % 500;
			particle_event.pos = new_position;
			float sizemulti = sin(rand_float())*12.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);
			m_pExplodeSparkBillboard->AddParticleEvent(particle_event);
			
		}

		if (i % 5 == 0)
		{
			particle_event.life = 1;
			particle_event.pos = new_position;
			float sizemulti = sin(rand_float())*1.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);
			m_pSparkIntenseCenterBillboard->AddParticleEvent(particle_event);
		}

	}






}

void CParticleMgr::PopLegionBoostTrail(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 1; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 100;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pLegionSmokeBillboard->AddParticleEvent(particle_event);

	}

	for (int i = 0; i < 5; ++i)
	{
		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime();
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 0.032;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 2;
		particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.0525;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);
	}
}

void CParticleMgr::PopLegionBulletTrail(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 2; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 100;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pLegionSmokeBillboard->AddParticleEvent(particle_event);

	}

	// 위치 살짝씩 뒤틀기용
	XMFLOAT3 randPosOffsetVector
		= XMFLOAT3(rand_int() % 10 - 5,
			rand_int() % 10 - 5,
			rand_int() % 10 - 5);

	particle_event.time = GetCurrTime();
	particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
	particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
	particle_event.life = 0.032;
	particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
	particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
	particle_event.speed = 1 + rand_int() % 2;
	particle_event.CustomColor = XMFLOAT4(0.225f, 0.35f, 0.225f, 0.55f);

	// 크기 배수 조정
	float sizemulti = sin(rand_float())*2.75;

	particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

	m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);
}

void CParticleMgr::PopLegionSpecial(const XMFLOAT3 & pos)
{
}

void CParticleMgr::PopLegionBulletExplosion(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;


	for (int i = 0; i < 10; ++i)
	{
		int MainpivotPositionOffset = 50 + i * 50;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// 파삭파삭
		for (int j = 0; j < 50; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5, rand_int() % 10 - 5, rand_int() % 10 - 5);

			float speed = rand_int() % 500;
			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, speed*(1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);
			particle_event.life = 3 + rand_int() % 6;
			particle_event.option = XMFLOAT3(1.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = speed + rand_int() % 50;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*4.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			m_pSparkBillboard->AddParticleEvent(particle_event);

		}

		// 스파크 원점 빛
		for (int j = 0; j < 6; ++j)
		{

			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);

			particle_event.time = GetCurrTime() + j * 0.016*0.25 + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(0, 0, 0);
			particle_event.life = 0.25;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = 0;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.75;

			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);
			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);


			m_pSparkIntenseCenterBillboard->AddParticleEvent(particle_event);


		}

	}

}

void CParticleMgr::PopLegionExplode(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 30; ++i)
	{
		int MainpivotPositionOffset = 200 + i * 7;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// m_pExplodeSmokeBillboard
		for (int j = 0; j < 10; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5,
					rand_int() % 10 - 5,
					rand_int() % 10 - 5);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*8.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSmokeBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeIntenseSmokeBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 1.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeIntenseSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeUpperCloudBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 2.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeUpperCloudBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeFireBlackSmokeBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 3.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 0.1f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeFireBlackSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeBackDraftCloudBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = Vector3::Subtract(Vector3::Add(Mainpivot, randPosOffsetVector), pos);
			particle_event.life = 0.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 30;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeBackDraftCloudBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeSparkBillboard
		for (int j = 0; j < 215; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay + j * 0.1;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 30 * (1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, rand_int() % 60 - 30, rand_int() % 60 - 30);
			particle_event.life = 5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 60;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSparkBillboard->AddParticleEvent(particle_event);

		}


	}
}

void CParticleMgr::PopLegionSpecialExplosion(const XMFLOAT3 & pos, const XMFLOAT3& attackerPos)
{

	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	XMFLOAT3 DirectionToPosition = Vector3::Normalize(Vector3::Subtract(attackerPos,pos));
	for (int i = 0; i < 60; ++i)
	{
		//int MainpivotPositionOffset = 200 + i * 7;
		float Maindelay = i * 5.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot = XMFLOAT3(pos.x,pos.y,pos.z);
		XMFLOAT3 DirectionNoize = Vector3::Normalize(
		XMFLOAT3(rand_int()%360-180, rand_int() % 360 - 180, rand_int() % 360 - 180));
		XMFLOAT3 NewDirection = Vector3::Add
		(
			DirectionToPosition,
		Vector3::ScalarProduct(DirectionNoize,i*0.05)
		);

		NewDirection = Vector3::Normalize(NewDirection);

		Mainpivot = Vector3::Add(attackerPos,
			Vector3::ScalarProduct(NewDirection,-250)
			);

		// m_pExplodeSmokeBillboard
		for (int j = 0; j < 10; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5,
					rand_int() % 10 - 5,
					rand_int() % 10 - 5);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.dir = Vector3::Add(particle_event.dir, NewDirection);
			
			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSmokeBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeIntenseSmokeBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.dir = Vector3::Add(particle_event.dir, NewDirection);

			particle_event.life = 1.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeIntenseSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeUpperCloudBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.dir = Vector3::Add(particle_event.dir, NewDirection);

			particle_event.life = 2.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeUpperCloudBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeFireBlackSmokeBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.dir = Vector3::Add(particle_event.dir, NewDirection);
			
			particle_event.life = 3.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 0.1f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.125;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeFireBlackSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeBackDraftCloudBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = Vector3::Subtract(Vector3::Add(Mainpivot, randPosOffsetVector), pos);
			particle_event.dir = Vector3::Add(particle_event.dir, NewDirection);
			
			particle_event.life = 0.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 30;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeBackDraftCloudBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeSparkBillboard
		for (int j = 0; j < 215; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay + j * 0.1;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 30 * (1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, rand_int() % 60 - 30, rand_int() % 60 - 30);
			particle_event.dir = Vector3::Add(particle_event.dir, NewDirection);
			
			particle_event.life = 5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 60;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSparkBillboard->AddParticleEvent(particle_event);

		}



		for (int j = 0; j < rand_int()%25; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 50 - 25,
					rand_int() % 50 - 25,
					rand_int() % 50 - 25);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
			particle_event.dir = Vector3::Subtract(Vector3::Add(Mainpivot, randPosOffsetVector), pos);
			particle_event.dir = Vector3::Add(particle_event.dir, NewDirection);

			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 2000;
			particle_event.CustomColor = XMFLOAT4(0.5f, 1.0f, 0.5f, ( 0.5 + sin(rand_float())*0.5 ) );

			// 크기 배수 조정
			float sizemulti = (0.5 + sin(rand_float())*0.5 )*8.0;

			particle_event.SizeMultiplier = XMFLOAT2(0.01f + sizemulti, 0.01f + sizemulti);

			m_pHexagonDefaultBillboard->AddParticleEvent(particle_event);
			
		}

		for (int j = 0; j < rand_int() % 25; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 50 - 25,
					rand_int() % 50 - 25,
					rand_int() % 50 - 25);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
			particle_event.dir = Vector3::Subtract(Vector3::Add(Mainpivot, randPosOffsetVector), pos);
			particle_event.dir = Vector3::Add(particle_event.dir, NewDirection);

			particle_event.life = 2;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 2000;
			particle_event.CustomColor = XMFLOAT4(0.025f, 0.05f, 0.025f, 0.01);

			// 크기 배수 조정
			float sizemulti = (0.5 + sin(rand_float())*0.5)*8.0;

			particle_event.SizeMultiplier = XMFLOAT2(0.01f + sizemulti, 0.01f + sizemulti);

			m_pHexagonAdditiveBillboard->AddParticleEvent(particle_event);
		}

	}










}

void CParticleMgr::PopEpsilonBoostTrail(const XMFLOAT3 & pos)
{

	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 1; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 10;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pEpsilonSmokeBillboard->AddParticleEvent(particle_event);

	}

	for (int i = 0; i < 5; ++i)
	{
		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime();
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 0.032;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 2;
		particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.0525;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);
	}

}

void CParticleMgr::PopEpsilonBulletTrail(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 2; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 10;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pEpsilonSmokeBillboard->AddParticleEvent(particle_event);

	}

	// 위치 살짝씩 뒤틀기용
	XMFLOAT3 randPosOffsetVector
		= XMFLOAT3(rand_int() % 10 - 5,
			rand_int() % 10 - 5,
			rand_int() % 10 - 5);

	particle_event.time = GetCurrTime();
	particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
	particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
	particle_event.life = 0.032;
	particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
	particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
	particle_event.speed = 1 + rand_int() % 2;
	particle_event.CustomColor = XMFLOAT4(0.225f, 0.225f, 0.45f, 0.55f);

	// 크기 배수 조정
	float sizemulti = sin(rand_float())*2.75;

	particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

	m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);
}

void CParticleMgr::PopEpsilonSpecial(const XMFLOAT3 & pos)
{
}

void CParticleMgr::PopEpsilonSpecialBulletTrail(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	XMFLOAT3 rand3Vector = XMFLOAT3(rand_int() % 100 - 50, rand_int() % 50, rand_int() % 100 - 50);

	for (int i = 0; i < 2; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 10;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pEpsilonSmokeBillboard->AddParticleEvent(particle_event);

	}




	// 파삭파삭
	for (int i = 0; i < 40; ++i)
	{
		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5, 
				rand_int() % 10 - 5, 
				rand_int() % 10 - 5);

		float speed = rand_int() % 50;
		particle_event.time = GetCurrTime();
		particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		particle_event.dir = randPosOffsetVector;
		particle_event.life = 3 + rand_int() % 2;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = speed + rand_int() % 50;

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*18.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		switch (rand_int() % 3)
		{
		case 0:
			particle_event.CustomColor = XMFLOAT4(0.7f, 0.7f, 1.f, 1.f);
			break;
		case 1:
			particle_event.CustomColor = XMFLOAT4(0.4f, 0.86f, 0.7f, 1.f);
			break;
		case 2:
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
			break;
		}


		m_pSparkBillboard->AddParticleEvent(particle_event);

	}



	for (int i = 0; i < 3; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime();
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 0.032;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 2;
		particle_event.CustomColor = XMFLOAT4(0.725f, 0.725f, 0.95f, 0.95f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*7.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);
	}
	
}

void CParticleMgr::PopEpsilonBulletExplosion(const XMFLOAT3 & pos)
{

	CAdvancedParticleBillBoards::PopParticleEvent particle_event;


	for (int i = 0; i < 10; ++i)
	{
		int MainpivotPositionOffset = 50 + i * 50;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// 파삭파삭
		for (int j = 0; j < 50; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5, rand_int() % 10 - 5, rand_int() % 10 - 5);

			float speed = rand_int() % 500;
			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, speed*(1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);
			particle_event.life = 3 + rand_int() % 6;
			particle_event.option = XMFLOAT3(1.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = speed + rand_int() % 50;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*4.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f);

			m_pSparkBillboard->AddParticleEvent(particle_event);

		}

		// 스파크 원점 빛
		for (int j = 0; j < 6; ++j)
		{

			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);

			particle_event.time = GetCurrTime() + j * 0.016*0.25 + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(0, 0, 0);
			particle_event.life = 0.25;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = 0;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.75;

			particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f);
			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);


			m_pSparkIntenseCenterBillboard->AddParticleEvent(particle_event);


		}

	}
}

void CParticleMgr::PopEpsilonExplode(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 30; ++i)
	{
		int MainpivotPositionOffset = 200 + i * 7;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// m_pExplodeSmokeBillboard
		for (int j = 0; j < 10; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5,
					rand_int() % 10 - 5,
					rand_int() % 10 - 5);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*8.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSmokeBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeIntenseSmokeBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 1.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeIntenseSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeUpperCloudBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 2.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeUpperCloudBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeFireBlackSmokeBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 3.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 0.1f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeFireBlackSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeBackDraftCloudBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = Vector3::Subtract(Vector3::Add(Mainpivot, randPosOffsetVector), pos);
			particle_event.life = 0.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 30;
			particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeBackDraftCloudBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeSparkBillboard
		for (int j = 0; j < 215; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay + j * 0.1;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 30 * (1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, rand_int() % 60 - 30, rand_int() % 60 - 30);
			particle_event.life = 5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 60;
			particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSparkBillboard->AddParticleEvent(particle_event);

		}


	}
}

void CParticleMgr::PopEpsilonSpecialExplosion(const XMFLOAT3 & pos)
{
	
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;

	for (int i = 0; i < 20; ++i)
	{
		int MainpivotPositionOffset = 200 + i * 50;
		float Maindelay = i * 20.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// 파삭파삭
		for (int j = 0; j < 500; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5, rand_int() % 10 - 5, rand_int() % 10 - 5);

			float speed = rand_int() % 500;
			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, speed*(1 + (rand_int() % 2)*0.1));
			particle_event.dir = XMFLOAT3(
				rand_int() % 100 - 50,
				rand_int() % 100 - 25,
				rand_int() % 100 - 50);
			particle_event.life = 12 + rand_int() % 6;;
			particle_event.option = XMFLOAT3(1.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = speed + rand_int() % 300;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*15.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
			switch (rand_int() % 3)
			{
			case 0:
				particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
				break;
			case 1:
				particle_event.CustomColor = XMFLOAT4(0.4f, 0.46f, 0.7f, 1.f);
				break;
			case 2:
				particle_event.CustomColor = XMFLOAT4(0.7f, 0.7f, 1.f, 1.f);
				break;
			}


			m_pSparkBillboard->AddParticleEvent(particle_event);

		}

		// 스파크 원점 빛
		for (int j = 0; j < 12; ++j)
		{

			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 350 - 175, rand_int() % 350 - 175, rand_int() % 350 - 175);

			particle_event.time = GetCurrTime() + j * 0.016*0.25 + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(0, 0, 0);
			particle_event.life = 0.25;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = 0;
			particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
			particle_event.SizeMultiplier = XMFLOAT2(3.f, 3.f);

			switch (rand_int() % 3)
			{
			case 0:
				particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
				break;
			case 1:
				particle_event.CustomColor = XMFLOAT4(0.2f, 0.2f, 1.0f, 1.f);
				break;
			case 2:
				particle_event.CustomColor = XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f);
				break;
			}

			m_pSparkIntenseCenterBillboard->AddParticleEvent(particle_event);
		}

	}


	

	
}

void CParticleMgr::PopGravisBoostTrail(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 1; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 100;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pGravisSmokeBillboard->AddParticleEvent(particle_event);

	}

	for (int i = 0; i < 5; ++i)
	{
		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime();
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 0.032;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 2;
		particle_event.CustomColor = XMFLOAT4(1.0f, 0.75f, 0.5f, 1.0f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.0525;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);
	}
	


}

void CParticleMgr::PopGravisBulletTrail(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 2; ++i)
	{

		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);

		particle_event.time = GetCurrTime() + i * 0.008;
		particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
		particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
		particle_event.life = 5;
		particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
		particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
		particle_event.speed = 1 + rand_int() % 100;
		particle_event.CustomColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

		// 크기 배수 조정
		float sizemulti = sin(rand_float())*0.75;

		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

		m_pGravisSmokeBillboard->AddParticleEvent(particle_event);

	}

	// 위치 살짝씩 뒤틀기용
	XMFLOAT3 randPosOffsetVector
		= XMFLOAT3(rand_int() % 10 - 5,
			rand_int() % 10 - 5,
			rand_int() % 10 - 5);

	particle_event.time = GetCurrTime();
	particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
	particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
	particle_event.life = 0.032;
	particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
	particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
	particle_event.speed = 1 + rand_int() % 2;
	particle_event.CustomColor = XMFLOAT4(0.35f, 0.35f, 0.0625f, 0.55f);

	// 크기 배수 조정
	float sizemulti = sin(rand_float())*2.75;

	particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

	m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);



}

void CParticleMgr::PopGravisSpecial(const XMFLOAT3 & pos)
{
}

void CParticleMgr::PopGravisBulletExplosion(const XMFLOAT3 & pos)
{

	CAdvancedParticleBillBoards::PopParticleEvent particle_event;


	for (int i = 0; i < 10; ++i)
	{
		int MainpivotPositionOffset = 50 + i * 50;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// 파삭파삭
		for (int j = 0; j < 50; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5, rand_int() % 10 - 5, rand_int() % 10 - 5);

			float speed = rand_int() % 500;
			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, 0.f, 0.f, speed*(1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);
			particle_event.life = 3 + rand_int() % 6;
			particle_event.option = XMFLOAT3(1.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = speed + rand_int() % 50;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*4.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			particle_event.CustomColor = XMFLOAT4(1.f, 0.75f, 0.5f, 1.f);

			m_pSparkBillboard->AddParticleEvent(particle_event);

		}

		// 스파크 원점 빛
		for (int j = 0; j < 6; ++j)
		{

			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50);

			particle_event.time = GetCurrTime() + j * 0.016*0.25 + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(0, 0, 0);
			particle_event.life = 0.25;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = 0;

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.75;

			particle_event.CustomColor = XMFLOAT4(1.f, 0.75f, 0.5f, 1.f);
			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);


			m_pSparkIntenseCenterBillboard->AddParticleEvent(particle_event);


		}

	}

}

void CParticleMgr::PopGravisExplode(const XMFLOAT3 & pos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	for (int i = 0; i < 30; ++i)
	{
		int MainpivotPositionOffset = 200 + i * 7;
		float Maindelay = i * 8.75;
		float BaseSpeed = 50;
		XMFLOAT3 Mainpivot =
			XMFLOAT3(
				pos.x + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.y + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5,
				pos.z + rand_int() % MainpivotPositionOffset - MainpivotPositionOffset * 0.5
			);

		// m_pExplodeSmokeBillboard
		for (int j = 0; j < 10; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 10 - 5,
					rand_int() % 10 - 5,
					rand_int() % 10 - 5);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.75f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*8.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSmokeBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeIntenseSmokeBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 360 - 180, rand_int() % 360 - 180, rand_int() % 360 - 180);
			particle_event.life = 1.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.75f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeIntenseSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeUpperCloudBillboard
		for (int j = 0; j < 5; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 2.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.75f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*1.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeUpperCloudBillboard->AddParticleEvent(particle_event);

		}


		//m_pExplodeFireBlackSmokeBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 20 - 10,
					rand_int() % 20 - 10,
					rand_int() % 20 - 10);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, 60, rand_int() % 60 - 30);
			particle_event.life = 3.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 10;
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.75f, 0.5f, 0.1f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.25;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeFireBlackSmokeBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeBackDraftCloudBillboard
		for (int j = 0; j < 15; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir = Vector3::Subtract(Vector3::Add(Mainpivot, randPosOffsetVector), pos);
			particle_event.life = 0.5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 30;
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.75f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*0.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeBackDraftCloudBillboard->AddParticleEvent(particle_event);

		}

		//m_pExplodeSparkBillboard
		for (int j = 0; j < 215; ++j)
		{
			// 위치 살짝씩 뒤틀기용
			XMFLOAT3 randPosOffsetVector
				= XMFLOAT3(rand_int() % 30 - 15,
					rand_int() % 30 - 15,
					rand_int() % 30 - 15);

			particle_event.time = GetCurrTime() + Maindelay + j * 0.1;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 30 * (1 + (rand_int() % 5)*0.1));
			particle_event.dir = XMFLOAT3(rand_int() % 60 - 30, rand_int() % 60 - 30, rand_int() % 60 - 30);
			particle_event.life = 5;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(Mainpivot, randPosOffsetVector);
			particle_event.speed = BaseSpeed + rand_int() % 60;
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.75f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*2.05;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSparkBillboard->AddParticleEvent(particle_event);

		}


	}
}

void CParticleMgr::PopGravisSpecialExplosion(const XMFLOAT3 & pos, const XMFLOAT3& attackerPos)
{
	CAdvancedParticleBillBoards::PopParticleEvent particle_event;
	XMFLOAT3 directionToAttacker = Vector3::Normalize( Vector3::Subtract(attackerPos, pos));
	XMFLOAT3 directionToPosition = Vector3::Normalize(Vector3::Subtract(pos, attackerPos));

	for (int i = 0; i < 10; ++i)
	{
		float Maindelay = i * 1;
		// 위치 살짝씩 뒤틀기용
		XMFLOAT3 randPosOffsetVector
			= XMFLOAT3(rand_int() % 10 - 5,
				rand_int() % 10 - 5,
				rand_int() % 10 - 5);
		



		XMFLOAT3 new_Direction =
			Vector3::Add(directionToPosition,
				Vector3::ScalarProduct(
					Vector3::Normalize(randPosOffsetVector)
					, 0.5));

		for (int j = 0; j < 200; ++j)
		{

			particle_event.time = GetCurrTime() + Maindelay + j;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir =
				Vector3::Add(new_Direction,
					Vector3::ScalarProduct(
						Vector3::Normalize(
							XMFLOAT3(
								rand_int() % 360 - 180,
								rand_int() % 360 - 180,
								rand_int() % 360 - 180))
						, 0.5)
				);

			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
			particle_event.speed = 200 + rand_int() % 2000;
			particle_event.CustomColor = XMFLOAT4(0.5f, 0.25f, 0.0f, 0.5f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*18.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);

			m_pExplodeSparkBillboard->AddParticleEvent(particle_event);



		}
			
		new_Direction =
			Vector3::Add(directionToAttacker,
				Vector3::ScalarProduct(
					Vector3::Normalize(randPosOffsetVector)
					, 0.5));

		for (int j = 0; j < 400; ++j)
		{

			particle_event.time = GetCurrTime() + Maindelay + j;
			particle_event.value = XMFLOAT4(0.f, rand_int() % 360, 0.f, 0.f);
			particle_event.dir =
				Vector3::Add(new_Direction,
					Vector3::ScalarProduct(
						Vector3::Normalize(
							XMFLOAT3(
								rand_int() % 360 - 180,
								rand_int() % 360 - 180, 
								rand_int() % 360 - 180))
						, 0.5)
					);
				
			particle_event.life = 3;
			particle_event.option = XMFLOAT3(0.f, 0.f, 0.f);
			particle_event.pos = Vector3::Add(pos, randPosOffsetVector);
			particle_event.speed = 200 + rand_int() % 1000;
			particle_event.CustomColor = XMFLOAT4(1.0f, 0.75f, 0.5f, 1.f);

			// 크기 배수 조정
			float sizemulti = sin(rand_float())*18.75;

			particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);
			
			m_pSparkBillboard->AddParticleEvent(particle_event);

			
			
		}

		
		
		float sizemulti = sin(rand_float())*1.75;
		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);
		particle_event.pos = Vector3::Add(particle_event.pos, XMFLOAT3(rand_int() % 100 - 50, rand_int() % 100 - 50, rand_int() % 100 - 50));
		particle_event.speed = 1000 + rand_int() % 100;
		m_pExplodeUpperCloudBillboard->AddParticleEvent(particle_event);

		particle_event.life = 1;
		sizemulti = sin(rand_float())*1.75;
		particle_event.SizeMultiplier = XMFLOAT2(1.f + sizemulti, 1.f + sizemulti);
		particle_event.speed = 1;
		m_pBulletTrailBlastBillboard->AddParticleEvent(particle_event);
		
		
	}
	






}

void CParticleMgr::Initialize(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCmdLst)
{
	m_pSparkBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleSpark
		, XMFLOAT2(1, 1)
		, 24120
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);

	m_pSparkIntenseCenterBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleSpark
		, XMFLOAT2(1, 1)
		, 64
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);

	m_pColorMuzzleBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleGunFlash
		, XMFLOAT2(1, 1)
		, 64
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);

	m_pWhiteBaseMuzzleBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleGunFlash
		, XMFLOAT2(1, 1)
		, 2048
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);


	m_pSmokeBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleSmoke
		, XMFLOAT2(1, 1)
		, 1024
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);

	m_pRubySmokeBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleSmoke
		, XMFLOAT2(1, 1)
		, 8192
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);

	m_pLegionSmokeBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleCloud_1
		, XMFLOAT2(1, 1)
		, 8192
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);

	m_pEpsilonSmokeBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleCloud_0
		, XMFLOAT2(1, 1)
		, 8192
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);

	m_pGravisSmokeBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleCloud_2
		, XMFLOAT2(1, 1)
		, 8192
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);
	m_pBulletTrailBlastBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleCrossSpark
		, XMFLOAT2(1, 1)
		, 48192
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);


	// Explode Ingrediant

	m_pExplodeSmokeBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleCloud_0
		, XMFLOAT2(1, 1)
		, 4096
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);
	m_pExplodeIntenseSmokeBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleCloud_3
		, XMFLOAT2(1, 1)
		, 4096
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);
	m_pExplodeUpperCloudBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleUpperCloud
		, XMFLOAT2(1, 1)
		, 4096
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);
	m_pExplodeFireBlackSmokeBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleUpperFireSmoke
		, XMFLOAT2(1, 1)
		, 4096
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);
	m_pExplodeBackDraftCloudBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleBackDraftSmoke
		, XMFLOAT2(1, 1)
		, 4096
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);

	m_pExplodeSparkBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleSpark
		, XMFLOAT2(1, 1)
		, 48192
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);

	m_pHexagonDefaultBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleHexagon
		, XMFLOAT2(1, 1)
		, 4096
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardDefault
		);
	m_pHexagonAdditiveBillboard = make_shared<CAdvancedParticleBillBoards>(
		pd3dDevice
		, pd3dCmdLst
		, eTex::Billboard::ParticleHexagon
		, XMFLOAT2(1, 1)
		, 4096
		, XMFLOAT2(1, 1)
		, ePSO::Graphic::ParticleBillboardAdditive
		);

	ParticlesParameterSettingInitialize();

}
