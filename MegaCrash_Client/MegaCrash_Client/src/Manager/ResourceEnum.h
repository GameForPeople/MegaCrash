#pragma once
namespace eTex {
	enum class Type {
		  RESOURCE_TEXTURE2D
		, RESOURCE_TEXTURE2D_ARRAY
		, RESOURCE_TEXTURE2DARRAY
		, RESOURCE_TEXTURE_CUBE
		, RESOURCE_BUFFER
	};
	enum class Buf {
		  RenderTarget
		, DepthStencil
	};
	enum class SkyBox {
		StormyDays
	};
	enum class Diffuse {
		  Stage_ground
		, Stage_marble
		, Stage_brick
		, Stage_Rock
		, Stage_CheckerBoard
		, Stage_Cell
		, Stage_BlackerTile
		, Ruby_Body
		, Ruby_Hammer
		, Ruby_Bullet
		, Legion_Spine
		, Legion_Shoulder
		, Legion_Mid
		, Legion_Leg
		, Legion_Booster
		, Legion_Arm
		, Legion_Head
		, Legion_Spear
		, Legion_Shield
		, Epsilon_Arm
		, Epsilon_Foot
		, Epsilon_Head
		, Epsilon_Leg
		, Epsilon_Pelvis
		, Epsilon_Upperbody
		, Epsilon_Weapon
		, Gravis_Upperbody
		, Gravis_Lowerbody
		, Gravis_Head
		, Gravis_Leg
		, Gravis_Arm
		, Gravis_Flight
		, Gravis_Weapon
	};
	enum class Normal {
		  Stage_ground
		, Stage_marble
		, Stage_brick
		, Stage_Rock
		, Stage_CheckerBoard
		, Stage_Cell
		, Stage_BlackerTile
		, Ruby_Body
		, Ruby_Hammer
		, Ruby_Bullet
		, Legion_Spine
		, Legion_Shoulder
		, Legion_Mid
		, Legion_Leg
		, Legion_Booster
		, Legion_Arm
		, Legion_Head
		, Legion_Spear
		, Legion_Shield
		, Epsilon_Arm
		, Epsilon_Foot
		, Epsilon_Head
		, Epsilon_Leg
		, Epsilon_Pelvis
		, Epsilon_Upperbody
		, Epsilon_Weapon
		, Gravis_Upperbody
		, Gravis_Lowerbody
		, Gravis_Head
		, Gravis_Leg
		, Gravis_Arm
		, Gravis_Flight
		, Gravis_Weapon
	};
	enum class Specular {
		  Test
		, Stage_CheckerBoard_Red
		, Stage_CheckerBoard_Blue
		, Stage_Cell
		, Stage_BlackerTile
		, Ruby_Body
		, Ruby_Hammer
		, Ruby_Bullet
		, Legion_Spine
		, Legion_Shoulder
		, Legion_Mid
		, Legion_Leg
		, Legion_Booster
		, Legion_Arm
		, Legion_Head
		, Legion_Spear
		, Legion_Shield
		, Epsilon_Arm
		, Epsilon_Foot
		, Epsilon_Head
		, Epsilon_Leg
		, Epsilon_Pelvis
		, Epsilon_Upperbody
		, Epsilon_Weapon
		, Gravis_Upperbody
		, Gravis_Lowerbody
		, Gravis_Head
		, Gravis_Leg
		, Gravis_Arm
		, Gravis_Flight
		, Gravis_Weapon
	};
	enum class Glossiness {
		  Stage_CheckerBoard
		, Stage_Cell
		, Stage_BlackerTile
		, Ruby_Body
		, Ruby_Hammer
		, Ruby_Bullet
		, Legion_Spine
		, Legion_Shoulder
		, Legion_Mid
		, Legion_Leg
		, Legion_Booster
		, Legion_Arm
		, Legion_Head
		, Legion_Spear
		, Legion_Shield
		, Epsilon_Arm
		, Epsilon_Foot
		, Epsilon_Head
		, Epsilon_Leg
		, Epsilon_Pelvis
		, Epsilon_Upperbody
		, Epsilon_Weapon
		, Gravis_Upperbody
		, Gravis_Lowerbody
		, Gravis_Head
		, Gravis_Leg
		, Gravis_Arm
		, Gravis_Flight
		, Gravis_Weapon
	};
	enum class Emissive {
		  Test
		, Stage_Circuit
		, Ruby_Body
		, Ruby_Hammer
		, Ruby_Bullet
		, Legion_Spine
		, Legion_Shoulder
		, Legion_Mid
		, Legion_Leg
		, Legion_Booster
		, Legion_Arm
		, Legion_Head
		, Legion_Spear
		, Legion_Shield
		, Epsilon_Arm
		, Epsilon_Foot
		, Epsilon_Head
		, Epsilon_Leg
		, Epsilon_Pelvis
		, Epsilon_Upperbody
		, Epsilon_Weapon
		, Gravis_Upperbody
		, Gravis_Lowerbody
		, Gravis_Head
		, Gravis_Leg
		, Gravis_Arm
		, Gravis_Flight
		, Gravis_Weapon
	};
	enum class Billboard {
		  Digit
		, DustParticle
		, PlayerUI
		, HUD_UI
		, RADER_UI
		, ShaderHelper
		, GreySquare
		, Logo
		, Title
		, LobbyBaseFrame
		, Particle45Ellipse
		, ParticleEllipse
		, ParticleSpark
		, ParticleSmoke
		, ParticleGunFlash
		, ParticleCloud_0
		, ParticleCloud_1
		, ParticleCloud_2
		, ParticleCloud_3
		, ParticleRing
		, ParticleBrightRing
		, ParticleThinRing
		, ParticleCrossSpark
		, ParticleTrailBlast
		, GameEnd_BackGround
		, GameEnd_NeonWire
		, GameEnd_Victory
		, GameEnd_Defeat
		, GameEnd_CloudAlpha
		, GameEnd_BackGroundAlpha
		, DeathUI
		, ParticleExplodeFire
		, ParticleBackDraftSmoke
		, ParticleUpperFireSmoke
		, ParticleUpperCloud
		, ParticleHexagon
	};
	enum class SkillColorName {
		Dash,
		BoostJump,
		HammerStrike,
		Bulwark,
		SnipeShot,
		Blaster
	};
	enum class SkillGreyName {
		Dash,
		BoostJump,
		HammerStrike,
		Bulwark,
		SnipeShot,
		Blaster
	};
	enum class Font {
		Default,
		Arial,
		Quantum
	};
	enum class Button {
		  Connect_N
		, Connect_H
		, Exit_N
		, Exit_H
		, Create_N
		, Create_H
		, Enter_N
		, Enter_H
		, Leave_N
		, Leave_H
		, Start_N
		, Start_H
		, Recycle_N
		, Recycle_H
		, Ruby_N
		, Ruby_H
		, Legion_N
		, Legion_H
		, Epsilon_N
		, Epsilon_H
		, Gravis_N
		, Gravis_H
	};
}
namespace eGeo {
	enum class Mesh {
		  Cube
		, Bullet
		, Stage
		, Ruby
		, Legion
		, Epsilon
		, Gravis
	};
	enum class Skel {
		  Ruby
		, Legion
		, Epsilon
		, Gravis
	};
	enum class Anim {
		  Ruby_Idle
		, Ruby_Hovering
		, Ruby_Hammering
		, Ruby_Shoot
		, Ruby_Running
		, Legion_Idle
		, Legion_Hovering
		, Legion_ShieldCharging
		, Legion_SpearCharging
		, Legion_Running
		, Epsilon_Idle
		, Epsilon_Hovering
		, Epsilon_Shooting
		, Epsilon_Shooting_Skill
		, Epsilon_Running
		, Gravis_Idle
		, Gravis_Hovering
		, Gravis_Running
		, Gravis_Attack
	};
	enum class AnimCtrlr {
		  Ruby
		, Legion
		, Epsilon
		, Gravis
	};
}
namespace ePSO { 
	enum class Graphic {
		  ShadowMapStage
		, ShadowMapSkinnedAnim
		, ShadowMapModel
		, ShadowMapCube
		, SkyBox
		, Stage
		, Cube
		, Model
		, SkinnedAnim
		, DeferredSkyBox
		, DeferredStage
		, DeferredCube
		, DeferredModel
		, DeferredSkinnedAnim
		, DeferredLighting
		, FullScreen
		, Billboard
		, ParticleBillboardDefault
		, ParticleBillboardAdditive
		, PlayerUI
		, HUD_UI
		, RADER_UI
		, Skill_Icon
		, Crosshair
		, Font
		, Logo
		, Title
		, LobbyWindow
		, LobbyRoomSlot
		, Button
		, Gameover
		, DeathUI
	};
	enum class Compute {

	};
}
namespace eMat {
	enum class Object {
		  Stage
		, SkyBox
		, Ruby
		, Legion
		, Epsilon
		, Gravis
		, Ruby_Bullet
		, PlayerUI
		, Crosshair
		, DashSkillIcon
		, JumpSkillIcon
		, RubySkillIcon
		, LegionSkillIcon
		, EpsilonSkillIcon
		, GravisSkillIcon
		, Font
		, LobbyWindow
		, LobbyRoomSlot
		, LobbyLogo
		, ConnectButton
		, ExitButton
		, CreateButton
		, EnterButton
		, LeaveButton
		, StartButton
		, RecycleButton
		, RubyButton
		, LegionButton
		, EpsilonButton
		, GravisButton
		, GameEnd
		, DeathUI
	};
}
namespace eDESC {
	enum class InputLayout {
		  SkyBox
		, Stage
		, SkinnedAnim
		, Model
		, Cube
		, Billboard
		, ParticleBillboard
		, PlayerUI
		, SkillIcon
		, RADER_UI
		, None
		, FullScreen
		, Font
	};
	enum class Rasterizer {
		  Default
		, CullModeNone
		, ShadowMap
	};
	enum class Blend {
		  Default
		, Alpha
		, UIAlpha
		, Font
		, Additive
		, ParticleAlpha
	};
	enum class DepthStencil {
		  DepthTestDefalut
		, DepthStencilDefault
		, DepthTestNoWriteDepth
		, BothDisable
	};
	enum class StaticSampler {
		  PointWrap
		, PointClamp
		, LinearWrap
		, LinearClamp
		, AnisotropicWrap
		, AnisotropicClamp
		, Shadow
	};
}

enum {
	  ROOT_PARAMETER_MATERIAL = 0
	, ROOT_PARAMETER_LIGHT
	, ROOT_PARAMETER_CAMERA
	, ROOT_PARAMETER_OBJECT
	, ROOT_PARAMETER_CONSTANT1
	, ROOT_PARAMETER_CONSTANT2
	, ROOT_PARAMETER_CONSTANT3
	, ROOT_PARAMETER_RESOURCE1
	, ROOT_PARAMETER_RESOURCE2
	, ROOT_PARAMETER_TEXTURE1
	, ROOT_PARAMETER_TEXTURE2
	, ROOT_PARAMETER_TEXTURE3
	, ROOT_PARAMETER_TEXTURE4
	, ROOT_PARAMETER_TEXTURE5
	, ROOT_PARAMETER_UA_RESOURCE1
	, ROOT_PARAMETER_UA_RESOURCE2
	, ROOT_PARAMETER_COUNT
};