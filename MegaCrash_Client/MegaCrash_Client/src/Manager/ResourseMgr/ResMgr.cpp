#include "stdafx.h"
#include "ResMgr.h"

CResMgr::CResMgr()
	: m_bMsaa4xEnable			{ false }
	, m_nMsaa4xQualityLevels	{ 0 }
{
}
CResMgr * const CResMgr::Instance()
{
	static CResMgr grm;
	return &grm;
}
CResMgr::~CResMgr()
{
	Release();
}

void CResMgr::ConnectDevice(const ComPtr<ID3D12Device>& pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;

	m_pd3dDevice->CheckFeatureSupport(
		  D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS
		, &d3dMsaaQualityLevels
		, sizeof(d3dMsaaQualityLevels));

	m_nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;
}

void CResMgr::LoadResources(ID3D12GraphicsCommandList* pd3dCmdLst)
{
	cout << "start load resources\n";
	auto pd3dDevice{ m_pd3dDevice.Get() };
	std::thread th{ [&]() {
		// ShadowMap
		{
			m_pShadowMap = make_shared<CShadowMap>(pd3dDevice, pd3dCmdLst);
			m_pShadowMap->CreateShaderVariables(pd3dDevice, pd3dCmdLst);
		}
		// SkyBox
		{
			wstring base_path{ L"../../../Assets/Textures/SkyBox/" };
			auto SetSkyBox = [&](eTex::SkyBox type, LPCWSTR filename) { 
				m_SkyBoxtexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
			};
			//SetSkyBox(eTex::SkyBox::LF, L"SkyBox_Left_0.dds");
			//SetSkyBox(eTex::SkyBox::RT, L"SkyBox_Right_0.dds"); 
			//SetSkyBox(eTex::SkyBox::FT, L"SkyBox_Front_0.dds");
			//SetSkyBox(eTex::SkyBox::BK, L"SkyBox_Back_0.dds");
			//SetSkyBox(eTex::SkyBox::UP, L"SkyBox_Top_0.dds");
			//SetSkyBox(eTex::SkyBox::DN, L"SkyBox_Bottom_0.dds");

			//SetSkyBox(eTex::SkyBox::StormyDays, L"Miramar/Miramar.dds");
			SetSkyBox(eTex::SkyBox::StormyDays, L"StormyDays/stormydays.dds");
			//SetSkyBox(eTex::SkyBox::StormyDays, L"ViolentDays/ViolentDays.dds");
		}
		// Diffuse
		{
			wstring base_path{ L"../../../Assets/3DModel/Final_Resources/Texture/" };
			auto SetDiffuse = [&](eTex::Diffuse type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_Diffusetexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_Diffusetexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetDiffuse(eTex::Diffuse::Stage_ground, L"../../../Assets/3DModel/Final_Resources/WorldChunk/test/9451-diffuse.DDS", false);
			SetDiffuse(eTex::Diffuse::Stage_marble, L"../../../Assets/3DModel/Final_Resources/WorldChunk/test/marble.DDS", false);
			SetDiffuse(eTex::Diffuse::Stage_brick, L"../../../Assets/3DModel/Final_Resources/WorldChunk/test/Brick.DDS", false);
			SetDiffuse(eTex::Diffuse::Stage_Rock, L"../../../Assets/3DModel/Final_Resources/WorldChunk/test/RockDiffuse.DDS", false);
			SetDiffuse(eTex::Diffuse::Stage_CheckerBoard, L"World/Outpost/CheckerBoard_Diffuse.DDS");
			SetDiffuse(eTex::Diffuse::Stage_Cell, L"World/Outpost/Cell_Diffuse.DDS");
			SetDiffuse(eTex::Diffuse::Stage_BlackerTile, L"World/Outpost/BlackerTile_Diffuse.DDS");
			SetDiffuse(eTex::Diffuse::Ruby_Body, L"Ruby/Ruby_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Ruby_Hammer, L"Ruby/Hammer/Hammer_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Ruby_Bullet, L"Ruby/Projectile/HammerMissile_Diffuse.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Spine, L"Legion/Spine/Spine_Diffuses.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Shoulder, L"Legion/Shoulder/Shoulder_Diffuses.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Mid, L"Legion/mid/Mid_Diffuses.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Leg, L"Legion/Leg/Leg_Diffuses.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Booster, L"Legion/Booster/Booster_Diffuses.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Arm, L"Legion/Arm/Arm_Diffuses.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Head, L"Legion/head/Head_Diffuses.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Spear, L"Legion/Spear/Spear_Diffuse.DDS");
			SetDiffuse(eTex::Diffuse::Legion_Shield, L"Legion/Shield/Shield_Diffuse.DDS");
			SetDiffuse(eTex::Diffuse::Epsilon_Arm, L"Epsilon/Arm/Arm_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Epsilon_Foot, L"Epsilon/Foot/Foot_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Epsilon_Head, L"Epsilon/Head/Head_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Epsilon_Leg, L"Epsilon/Leg/Leg_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Epsilon_Pelvis, L"Epsilon/PelvisFlight/PelvisFlight_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Epsilon_Upperbody, L"Epsilon/UpperBody/UpperBody_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Epsilon_Weapon, L"Epsilon/Weapon/Epsilon_Rifle_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Gravis_Upperbody, L"Gravis/Upper_Body/Upper_Body_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Gravis_Lowerbody, L"Gravis/Lower_Body/Lower_Body_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Gravis_Head, L"Gravis/head/head_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Gravis_Leg, L"Gravis/Leg/Leg_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Gravis_Arm, L"Gravis/Arm/Arm_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Gravis_Flight, L"Gravis/Flight/Flight_Base_Color.DDS");
			SetDiffuse(eTex::Diffuse::Gravis_Weapon, L"Gravis/Weapon/Weapon_Base_Color.DDS");
		}
		// Normal
		{
			wstring base_path{ L"../../../Assets/3DModel/Final_Resources/Texture/" };
			auto SetNormal = [&](eTex::Normal type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_Normaltexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_Normaltexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetNormal(eTex::Normal::Stage_ground, L"../../../Assets/3DModel/Final_Resources/WorldChunk/test/9451-normal.DDS", false);
			SetNormal(eTex::Normal::Stage_marble, L"../../../Assets/3DModel/Final_Resources/WorldChunk/test/marble_n.DDS", false);
			SetNormal(eTex::Normal::Stage_brick, L"../../../Assets/3DModel/Final_Resources/WorldChunk/test/Brick Normal.DDS", false);
			SetNormal(eTex::Normal::Stage_Rock, L"../../../Assets/3DModel/Final_Resources/WorldChunk/test/RockNormal.DDS", false);
			SetNormal(eTex::Normal::Stage_CheckerBoard, L"World/Outpost/CheckerBoard_Normal.DDS");
			SetNormal(eTex::Normal::Stage_Cell, L"World/Outpost/Cell_Normal.DDS");
			SetNormal(eTex::Normal::Stage_BlackerTile, L"World/Outpost/BlackerTile_Normal.DDS");
			SetNormal(eTex::Normal::Ruby_Body, L"Ruby/Ruby_Normal.DDS");
			SetNormal(eTex::Normal::Ruby_Hammer, L"Ruby/Hammer/Hammer_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Ruby_Bullet, L"Ruby/Projectile/HammerMissile_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Spine, L"Legion/Spine/Spine_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Shoulder, L"Legion/Shoulder/Shoulder_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Mid, L"Legion/mid/Mid_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Leg, L"Legion/Leg/Leg_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Booster, L"Legion/Booster/Booster_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Arm, L"Legion/Arm/Arm_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Head, L"Legion/head/Head_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Spear, L"Legion/Spear/Spear_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Legion_Shield, L"Legion/Shield/Shield_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Epsilon_Arm, L"Epsilon/Arm/Arm_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Epsilon_Foot, L"Epsilon/Foot/Foot_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Epsilon_Head, L"Epsilon/Head/Head_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Epsilon_Leg, L"Epsilon/Leg/Leg_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Epsilon_Pelvis, L"Epsilon/PelvisFlight/PelvisFlight_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Epsilon_Upperbody, L"Epsilon/UpperBody/UpperBody_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Epsilon_Weapon, L"Epsilon/Weapon/Epsilon_Rifle_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Gravis_Upperbody, L"Gravis/Upper_Body/Upper_Body_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Gravis_Lowerbody, L"Gravis/Lower_Body/Lower_Body_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Gravis_Head, L"Gravis/head/head_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Gravis_Leg, L"Gravis/Leg/Leg_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Gravis_Arm, L"Gravis/Arm/Arm_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Gravis_Flight, L"Gravis/Flight/Flight_Normal_DirectX.DDS");
			SetNormal(eTex::Normal::Gravis_Weapon, L"Gravis/Weapon/Weapon_Normal_DirectX.DDS");
		}
		// Specular
		{
			wstring base_path{ L"../../../Assets/3DModel/Final_Resources/Texture/" };
			auto SetSpecular = [&](eTex::Specular type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_Speculartexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_Speculartexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetSpecular(eTex::Specular::Stage_CheckerBoard_Red, L"World/Outpost/CheckerBoard_Specular_Red.DDS");
			SetSpecular(eTex::Specular::Stage_CheckerBoard_Blue, L"World/Outpost/CheckerBoard_Specular_Blue.DDS");
			SetSpecular(eTex::Specular::Stage_Cell, L"World/Outpost/Cell_Specular.DDS");
			SetSpecular(eTex::Specular::Stage_BlackerTile, L"World/Outpost/BlackerTile_Specular.DDS");
			SetSpecular(eTex::Specular::Ruby_Body, L"Ruby/Ruby_Specular.DDS");
			SetSpecular(eTex::Specular::Ruby_Hammer, L"Ruby/Hammer/Hammer_Specular.DDS");
			SetSpecular(eTex::Specular::Ruby_Bullet, L"Ruby/Projectile/HammerMissile_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Spine, L"Legion/Spine/Spine_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Shoulder, L"Legion/Shoulder/Shoulder_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Mid, L"Legion/mid/Mid_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Leg, L"Legion/Leg/Leg_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Booster, L"Legion/Booster/Booster_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Arm, L"Legion/Arm/Arm_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Head, L"Legion/head/Head_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Spear, L"Legion/Spear/Spear_Specular.DDS");
			SetSpecular(eTex::Specular::Legion_Shield, L"Legion/Shield/Shield_Specular.DDS");
			SetSpecular(eTex::Specular::Epsilon_Arm, L"Epsilon/Arm/Arm_Specular.DDS");
			SetSpecular(eTex::Specular::Epsilon_Foot, L"Epsilon/Foot/Foot_Specular.DDS");
			SetSpecular(eTex::Specular::Epsilon_Head, L"Epsilon/Head/Head_Specular.DDS");
			SetSpecular(eTex::Specular::Epsilon_Leg, L"Epsilon/Leg/Leg_Specular.DDS");
			SetSpecular(eTex::Specular::Epsilon_Pelvis, L"Epsilon/PelvisFlight/PelvisFlight_Specular.DDS");
			SetSpecular(eTex::Specular::Epsilon_Upperbody, L"Epsilon/UpperBody/UpperBody_Specular.DDS");
			SetSpecular(eTex::Specular::Epsilon_Weapon, L"Epsilon/Weapon/Epsilon_Rifle_Specular.DDS");
			SetSpecular(eTex::Specular::Gravis_Upperbody, L"Gravis/Upper_Body/Upper_Body_Specular.DDS");
			SetSpecular(eTex::Specular::Gravis_Lowerbody, L"Gravis/Lower_Body/Lower_Body_Specular.DDS");
			SetSpecular(eTex::Specular::Gravis_Head, L"Gravis/head/head_Specular.DDS");
			SetSpecular(eTex::Specular::Gravis_Leg, L"Gravis/Leg/Leg_Specular.DDS");
			SetSpecular(eTex::Specular::Gravis_Arm, L"Gravis/Arm/Arm_Specular.DDS");
			SetSpecular(eTex::Specular::Gravis_Flight, L"Gravis/Flight/Flight_Specular.DDS");
			SetSpecular(eTex::Specular::Gravis_Weapon, L"Gravis/Weapon/Weapon_Specular.DDS");
		}
		// Glossiness
		{
			wstring base_path{ L"../../../Assets/3DModel/Final_Resources/Texture/" };
			auto SetGlossiness = [&](eTex::Glossiness type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_Glossinesstexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_Glossinesstexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetGlossiness(eTex::Glossiness::Stage_CheckerBoard, L"World/Outpost/CheckerBoard_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Stage_Cell, L"World/Outpost/Cell_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Stage_BlackerTile, L"World/Outpost/BlackerTile_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Ruby_Body, L"Ruby/Ruby_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Ruby_Hammer, L"Ruby/Hammer/Hammer_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Ruby_Bullet, L"Ruby/Projectile/HammerMissile_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Spine, L"Legion/Spine/Spine_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Shoulder, L"Legion/Shoulder/Shoulder_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Mid, L"Legion/mid/Mid_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Leg, L"Legion/Leg/Leg_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Booster, L"Legion/Booster/Booster_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Arm, L"Legion/Arm/Arm_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Head, L"Legion/head/Head_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Spear, L"Legion/Spear/Spear_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Legion_Shield, L"Legion/Shield/Shield_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Epsilon_Arm, L"Epsilon/Arm/Arm_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Epsilon_Foot, L"Epsilon/Foot/Foot_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Epsilon_Head, L"Epsilon/Head/Head_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Epsilon_Leg, L"Epsilon/Leg/Leg_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Epsilon_Pelvis, L"Epsilon/PelvisFlight/PelvisFlight_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Epsilon_Upperbody, L"Epsilon/UpperBody/UpperBody_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Epsilon_Weapon, L"Epsilon/Weapon/Epsilon_Rifle_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Gravis_Upperbody, L"Gravis/Upper_Body/Upper_Body_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Gravis_Lowerbody, L"Gravis/Lower_Body/Lower_Body_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Gravis_Head, L"Gravis/head/head_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Gravis_Leg, L"Gravis/Leg/Leg_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Gravis_Arm, L"Gravis/Arm/Arm_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Gravis_Flight, L"Gravis/Flight/Flight_Glossiness.DDS");
			SetGlossiness(eTex::Glossiness::Gravis_Weapon, L"Gravis/Weapon/Weapon_Glossiness.DDS");
		}
		// Emissive
		{
			wstring base_path{ L"../../../Assets/3DModel/Final_Resources/Texture/" };
			auto SetEmissive = [&](eTex::Emissive type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_Emissivetexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_Emissivetexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetEmissive(eTex::Emissive::Stage_Circuit, L"World/Outpost/circuit.DDS");
			SetEmissive(eTex::Emissive::Ruby_Body, L"Ruby/Ruby_Emissive.DDS");
			SetEmissive(eTex::Emissive::Ruby_Hammer, L"Ruby/Hammer/Hammer_Emissive.DDS");
			SetEmissive(eTex::Emissive::Ruby_Bullet, L"Ruby/Projectile/HammerMissile_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Spine, L"Legion/Spine/Spine_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Shoulder, L"Legion/Shoulder/Shoulder_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Mid, L"Legion/mid/Mid_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Leg, L"Legion/Leg/Leg_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Booster, L"Legion/Booster/Booster_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Arm, L"Legion/Arm/Arm_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Head, L"Legion/head/Head_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Spear, L"Legion/Spear/Spear_Emissive.DDS");
			SetEmissive(eTex::Emissive::Legion_Shield, L"Legion/Shield/Shield_Emissive.DDS");
			SetEmissive(eTex::Emissive::Epsilon_Arm, L"Epsilon/Arm/Arm_Emissive.DDS");
			SetEmissive(eTex::Emissive::Epsilon_Foot, L"Epsilon/Foot/Foot_Emissive.DDS");
			SetEmissive(eTex::Emissive::Epsilon_Head, L"Epsilon/Head/Head_Emissive.DDS");
			SetEmissive(eTex::Emissive::Epsilon_Leg, L"Epsilon/Leg/Leg_Emissive.DDS");
			SetEmissive(eTex::Emissive::Epsilon_Pelvis, L"Epsilon/PelvisFlight/PelvisFlight_Emissive.DDS");
			SetEmissive(eTex::Emissive::Epsilon_Upperbody, L"Epsilon/UpperBody/UpperBody_Emissive.DDS");
			SetEmissive(eTex::Emissive::Epsilon_Weapon, L"Epsilon/Weapon/Epsilon_Rifle_Emissive.DDS");
			SetEmissive(eTex::Emissive::Gravis_Upperbody, L"Gravis/Upper_Body/Upper_Body_Emissive.DDS");
			SetEmissive(eTex::Emissive::Gravis_Lowerbody, L"Gravis/Lower_Body/Lower_Body_Emissive.DDS");
			SetEmissive(eTex::Emissive::Gravis_Head, L"Gravis/head/head_Emissive.DDS");
			SetEmissive(eTex::Emissive::Gravis_Leg, L"Gravis/Leg/Leg_Emissive.DDS");
			SetEmissive(eTex::Emissive::Gravis_Arm, L"Gravis/Arm/Arm_Emissive.DDS");
			SetEmissive(eTex::Emissive::Gravis_Flight, L"Gravis/Flight/Flight_Emissive.DDS");
			SetEmissive(eTex::Emissive::Gravis_Weapon, L"Gravis/Weapon/Weapon_Emissive.DDS");
		}
		// Billboard
		{
			wstring base_path{ L"../../../Assets/Textures/" };
			auto SetBillboard = [&](eTex::Billboard type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_Billboardtexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_Billboardtexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetBillboard(eTex::Billboard::Digit, L"Font/digit_font.DDS");
			SetBillboard(eTex::Billboard::DustParticle, L"Effect/dust_particle.DDS");
			SetBillboard(eTex::Billboard::PlayerUI, L"Interface/StatusInterface.DDS");
			SetBillboard(eTex::Billboard::HUD_UI, L"Interface/Hullhud.DDS");
			SetBillboard(eTex::Billboard::RADER_UI, L"Interface/Rader.DDS");
			SetBillboard(eTex::Billboard::ShaderHelper, L"Interface/Hullhud.DDS");
			SetBillboard(eTex::Billboard::GreySquare, L"Interface/TransParentSquare.DDS");
			SetBillboard(eTex::Billboard::Logo, L"Interface/WARP_LOGO.DDS");
			SetBillboard(eTex::Billboard::Title, L"Interface/logo.DDS");
			SetBillboard(eTex::Billboard::LobbyBaseFrame, L"Interface/BaseFrame.DDS");
			SetBillboard(eTex::Billboard::ParticleEllipse, L"Effect/Ellipse.DDS");
			SetBillboard(eTex::Billboard::Particle45Ellipse, L"Effect/45Ellipse.DDS");
			SetBillboard(eTex::Billboard::ParticleSmoke, L"Effect/Smoke.DDS");
			SetBillboard(eTex::Billboard::ParticleSpark, L"Effect/Spark.DDS");
			SetBillboard(eTex::Billboard::ParticleGunFlash, L"Effect/gunflash.DDS");
			SetBillboard(eTex::Billboard::ParticleCloud_0, L"Effect/Cloud_0.DDS");
			SetBillboard(eTex::Billboard::ParticleCloud_1, L"Effect/Cloud_1.DDS");
			SetBillboard(eTex::Billboard::ParticleCloud_2, L"Effect/Cloud_2.DDS");
			SetBillboard(eTex::Billboard::ParticleCloud_3, L"Effect/Cloud_3.DDS");
			SetBillboard(eTex::Billboard::ParticleRing, L"Effect/Ring.DDS");
			SetBillboard(eTex::Billboard::ParticleBrightRing, L"Effect/Bright_Ring.DDS");
			SetBillboard(eTex::Billboard::ParticleThinRing, L"Effect/Thin_Ring.DDS");
			SetBillboard(eTex::Billboard::ParticleCrossSpark, L"Effect/CrossSpark.DDS");
			SetBillboard(eTex::Billboard::ParticleTrailBlast, L"Effect/TrailBlast.DDS");
			SetBillboard(eTex::Billboard::GameEnd_BackGround, L"Interface/GameEndBackGround.DDS");
			SetBillboard(eTex::Billboard::GameEnd_BackGroundAlpha, L"Interface/GameEndBackGroundAlphaMap.DDS");
			SetBillboard(eTex::Billboard::GameEnd_CloudAlpha, L"Interface/CloudAlphaMap.DDS");
			SetBillboard(eTex::Billboard::GameEnd_Defeat, L"Interface/Defeat.DDS");
			SetBillboard(eTex::Billboard::GameEnd_NeonWire, L"Interface/NeonWire.DDS");
			SetBillboard(eTex::Billboard::GameEnd_Victory, L"Interface/Victory.DDS");
			SetBillboard(eTex::Billboard::DeathUI, L"Interface/BrokenGlass.DDS");
			SetBillboard(eTex::Billboard::ParticleExplodeFire, L"Effect/ExplodeFire.DDS");
			SetBillboard(eTex::Billboard::ParticleBackDraftSmoke, L"Effect/BackDraftSmoke.DDS");
			SetBillboard(eTex::Billboard::ParticleUpperFireSmoke, L"Effect/UpperFireSmoke.DDS");
			SetBillboard(eTex::Billboard::ParticleUpperCloud, L"Effect/UpperCloud.DDS");
			SetBillboard(eTex::Billboard::ParticleHexagon, L"Effect/hexagon.DDS");

		
		}
		// Font
		{
			wstring base_path{ L"../../../Assets/Textures/" };
			auto SetFont = [&](eTex::Font type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_Fonttexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_Fonttexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetFont(eTex::Font::Default, L"Font/Default32.DDS");
			SetFont(eTex::Font::Arial, L"Font/Arial.DDS");
			SetFont(eTex::Font::Quantum, L"Font/Quantum60.DDS");
		}
		// SkillColorIcon
		{
			wstring base_path{ L"../../../Assets/Textures/" };
			auto SetSkillColor = [&](eTex::SkillColorName type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_SkillColorIcontexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_SkillColorIcontexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetSkillColor(eTex::SkillColorName::Dash, L"Interface/Skill_Dash_Color.DDS");
			SetSkillColor(eTex::SkillColorName::BoostJump, L"Interface/Skill_Jump_Color.DDS");
			SetSkillColor(eTex::SkillColorName::HammerStrike, L"Interface/Skill_Strike_Color.DDS");
			SetSkillColor(eTex::SkillColorName::Bulwark, L"Interface/Skill_Bulwark_Color.DDS");
			SetSkillColor(eTex::SkillColorName::SnipeShot, L"Interface/Skill_Snipe_Color.DDS");
			SetSkillColor(eTex::SkillColorName::Blaster, L"Interface/Skill_Blast_Color.DDS");
		}
		// SkillGreyrIcon
		{
			wstring base_path{ L"../../../Assets/Textures/" };
			auto SetSkillGrey = [&](eTex::SkillGreyName type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_SkillGreyIcontexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_SkillGreyIcontexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetSkillGrey(eTex::SkillGreyName::Dash, L"Interface/Skill_Dash_Grey.DDS");
			SetSkillGrey(eTex::SkillGreyName::BoostJump, L"Interface/Skill_Jump_Grey.DDS");
			SetSkillGrey(eTex::SkillGreyName::HammerStrike, L"Interface/Skill_Strike_Grey.DDS");
			SetSkillGrey(eTex::SkillGreyName::Bulwark, L"Interface/Skill_Bulwark_Grey.DDS");
			SetSkillGrey(eTex::SkillGreyName::SnipeShot, L"Interface/Skill_Snipe_Grey.DDS");
			SetSkillGrey(eTex::SkillGreyName::Blaster, L"Interface/Skill_Blast_Grey.DDS");
		}
		// Button
		{
			wstring base_path{ L"../../../Assets/Textures/" };
			auto SetButton = [&](eTex::Button type, LPCWSTR filename, bool bUseBP = true) {
				if (bUseBP) m_Buttontexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, (base_path + filename).c_str());
				else m_Buttontexes[type] = CD3DBufMgr::Instance()->CreateTextureResourceFromFile(
					pd3dCmdLst, filename);
			};
			SetButton(eTex::Button::Connect_N, L"Interface/Button_Connect_Norm.DDS");
			SetButton(eTex::Button::Connect_H, L"Interface/Button_Connect_HigL.DDS");
			SetButton(eTex::Button::Exit_N, L"Interface/Button_Exit_Norm.DDS");
			SetButton(eTex::Button::Exit_H, L"Interface/Button_Exit_HigL.DDS");
			SetButton(eTex::Button::Create_N, L"Interface/Button_Create_Norm.DDS");
			SetButton(eTex::Button::Create_H, L"Interface/Button_Create_HigL.DDS");
			SetButton(eTex::Button::Enter_N, L"Interface/Button_Enter_Norm.DDS");
			SetButton(eTex::Button::Enter_H, L"Interface/Button_Enter_HigL.DDS");
			SetButton(eTex::Button::Leave_N, L"Interface/Button_Leave_Norm.DDS");
			SetButton(eTex::Button::Leave_H, L"Interface/Button_Leave_HigL.DDS");
			SetButton(eTex::Button::Start_N, L"Interface/Button_Start_Norm.DDS");
			SetButton(eTex::Button::Start_H, L"Interface/Button_Start_HigL.DDS");
			SetButton(eTex::Button::Recycle_N, L"Interface/Button_Recycle_Norm.DDS");
			SetButton(eTex::Button::Recycle_H, L"Interface/Button_Recycle_HigL.DDS");
			SetButton(eTex::Button::Ruby_N, L"Interface/Button_Ruby_Norm.DDS");
			SetButton(eTex::Button::Ruby_H, L"Interface/Button_Ruby_HigL.DDS");
			SetButton(eTex::Button::Legion_N, L"Interface/Button_Legion_Norm.DDS");
			SetButton(eTex::Button::Legion_H, L"Interface/Button_Legion_HigL.DDS");
			SetButton(eTex::Button::Epsilon_N, L"Interface/Button_Epsilon_Norm.DDS");
			SetButton(eTex::Button::Epsilon_H, L"Interface/Button_Epsilon_HigL.DDS");
			SetButton(eTex::Button::Gravis_N, L"Interface/Button_Gravis_Norm.DDS");
			SetButton(eTex::Button::Gravis_H, L"Interface/Button_Gravis_HigL.DDS");
		}
		// Texture Set
		{
			shared_ptr<CTexture> pTexSet{ nullptr };
			// Stage
			{
				pTexSet = make_shared<CTexture>(6, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
				pTexSet->CreateTextureSet(0, 3, ROOT_PARAMETER_TEXTURE1);
				pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Diffuse::Stage_CheckerBoard));
				pTexSet->SetTexture(pd3dDevice, 0, 1, GetTexture(eTex::Diffuse::Stage_Cell));
				pTexSet->SetTexture(pd3dDevice, 0, 2, GetTexture(eTex::Diffuse::Stage_BlackerTile));
				pTexSet->CreateTextureSet(1, 3, ROOT_PARAMETER_TEXTURE2);
				pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Normal::Stage_CheckerBoard));
				pTexSet->SetTexture(pd3dDevice, 1, 1, GetTexture(eTex::Normal::Stage_Cell));
				pTexSet->SetTexture(pd3dDevice, 1, 2, GetTexture(eTex::Normal::Stage_BlackerTile));
				pTexSet->CreateTextureSet(2, 3, ROOT_PARAMETER_TEXTURE3);
				pTexSet->SetTexture(pd3dDevice, 2, 0, GetTexture(eTex::Specular::Stage_CheckerBoard_Blue));
				//pTexSet->SetTexture(pd3dDevice, 2, 0, GetTexture(eTex::Specular::Stage_CheckerBoard_Red));
				pTexSet->SetTexture(pd3dDevice, 2, 1, GetTexture(eTex::Specular::Stage_Cell));
				pTexSet->SetTexture(pd3dDevice, 2, 2, GetTexture(eTex::Specular::Stage_BlackerTile));
				pTexSet->CreateTextureSet(3, 3, ROOT_PARAMETER_TEXTURE4);
				pTexSet->SetTexture(pd3dDevice, 3, 0, GetTexture(eTex::Glossiness::Stage_CheckerBoard));
				pTexSet->SetTexture(pd3dDevice, 3, 1, GetTexture(eTex::Glossiness::Stage_Cell));
				pTexSet->SetTexture(pd3dDevice, 3, 2, GetTexture(eTex::Glossiness::Stage_BlackerTile));
				pTexSet->CreateTextureSet(4, 1, ROOT_PARAMETER_TEXTURE5);
				pTexSet->SetTexture(pd3dDevice, 4, 0, GetTexture(eTex::Emissive::Stage_Circuit));
				pTexSet->CreateTextureSet(5, 1, ROOT_PARAMETER_RESOURCE2);
				pTexSet->SetTexture(pd3dDevice, 5, 0, GetShadowMap()->GetDSBuf(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
				m_Textures[eMat::Object::Stage] = pTexSet;
			}
			// SkyBox
			{
				pTexSet = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE_CUBE);
				pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_RESOURCE1);
				pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::SkyBox::StormyDays));
				m_Textures[eMat::Object::SkyBox] = pTexSet;
			}
			// Ruby
			{
				pTexSet = make_shared<CTexture>(6, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
				pTexSet->CreateTextureSet(0, 2, ROOT_PARAMETER_TEXTURE1);
				pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Diffuse::Ruby_Body));
				pTexSet->SetTexture(pd3dDevice, 0, 1, GetTexture(eTex::Diffuse::Ruby_Hammer));
				pTexSet->CreateTextureSet(1, 2, ROOT_PARAMETER_TEXTURE2);
				pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Normal::Ruby_Body));
				pTexSet->SetTexture(pd3dDevice, 1, 1, GetTexture(eTex::Normal::Ruby_Hammer));
				pTexSet->CreateTextureSet(2, 2, ROOT_PARAMETER_TEXTURE3);
				pTexSet->SetTexture(pd3dDevice, 2, 0, GetTexture(eTex::Specular::Ruby_Body));
				pTexSet->SetTexture(pd3dDevice, 2, 1, GetTexture(eTex::Specular::Ruby_Hammer));
				pTexSet->CreateTextureSet(3, 2, ROOT_PARAMETER_TEXTURE4);
				pTexSet->SetTexture(pd3dDevice, 3, 0, GetTexture(eTex::Glossiness::Ruby_Body));
				pTexSet->SetTexture(pd3dDevice, 3, 1, GetTexture(eTex::Glossiness::Ruby_Hammer));
				pTexSet->CreateTextureSet(4, 2, ROOT_PARAMETER_TEXTURE5);
				pTexSet->SetTexture(pd3dDevice, 4, 0, GetTexture(eTex::Emissive::Ruby_Body));
				pTexSet->SetTexture(pd3dDevice, 4, 1, GetTexture(eTex::Emissive::Ruby_Hammer));
				pTexSet->CreateTextureSet(5, 1, ROOT_PARAMETER_RESOURCE2);
				pTexSet->SetTexture(pd3dDevice, 5, 0, GetShadowMap()->GetDSBuf(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

				m_Textures[eMat::Object::Ruby] = pTexSet;
			}
			// Legion
			{
				pTexSet = make_shared<CTexture>(6, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
				pTexSet->CreateTextureSet(0, 9, ROOT_PARAMETER_TEXTURE1);
				pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Diffuse::Legion_Spine));
				pTexSet->SetTexture(pd3dDevice, 0, 1, GetTexture(eTex::Diffuse::Legion_Shoulder));
				pTexSet->SetTexture(pd3dDevice, 0, 2, GetTexture(eTex::Diffuse::Legion_Mid));
				pTexSet->SetTexture(pd3dDevice, 0, 3, GetTexture(eTex::Diffuse::Legion_Leg));
				pTexSet->SetTexture(pd3dDevice, 0, 4, GetTexture(eTex::Diffuse::Legion_Booster));
				pTexSet->SetTexture(pd3dDevice, 0, 5, GetTexture(eTex::Diffuse::Legion_Arm));
				pTexSet->SetTexture(pd3dDevice, 0, 6, GetTexture(eTex::Diffuse::Legion_Head));
				pTexSet->SetTexture(pd3dDevice, 0, 7, GetTexture(eTex::Diffuse::Legion_Shield));
				pTexSet->SetTexture(pd3dDevice, 0, 8, GetTexture(eTex::Diffuse::Legion_Spear));
				pTexSet->CreateTextureSet(1, 9, ROOT_PARAMETER_TEXTURE2);
				pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Normal::Legion_Spine));
				pTexSet->SetTexture(pd3dDevice, 1, 1, GetTexture(eTex::Normal::Legion_Shoulder));
				pTexSet->SetTexture(pd3dDevice, 1, 2, GetTexture(eTex::Normal::Legion_Mid));
				pTexSet->SetTexture(pd3dDevice, 1, 3, GetTexture(eTex::Normal::Legion_Leg));
				pTexSet->SetTexture(pd3dDevice, 1, 4, GetTexture(eTex::Normal::Legion_Booster));
				pTexSet->SetTexture(pd3dDevice, 1, 5, GetTexture(eTex::Normal::Legion_Arm));
				pTexSet->SetTexture(pd3dDevice, 1, 6, GetTexture(eTex::Normal::Legion_Head));
				pTexSet->SetTexture(pd3dDevice, 1, 7, GetTexture(eTex::Normal::Legion_Shield));
				pTexSet->SetTexture(pd3dDevice, 1, 8, GetTexture(eTex::Normal::Legion_Spear));
				pTexSet->CreateTextureSet(2, 9, ROOT_PARAMETER_TEXTURE3);
				pTexSet->SetTexture(pd3dDevice, 2, 0, GetTexture(eTex::Specular::Legion_Spine));
				pTexSet->SetTexture(pd3dDevice, 2, 1, GetTexture(eTex::Specular::Legion_Shoulder));
				pTexSet->SetTexture(pd3dDevice, 2, 2, GetTexture(eTex::Specular::Legion_Mid));
				pTexSet->SetTexture(pd3dDevice, 2, 3, GetTexture(eTex::Specular::Legion_Leg));
				pTexSet->SetTexture(pd3dDevice, 2, 4, GetTexture(eTex::Specular::Legion_Booster));
				pTexSet->SetTexture(pd3dDevice, 2, 5, GetTexture(eTex::Specular::Legion_Arm));
				pTexSet->SetTexture(pd3dDevice, 2, 6, GetTexture(eTex::Specular::Legion_Head));
				pTexSet->SetTexture(pd3dDevice, 2, 7, GetTexture(eTex::Specular::Legion_Shield));
				pTexSet->SetTexture(pd3dDevice, 2, 8, GetTexture(eTex::Specular::Legion_Spear));
				pTexSet->CreateTextureSet(3, 9, ROOT_PARAMETER_TEXTURE4);
				pTexSet->SetTexture(pd3dDevice, 3, 0, GetTexture(eTex::Glossiness::Legion_Spine));
				pTexSet->SetTexture(pd3dDevice, 3, 1, GetTexture(eTex::Glossiness::Legion_Shoulder));
				pTexSet->SetTexture(pd3dDevice, 3, 2, GetTexture(eTex::Glossiness::Legion_Mid));
				pTexSet->SetTexture(pd3dDevice, 3, 3, GetTexture(eTex::Glossiness::Legion_Leg));
				pTexSet->SetTexture(pd3dDevice, 3, 4, GetTexture(eTex::Glossiness::Legion_Booster));
				pTexSet->SetTexture(pd3dDevice, 3, 5, GetTexture(eTex::Glossiness::Legion_Arm));
				pTexSet->SetTexture(pd3dDevice, 3, 6, GetTexture(eTex::Glossiness::Legion_Head));
				pTexSet->SetTexture(pd3dDevice, 3, 7, GetTexture(eTex::Glossiness::Legion_Shield));
				pTexSet->SetTexture(pd3dDevice, 3, 8, GetTexture(eTex::Glossiness::Legion_Spear));
				pTexSet->CreateTextureSet(4, 9, ROOT_PARAMETER_TEXTURE5);
				pTexSet->SetTexture(pd3dDevice, 4, 0, GetTexture(eTex::Emissive::Legion_Spine));
				pTexSet->SetTexture(pd3dDevice, 4, 1, GetTexture(eTex::Emissive::Legion_Shoulder));
				pTexSet->SetTexture(pd3dDevice, 4, 2, GetTexture(eTex::Emissive::Legion_Mid));
				pTexSet->SetTexture(pd3dDevice, 4, 3, GetTexture(eTex::Emissive::Legion_Leg));
				pTexSet->SetTexture(pd3dDevice, 4, 4, GetTexture(eTex::Emissive::Legion_Booster));
				pTexSet->SetTexture(pd3dDevice, 4, 5, GetTexture(eTex::Emissive::Legion_Arm));
				pTexSet->SetTexture(pd3dDevice, 4, 6, GetTexture(eTex::Emissive::Legion_Head));
				pTexSet->SetTexture(pd3dDevice, 4, 7, GetTexture(eTex::Emissive::Legion_Shield));
				pTexSet->SetTexture(pd3dDevice, 4, 8, GetTexture(eTex::Emissive::Legion_Spear));
				pTexSet->CreateTextureSet(5, 1, ROOT_PARAMETER_RESOURCE2);
				pTexSet->SetTexture(pd3dDevice, 5, 0, GetShadowMap()->GetDSBuf(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

				m_Textures[eMat::Object::Legion] = pTexSet;
			}
			// Epsilon
			{
				pTexSet = make_shared<CTexture>(6, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
				pTexSet->CreateTextureSet(0, 7, ROOT_PARAMETER_TEXTURE1);
				pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Diffuse::Epsilon_Arm));
				pTexSet->SetTexture(pd3dDevice, 0, 1, GetTexture(eTex::Diffuse::Epsilon_Foot));
				pTexSet->SetTexture(pd3dDevice, 0, 2, GetTexture(eTex::Diffuse::Epsilon_Head));
				pTexSet->SetTexture(pd3dDevice, 0, 3, GetTexture(eTex::Diffuse::Epsilon_Leg));
				pTexSet->SetTexture(pd3dDevice, 0, 4, GetTexture(eTex::Diffuse::Epsilon_Pelvis));
				pTexSet->SetTexture(pd3dDevice, 0, 5, GetTexture(eTex::Diffuse::Epsilon_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 0, 6, GetTexture(eTex::Diffuse::Epsilon_Weapon));
				pTexSet->CreateTextureSet(1, 7, ROOT_PARAMETER_TEXTURE2);
				pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Normal::Epsilon_Arm));
				pTexSet->SetTexture(pd3dDevice, 1, 1, GetTexture(eTex::Normal::Epsilon_Foot));
				pTexSet->SetTexture(pd3dDevice, 1, 2, GetTexture(eTex::Normal::Epsilon_Head));
				pTexSet->SetTexture(pd3dDevice, 1, 3, GetTexture(eTex::Normal::Epsilon_Leg));
				pTexSet->SetTexture(pd3dDevice, 1, 4, GetTexture(eTex::Normal::Epsilon_Pelvis));
				pTexSet->SetTexture(pd3dDevice, 1, 5, GetTexture(eTex::Normal::Epsilon_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 1, 6, GetTexture(eTex::Normal::Epsilon_Weapon));
				pTexSet->CreateTextureSet(2, 7, ROOT_PARAMETER_TEXTURE3);
				pTexSet->SetTexture(pd3dDevice, 2, 0, GetTexture(eTex::Specular::Epsilon_Arm));
				pTexSet->SetTexture(pd3dDevice, 2, 1, GetTexture(eTex::Specular::Epsilon_Foot));
				pTexSet->SetTexture(pd3dDevice, 2, 2, GetTexture(eTex::Specular::Epsilon_Head));
				pTexSet->SetTexture(pd3dDevice, 2, 3, GetTexture(eTex::Specular::Epsilon_Leg));
				pTexSet->SetTexture(pd3dDevice, 2, 4, GetTexture(eTex::Specular::Epsilon_Pelvis));
				pTexSet->SetTexture(pd3dDevice, 2, 5, GetTexture(eTex::Specular::Epsilon_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 2, 6, GetTexture(eTex::Specular::Epsilon_Weapon));
				pTexSet->CreateTextureSet(3, 7, ROOT_PARAMETER_TEXTURE4);
				pTexSet->SetTexture(pd3dDevice, 3, 0, GetTexture(eTex::Glossiness::Epsilon_Arm));
				pTexSet->SetTexture(pd3dDevice, 3, 1, GetTexture(eTex::Glossiness::Epsilon_Foot));
				pTexSet->SetTexture(pd3dDevice, 3, 2, GetTexture(eTex::Glossiness::Epsilon_Head));
				pTexSet->SetTexture(pd3dDevice, 3, 3, GetTexture(eTex::Glossiness::Epsilon_Leg));
				pTexSet->SetTexture(pd3dDevice, 3, 4, GetTexture(eTex::Glossiness::Epsilon_Pelvis));
				pTexSet->SetTexture(pd3dDevice, 3, 5, GetTexture(eTex::Glossiness::Epsilon_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 3, 6, GetTexture(eTex::Glossiness::Epsilon_Weapon));
				pTexSet->CreateTextureSet(4, 7, ROOT_PARAMETER_TEXTURE5);
				pTexSet->SetTexture(pd3dDevice, 4, 0, GetTexture(eTex::Emissive::Epsilon_Arm));
				pTexSet->SetTexture(pd3dDevice, 4, 1, GetTexture(eTex::Emissive::Epsilon_Foot));
				pTexSet->SetTexture(pd3dDevice, 4, 2, GetTexture(eTex::Emissive::Epsilon_Head));
				pTexSet->SetTexture(pd3dDevice, 4, 3, GetTexture(eTex::Emissive::Epsilon_Leg));
				pTexSet->SetTexture(pd3dDevice, 4, 4, GetTexture(eTex::Emissive::Epsilon_Pelvis));
				pTexSet->SetTexture(pd3dDevice, 4, 5, GetTexture(eTex::Emissive::Epsilon_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 4, 6, GetTexture(eTex::Emissive::Epsilon_Weapon));
				pTexSet->CreateTextureSet(5, 1, ROOT_PARAMETER_RESOURCE2);
				pTexSet->SetTexture(pd3dDevice, 5, 0, GetShadowMap()->GetDSBuf(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

				m_Textures[eMat::Object::Epsilon] = pTexSet;
			}
			// Gravis
			{
				pTexSet = make_shared<CTexture>(6, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
				pTexSet->CreateTextureSet(0, 7, ROOT_PARAMETER_TEXTURE1);
				pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Diffuse::Gravis_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 0, 1, GetTexture(eTex::Diffuse::Gravis_Lowerbody));
				pTexSet->SetTexture(pd3dDevice, 0, 2, GetTexture(eTex::Diffuse::Gravis_Head));
				pTexSet->SetTexture(pd3dDevice, 0, 3, GetTexture(eTex::Diffuse::Gravis_Leg));
				pTexSet->SetTexture(pd3dDevice, 0, 4, GetTexture(eTex::Diffuse::Gravis_Arm));
				pTexSet->SetTexture(pd3dDevice, 0, 5, GetTexture(eTex::Diffuse::Gravis_Flight));
				pTexSet->SetTexture(pd3dDevice, 0, 6, GetTexture(eTex::Diffuse::Gravis_Weapon));
				pTexSet->CreateTextureSet(1, 7, ROOT_PARAMETER_TEXTURE2);
				pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Normal::Gravis_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 1, 1, GetTexture(eTex::Normal::Gravis_Lowerbody));
				pTexSet->SetTexture(pd3dDevice, 1, 2, GetTexture(eTex::Normal::Gravis_Head));
				pTexSet->SetTexture(pd3dDevice, 1, 3, GetTexture(eTex::Normal::Gravis_Leg));
				pTexSet->SetTexture(pd3dDevice, 1, 4, GetTexture(eTex::Normal::Gravis_Arm));
				pTexSet->SetTexture(pd3dDevice, 1, 5, GetTexture(eTex::Normal::Gravis_Flight));
				pTexSet->SetTexture(pd3dDevice, 1, 6, GetTexture(eTex::Normal::Gravis_Weapon));
				pTexSet->CreateTextureSet(2, 7, ROOT_PARAMETER_TEXTURE3);
				pTexSet->SetTexture(pd3dDevice, 2, 0, GetTexture(eTex::Specular::Gravis_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 2, 1, GetTexture(eTex::Specular::Gravis_Lowerbody));
				pTexSet->SetTexture(pd3dDevice, 2, 2, GetTexture(eTex::Specular::Gravis_Head));
				pTexSet->SetTexture(pd3dDevice, 2, 3, GetTexture(eTex::Specular::Gravis_Leg));
				pTexSet->SetTexture(pd3dDevice, 2, 4, GetTexture(eTex::Specular::Gravis_Arm));
				pTexSet->SetTexture(pd3dDevice, 2, 5, GetTexture(eTex::Specular::Gravis_Flight));
				pTexSet->SetTexture(pd3dDevice, 2, 6, GetTexture(eTex::Specular::Gravis_Weapon));
				pTexSet->CreateTextureSet(3, 7, ROOT_PARAMETER_TEXTURE4);
				pTexSet->SetTexture(pd3dDevice, 3, 0, GetTexture(eTex::Glossiness::Gravis_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 3, 1, GetTexture(eTex::Glossiness::Gravis_Lowerbody));
				pTexSet->SetTexture(pd3dDevice, 3, 2, GetTexture(eTex::Glossiness::Gravis_Head));
				pTexSet->SetTexture(pd3dDevice, 3, 3, GetTexture(eTex::Glossiness::Gravis_Leg));
				pTexSet->SetTexture(pd3dDevice, 3, 4, GetTexture(eTex::Glossiness::Gravis_Arm));
				pTexSet->SetTexture(pd3dDevice, 3, 5, GetTexture(eTex::Glossiness::Gravis_Flight));
				pTexSet->SetTexture(pd3dDevice, 3, 6, GetTexture(eTex::Glossiness::Gravis_Weapon));
				pTexSet->CreateTextureSet(4, 7, ROOT_PARAMETER_TEXTURE5);
				pTexSet->SetTexture(pd3dDevice, 4, 0, GetTexture(eTex::Emissive::Gravis_Upperbody));
				pTexSet->SetTexture(pd3dDevice, 4, 1, GetTexture(eTex::Emissive::Gravis_Lowerbody));
				pTexSet->SetTexture(pd3dDevice, 4, 2, GetTexture(eTex::Emissive::Gravis_Head));
				pTexSet->SetTexture(pd3dDevice, 4, 3, GetTexture(eTex::Emissive::Gravis_Leg));
				pTexSet->SetTexture(pd3dDevice, 4, 4, GetTexture(eTex::Emissive::Gravis_Arm));
				pTexSet->SetTexture(pd3dDevice, 4, 5, GetTexture(eTex::Emissive::Gravis_Flight));
				pTexSet->SetTexture(pd3dDevice, 4, 6, GetTexture(eTex::Emissive::Gravis_Weapon));
				pTexSet->CreateTextureSet(5, 1, ROOT_PARAMETER_RESOURCE2);
				pTexSet->SetTexture(pd3dDevice, 5, 0, GetShadowMap()->GetDSBuf(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

				m_Textures[eMat::Object::Gravis] = pTexSet;
			}
			// Ruby_Bullet
			{
				pTexSet = make_shared<CTexture>(6, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
				pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
				pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Diffuse::Ruby_Bullet));
				pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
				pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Normal::Ruby_Bullet));
				pTexSet->CreateTextureSet(2, 1, ROOT_PARAMETER_TEXTURE3);
				pTexSet->SetTexture(pd3dDevice, 2, 0, GetTexture(eTex::Specular::Ruby_Bullet));
				pTexSet->CreateTextureSet(3, 1, ROOT_PARAMETER_TEXTURE4);
				pTexSet->SetTexture(pd3dDevice, 3, 0, GetTexture(eTex::Glossiness::Ruby_Bullet));
				pTexSet->CreateTextureSet(4, 1, ROOT_PARAMETER_TEXTURE5);
				pTexSet->SetTexture(pd3dDevice, 4, 0, GetTexture(eTex::Emissive::Ruby_Bullet));
				pTexSet->CreateTextureSet(5, 1, ROOT_PARAMETER_RESOURCE2);
				pTexSet->SetTexture(pd3dDevice, 5, 0, GetShadowMap()->GetDSBuf(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
				m_Textures[eMat::Object::Ruby_Bullet] = pTexSet;
			}
			// Player UI
			{
				pTexSet = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
				pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
				pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Billboard::PlayerUI));
				m_Textures[eMat::Object::PlayerUI] = pTexSet;
			}
			// Player Skill
			{
				// Dash
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::SkillColorName::Dash));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::SkillGreyName::Dash));
					m_Textures[eMat::Object::DashSkillIcon] = pTexSet;
				}
				// BoostJump
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::SkillColorName::BoostJump));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::SkillGreyName::BoostJump));
					m_Textures[eMat::Object::JumpSkillIcon] = pTexSet;
				}
				// HammerStrike
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::SkillColorName::HammerStrike));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::SkillGreyName::HammerStrike));
					m_Textures[eMat::Object::RubySkillIcon] = pTexSet;
				}
				// Bulwark
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::SkillColorName::Bulwark));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::SkillGreyName::Bulwark));
					m_Textures[eMat::Object::LegionSkillIcon] = pTexSet;
				}
				// SnipeShot
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::SkillColorName::SnipeShot));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::SkillGreyName::SnipeShot));
					m_Textures[eMat::Object::EpsilonSkillIcon] = pTexSet;
				}
				// Blaster
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::SkillColorName::Blaster));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::SkillGreyName::Blaster));
					m_Textures[eMat::Object::GravisSkillIcon] = pTexSet;
				}
			}
			// Button
			{
				// Logo
				{
					pTexSet = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Billboard::Title));
					m_Textures[eMat::Object::LobbyLogo] = pTexSet;
				}
				// Connect
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Connect_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Connect_H));
					m_Textures[eMat::Object::ConnectButton] = pTexSet;
				}
				// Exit
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Exit_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Exit_H));
					m_Textures[eMat::Object::ExitButton] = pTexSet;
				}
				// Create
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Create_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Create_H));
					m_Textures[eMat::Object::CreateButton] = pTexSet;
				}
				// Enter
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Enter_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Enter_H));
					m_Textures[eMat::Object::EnterButton] = pTexSet;
				}
				// Leave
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Leave_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Leave_H));
					m_Textures[eMat::Object::LeaveButton] = pTexSet;
				}
				// Start
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Start_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Start_H));
					m_Textures[eMat::Object::StartButton] = pTexSet;
				}
				// Recycle
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Recycle_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Recycle_H));
					m_Textures[eMat::Object::RecycleButton] = pTexSet;
				}
				// Ruby
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Ruby_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Ruby_H));
					m_Textures[eMat::Object::RubyButton] = pTexSet;
				}
				// Legion
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Legion_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Legion_H));
					m_Textures[eMat::Object::LegionButton] = pTexSet;
				}
				// Epsilon
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Epsilon_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Epsilon_H));
					m_Textures[eMat::Object::EpsilonButton] = pTexSet;
				}
				// Gravis
				{
					pTexSet = make_shared<CTexture>(2, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Button::Gravis_N));
					pTexSet->CreateTextureSet(1, 1, ROOT_PARAMETER_TEXTURE2);
					pTexSet->SetTexture(pd3dDevice, 1, 0, GetTexture(eTex::Button::Gravis_H));
					m_Textures[eMat::Object::GravisButton] = pTexSet;
				}
			}
			// Gameover
			{
				// GameEnd
				{
					pTexSet = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
					pTexSet->CreateTextureSet(0, 6, ROOT_PARAMETER_TEXTURE1);
					pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Billboard::GameEnd_BackGround));
					pTexSet->SetTexture(pd3dDevice, 0, 1, GetTexture(eTex::Billboard::GameEnd_NeonWire));
					pTexSet->SetTexture(pd3dDevice, 0, 2, GetTexture(eTex::Billboard::GameEnd_Victory));
					pTexSet->SetTexture(pd3dDevice, 0, 3, GetTexture(eTex::Billboard::GameEnd_Defeat));
					pTexSet->SetTexture(pd3dDevice, 0, 4, GetTexture(eTex::Billboard::GameEnd_CloudAlpha));
					pTexSet->SetTexture(pd3dDevice, 0, 5, GetTexture(eTex::Billboard::GameEnd_BackGroundAlpha));
					m_Textures[eMat::Object::GameEnd] = pTexSet;
				}
			}
			// DeathUI
			pTexSet = make_shared<CTexture>(1, pd3dDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, eTex::Type::RESOURCE_TEXTURE2D_ARRAY);
			pTexSet->CreateTextureSet(0, 1, ROOT_PARAMETER_TEXTURE1);
			pTexSet->SetTexture(pd3dDevice, 0, 0, GetTexture(eTex::Billboard::DeathUI));
			m_Textures[eMat::Object::DeathUI] = pTexSet;
		}
		cout << "texture load done\n";
		// Mesh
		{
			auto SetMesh = [&](eGeo::Mesh type, CMesh* pMesh) {
				m_Meshes[type] = make_shared<CMesh>(*pMesh);
			};
			SetMesh(eGeo::Mesh::Ruby, new CAnimationModelMesh(pd3dCmdLst, "./Assets/Meshes/Ruby.mbox"));
			SetMesh(eGeo::Mesh::Legion, new CAnimationModelMesh(pd3dCmdLst, "./Assets/Meshes/Legion.mbox"));
			SetMesh(eGeo::Mesh::Epsilon, new CAnimationModelMesh(pd3dCmdLst, "./Assets/Meshes/Epsilon.mbox"));
			SetMesh(eGeo::Mesh::Gravis, new CAnimationModelMesh(pd3dCmdLst, "./Assets/Meshes/Gravis.mbox"));
			SetMesh(eGeo::Mesh::Cube, new CCubeMesh(pd3dCmdLst, "./Assets/Meshes/GroundBlock.mbox", 3.0f));
			SetMesh(eGeo::Mesh::Bullet, new CModelMesh(pd3dCmdLst, "./Assets/Meshes/Ruby_Bullet.mbox", 0.5f));
		}
		// StageMeshes
		{
			const int nStageMeshes{ 563 };
			string base_path { "./Assets/Meshes/Stage/OutPostBreakable" };
			m_StageMeshes.resize(nStageMeshes);
			for (int i = 0; i < nStageMeshes; ++i) {
				m_StageMeshes[i] = make_shared<CStageMesh>(
					  pd3dCmdLst
					, (base_path + std::to_string(i) + ".mbox").c_str()
					, 1.0f);
			}
		}
		// Skeleton
		{
			auto SetSkel = [&](eGeo::Skel type, string filename) {
				shared_ptr<Skeleton> pSkel = make_shared<Skeleton>();
				pSkel->Init(("./Assets/Animations/Skeletons/" + filename).c_str());
				m_Skeletons[type] = pSkel;
			};
			SetSkel(eGeo::Skel::Ruby, "Ruby.sbox");
			SetSkel(eGeo::Skel::Legion, "Legion.sbox");
			SetSkel(eGeo::Skel::Epsilon, "Epsilon.sbox");
			SetSkel(eGeo::Skel::Gravis, "Gravis.sbox");
		}
		// Animation
		{
			auto SetAnim = [&](eGeo::Skel skeltype, eGeo::Anim type, string filename) {
				m_Animations[type] = make_shared<CAnimation>(
					  m_Skeletons[skeltype]->nBones
					, ("./Assets/Animations/Keyframes/" + filename).c_str()
				);
			};
			SetAnim(eGeo::Skel::Ruby, eGeo::Anim::Ruby_Idle, "Ruby/Idle.abox");
			SetAnim(eGeo::Skel::Ruby, eGeo::Anim::Ruby_Hovering, "Ruby/Hovering.abox");
			SetAnim(eGeo::Skel::Ruby, eGeo::Anim::Ruby_Hammering, "Ruby/Hammering.abox");
			SetAnim(eGeo::Skel::Ruby, eGeo::Anim::Ruby_Shoot, "Ruby/Shooting.abox");
			SetAnim(eGeo::Skel::Ruby, eGeo::Anim::Ruby_Running, "Ruby/Running.abox");
			SetAnim(eGeo::Skel::Legion, eGeo::Anim::Legion_Idle, "Legion/Idle.abox");
			SetAnim(eGeo::Skel::Legion, eGeo::Anim::Legion_Hovering, "Legion/Hovering.abox");
			SetAnim(eGeo::Skel::Legion, eGeo::Anim::Legion_ShieldCharging, "Legion/Shield Charge.abox");
			SetAnim(eGeo::Skel::Legion, eGeo::Anim::Legion_SpearCharging, "Legion/Spear Charge.abox");
			SetAnim(eGeo::Skel::Legion, eGeo::Anim::Legion_Running, "Legion/Running.abox");
			SetAnim(eGeo::Skel::Epsilon, eGeo::Anim::Epsilon_Idle, "Epsilon/Idle.abox");
			SetAnim(eGeo::Skel::Epsilon, eGeo::Anim::Epsilon_Hovering, "Epsilon/Hovering.abox");
			SetAnim(eGeo::Skel::Epsilon, eGeo::Anim::Epsilon_Shooting, "Epsilon/Shooting.abox");
			SetAnim(eGeo::Skel::Epsilon, eGeo::Anim::Epsilon_Shooting_Skill, "Epsilon/Shooting_Skill.abox");
			SetAnim(eGeo::Skel::Epsilon, eGeo::Anim::Epsilon_Running, "Epsilon/Running.abox");
			SetAnim(eGeo::Skel::Gravis, eGeo::Anim::Gravis_Idle, "Gravis/Idle.abox");
			SetAnim(eGeo::Skel::Gravis, eGeo::Anim::Gravis_Hovering, "Gravis/Hovering.abox");
			SetAnim(eGeo::Skel::Gravis, eGeo::Anim::Gravis_Attack, "Gravis/Attack.abox");
			SetAnim(eGeo::Skel::Gravis, eGeo::Anim::Gravis_Running, "Gravis/Running.abox");
		}
	} };
	// ShaderCompile
#if USE_LOADING_CACHE_SHADER
	{
		m_CompiledShaders[ePSO::Graphic::ShadowMapStage] = CompiledShaderFromCacheFile("./Assets/pchlsl/ShadowMapStage.pchlsl");
		m_CompiledShaders[ePSO::Graphic::ShadowMapSkinnedAnim] = CompiledShaderFromCacheFile("./Assets/pchlsl/ShadowMapSkinnedAnim.pchlsl");
		m_CompiledShaders[ePSO::Graphic::ShadowMapModel] = CompiledShaderFromCacheFile("./Assets/pchlsl/ShadowMapModel.pchlsl");
		m_CompiledShaders[ePSO::Graphic::ShadowMapCube] = CompiledShaderFromCacheFile("./Assets/pchlsl/ShadowMapCube.pchlsl");
		m_CompiledShaders[ePSO::Graphic::SkyBox] = CompiledShaderFromCacheFile("./Assets/pchlsl/SkyBox.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Stage] = CompiledShaderFromCacheFile("./Assets/pchlsl/Stage.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Cube] = CompiledShaderFromCacheFile("./Assets/pchlsl/Cube.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Model] = CompiledShaderFromCacheFile("./Assets/pchlsl/Model.pchlsl");
		m_CompiledShaders[ePSO::Graphic::SkinnedAnim] = CompiledShaderFromCacheFile("./Assets/pchlsl/SkinnedAnim.pchlsl");
		m_CompiledShaders[ePSO::Graphic::DeferredSkyBox] = CompiledShaderFromCacheFile("./Assets/pchlsl/DeferredSkyBox.pchlsl");
		m_CompiledShaders[ePSO::Graphic::DeferredStage] = CompiledShaderFromCacheFile("./Assets/pchlsl/DeferredStage.pchlsl");
		m_CompiledShaders[ePSO::Graphic::DeferredCube] = CompiledShaderFromCacheFile("./Assets/pchlsl/DeferredCube.pchlsl");
		m_CompiledShaders[ePSO::Graphic::DeferredModel] = CompiledShaderFromCacheFile("./Assets/pchlsl/DeferredModel.pchlsl");
		m_CompiledShaders[ePSO::Graphic::DeferredSkinnedAnim] = CompiledShaderFromCacheFile("./Assets/pchlsl/DeferredSkinnedAnim.pchlsl");
		m_CompiledShaders[ePSO::Graphic::DeferredLighting] = CompiledShaderFromCacheFile("./Assets/pchlsl/DeferredLighting.pchlsl");
		m_CompiledShaders[ePSO::Graphic::FullScreen] = CompiledShaderFromCacheFile("./Assets/pchlsl/FullScreen.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Billboard] = CompiledShaderFromCacheFile("./Assets/pchlsl/Billboard.pchlsl");
		m_CompiledShaders[ePSO::Graphic::ParticleBillboardDefault] = CompiledShaderFromCacheFile("./Assets/pchlsl/ParticleBillboardDefault.pchlsl");
		m_CompiledShaders[ePSO::Graphic::ParticleBillboardAdditive] = CompiledShaderFromCacheFile("./Assets/pchlsl/ParticleBillboardAdditive.pchlsl");
		m_CompiledShaders[ePSO::Graphic::PlayerUI] = CompiledShaderFromCacheFile("./Assets/pchlsl/PlayerUI.pchlsl");
		m_CompiledShaders[ePSO::Graphic::HUD_UI] = CompiledShaderFromCacheFile("./Assets/pchlsl/HUD_UI.pchlsl");
		m_CompiledShaders[ePSO::Graphic::RADER_UI] = CompiledShaderFromCacheFile("./Assets/pchlsl/RADER_UI.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Skill_Icon] = CompiledShaderFromCacheFile("./Assets/pchlsl/Skill_Icon.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Crosshair] = CompiledShaderFromCacheFile("./Assets/pchlsl/Crosshair.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Font] = CompiledShaderFromCacheFile("./Assets/pchlsl/Font.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Logo] = CompiledShaderFromCacheFile("./Assets/pchlsl/Logo.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Title] = CompiledShaderFromCacheFile("./Assets/pchlsl/Title.pchlsl");
		m_CompiledShaders[ePSO::Graphic::LobbyWindow] = CompiledShaderFromCacheFile("./Assets/pchlsl/LobbyWindow.pchlsl");
		m_CompiledShaders[ePSO::Graphic::LobbyRoomSlot] = CompiledShaderFromCacheFile("./Assets/pchlsl/LobbyRoomSlot.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Button] = CompiledShaderFromCacheFile("./Assets/pchlsl/Button.pchlsl");
		m_CompiledShaders[ePSO::Graphic::Gameover] = CompiledShaderFromCacheFile("./Assets/pchlsl/Gameover.pchlsl");
		m_CompiledShaders[ePSO::Graphic::DeathUI] = CompiledShaderFromCacheFile("./Assets/pchlsl/DeathUI.pchlsl");
	}
#else
	{
		CompiledShader cs;
		wstring path;
		LPCSTR vs_func_name = "VS_MAIN";
		LPCSTR gs_func_name = "GS_MAIN";
		LPCSTR ps_func_name = "PS_MAIN";
		LPCSTR vs_ver = "vs_5_1";
		LPCSTR gs_ver = "gs_5_1";
		LPCSTR ps_ver = "ps_5_1";

		auto filename = [&](LPCWSTR file_name)-> LPCWSTR {
			path.clear();
			path = L"src/Shader/hlsl/" + wstring{ file_name } +L".hlsl";
			return path.c_str();
		};
		
		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"SkyBox"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"SkyBox"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "SkyBox");
		m_CompiledShaders[ePSO::Graphic::SkyBox] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Stage"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Stage"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Stage");
		m_CompiledShaders[ePSO::Graphic::Stage] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Cube"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Cube"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Cube");
		m_CompiledShaders[ePSO::Graphic::Cube] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"SkinnedAnimationModel"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"SkinnedAnimationModel"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "SkinnedAnim");
		m_CompiledShaders[ePSO::Graphic::SkinnedAnim] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Model"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Model"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Model");
		m_CompiledShaders[ePSO::Graphic::Model] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"SkyBox"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"SkyBox"), "PS_DEFFERED", ps_ver);
		CompiledShaderToCacheFile(cs, "DeferredSkyBox");
		m_CompiledShaders[ePSO::Graphic::DeferredSkyBox] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Stage"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Stage"), "PS_DEFFERED", ps_ver);
		CompiledShaderToCacheFile(cs, "DeferredStage");
		m_CompiledShaders[ePSO::Graphic::DeferredStage] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Cube"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Cube"), "PS_DEFFERED", ps_ver);
		CompiledShaderToCacheFile(cs, "DeferredCube");
		m_CompiledShaders[ePSO::Graphic::DeferredCube] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"SkinnedAnimationModel"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"SkinnedAnimationModel"), "PS_DEFFERED", ps_ver);
		CompiledShaderToCacheFile(cs, "DeferredSkinnedAnim");
		m_CompiledShaders[ePSO::Graphic::DeferredSkinnedAnim] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Model"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Model"), "PS_DEFFERED", ps_ver);
		CompiledShaderToCacheFile(cs, "DeferredModel");
		m_CompiledShaders[ePSO::Graphic::DeferredModel] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"DeferredLighting"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"DeferredLighting"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "DeferredLighting");
		m_CompiledShaders[ePSO::Graphic::DeferredLighting] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Billboard"), vs_func_name, vs_ver);
		cs.geomery = CompileShaderFromFile(filename(L"Billboard"), gs_func_name, gs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Billboard"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Billboard");
		m_CompiledShaders[ePSO::Graphic::Billboard] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"ParticleBillboard"), vs_func_name, vs_ver);
		cs.geomery = CompileShaderFromFile(filename(L"ParticleBillboard"), gs_func_name, gs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"ParticleBillboard"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "ParticleBillboardAdditive");
		m_CompiledShaders[ePSO::Graphic::ParticleBillboardAdditive] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"ParticleBillboard"), vs_func_name, vs_ver);
		cs.geomery = CompileShaderFromFile(filename(L"ParticleBillboard"), gs_func_name, gs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"ParticleBillboard"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "ParticleBillboardDefault");
		m_CompiledShaders[ePSO::Graphic::ParticleBillboardDefault] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"PlayerUI"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"PlayerUI"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "PlayerUI");
		m_CompiledShaders[ePSO::Graphic::PlayerUI] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"TextureToFullScreen"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"TextureToFullScreen"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "FullScreen");
		m_CompiledShaders[ePSO::Graphic::FullScreen] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"HUDUI"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"HUDUI"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "HUD_UI");
		m_CompiledShaders[ePSO::Graphic::HUD_UI] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Rader"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Rader"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "RADER_UI");
		m_CompiledShaders[ePSO::Graphic::RADER_UI] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"SkillIcon"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"SkillIcon"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Skill_Icon");
		m_CompiledShaders[ePSO::Graphic::Skill_Icon] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Crosshair"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Crosshair"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Crosshair");
		m_CompiledShaders[ePSO::Graphic::Crosshair] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Font"), vs_func_name, vs_ver);
		cs.geomery = CompileShaderFromFile(filename(L"Font"), gs_func_name, gs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Font"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Font");
		m_CompiledShaders[ePSO::Graphic::Font] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Button"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Button"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Button");
		m_CompiledShaders[ePSO::Graphic::Button] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Gameover"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Gameover"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Gameover");
		m_CompiledShaders[ePSO::Graphic::Gameover] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"DeathUI"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"DeathUI"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "DeathUI");
		m_CompiledShaders[ePSO::Graphic::DeathUI] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Logo"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Logo"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Logo");
		m_CompiledShaders[ePSO::Graphic::Logo] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"LobbyWindow"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"LobbyWindow"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "LobbyWindow");
		m_CompiledShaders[ePSO::Graphic::LobbyWindow] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"LobbyRoomSlot"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"LobbyRoomSlot"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "LobbyRoomSlot");
		m_CompiledShaders[ePSO::Graphic::LobbyRoomSlot] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(filename(L"Title"), vs_func_name, vs_ver);
		cs.pixel = CompileShaderFromFile(filename(L"Title"), ps_func_name, ps_ver);
		CompiledShaderToCacheFile(cs, "Title");
		m_CompiledShaders[ePSO::Graphic::Title] = cs;

		LPCWSTR shadow_file = filename(L"Shadow");
		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(shadow_file, "VS_ShadowMap_Stage", vs_ver);
		cs.pixel = CompileShaderFromFile(shadow_file, "PS_ShadowMap_NULL", ps_ver);
		CompiledShaderToCacheFile(cs, "ShadowMapStage");
		m_CompiledShaders[ePSO::Graphic::ShadowMapStage] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(shadow_file, "VS_ShadowMap_Cube", vs_ver);
		cs.pixel = CompileShaderFromFile(shadow_file, "PS_ShadowMap_NULL", ps_ver);
		CompiledShaderToCacheFile(cs, "ShadowMapCube");
		m_CompiledShaders[ePSO::Graphic::ShadowMapCube] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(shadow_file, "VS_ShadowMap_Model", vs_ver);
		cs.pixel = CompileShaderFromFile(shadow_file, "PS_ShadowMap_NULL", ps_ver);
		CompiledShaderToCacheFile(cs, "ShadowMapModel");
		m_CompiledShaders[ePSO::Graphic::ShadowMapModel] = cs;

		::ZeroMemory(&cs, sizeof(cs));
		cs.vertex = CompileShaderFromFile(shadow_file, "VS_ShadowMap_AnimMesh", vs_ver);
		cs.pixel = CompileShaderFromFile(shadow_file, "PS_ShadowMap_NULL", ps_ver);
		CompiledShaderToCacheFile(cs, "ShadowMapSkinnedAnim");
		m_CompiledShaders[ePSO::Graphic::ShadowMapSkinnedAnim] = cs;
	}
#endif
	th.join();
	// Shader
	{
		m_Shaders[ePSO::Graphic::SkyBox] = make_shared<CSkyBoxShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Stage] = make_shared<CStageShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Cube] = make_shared<CCubeShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Model] = make_shared<CModelShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::SkinnedAnim] = make_shared<CAnimationShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::DeferredSkyBox] = make_shared<CSkyBoxDeferredShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::DeferredStage] = make_shared<CStageDeferredShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::DeferredCube] = make_shared<CCubeDeferredShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::DeferredModel] = make_shared<CModelDeferredShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::DeferredSkinnedAnim] = make_shared<CAnimationDeferredShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::DeferredLighting] = make_shared<CDeferredLightingShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Billboard] = make_shared<CParticleBillboardShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::ParticleBillboardDefault] = make_shared<CAdvancedParticleAlphaBlendBillboardShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::ParticleBillboardAdditive] = make_shared<CAdvancedParticleAdditiveBillboardShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::PlayerUI] = make_shared<CPlayerUIShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::HUD_UI] = make_shared<CHUDUIShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::RADER_UI] = make_shared<CRaderUIShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Skill_Icon] = make_shared<CSkillIconShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Crosshair] = make_shared<CCrossHairShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Font] = make_shared<CFontShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Logo] = make_shared<CLogoShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Title] = make_shared<CTitleShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::LobbyWindow] = make_shared<CLobbyWindowShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::LobbyRoomSlot] = make_shared<CLobbyRoomSlotShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Button] = make_shared<CButtonShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::Gameover] = make_shared<CGameoverShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::DeathUI] = make_shared<CDeathUIShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::ShadowMapStage] = make_shared<CStageShadowMapShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::ShadowMapCube] = make_shared<CCubeShadowMapShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::ShadowMapModel] = make_shared<CModelShadowMapShader>(pd3dDevice);
		m_Shaders[ePSO::Graphic::ShadowMapSkinnedAnim] = make_shared<CAnimShadowMapShader>(pd3dDevice);
	}
	// Material
	{
		shared_ptr<CMaterial> pMaterial{ nullptr };

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::SkyBox));
		//pMaterial->SetShader(GetShader(ePSO::Graphic::SkyBox));
		pMaterial->SetShader(GetShader(ePSO::Graphic::DeferredSkyBox));
		m_Materials[eMat::Object::SkyBox] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::Stage));
		//pMaterial->SetShader(GetShader(ePSO::Graphic::Stage));
		pMaterial->SetShader(GetShader(ePSO::Graphic::DeferredStage));
		pMaterial->SetShadowMapShader(GetShader(ePSO::Graphic::ShadowMapStage));
		pMaterial->SetGlobalMaterialIdx(0);
		m_Materials[eMat::Object::Stage] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::Ruby));
		//pMaterial->SetShader(GetShader(ePSO::Graphic::SkinnedAnim));
		pMaterial->SetShader(GetShader(ePSO::Graphic::DeferredSkinnedAnim));
		pMaterial->SetShadowMapShader(GetShader(ePSO::Graphic::ShadowMapSkinnedAnim));
		m_Materials[eMat::Object::Ruby] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::Legion));
		//pMaterial->SetShader(GetShader(ePSO::Graphic::SkinnedAnim));
		pMaterial->SetShader(GetShader(ePSO::Graphic::DeferredSkinnedAnim));
		pMaterial->SetShadowMapShader(GetShader(ePSO::Graphic::ShadowMapSkinnedAnim));
		m_Materials[eMat::Object::Legion] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::Epsilon));
		//pMaterial->SetShader(GetShader(ePSO::Graphic::SkinnedAnim));
		pMaterial->SetShader(GetShader(ePSO::Graphic::DeferredSkinnedAnim));
		pMaterial->SetShadowMapShader(GetShader(ePSO::Graphic::ShadowMapSkinnedAnim));
		m_Materials[eMat::Object::Epsilon] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::Gravis));
		//pMaterial->SetShader(GetShader(ePSO::Graphic::SkinnedAnim));
		pMaterial->SetShader(GetShader(ePSO::Graphic::DeferredSkinnedAnim));
		pMaterial->SetShadowMapShader(GetShader(ePSO::Graphic::ShadowMapSkinnedAnim));
		m_Materials[eMat::Object::Gravis] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::Ruby_Bullet));
		//pMaterial->SetShader(GetShader(ePSO::Graphic::Model));
		pMaterial->SetShader(GetShader(ePSO::Graphic::DeferredModel));
		pMaterial->SetShadowMapShader(GetShader(ePSO::Graphic::ShadowMapModel));
		m_Materials[eMat::Object::Ruby_Bullet] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::PlayerUI));
		pMaterial->SetShader(GetShader(ePSO::Graphic::PlayerUI));
		m_Materials[eMat::Object::PlayerUI] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::Crosshair));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Crosshair));
		m_Materials[eMat::Object::Crosshair] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::DashSkillIcon));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Skill_Icon));
		m_Materials[eMat::Object::DashSkillIcon] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::JumpSkillIcon));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Skill_Icon));
		m_Materials[eMat::Object::JumpSkillIcon] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::RubySkillIcon));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Skill_Icon));
		m_Materials[eMat::Object::RubySkillIcon] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::LegionSkillIcon));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Skill_Icon));
		m_Materials[eMat::Object::LegionSkillIcon] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::EpsilonSkillIcon));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Skill_Icon));
		m_Materials[eMat::Object::EpsilonSkillIcon] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::GravisSkillIcon));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Skill_Icon));
		m_Materials[eMat::Object::GravisSkillIcon] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::LobbyLogo));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Title));
		m_Materials[eMat::Object::LobbyLogo] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::ConnectButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::ConnectButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::ExitButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::ExitButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::CreateButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::CreateButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::EnterButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::EnterButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::LeaveButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::LeaveButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::StartButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::StartButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::RecycleButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::RecycleButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::RubyButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::RubyButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::LegionButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::LegionButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::EpsilonButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::EpsilonButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::GravisButton));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Button));
		m_Materials[eMat::Object::GravisButton] = pMaterial;

		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::GameEnd));
		pMaterial->SetShader(GetShader(ePSO::Graphic::Gameover));
		m_Materials[eMat::Object::GameEnd] = pMaterial;


		pMaterial = make_shared<CMaterial>();
		pMaterial->SetTexture(GetTexture(eMat::Object::DeathUI));
		pMaterial->SetShader(GetShader(ePSO::Graphic::DeathUI));
		m_Materials[eMat::Object::DeathUI] = pMaterial;
	}
	cout << "resource load done\n";
	// FBX
	{
		const int StageMeshType		{ 0 };
		const int CubeMeshType		{ 1 };
		const int ModelMeshType		{ 2 };
		const int AnimMeshType		{ 3 };
		auto read_fbx = [&](const char* file_name, int mesh_idx, int type) {
			FBXExporter FBXLoader;
			FBXLoader.Initialize();
			if (!FBXLoader.LoadScene(file_name)) {
				cout << "fbx file load fail \n";
				exit(0);
			}
			FBXLoader.ExportFBX();
			switch (type)
			{
			case 0: 
			{
				for (int i = 0; i < mesh_idx; ++i) {
					CStageMesh * pMesh = new CStageMesh(pd3dCmdLst, FBXLoader, i);
					delete pMesh;
				}
				break;
			}
			case 1: 
			{
				CCubeMesh * pMesh = new CCubeMesh(pd3dCmdLst, FBXLoader, mesh_idx);
				delete pMesh;
				break;
			}
			case 2: 
			{
				CModelMesh * pMesh = new CModelMesh(pd3dCmdLst, FBXLoader, mesh_idx);
				delete pMesh;
				break;
			}
			case 3: 
			{
				CAnimationModelMesh * pMesh = new CAnimationModelMesh(pd3dCmdLst, FBXLoader, mesh_idx);
				delete pMesh;
				Skeleton* pSkel = new Skeleton();
				pSkel->Init(FBXLoader, 0);
				CAnimation* pAnimation = new CAnimation(pSkel->nBones, FBXLoader, mesh_idx);
				delete pSkel;
				delete pAnimation;
				break;
			}
			}
		};

		//read_fbx("../../../Assets/Animations/FBX/1/Hammering.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/1/Shooting.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/1/Hovering.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/1/Idle.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/1/Running.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/2/Idle.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/2/Hovering.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/2/Shield Charge.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/2/Spear Charge.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/2/Running.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/3/Idle.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/3/Hovering.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/3/Shooting.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/3/Shooting_Skill.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/3/Running.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/4/Idle.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/4/Hovering.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/4/Attack.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/4/Running.FBX", 0, AnimMeshType);
		//read_fbx("../../../Assets/Animations/FBX/GroundBlock.FBX", 0, CubeMeshType);
		//read_fbx("../../../Assets/3DModel/Animations/FBX/cube.FBX", 0, CubeMeshType);
		//read_fbx("../../../Assets/3DModel/Final_Resources/HighCube.FBX", 0, CubeMeshType);
		//read_fbx("../../../Assets/3DModel/Final_Resources/Weapon/HammerBullet.FBX", 0, ModelMeshType);
		//read_fbx("../../../Assets/3DModel/Final_Resources/WorldChunk/island_basic01.FBX", 0, StageMeshType);
		//read_fbx("../../../Assets/3DModel/Final_Resources/WorldChunk/test/OutPostBreakable.FBX", 563, StageMeshType);
		//read_fbx("../../../Assets/3DModel/Final_Resources/WorldChunk/outpost_model.fbx", 0, StageMeshType);
	}
}

void CResMgr::BuildDescs()
{
	// InputLayout
	{
		shared_ptr<InputLayout> pIL{ nullptr };
		pIL = make_shared<InputLayout>(2);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::SkyBox] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(2);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::Cube] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(4);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(2, { "CURRENT_IMAGE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(3, { "DIVIDED_TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::Billboard] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(4);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(2, { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(3, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::Model] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(7);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(2, { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(3, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(4, { "WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(5, { "BONEINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(6, { "MATERIAL", 0, DXGI_FORMAT_R8_UINT, 0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::SkinnedAnim] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(8);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(2, { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(3, { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(4, { "OPTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(5, { "VALUE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(6, { "CUSTOMCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 76, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(7, { "SIZEMULTI", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 92, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::ParticleBillboard] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(5);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(2, { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(3, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(4, { "MATERIAL", 0, DXGI_FORMAT_R8_UINT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::Stage] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(2);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::PlayerUI] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(3);
		pIL->SetElement(0, { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(1, { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		pIL->SetElement(2, { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayoutDescs[eDESC::InputLayout::Font] = pIL;
		pIL.reset();

		pIL = make_shared<InputLayout>(0);
		m_InputLayoutDescs[eDESC::InputLayout::None] = pIL;
		pIL.reset();
	}
	// Rasterizer
	{
		D3D12_RASTERIZER_DESC d3dRasterizerDesc;

		::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		d3dRasterizerDesc.FrontCounterClockwise = FALSE;
		d3dRasterizerDesc.DepthBias = 0;
		d3dRasterizerDesc.DepthBiasClamp = 0.0f;
		d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
		d3dRasterizerDesc.DepthClipEnable = TRUE;
		d3dRasterizerDesc.MultisampleEnable = TRUE;
		d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
		d3dRasterizerDesc.ForcedSampleCount = 0;
		d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		m_RasterizerDescs[eDESC::Rasterizer::Default] = d3dRasterizerDesc;

		::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID; // D3D12_FILL_MODE_WIREFRAME
		d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_NONE; // D3D12_CULL_MODE_BACK ||| D3D12_CULL_MODE_FRONT
		d3dRasterizerDesc.FrontCounterClockwise = FALSE;
		d3dRasterizerDesc.DepthBias = 0;
		d3dRasterizerDesc.DepthBiasClamp = 0.0f;
		d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
		d3dRasterizerDesc.DepthClipEnable = TRUE;
		d3dRasterizerDesc.MultisampleEnable = FALSE;
		d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
		d3dRasterizerDesc.ForcedSampleCount = 0;
		d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		m_RasterizerDescs[eDESC::Rasterizer::CullModeNone] = d3dRasterizerDesc;

		::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		d3dRasterizerDesc.FrontCounterClockwise = FALSE;
		d3dRasterizerDesc.DepthBias = 1200;
		d3dRasterizerDesc.DepthBiasClamp = 0.0f;
		d3dRasterizerDesc.SlopeScaledDepthBias = 1.0f;
		d3dRasterizerDesc.DepthClipEnable = TRUE;
		d3dRasterizerDesc.MultisampleEnable = FALSE;
		d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
		d3dRasterizerDesc.ForcedSampleCount = 0;
		d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		m_RasterizerDescs[eDESC::Rasterizer::ShadowMap] = d3dRasterizerDesc;
	}
	// Blend
	{
		D3D12_BLEND_DESC d3dBlendDesc;

		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		for (int i = 0; i < 8; ++i) {
			d3dBlendDesc.RenderTarget[i].BlendEnable = FALSE;
			d3dBlendDesc.RenderTarget[i].LogicOpEnable = FALSE;
			d3dBlendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
			d3dBlendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
			d3dBlendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
			d3dBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		m_BlendDescs[eDESC::Blend::Default] = d3dBlendDesc;

		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		for (int i = 0; i < 8; ++i) {
			d3dBlendDesc.RenderTarget[i].BlendEnable = TRUE;
			d3dBlendDesc.RenderTarget[i].LogicOpEnable = FALSE;
			d3dBlendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
			d3dBlendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
			d3dBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		m_BlendDescs[eDESC::Blend::Alpha] = d3dBlendDesc;

		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		for (int i = 0; i < 8; ++i) {
			d3dBlendDesc.RenderTarget[i].BlendEnable = TRUE;
			d3dBlendDesc.RenderTarget[i].LogicOpEnable = FALSE;
			d3dBlendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
			d3dBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		m_BlendDescs[eDESC::Blend::UIAlpha] = d3dBlendDesc;

		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		for (int i = 0; i < 8; ++i) {
			d3dBlendDesc.RenderTarget[i].BlendEnable = TRUE;
			d3dBlendDesc.RenderTarget[i].LogicOpEnable = FALSE;
			d3dBlendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
			d3dBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		m_BlendDescs[eDESC::Blend::Font] = d3dBlendDesc;

		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		for (int i = 0; i < 8; ++i) {
			d3dBlendDesc.RenderTarget[i].BlendEnable = TRUE;
			d3dBlendDesc.RenderTarget[i].LogicOpEnable = FALSE;
			d3dBlendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			d3dBlendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
			d3dBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		m_BlendDescs[eDESC::Blend::ParticleAlpha] = d3dBlendDesc;

		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		for (int i = 0; i < 8; ++i) {
			d3dBlendDesc.RenderTarget[i].BlendEnable = TRUE;
			d3dBlendDesc.RenderTarget[i].LogicOpEnable = FALSE;
			d3dBlendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
			d3dBlendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
			d3dBlendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			d3dBlendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
			d3dBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		m_BlendDescs[eDESC::Blend::Additive] = d3dBlendDesc;
	}
	// DepthStencil
	{
		D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;

		::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
		d3dDepthStencilDesc.DepthEnable = TRUE;
		d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		d3dDepthStencilDesc.StencilEnable = TRUE;
		d3dDepthStencilDesc.StencilReadMask = 0xff;
		d3dDepthStencilDesc.StencilWriteMask = 0xff;
		d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
		d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
		d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		m_DepthStencilDescs[eDESC::DepthStencil::DepthStencilDefault] = d3dDepthStencilDesc;

		::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
		d3dDepthStencilDesc.DepthEnable = TRUE;
		d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		d3dDepthStencilDesc.StencilEnable = FALSE;
		d3dDepthStencilDesc.StencilReadMask = 0x00;
		d3dDepthStencilDesc.StencilWriteMask = 0x00;
		d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		m_DepthStencilDescs[eDESC::DepthStencil::DepthTestDefalut] = d3dDepthStencilDesc;

		::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
		d3dDepthStencilDesc.DepthEnable = TRUE;
		d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		d3dDepthStencilDesc.StencilEnable = FALSE;
		d3dDepthStencilDesc.StencilReadMask = 0x00;
		d3dDepthStencilDesc.StencilWriteMask = 0x00;
		d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		m_DepthStencilDescs[eDESC::DepthStencil::DepthTestNoWriteDepth] = d3dDepthStencilDesc;

		::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
		d3dDepthStencilDesc.DepthEnable = FALSE;
		d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
		d3dDepthStencilDesc.StencilEnable = FALSE;
		d3dDepthStencilDesc.StencilReadMask = 0x00;
		d3dDepthStencilDesc.StencilWriteMask = 0x00;
		d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
		d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
		d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		m_DepthStencilDescs[eDESC::DepthStencil::BothDisable] = d3dDepthStencilDesc;
	}
	// StaticSampler
	{
		D3D12_STATIC_SAMPLER_DESC d3dSamplerDesc;

		::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
		d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.MipLODBias = 0;
		d3dSamplerDesc.MaxAnisotropy = 16;
		d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		d3dSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		d3dSamplerDesc.MinLOD = 0;
		d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		d3dSamplerDesc.ShaderRegister = 0;
		d3dSamplerDesc.RegisterSpace = 0;
		d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		m_SamplerDescs[eDESC::StaticSampler::LinearWrap] = d3dSamplerDesc;

		::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
		d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.MipLODBias = 0;
		d3dSamplerDesc.MaxAnisotropy = 16;
		d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		d3dSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		d3dSamplerDesc.MinLOD = 0;
		d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		d3dSamplerDesc.ShaderRegister = 1;
		d3dSamplerDesc.RegisterSpace = 0;
		d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		m_SamplerDescs[eDESC::StaticSampler::LinearClamp] = d3dSamplerDesc;

		::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
		d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.MipLODBias = 0;
		d3dSamplerDesc.MaxAnisotropy = 16;
		d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		d3dSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		d3dSamplerDesc.MinLOD = 0;
		d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		d3dSamplerDesc.ShaderRegister = 2;
		d3dSamplerDesc.RegisterSpace = 0;
		d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		m_SamplerDescs[eDESC::StaticSampler::PointWrap] = d3dSamplerDesc;

		::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
		d3dSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.MipLODBias = 0;
		d3dSamplerDesc.MaxAnisotropy = 16;
		d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		d3dSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		d3dSamplerDesc.MinLOD = 0;
		d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		d3dSamplerDesc.ShaderRegister = 3;
		d3dSamplerDesc.RegisterSpace = 0;
		d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		m_SamplerDescs[eDESC::StaticSampler::PointClamp] = d3dSamplerDesc;

		::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
		d3dSamplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
		d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		d3dSamplerDesc.MipLODBias = 0;
		d3dSamplerDesc.MaxAnisotropy = 8;
		d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		d3dSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		d3dSamplerDesc.MinLOD = 0;
		d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		d3dSamplerDesc.ShaderRegister = 4;
		d3dSamplerDesc.RegisterSpace = 0;
		d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		m_SamplerDescs[eDESC::StaticSampler::AnisotropicWrap] = d3dSamplerDesc;

		::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
		d3dSamplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
		d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		d3dSamplerDesc.MipLODBias = 0;
		d3dSamplerDesc.MaxAnisotropy = 8;
		d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		d3dSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		d3dSamplerDesc.MinLOD = 0;
		d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		d3dSamplerDesc.ShaderRegister = 5;
		d3dSamplerDesc.RegisterSpace = 0;
		d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		m_SamplerDescs[eDESC::StaticSampler::AnisotropicClamp] = d3dSamplerDesc;

		::ZeroMemory(&d3dSamplerDesc, sizeof(D3D12_STATIC_SAMPLER_DESC));
		d3dSamplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		d3dSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		d3dSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		d3dSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		d3dSamplerDesc.MipLODBias = 0;
		d3dSamplerDesc.MaxAnisotropy = 16;
		d3dSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		d3dSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		d3dSamplerDesc.MinLOD = 0;
		d3dSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		d3dSamplerDesc.ShaderRegister = 6;
		d3dSamplerDesc.RegisterSpace = 0;
		d3dSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		m_SamplerDescs[eDESC::StaticSampler::Shadow] = d3dSamplerDesc;
	}
}

void CResMgr::CreateRootSignature()
{
	D3D12_DESCRIPTOR_RANGE DescRanges[7];
	DescRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	DescRanges[0].NumDescriptors = 1;
	DescRanges[0].BaseShaderRegister = 0; // Resource1 t0
	DescRanges[0].RegisterSpace = 0;
	DescRanges[0].OffsetInDescriptorsFromTableStart = 0;

	DescRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	DescRanges[1].NumDescriptors = 1;
	DescRanges[1].BaseShaderRegister = 1; // Resource2 t1
	DescRanges[1].RegisterSpace = 0;
	DescRanges[1].OffsetInDescriptorsFromTableStart = 0;

	DescRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	DescRanges[2].NumDescriptors = 10;
	DescRanges[2].BaseShaderRegister = 2; // Texture1 t2 ~ t11
	DescRanges[2].RegisterSpace = 0;
	DescRanges[2].OffsetInDescriptorsFromTableStart = 0;

	DescRanges[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	DescRanges[3].NumDescriptors = 10;
	DescRanges[3].BaseShaderRegister = 12; // Texture2 t12 ~ t21
	DescRanges[3].RegisterSpace = 0;
	DescRanges[3].OffsetInDescriptorsFromTableStart = 0;

	DescRanges[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	DescRanges[4].NumDescriptors = 10;
	DescRanges[4].BaseShaderRegister = 22; // Texture3 t22 ~ t31
	DescRanges[4].RegisterSpace = 0;
	DescRanges[4].OffsetInDescriptorsFromTableStart = 0;

	DescRanges[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	DescRanges[5].NumDescriptors = 10;
	DescRanges[5].BaseShaderRegister = 32; // Texture4 t32 ~ t41
	DescRanges[5].RegisterSpace = 0;
	DescRanges[5].OffsetInDescriptorsFromTableStart = 0;

	DescRanges[6].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	DescRanges[6].NumDescriptors = 10;
	DescRanges[6].BaseShaderRegister = 42; // Texture5 t42 ~ t51
	DescRanges[6].RegisterSpace = 0;
	DescRanges[6].OffsetInDescriptorsFromTableStart = 0;
	
	m_pRootSignature = make_shared<RootSignature>(ROOT_PARAMETER_COUNT, sizeof(DescRanges) / sizeof(D3D12_DESCRIPTOR_RANGE));

	D3D12_ROOT_PARAMETER RootParam;
	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	RootParam.Descriptor.ShaderRegister = 0; // Materials b0
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_MATERIAL, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	RootParam.Descriptor.ShaderRegister = 1; // Lights b1
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_LIGHT, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	RootParam.Descriptor.ShaderRegister = 2; // Camera b2
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_CAMERA, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	RootParam.Descriptor.ShaderRegister = 3; // GameObjects b3
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_OBJECT, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	RootParam.Descriptor.ShaderRegister = 4; // Constant1 b4
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_CONSTANT1, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	RootParam.Descriptor.ShaderRegister = 5; // Constant2 b5
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_CONSTANT2, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	RootParam.Descriptor.ShaderRegister = 6; // Constant3 b6
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_CONSTANT3, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam.DescriptorTable.NumDescriptorRanges = 1;
	RootParam.DescriptorTable.pDescriptorRanges = &DescRanges[0]; // Resource1 t0
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_RESOURCE1, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam.DescriptorTable.NumDescriptorRanges = 1;
	RootParam.DescriptorTable.pDescriptorRanges = &DescRanges[1]; // Resource2 t1
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_RESOURCE2, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam.DescriptorTable.NumDescriptorRanges = 1;
	RootParam.DescriptorTable.pDescriptorRanges = &DescRanges[2]; // Texture1 t2 ~ t11
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_TEXTURE1, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam.DescriptorTable.NumDescriptorRanges = 1;
	RootParam.DescriptorTable.pDescriptorRanges = &DescRanges[3]; // Texture2 t12 ~ t21
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_TEXTURE2, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam.DescriptorTable.NumDescriptorRanges = 1;
	RootParam.DescriptorTable.pDescriptorRanges = &DescRanges[4]; // Texture3 t22 ~ t31
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_TEXTURE3, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam.DescriptorTable.NumDescriptorRanges = 1;
	RootParam.DescriptorTable.pDescriptorRanges = &DescRanges[5]; // Texture4 t32 ~ t41
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_TEXTURE4, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam.DescriptorTable.NumDescriptorRanges = 1;
	RootParam.DescriptorTable.pDescriptorRanges = &DescRanges[6]; // Texture5 t42 ~ t51
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_TEXTURE5, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	RootParam.Descriptor.ShaderRegister = 0; // UA Resource1 u0
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_UA_RESOURCE1, RootParam);

	RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	RootParam.Descriptor.ShaderRegister = 1; // UA Resource2 u1
	RootParam.Descriptor.RegisterSpace = 0;
	RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	m_pRootSignature->SetRootParam(ROOT_PARAMETER_UA_RESOURCE2, RootParam);

	m_pRootSignature->SetStaticSamplerParam(0, m_SamplerDescs[eDESC::StaticSampler::LinearWrap ]);
	m_pRootSignature->SetStaticSamplerParam(1, m_SamplerDescs[eDESC::StaticSampler::LinearClamp]);
	m_pRootSignature->SetStaticSamplerParam(2, m_SamplerDescs[eDESC::StaticSampler::PointWrap]);
	m_pRootSignature->SetStaticSamplerParam(3, m_SamplerDescs[eDESC::StaticSampler::PointClamp]);
	m_pRootSignature->SetStaticSamplerParam(4, m_SamplerDescs[eDESC::StaticSampler::AnisotropicWrap]);
	m_pRootSignature->SetStaticSamplerParam(5, m_SamplerDescs[eDESC::StaticSampler::AnisotropicClamp]);
	m_pRootSignature->SetStaticSamplerParam(6, m_SamplerDescs[eDESC::StaticSampler::Shadow]);

	m_pRootSignature->CreateRootSignature(m_pd3dDevice.Get());
}

void CResMgr::Release()
{
	m_pd3dDevice.Reset();

	m_pRootSignature.reset();
	m_pShadowMap.reset();

	m_Diffusetexes.clear();
	m_Normaltexes.clear();
	m_Speculartexes.clear();
	m_Glossinesstexes.clear();
	m_Emissivetexes.clear();

	m_SkyBoxtexes.clear();
	m_Billboardtexes.clear();
	m_Buttontexes.clear();

	m_SkillColorIcontexes.clear();
	m_SkillGreyIcontexes.clear();
	m_Fonttexes.clear();

	m_Meshes.clear();
	m_StageMeshes.clear();
	m_Skeletons.clear();
	m_Animations.clear();
	m_Shaders.clear();
	m_Textures.clear();
	m_Materials.clear();

	for (auto p : m_CacheShaders) {
		delete p.vertex.pShaderBytecode;
		delete p.domain.pShaderBytecode;
		delete p.geomery.pShaderBytecode;
		delete p.hull.pShaderBytecode;
		delete p.pixel.pShaderBytecode;
		delete p.compute.pShaderBytecode;
	}
	m_CacheShaders.clear();
	m_CompiledShaderBlobs.clear();
	m_CompiledShaders.clear();

	m_InputLayoutDescs.clear();
	m_RasterizerDescs.clear();
	m_BlendDescs.clear();
	m_DepthStencilDescs.clear();
	m_SamplerDescs.clear();
}

D3D12_SHADER_RESOURCE_VIEW_DESC CResMgr::GetShaderResourceViewDesc(
	  D3D12_RESOURCE_DESC		d3dResourceDesc
	, eTex::Type				eTextureType)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
	d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	switch (eTextureType)
	{
	case eTex::Type::RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
	case eTex::Type::RESOURCE_TEXTURE2D_ARRAY:
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		d3dShaderResourceViewDesc.Texture2D.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		break;
	case  eTex::Type::RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dShaderResourceViewDesc.Texture2DArray.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
		d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
		break;
	case  eTex::Type::RESOURCE_TEXTURE_CUBE: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 6)
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		d3dShaderResourceViewDesc.TextureCube.MipLevels = -1;
		d3dShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		break;
	case  eTex::Type::RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		d3dShaderResourceViewDesc.Buffer.FirstElement = 0;
		d3dShaderResourceViewDesc.Buffer.NumElements = 0;
		d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
		d3dShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		break;
	}
	return(d3dShaderResourceViewDesc);
}

D3D12_SHADER_BYTECODE CResMgr::CompileShaderFromFile(
	  LPCWSTR		pFileName
	, LPCSTR		pShaderFuncName
	, LPCSTR		pShaderType_ver)
{
	ComPtr<ID3DBlob> pd3dShaderBlob	{ nullptr };
	ComPtr<ID3DBlob> pd3dErrorBlob	{ nullptr };
	UINT nCompileFlags = 0;

#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(::D3DCompileFromFile(
		  pFileName
		, NULL
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, pShaderFuncName
		, pShaderType_ver
		, nCompileFlags
		, 0
		, &pd3dShaderBlob
		, &pd3dErrorBlob)))
	{
		int size = pd3dErrorBlob->GetBufferSize();
		void *ack = pd3dErrorBlob->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf_s(str, size, (const char*)ack, size);
			OutputDebugStringA(str);
			delete[] str;
		}
		MessageBox(
			nullptr
			, TEXT("D3DCompileFromFile() Failed.")
			, TEXT("Error!"), MB_OK);
	}

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength =  pd3dShaderBlob->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = pd3dShaderBlob->GetBufferPointer();

	m_CompiledShaderBlobs.push_back(pd3dShaderBlob);

	return(d3dShaderByteCode);
}

void CResMgr::CompiledShaderToCacheFile(const CompiledShader& compiled_shader, LPCSTR pFileName)
{
	std::string file_name = "./Assets/pchlsl/" + std::string(pFileName) + ".pchlsl";
	FILE* fp{ nullptr };
	fopen_s(&fp, file_name.c_str(), "wb");
	fwrite(&compiled_shader.vertex.BytecodeLength, sizeof(SIZE_T), 1, fp);
	if (compiled_shader.vertex.BytecodeLength != 0)
		fwrite(compiled_shader.vertex.pShaderBytecode, sizeof(BYTE), compiled_shader.vertex.BytecodeLength, fp);
	fwrite(&compiled_shader.domain.BytecodeLength, sizeof(SIZE_T), 1, fp);
	if (compiled_shader.domain.BytecodeLength != 0)
		fwrite(compiled_shader.domain.pShaderBytecode, sizeof(BYTE), compiled_shader.domain.BytecodeLength, fp);
	fwrite(&compiled_shader.geomery.BytecodeLength, sizeof(SIZE_T), 1, fp);
	if (compiled_shader.geomery.BytecodeLength != 0)
		fwrite(compiled_shader.geomery.pShaderBytecode, sizeof(BYTE), compiled_shader.geomery.BytecodeLength, fp);
	fwrite(&compiled_shader.hull.BytecodeLength, sizeof(SIZE_T), 1, fp);
	if (compiled_shader.hull.BytecodeLength != 0)
		fwrite(compiled_shader.hull.pShaderBytecode, sizeof(BYTE), compiled_shader.hull.BytecodeLength, fp);
	fwrite(&compiled_shader.pixel.BytecodeLength, sizeof(SIZE_T), 1, fp);
	if (compiled_shader.pixel.BytecodeLength != 0)
		fwrite(compiled_shader.pixel.pShaderBytecode, sizeof(BYTE), compiled_shader.pixel.BytecodeLength, fp);
	fwrite(&compiled_shader.compute.BytecodeLength, sizeof(SIZE_T), 1, fp);
	if (compiled_shader.compute.BytecodeLength != 0)
		fwrite(compiled_shader.compute.pShaderBytecode, sizeof(BYTE), compiled_shader.compute.BytecodeLength, fp);
	fclose(fp);
}

CompiledShader CResMgr::CompiledShaderFromCacheFile(LPCSTR pFileName)
{
	CompiledShader compiled_shader;
	SIZE_T nByteCodeSize	{ 0 };
	BYTE* pByteCode			{ nullptr };
	FILE* fp				{ nullptr };
	fopen_s(&fp, pFileName, "rb");
	fread_s(&nByteCodeSize, sizeof(SIZE_T), sizeof(SIZE_T), 1, fp);
	if (nByteCodeSize != 0) {
		pByteCode = new BYTE[nByteCodeSize];
		fread_s(pByteCode, nByteCodeSize, sizeof(BYTE), nByteCodeSize, fp);
	}
	else {
		pByteCode = nullptr;
	}
	compiled_shader.vertex.BytecodeLength = nByteCodeSize;
	compiled_shader.vertex.pShaderBytecode = pByteCode;
	fread_s(&nByteCodeSize, sizeof(SIZE_T), sizeof(SIZE_T), 1, fp);
	if (nByteCodeSize != 0) {
		pByteCode = new BYTE[nByteCodeSize];
		fread_s(pByteCode, nByteCodeSize, sizeof(BYTE), nByteCodeSize, fp);
	}
	else {
		pByteCode = nullptr;
	}
	compiled_shader.domain.BytecodeLength = nByteCodeSize;
	compiled_shader.domain.pShaderBytecode = pByteCode;
	fread_s(&nByteCodeSize, sizeof(SIZE_T), sizeof(SIZE_T), 1, fp);
	if (nByteCodeSize != 0) {
		pByteCode = new BYTE[nByteCodeSize];
		fread_s(pByteCode, nByteCodeSize, sizeof(BYTE), nByteCodeSize, fp);
	}
	else {
		pByteCode = nullptr;
	}
	compiled_shader.geomery.BytecodeLength = nByteCodeSize;
	compiled_shader.geomery.pShaderBytecode = pByteCode;
	fread_s(&nByteCodeSize, sizeof(SIZE_T), sizeof(SIZE_T), 1, fp);
	if (nByteCodeSize != 0) {
		pByteCode = new BYTE[nByteCodeSize];
		fread_s(pByteCode, nByteCodeSize, sizeof(BYTE), nByteCodeSize, fp);
	}
	else {
		pByteCode = nullptr;
	}
	compiled_shader.hull.BytecodeLength = nByteCodeSize;
	compiled_shader.hull.pShaderBytecode = pByteCode;
	fread_s(&nByteCodeSize, sizeof(SIZE_T), sizeof(SIZE_T), 1, fp);
	if (nByteCodeSize != 0) {
		pByteCode = new BYTE[nByteCodeSize];
		fread_s(pByteCode, nByteCodeSize, sizeof(BYTE), nByteCodeSize, fp);
	}
	else {
		pByteCode = nullptr;
	}
	compiled_shader.pixel.BytecodeLength = nByteCodeSize;
	compiled_shader.pixel.pShaderBytecode = pByteCode;
	fread_s(&nByteCodeSize, sizeof(SIZE_T), sizeof(SIZE_T), 1, fp);
	if (nByteCodeSize != 0) {
		pByteCode = new BYTE[nByteCodeSize];
		fread_s(pByteCode, nByteCodeSize, sizeof(BYTE), nByteCodeSize, fp);
	}
	else {
		pByteCode = nullptr;
	}
	compiled_shader.compute.BytecodeLength = nByteCodeSize;
	compiled_shader.compute.pShaderBytecode = pByteCode;
	fclose(fp);
	m_CacheShaders.push_back(compiled_shader);
	return compiled_shader;
}

DXGI_SAMPLE_DESC CResMgr::GetSampleDesc()
{
	UINT count = static_cast<UINT>((m_bMsaa4xEnable) ? 4 : 1);
	UINT quality = static_cast<UINT>((m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0);
	return DXGI_SAMPLE_DESC{ count, quality };
}

shared_ptr<CAnimCtrlr> CResMgr::GetAnimCtrlr(eGeo::AnimCtrlr type)
{
	shared_ptr<CAnimCtrlr> pAnimCtrlr{ nullptr };
	switch (type)
	{
	case eGeo::AnimCtrlr::Ruby:
	{
		pAnimCtrlr = make_shared<CAnimCtrlr>();
		pAnimCtrlr->SetSkeleton(GetSkeleton(eGeo::Skel::Ruby));

		const auto& pAnimIdle		{ GetAnimation(eGeo::Anim::Ruby_Idle) };
		const auto& pAnimRunning	{ GetAnimation(eGeo::Anim::Ruby_Running) };
		const auto& pAnimHovering	{ GetAnimation(eGeo::Anim::Ruby_Hovering) };
		const auto& pAnimHammering	{ GetAnimation(eGeo::Anim::Ruby_Hammering) };
		const auto& pAnimShoot		{ GetAnimation(eGeo::Anim::Ruby_Shoot) };

		CAnimState Idle;
		Idle.SetAnim(pAnimIdle);
		Idle.SetParam(CAnimCtrlr::Tag::Idle, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, true, true);
		Idle.SetNextState(CAnimCtrlr::Tag::Idle);
		Idle.SetNextState(CAnimCtrlr::Tag::MoveStart);
		Idle.SetNextState(CAnimCtrlr::Tag::TakeOff);
		Idle.SetNextState(CAnimCtrlr::Tag::Jump);
		Idle.SetNextState(CAnimCtrlr::Tag::Dash);
		Idle.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Idle.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Idle.SetNextState(CAnimCtrlr::Tag::Skill);
		Idle.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Idle, Idle);
		CAnimState MoveStart;
		MoveStart.SetAnim(pAnimRunning);
		MoveStart.SetParam(CAnimCtrlr::Tag::MoveStart, 0.0f, 0.25f, 0.0f, 0.0f, 0.2f, true, false);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Idle);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Move);
		MoveStart.SetNextState(CAnimCtrlr::Tag::TakeOff);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Jump);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Dash);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Attack_1);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Attack_2);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Skill);
		MoveStart.SetDefaultNextState(CAnimCtrlr::Tag::Move);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::MoveStart, MoveStart);
		CAnimState Move;
		Move.SetAnim(pAnimRunning);
		Move.SetParam(CAnimCtrlr::Tag::Move, 0.25f, 0.75f, 0.25f, 0.75f, 0.8f, true, true);
		Move.SetNextState(CAnimCtrlr::Tag::Idle);
		Move.SetNextState(CAnimCtrlr::Tag::Move);
		Move.SetNextState(CAnimCtrlr::Tag::MoveEnd);
		Move.SetNextState(CAnimCtrlr::Tag::TakeOff);
		Move.SetNextState(CAnimCtrlr::Tag::Jump);
		Move.SetNextState(CAnimCtrlr::Tag::Dash);
		Move.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Move.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Move.SetNextState(CAnimCtrlr::Tag::Skill);
		Move.SetDefaultNextState(CAnimCtrlr::Tag::MoveEnd);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Move, Move);
		CAnimState MoveEnd;
		MoveEnd.SetAnim(pAnimRunning);
		MoveEnd.SetParam(CAnimCtrlr::Tag::MoveEnd, 0.75f, 1.0f, 0.0f, 0.0f, 0.2f, true, false);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Idle);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::TakeOff);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Jump);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Dash);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Attack_1);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Attack_2);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Skill);
		MoveEnd.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::MoveEnd, MoveEnd);
		CAnimState TakeOff;
		TakeOff.SetAnim(pAnimHovering);
		TakeOff.SetParam(CAnimCtrlr::Tag::TakeOff, 0.0f, 0.3f, 0.0f, 0.0f, 0.2f, true, false);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Hovering);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Landing);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Dash);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Attack_1);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Attack_2);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Skill);
		TakeOff.SetDefaultNextState(CAnimCtrlr::Tag::Hovering);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::TakeOff, TakeOff);
		CAnimState Hovering;
		Hovering.SetAnim(pAnimHovering);
		Hovering.SetParam(CAnimCtrlr::Tag::Hovering, 0.3f, 0.5f, 0.48f, 0.49f, 0.4f, true, true);
		Hovering.SetNextState(CAnimCtrlr::Tag::Hovering);
		Hovering.SetNextState(CAnimCtrlr::Tag::Landing);
		Hovering.SetNextState(CAnimCtrlr::Tag::Dash);
		Hovering.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Hovering.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Hovering.SetNextState(CAnimCtrlr::Tag::Skill);
		Hovering.SetDefaultNextState(CAnimCtrlr::Tag::Landing);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Hovering, Hovering);
		CAnimState Landing;
		Landing.SetAnim(pAnimHovering);
		Landing.SetParam(CAnimCtrlr::Tag::Landing, 0.5f, 1.0f, 0.6f, 0.6f, 0.2f, true, true);
		Landing.SetNextState(CAnimCtrlr::Tag::Idle);
		Landing.SetNextState(CAnimCtrlr::Tag::Dash);
		Landing.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Landing.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Landing.SetNextState(CAnimCtrlr::Tag::Skill);
		Landing.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Landing, Landing);
		CAnimState Jump;
		Jump.SetAnim(pAnimHovering);
		Jump.SetParam(CAnimCtrlr::Tag::Jump, 0.0f, 0.3f, 0.0f, 0.0f, 1.0f, false, false);
		Jump.SetNextState(CAnimCtrlr::Tag::Hovering);
		Jump.SetDefaultNextState(CAnimCtrlr::Tag::Hovering);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Jump, Jump);
		CAnimState Dash;
		Dash.SetAnim(pAnimHovering);
		Dash.SetParam(CAnimCtrlr::Tag::Dash, 0.3f, 0.5f, 0.48f, 0.49f, 0.8f, false, true);
		Dash.SetNextState(CAnimCtrlr::Tag::Idle);
		Dash.SetNextState(CAnimCtrlr::Tag::Dash);
		Dash.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Dash.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Dash.SetNextState(CAnimCtrlr::Tag::Skill);
		Dash.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Dash, Dash);
		CAnimState Attack_1;
		Attack_1.SetAnim(pAnimHammering);
		Attack_1.SetParam(CAnimCtrlr::Tag::Attack_1, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, false, true);
		Attack_1.SetNextState(CAnimCtrlr::Tag::Idle);
		Attack_1.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Attack_1.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Attack_1, Attack_1);
		CAnimState Attack_2;
		Attack_2.SetAnim(pAnimShoot);
		Attack_2.SetParam(CAnimCtrlr::Tag::Attack_2, 0.0f, 1.0f, 0.25f, 0.75f, 0.5f, false, true);
		Attack_2.SetNextState(CAnimCtrlr::Tag::Idle);
		Attack_2.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Attack_2.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Attack_2, Attack_2);
		CAnimState Skill;
		Skill.SetAnim(pAnimHammering);
		Skill.SetParam(CAnimCtrlr::Tag::Skill, 0.0f, 1.0f, 0.0f, 1.0f, 1.5f, false, false);
		Skill.SetNextState(CAnimCtrlr::Tag::Idle);
		Skill.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Skill, Skill);

		pAnimCtrlr->ChangeCurrAnim(CAnimCtrlr::Tag::Idle);
		break;
	}
	case eGeo::AnimCtrlr::Legion:
	{
		pAnimCtrlr = make_shared<CAnimCtrlr>();
		pAnimCtrlr->SetSkeleton(GetSkeleton(eGeo::Skel::Legion));

		const auto& pAnimIdle			{ GetAnimation(eGeo::Anim::Legion_Idle) };
		const auto& pAnimRunning		{ GetAnimation(eGeo::Anim::Legion_Running) };
		const auto& pAnimHovering		{ GetAnimation(eGeo::Anim::Legion_Hovering) };
		const auto& pAnimSpearCharging	{ GetAnimation(eGeo::Anim::Legion_SpearCharging) };
		const auto& pAnimShieldCharging	{ GetAnimation(eGeo::Anim::Legion_ShieldCharging) };

		CAnimState Idle;
		Idle.SetAnim(pAnimIdle);
		Idle.SetParam(CAnimCtrlr::Tag::Idle, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, true, true);
		Idle.SetNextState(CAnimCtrlr::Tag::Idle);
		Idle.SetNextState(CAnimCtrlr::Tag::MoveStart);
		Idle.SetNextState(CAnimCtrlr::Tag::TakeOff);
		Idle.SetNextState(CAnimCtrlr::Tag::Jump);
		Idle.SetNextState(CAnimCtrlr::Tag::Dash);
		Idle.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Idle.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Idle.SetNextState(CAnimCtrlr::Tag::Skill);
		Idle.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Idle, Idle);
		CAnimState MoveStart;
		MoveStart.SetAnim(pAnimRunning);
		MoveStart.SetParam(CAnimCtrlr::Tag::MoveStart, 0.0f, 0.25f, 0.0f, 0.0f, 0.2f, true, false);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Idle);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Move);
		MoveStart.SetNextState(CAnimCtrlr::Tag::TakeOff);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Jump);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Dash);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Attack_1);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Attack_2);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Skill);
		MoveStart.SetDefaultNextState(CAnimCtrlr::Tag::Move);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::MoveStart, MoveStart);
		CAnimState Move;
		Move.SetAnim(pAnimRunning);
		Move.SetParam(CAnimCtrlr::Tag::Move, 0.25f, 0.75f, 0.25f, 0.75f, 0.8f, true, true);
		Move.SetNextState(CAnimCtrlr::Tag::Idle);
		Move.SetNextState(CAnimCtrlr::Tag::Move);
		Move.SetNextState(CAnimCtrlr::Tag::MoveEnd);
		Move.SetNextState(CAnimCtrlr::Tag::TakeOff);
		Move.SetNextState(CAnimCtrlr::Tag::Jump);
		Move.SetNextState(CAnimCtrlr::Tag::Dash);
		Move.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Move.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Move.SetNextState(CAnimCtrlr::Tag::Skill);
		Move.SetDefaultNextState(CAnimCtrlr::Tag::MoveEnd);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Move, Move);
		CAnimState MoveEnd;
		MoveEnd.SetAnim(pAnimRunning);
		MoveEnd.SetParam(CAnimCtrlr::Tag::MoveEnd, 0.75f, 1.0f, 0.0f, 0.0f, 0.2f, true, false);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Idle);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::TakeOff);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Jump);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Dash);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Attack_1);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Attack_2);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Skill);
		MoveEnd.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::MoveEnd, MoveEnd);
		CAnimState TakeOff;
		TakeOff.SetAnim(pAnimHovering);
		TakeOff.SetParam(CAnimCtrlr::Tag::TakeOff, 0.0f, 0.3f, 0.0f, 0.0f, 0.2f, true, false);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Hovering);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Landing);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Dash);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Attack_1);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Attack_2);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Skill);
		TakeOff.SetDefaultNextState(CAnimCtrlr::Tag::Hovering);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::TakeOff, TakeOff);
		CAnimState Hovering;
		Hovering.SetAnim(pAnimHovering);
		Hovering.SetParam(CAnimCtrlr::Tag::Hovering, 0.3f, 0.5f, 0.48f, 0.49f, 0.4f, true, true);
		Hovering.SetNextState(CAnimCtrlr::Tag::Hovering);
		Hovering.SetNextState(CAnimCtrlr::Tag::Landing);
		Hovering.SetNextState(CAnimCtrlr::Tag::Dash);
		Hovering.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Hovering.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Hovering.SetNextState(CAnimCtrlr::Tag::Skill);
		Hovering.SetDefaultNextState(CAnimCtrlr::Tag::Landing);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Hovering, Hovering);
		CAnimState Landing;
		Landing.SetAnim(pAnimHovering);
		Landing.SetParam(CAnimCtrlr::Tag::Landing, 0.5f, 1.0f, 0.6f, 0.6f, 0.2f, true, true);
		Landing.SetNextState(CAnimCtrlr::Tag::Idle);
		Landing.SetNextState(CAnimCtrlr::Tag::Dash);
		Landing.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Landing.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Landing.SetNextState(CAnimCtrlr::Tag::Skill);
		Landing.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Landing, Landing);
		CAnimState Jump;
		Jump.SetAnim(pAnimHovering);
		Jump.SetParam(CAnimCtrlr::Tag::Jump, 0.0f, 0.3f, 0.0f, 0.0f, 1.0f, false, false);
		Jump.SetNextState(CAnimCtrlr::Tag::Hovering);
		Jump.SetDefaultNextState(CAnimCtrlr::Tag::Hovering);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Jump, Jump);
		CAnimState Dash;
		Dash.SetAnim(pAnimHovering);
		Dash.SetParam(CAnimCtrlr::Tag::Dash, 0.3f, 0.5f, 0.48f, 0.49f, 0.8f, false, true);
		Dash.SetNextState(CAnimCtrlr::Tag::Idle);
		Dash.SetNextState(CAnimCtrlr::Tag::Dash);
		Dash.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Dash.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Dash.SetNextState(CAnimCtrlr::Tag::Skill);
		Dash.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Dash, Dash);
		CAnimState Attack_1;
		Attack_1.SetAnim(pAnimSpearCharging);
		Attack_1.SetParam(CAnimCtrlr::Tag::Attack_1, 0.66f, 1.0f, 0.66f, 0.88f, 1.0f, false, true);
		Attack_1.SetNextState(CAnimCtrlr::Tag::Idle);
		Attack_1.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Attack_1.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Attack_1, Attack_1);
		CAnimState Attack_2;
		Attack_2.SetAnim(pAnimShieldCharging);
		Attack_2.SetParam(CAnimCtrlr::Tag::Attack_2, 0.66f, 1.0f, 0.66f, 0.88f, 0.5f, false, true);
		Attack_2.SetNextState(CAnimCtrlr::Tag::Idle);
		Attack_2.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Attack_2.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Attack_2, Attack_2);
		CAnimState Skill;
		Skill.SetAnim(pAnimSpearCharging);
		Skill.SetParam(CAnimCtrlr::Tag::Skill, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f, false, false);
		Skill.SetNextState(CAnimCtrlr::Tag::Idle);
		Skill.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Skill, Skill);

		pAnimCtrlr->ChangeCurrAnim(CAnimCtrlr::Tag::Idle);
		break;
	}
	case eGeo::AnimCtrlr::Epsilon:
	{
		pAnimCtrlr = make_shared<CAnimCtrlr>();
		pAnimCtrlr->SetSkeleton(GetSkeleton(eGeo::Skel::Epsilon));

		const auto& pAnimIdle			{ GetAnimation(eGeo::Anim::Epsilon_Idle) };
		const auto& pAnimRunning		{ GetAnimation(eGeo::Anim::Epsilon_Running) };
		const auto& pAnimHovering		{ GetAnimation(eGeo::Anim::Epsilon_Hovering) };
		const auto& pAnimShooting		{ GetAnimation(eGeo::Anim::Epsilon_Shooting) };
		const auto& pAnimShootingSkiil	{ GetAnimation(eGeo::Anim::Epsilon_Shooting_Skill) };

		CAnimState Idle;
		Idle.SetAnim(pAnimIdle);
		Idle.SetParam(CAnimCtrlr::Tag::Idle, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, true, true);
		Idle.SetNextState(CAnimCtrlr::Tag::Idle);
		Idle.SetNextState(CAnimCtrlr::Tag::MoveStart);
		Idle.SetNextState(CAnimCtrlr::Tag::TakeOff);
		Idle.SetNextState(CAnimCtrlr::Tag::Jump);
		Idle.SetNextState(CAnimCtrlr::Tag::Dash);
		Idle.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Idle.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Idle.SetNextState(CAnimCtrlr::Tag::Skill);
		Idle.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Idle, Idle);
		CAnimState MoveStart;
		MoveStart.SetAnim(pAnimRunning);
		MoveStart.SetParam(CAnimCtrlr::Tag::MoveStart, 0.0f, 0.25f, 0.0f, 0.0f, 0.2f, true, false);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Idle);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Move);
		MoveStart.SetNextState(CAnimCtrlr::Tag::TakeOff);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Jump);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Dash);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Attack_1);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Attack_2);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Skill);
		MoveStart.SetDefaultNextState(CAnimCtrlr::Tag::Move);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::MoveStart, MoveStart);
		CAnimState Move;
		Move.SetAnim(pAnimRunning);
		Move.SetParam(CAnimCtrlr::Tag::Move, 0.25f, 0.75f, 0.25f, 0.75f, 0.8f, true, true);
		Move.SetNextState(CAnimCtrlr::Tag::Idle);
		Move.SetNextState(CAnimCtrlr::Tag::Move);
		Move.SetNextState(CAnimCtrlr::Tag::MoveEnd);
		Move.SetNextState(CAnimCtrlr::Tag::TakeOff);
		Move.SetNextState(CAnimCtrlr::Tag::Jump);
		Move.SetNextState(CAnimCtrlr::Tag::Dash);
		Move.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Move.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Move.SetNextState(CAnimCtrlr::Tag::Skill);
		Move.SetDefaultNextState(CAnimCtrlr::Tag::MoveEnd);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Move, Move);
		CAnimState MoveEnd;
		MoveEnd.SetAnim(pAnimRunning);
		MoveEnd.SetParam(CAnimCtrlr::Tag::MoveEnd, 0.75f, 1.0f, 0.0f, 0.0f, 0.2f, true, false);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Idle);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::TakeOff);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Jump);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Dash);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Attack_1);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Attack_2);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Skill);
		MoveEnd.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::MoveEnd, MoveEnd);
		CAnimState TakeOff;
		TakeOff.SetAnim(pAnimHovering);
		TakeOff.SetParam(CAnimCtrlr::Tag::TakeOff, 0.0f, 0.3f, 0.0f, 0.0f, 0.2f, true, false);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Hovering);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Landing);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Dash);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Attack_1);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Attack_2);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Skill);
		TakeOff.SetDefaultNextState(CAnimCtrlr::Tag::Hovering);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::TakeOff, TakeOff);
		CAnimState Hovering;
		Hovering.SetAnim(pAnimHovering);
		Hovering.SetParam(CAnimCtrlr::Tag::Hovering, 0.3f, 0.5f, 0.48f, 0.49f, 0.4f, true, true);
		Hovering.SetNextState(CAnimCtrlr::Tag::Hovering);
		Hovering.SetNextState(CAnimCtrlr::Tag::Landing);
		Hovering.SetNextState(CAnimCtrlr::Tag::Dash);
		Hovering.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Hovering.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Hovering.SetNextState(CAnimCtrlr::Tag::Skill);
		Hovering.SetDefaultNextState(CAnimCtrlr::Tag::Landing);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Hovering, Hovering);
		CAnimState Landing;
		Landing.SetAnim(pAnimHovering);
		Landing.SetParam(CAnimCtrlr::Tag::Landing, 0.5f, 1.0f, 0.6f, 0.6f, 0.2f, true, true);
		Landing.SetNextState(CAnimCtrlr::Tag::Idle);
		Landing.SetNextState(CAnimCtrlr::Tag::Dash);
		Landing.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Landing.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Landing.SetNextState(CAnimCtrlr::Tag::Skill);
		Landing.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Landing, Landing);
		CAnimState Jump;
		Jump.SetAnim(pAnimHovering);
		Jump.SetParam(CAnimCtrlr::Tag::Jump, 0.0f, 0.3f, 0.0f, 0.0f, 1.0f, false, false);
		Jump.SetNextState(CAnimCtrlr::Tag::Hovering);
		Jump.SetDefaultNextState(CAnimCtrlr::Tag::Hovering);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Jump, Jump);
		CAnimState Dash;
		Dash.SetAnim(pAnimHovering);
		Dash.SetParam(CAnimCtrlr::Tag::Dash, 0.3f, 0.5f, 0.48f, 0.49f, 0.8f, false, true);
		Dash.SetNextState(CAnimCtrlr::Tag::Idle);
		Dash.SetNextState(CAnimCtrlr::Tag::Dash);
		Dash.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Dash.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Dash.SetNextState(CAnimCtrlr::Tag::Skill);
		Dash.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Dash, Dash);
		CAnimState Attack_1;
		Attack_1.SetAnim(pAnimShooting);
		Attack_1.SetParam(CAnimCtrlr::Tag::Attack_1, 0.0f, 1.0f, 0.25f, 0.75f, 0.5f, false, true);
		Attack_1.SetNextState(CAnimCtrlr::Tag::Idle);
		Attack_1.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Attack_1.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Attack_1, Attack_1);
		CAnimState Attack_2;
		Attack_2.SetAnim(pAnimShootingSkiil);
		Attack_2.SetParam(CAnimCtrlr::Tag::Attack_2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, false, true);
		Attack_2.SetNextState(CAnimCtrlr::Tag::Idle);
		Attack_2.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Attack_2.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Attack_2, Attack_2);
		CAnimState Skill;
		Skill.SetAnim(pAnimShootingSkiil);
		Skill.SetParam(CAnimCtrlr::Tag::Skill, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, false, false);
		Skill.SetNextState(CAnimCtrlr::Tag::Idle);
		Skill.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Skill, Skill);

		pAnimCtrlr->ChangeCurrAnim(CAnimCtrlr::Tag::Idle);
		break;
	}
	case eGeo::AnimCtrlr::Gravis:
	{
		pAnimCtrlr = make_shared<CAnimCtrlr>();
		pAnimCtrlr->SetSkeleton(GetSkeleton(eGeo::Skel::Gravis));

		const auto& pAnimIdle		{ GetAnimation(eGeo::Anim::Gravis_Idle) };
		const auto& pAnimRunning	{ GetAnimation(eGeo::Anim::Gravis_Running) };
		const auto& pAnimHovering	{ GetAnimation(eGeo::Anim::Gravis_Hovering) };
		const auto& pAnimAttack		{ GetAnimation(eGeo::Anim::Gravis_Attack) };

		CAnimState Idle;
		Idle.SetAnim(pAnimIdle);
		Idle.SetParam(CAnimCtrlr::Tag::Idle, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, true, true);
		Idle.SetNextState(CAnimCtrlr::Tag::Idle);
		Idle.SetNextState(CAnimCtrlr::Tag::MoveStart);
		Idle.SetNextState(CAnimCtrlr::Tag::TakeOff);
		Idle.SetNextState(CAnimCtrlr::Tag::Jump);
		Idle.SetNextState(CAnimCtrlr::Tag::Dash);
		Idle.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Idle.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Idle.SetNextState(CAnimCtrlr::Tag::Skill);
		Idle.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Idle, Idle);
		CAnimState MoveStart;
		MoveStart.SetAnim(pAnimRunning);
		MoveStart.SetParam(CAnimCtrlr::Tag::MoveStart, 0.0f, 0.25f, 0.0f, 0.0f, 0.2f, true, false);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Idle);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Move);
		MoveStart.SetNextState(CAnimCtrlr::Tag::TakeOff);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Jump);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Dash);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Attack_1);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Attack_2);
		MoveStart.SetNextState(CAnimCtrlr::Tag::Skill);
		MoveStart.SetDefaultNextState(CAnimCtrlr::Tag::Move);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::MoveStart, MoveStart);
		CAnimState Move;
		Move.SetAnim(pAnimRunning);
		Move.SetParam(CAnimCtrlr::Tag::Move, 0.25f, 0.75f, 0.25f, 0.75f, 0.8f, true, true);
		Move.SetNextState(CAnimCtrlr::Tag::Idle);
		Move.SetNextState(CAnimCtrlr::Tag::Move);
		Move.SetNextState(CAnimCtrlr::Tag::MoveEnd);
		Move.SetNextState(CAnimCtrlr::Tag::TakeOff);
		Move.SetNextState(CAnimCtrlr::Tag::Jump);
		Move.SetNextState(CAnimCtrlr::Tag::Dash);
		Move.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Move.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Move.SetNextState(CAnimCtrlr::Tag::Skill);
		Move.SetDefaultNextState(CAnimCtrlr::Tag::MoveEnd);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Move, Move);
		CAnimState MoveEnd;
		MoveEnd.SetAnim(pAnimRunning);
		MoveEnd.SetParam(CAnimCtrlr::Tag::MoveEnd, 0.75f, 1.0f, 0.0f, 0.0f, 0.2f, true, false);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Idle);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::TakeOff);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Jump);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Dash);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Attack_1);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Attack_2);
		MoveEnd.SetNextState(CAnimCtrlr::Tag::Skill);
		MoveEnd.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::MoveEnd, MoveEnd);
		CAnimState TakeOff;
		TakeOff.SetAnim(pAnimHovering);
		TakeOff.SetParam(CAnimCtrlr::Tag::TakeOff, 0.0f, 0.3f, 0.0f, 0.0f, 0.2f, true, false);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Hovering);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Landing);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Dash);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Attack_1);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Attack_2);
		TakeOff.SetNextState(CAnimCtrlr::Tag::Skill);
		TakeOff.SetDefaultNextState(CAnimCtrlr::Tag::Hovering);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::TakeOff, TakeOff);
		CAnimState Hovering;
		Hovering.SetAnim(pAnimHovering);
		Hovering.SetParam(CAnimCtrlr::Tag::Hovering, 0.3f, 0.5f, 0.48f, 0.49f, 0.4f, true, true);
		Hovering.SetNextState(CAnimCtrlr::Tag::Hovering);
		Hovering.SetNextState(CAnimCtrlr::Tag::Landing);
		Hovering.SetNextState(CAnimCtrlr::Tag::Dash);
		Hovering.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Hovering.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Hovering.SetNextState(CAnimCtrlr::Tag::Skill);
		Hovering.SetDefaultNextState(CAnimCtrlr::Tag::Landing);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Hovering, Hovering);
		CAnimState Landing;
		Landing.SetAnim(pAnimHovering);
		Landing.SetParam(CAnimCtrlr::Tag::Landing, 0.5f, 1.0f, 0.6f, 0.6f, 0.2f, true, true);
		Landing.SetNextState(CAnimCtrlr::Tag::Idle);
		Landing.SetNextState(CAnimCtrlr::Tag::Dash);
		Landing.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Landing.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Landing.SetNextState(CAnimCtrlr::Tag::Skill);
		Landing.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Landing, Landing);
		CAnimState Jump;
		Jump.SetAnim(pAnimHovering);
		Jump.SetParam(CAnimCtrlr::Tag::Jump, 0.0f, 0.3f, 0.0f, 0.0f, 1.0f, false, false);
		Jump.SetNextState(CAnimCtrlr::Tag::Hovering);
		Jump.SetDefaultNextState(CAnimCtrlr::Tag::Hovering);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Jump, Jump);
		CAnimState Dash;
		Dash.SetAnim(pAnimHovering);
		Dash.SetParam(CAnimCtrlr::Tag::Dash, 0.3f, 0.5f, 0.48f, 0.49f, 0.8f, false, true);
		Dash.SetNextState(CAnimCtrlr::Tag::Idle);
		Dash.SetNextState(CAnimCtrlr::Tag::Dash);
		Dash.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Dash.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Dash.SetNextState(CAnimCtrlr::Tag::Skill);
		Dash.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Dash, Dash);
		CAnimState Attack_1;
		Attack_1.SetAnim(pAnimAttack);
		Attack_1.SetParam(CAnimCtrlr::Tag::Attack_1, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, false, true);
		Attack_1.SetNextState(CAnimCtrlr::Tag::Idle);
		Attack_1.SetNextState(CAnimCtrlr::Tag::Attack_1);
		Attack_1.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Attack_1, Attack_1);
		CAnimState Attack_2;
		Attack_2.SetAnim(pAnimAttack);
		Attack_2.SetParam(CAnimCtrlr::Tag::Attack_2, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, false, true);
		Attack_2.SetNextState(CAnimCtrlr::Tag::Idle);
		Attack_2.SetNextState(CAnimCtrlr::Tag::Attack_2);
		Attack_2.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Attack_2, Attack_2);
		CAnimState Skill;
		Skill.SetAnim(pAnimAttack);
		Skill.SetParam(CAnimCtrlr::Tag::Skill, 0.0f, 1.0f, 0.0f, 1.0f, 1.5f, false, false);
		Skill.SetNextState(CAnimCtrlr::Tag::Idle);
		Skill.SetDefaultNextState(CAnimCtrlr::Tag::Idle);
		pAnimCtrlr->SetAnimState(CAnimCtrlr::Tag::Skill, Skill);

		pAnimCtrlr->ChangeCurrAnim(CAnimCtrlr::Tag::Idle);
		break;
	}
	}
	return pAnimCtrlr;
}
