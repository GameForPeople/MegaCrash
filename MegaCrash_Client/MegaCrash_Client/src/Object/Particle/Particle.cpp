#pragma once
#include "stdafx.h"
#include "Particle.h"

void CParticleParamManager::ResetColoraAndAlphaAndSizeParam()
{
	ColorParamList.clear();
	AlphaParamList.clear();
	SizeParamList.clear();

	ColorParamList.emplace_back(ColorParam(ParticleMinParam, XMFLOAT3(1, 1, 1)));
	ColorParamList.emplace_back(ColorParam(ParticleMaxParam, XMFLOAT3(1, 1, 1)));

	AlphaParamList.emplace_back(AlphaParam(ParticleMinParam, 1));
	AlphaParamList.emplace_back(AlphaParam(ParticleMaxParam, 1));

	SizeParamList.emplace_back(SizeParam(ParticleMinParam, XMFLOAT2(1, 1)));
	SizeParamList.emplace_back(SizeParam(ParticleMaxParam, XMFLOAT2(1, 1)));


	std::sort(ColorParamList.begin(), ColorParamList.end(),
		[&](ColorParam a, ColorParam b) {return a.PinPos < b.PinPos; });
	std::sort(AlphaParamList.begin(), AlphaParamList.end(),
		[&](AlphaParam a, AlphaParam b) {return a.PinPos < b.PinPos; });
	std::sort(SizeParamList.begin(), SizeParamList.end(),
		[&](SizeParam a, SizeParam b) {return a.PinPos < b.PinPos; });
}

bool CParticleParamManager::AddColorParam(ColorParam new_param)
{
	auto p = std::find_if(ColorParamList.begin(), ColorParamList.end(),
		[&new_param](ColorParam a) {return a.PinPos == new_param.PinPos; });
	if (p != ColorParamList.end())
	{
		(*p).color = new_param.color;
		return true;
	}
	else
	{
		ColorParamList.emplace_back(new_param);
		std::sort(ColorParamList.begin(), ColorParamList.end(),
			[&](ColorParam a, ColorParam b) {return a.PinPos < b.PinPos; });
		return true;
	}

	return false;
}

bool CParticleParamManager::AddAlphaParam(AlphaParam new_param)
{
	auto p = std::find_if(AlphaParamList.begin(), AlphaParamList.end(),
		[&new_param](AlphaParam a) {return a.PinPos == new_param.PinPos; });
	if (p != AlphaParamList.end())
	{
		(*p).alpha = new_param.alpha;
		return true;
	}
	else
	{
		AlphaParamList.emplace_back(new_param);
		std::sort(AlphaParamList.begin(), AlphaParamList.end(),
			[&](AlphaParam a, AlphaParam b) {return a.PinPos < b.PinPos; });
		return true;
	}
	return false;
}

bool CParticleParamManager::AddSizeParam(SizeParam new_param)
{
	auto p = std::find_if(SizeParamList.begin(), SizeParamList.end(),
		[&new_param](SizeParam a) {return a.PinPos == new_param.PinPos; });
	if (p != SizeParamList.end())
	{
		(*p).size = new_param.size;
		return true;
	}
	else
	{
		SizeParamList.emplace_back(new_param);
		std::sort(SizeParamList.begin(), SizeParamList.end(),
			[&](SizeParam a, SizeParam b) {return a.PinPos < b.PinPos; });
		return true;
	}

	return false;
}

bool CParticleParamManager::RemoveColorParam(int PinPos)
{
	if (PinPos == 0)
	{
		ColorParamList.begin()->color = XMFLOAT3(0, 0, 0);
		return true;
	}
	else if (PinPos == 1)
	{
		(ColorParamList.end() - 1)->color = XMFLOAT3(0, 0, 0);
		return true;
	}
	auto p = std::find_if(ColorParamList.begin(), ColorParamList.end(),
		[&PinPos](ColorParam a) {return a.PinPos == PinPos; });
	if (p != ColorParamList.end())
	{
		ColorParamList.erase(p);
	}
	return true;
}

bool CParticleParamManager::RemoveAlphaParam(int PinPos)
{
	if (PinPos == 0)
	{
		AlphaParamList.begin()->alpha = 0;
		return true;
	}
	else if (PinPos == 1)
	{
		(AlphaParamList.end() - 1)->alpha = 0;
		return true;
	}
	auto p = std::find_if(AlphaParamList.begin(), AlphaParamList.end(),
		[&PinPos](AlphaParam a) {return a.PinPos == PinPos; });
	if (p != AlphaParamList.end())
	{
		AlphaParamList.erase(p);
	}

	return true;
}

bool CParticleParamManager::RemoveSizeParam(int PinPos)
{
	if (PinPos == 0)
	{
		SizeParamList.begin()->size = XMFLOAT2(0, 0);
		return true;
	}
	else if (PinPos == 1)
	{
		(SizeParamList.end() - 1)->size = XMFLOAT2(0, 0);
		return true;
	}
	auto p = std::find_if(SizeParamList.begin(), SizeParamList.end(),
		[&PinPos](SizeParam a) {return a.PinPos == PinPos; });
	if (p != SizeParamList.end())
	{
		SizeParamList.erase(p);
	}
	return true;
}

void CParticleParamManager::CalcColorAndSizeByElapsedTime()
{
	// 라이프 대비 핀의 위치
	int CurrPinPos = 0;
	if (m_Life != 0) CurrPinPos = static_cast<int>((m_currElapsedTime / m_Life)*ParticleMaxParam);

	std::vector<ColorParam>::iterator Colorpin = ColorParamList.begin();
	std::vector<AlphaParam>::iterator Alphapin = AlphaParamList.begin();
	std::vector<SizeParam>::iterator Sizepin = SizeParamList.begin();


	int temp = ParticleMaxParam;
	for (auto p = ColorParamList.begin(); p != ColorParamList.end(); p++)
	{
		// 현재 핀의 위치와 비교할 핀의 위치의 차이값.
		int temp2 = CurrPinPos - (*p).PinPos;
		if (temp2 >= 0 && (temp >= temp2))
		{
			// 현재 임시로 저장한 값보다 지금 비교하고 있는 핀의 거리차이 값이 더 좁으면 대체한다.
			Colorpin = p;
			temp = temp2;
		}
	}

	temp = ParticleMaxParam;
	for (auto p = AlphaParamList.begin(); p != AlphaParamList.end(); p++)
	{
		// 현재 핀의 위치와 비교할 핀의 위치의 차이값.
		int temp2 = CurrPinPos - (*p).PinPos;
		if (temp2 >= 0 && (temp >= temp2))
		{
			// 현재 임시로 저장한 값보다 지금 비교하고 있는 핀의 거리차이 값이 더 좁으면 대체한다.
			Alphapin = p;
			temp = temp2;
		}
	}

	temp = ParticleMaxParam;
	for (auto p = SizeParamList.begin(); p != SizeParamList.end(); p++)
	{
		// 현재 핀의 위치와 비교할 핀의 위치의 차이값.
		int temp2 = CurrPinPos - (*p).PinPos;
		if (temp2 >= 0 && (temp >= temp2))
		{
			// 현재 임시로 저장한 값보다 지금 비교하고 있는 핀의 거리차이 값이 더 좁으면 대체한다.
			Sizepin = p;
			temp = temp2;
		}
	}

	// 두 핀간 색상 가중치 계산
	float delta = 0;
	// 분자
	int numerator = CurrPinPos - (*Colorpin).PinPos;
	// 분모
	int denominator = (*(Colorpin + 1)).PinPos - (*Colorpin).PinPos;
	if (denominator != 0)
		delta = (float)numerator / (float)denominator;

	//v0 + t * (v1 - v0);
	XMFLOAT3 new_color = Vector3::Lerp((*Colorpin).color, (*(Colorpin + 1)).color, delta);


	// 두 핀간 색상 가중치 계산
	delta = 0;
	// 분자
	numerator = CurrPinPos - (*Alphapin).PinPos;
	// 분모
	denominator = (*(Alphapin + 1)).PinPos - (*Alphapin).PinPos;
	if (denominator != 0)
		delta = (float)numerator / (float)denominator;

	//v0 + t * (v1 - v0);
	float alpha = (*Alphapin).alpha + delta * ((*(Alphapin + 1)).alpha - (*Alphapin).alpha);

	m_xmf4Color = XMFLOAT4(new_color.x, new_color.y, new_color.z, alpha);



	// 두 핀간 색상 가중치 계산
	delta = 0;
	// 분자
	numerator = CurrPinPos - (*Sizepin).PinPos;
	// 분모
	denominator = (*(Sizepin + 1)).PinPos - (*Sizepin).PinPos;
	if (denominator != 0)
		delta = (float)numerator / (float)denominator;

	//v0 + t * (v1 - v0);
	m_xmf2Size = Vector2::Lerp((*Sizepin).size, (*(Sizepin + 1)).size, delta);

	return;
}

void CParticleParamManager::Animate(float fTimeElapsed)
{
	if (!m_Activate) return;

	m_currElapsedTime += fTimeElapsed;
	if (m_currElapsedTime > m_Life)
	{
		m_Activate = false;
		return;
	}
	//색상 계산
	CalcColorAndSizeByElapsedTime();
	//std::cout << m_xmf4Color.x << "," << m_xmf4Color.y << "," << m_xmf4Color.z << "," << m_xmf4Color.w << std::endl;

}

void CParticleParamManager::ResetAllParameter()
{
	ResetColoraAndAlphaAndSizeParam();
	m_currElapsedTime = 0.0f;
	m_Life = 0.f;
	m_Activate = false;
}

void CParticleParamManager::ResetTimeAndLife()
{
	m_currElapsedTime = 0.0f;
	m_Life = 0.f;
	m_Activate = false;
}

void CParticleParamManager::Activate()
{
	m_currElapsedTime = 0.0f;
	m_Activate = true;
}

void CParticleParamManager::PrintData()
{
	std::cout << ":: Color :: " << std::endl;
	for (auto& p : ColorParamList)
	{
		std::cout << p;
	}
	std::cout << ":: Alpha :: " << std::endl;
	for (auto& p : AlphaParamList)
	{
		std::cout << p;
	}
	std::cout << ":: Size :: " << std::endl;
	for (auto& p : SizeParamList)
	{
		std::cout << p;
	}
}

std::ostream & operator<<(std::ostream & os, const CParticleParamManager::ColorParam & p)
{
	std::cout << "Color Pos::" << p.PinPos << "|" << p.color.x << "," << p.color.y << "," << p.color.z << std::endl;
	return os;
}

std::ostream & operator<<(std::ostream & os, const CParticleParamManager::AlphaParam & p)
{
	std::cout << "Color Pos::" << p.PinPos << "|" << p.alpha << std::endl;
	return os;
}

std::ostream & operator<<(std::ostream & os, const CParticleParamManager::SizeParam & p)
{
	std::cout << "Size Pos::" << p.PinPos << "|" << p.size.x << "," << p.size.y << std::endl;
	return os;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CParticleBillboards::CParticleBillboards(
	ID3D12Device*					pd3dDevice
	, ID3D12GraphicsCommandList*	pd3dCmdLst
	, eTex::Billboard				eTexture
	, XMFLOAT2						szSprite
	, UINT							nBillboards
	, XMFLOAT2						xmf2Size)
	: m_iCurrControler(0)
{
	shared_ptr<CParticleBillBoardMesh> billboards = make_shared<CParticleBillBoardMesh>(
		pd3dCmdLst, nBillboards, xmf2Size);

	m_nControlers = billboards->GetnVertices();
	m_ppControlers = new ParticleControler*[m_nControlers];
	for (int i = 0; i < m_nControlers; ++i)
	{
		m_ppControlers[i] = new ParticleControler(billboards->GetVertices()[i], szSprite);
	}

	SetMesh(0, billboards);
	CreateShaderVariables();

	CResMgr* pRes = CResMgr::Instance();
	shared_ptr<CTexture> pTexture = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	pTexture->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
	pTexture->SetTexture(pd3dDevice, 0, 0, pRes->GetTexture(eTexture));

	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>();
	pMaterial->SetTexture(pTexture);
	pMaterial->SetGlobalMaterialIdx(0);
	pMaterial->SetShader(pRes->GetShader(ePSO::Graphic::Billboard));
	SetMaterial(pMaterial);
}

CParticleBillboards::~CParticleBillboards()
{
	for (int i = 0; i < m_nControlers; ++i)
		delete m_ppControlers[i];
	delete[] m_ppControlers;
}

void CParticleBillboards::Animate(float fTimeElapsed)
{
	for (int i = 0; i < m_nControlers; ++i)
		m_ppControlers[i]->Animate(fTimeElapsed);
}

void CParticleBillboards::PopParticle(
	const XMFLOAT3&		pos
	, const XMFLOAT2&		coord
	, XMFLOAT3&				dir
	, float					speed
	, float					animateTime
	, float					animateSpeed)
{
	m_ppControlers[m_iCurrControler]->SetTexCoord(coord);
	m_ppControlers[m_iCurrControler++]->StartAnimate(pos, dir, speed, animateTime, animateSpeed);
	if (m_iCurrControler >= m_nControlers) m_iCurrControler = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CAdvancedParticleBillBoards::CAdvancedParticleBillBoards(ID3D12Device * pd3dDevice, ID3D12GraphicsCommandList * pd3dCmdLst, eTex::Billboard eTexture,
	XMFLOAT2 szSprite, UINT nBillboards, XMFLOAT2 xmf2Size, ePSO::Graphic PSO)
	: m_iCurrControler(0)
{
	shared_ptr<CAdvancedParticleBillBoardMesh> billboards = make_shared<CAdvancedParticleBillBoardMesh>(
		pd3dCmdLst, nBillboards, xmf2Size);

	m_nControlers = billboards->GetnVertices();
	m_ppControlers = new AdvancedParticleControler*[m_nControlers];
	for (int i = 0; i < m_nControlers; ++i)
	{
		m_ppControlers[i] = new AdvancedParticleControler
		(billboards->GetVertices()[i]);
	}

	SetMesh(0, billboards);
	CreateShaderVariables();

	CResMgr* pRes = CResMgr::Instance();
	shared_ptr<CTexture> pTexture = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
	pTexture->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
	pTexture->SetTexture(pd3dDevice, 0, 0, pRes->GetTexture(eTexture));

	shared_ptr<CMaterial> pMaterial = make_shared<CMaterial>();
	pMaterial->SetTexture(pTexture);
	pMaterial->SetGlobalMaterialIdx(0);
	pMaterial->SetShader(pRes->GetShader(PSO));
	SetMaterial(pMaterial);
}

CAdvancedParticleBillBoards::~CAdvancedParticleBillBoards()
{
	for (int i = 0; i < m_nControlers; ++i)
		delete m_ppControlers[i];
	delete[] m_ppControlers;
}

void CAdvancedParticleBillBoards::AddColorParam(CParticleParamManager::ColorParam colorParam)
{
	for (int i = 0; i < m_nControlers; ++i)
		m_ppControlers[i]->AddColorParam(colorParam);
}

void CAdvancedParticleBillBoards::AddAlphaParam(CParticleParamManager::AlphaParam alphaParam)
{
	for (int i = 0; i < m_nControlers; ++i)
		m_ppControlers[i]->AddAlphaParam(alphaParam);
}

void CAdvancedParticleBillBoards::AddSizeParam(CParticleParamManager::SizeParam sizeParam)
{
	for (int i = 0; i < m_nControlers; ++i)
		m_ppControlers[i]->AddSizeParam(sizeParam);
}



void CAdvancedParticleBillBoards::Animate(float fTimeElapsed)
{

	for (int i = 0; i < m_nControlers; ++i)
		m_ppControlers[i]->Animate(fTimeElapsed);

	while (1)
	{

		if (m_particleEventQueue.empty())
		{
			break;
		}
		PopParticleEvent* new_event = m_particleEventQueue.top();
		m_particleEventQueue.pop();

		if (new_event->time > GetCurrTime())
			break;


		m_ppControlers[m_iCurrControler++]
			->StartAnimate(
				new_event->pos, new_event->dir,
				new_event->speed, new_event->life,
				new_event->option, new_event->value,
				new_event->CustomColor, new_event->SizeMultiplier);

		if (m_iCurrControler >= m_nControlers) m_iCurrControler = 0;


		delete new_event;
	}


}

void CAdvancedParticleBillBoards::AddParticleEvent(PopParticleEvent& _new_event)
{
	PopParticleEvent* new_event = new PopParticleEvent(_new_event);
	m_particleEventQueue.push(new_event);
}
