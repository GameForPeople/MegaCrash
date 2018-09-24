#pragma once
#include "Object/Object.h"

constexpr int ParticleMinParam	{ 0 };
constexpr int ParticleMaxParam	{ 10000 };

class CParticleParamManager
{
public:
	struct ColorParam
	{
		int PinPos{ 0 };			// 0 ~ 10000
		XMFLOAT3 color{ 0 };
		friend std::ostream& operator<<(std::ostream& os, const ColorParam& p);
		ColorParam(int _PinPos, XMFLOAT3 _color) : PinPos(_PinPos), color(_color) {};
	};

	struct AlphaParam
	{
		int PinPos{ 0 };			// 0 ~ 10000
		float alpha{ 0 };
		friend std::ostream& operator<<(std::ostream& os, const AlphaParam& p);
		AlphaParam(int _PinPos, float _alpha) : PinPos(_PinPos), alpha(_alpha) {};
	};

	struct SizeParam
	{
		int PinPos{ 0 };			// 0 ~ 10000
		XMFLOAT2 size{ 0 };
		friend std::ostream& operator<<(std::ostream& os, const SizeParam& p);
		SizeParam(int _PinPos, XMFLOAT2 _size) : PinPos(_PinPos), size(_size) {};
	};


private:
	std::vector<ColorParam> ColorParamList;
	std::vector<AlphaParam> AlphaParamList;
	std::vector<SizeParam> SizeParamList;

	bool m_Activate{ false };
	float m_Life{ 0 };
	float m_currElapsedTime{ 0 };	// 현재 라이프
	XMFLOAT4 m_xmf4Color{ 0,0,0,0 };
	XMFLOAT2 m_xmf2Size{ 0,0 };

public:
	CParticleParamManager() {
		ResetColoraAndAlphaAndSizeParam();
	};
	virtual ~CParticleParamManager() {};

	void ResetColoraAndAlphaAndSizeParam();

	bool AddColorParam(ColorParam new_param);
	bool AddAlphaParam(AlphaParam  new_param);
	bool AddSizeParam(SizeParam  new_param);
	bool RemoveColorParam(int PinPos);
	bool RemoveAlphaParam(int PinPos);
	bool RemoveSizeParam(int PinPos);

	void CalcColorAndSizeByElapsedTime();

	void Animate(float fTimeElapsed);
	void ResetAllParameter();
	void ResetTimeAndLife();

	void Activate();
	void PrintData();
public:
	XMFLOAT4 GetColor() const { return m_xmf4Color; }
	XMFLOAT2 GetSize() const { return m_xmf2Size; }
	float GetLife() const { return m_Life; }
	float GetCurrElapsedTime() const { return m_currElapsedTime; }
	bool GetIsActive() const { return m_Activate; }
	void SetLife(float _life) { m_Life = _life; }
};


class CParticleBillboards : public CGameObject
{
public:
	struct ParticleControler {
		CParticleBillboardVertex&		Particle;
		const XMFLOAT2					szSprite;
		XMFLOAT3						xmf3Direction;
		float							fSpeed;
		float							fAnimateSpeed;
		float							fAnimateTime;
		float							fTimer;
		bool							bActive;

		ParticleControler(
			CParticleBillboardVertex&		particle
			, const XMFLOAT2&				xmf2SpriteSize)
			: Particle(particle)
			, szSprite(xmf2SpriteSize)
			, xmf3Direction(XMFLOAT3(0.0f, 0.0f, 0.0f))
			, fAnimateSpeed(0)
			, fAnimateTime(0)
			, fSpeed(0)
			, fTimer(0)
			, bActive(false)
		{
			Particle.m_xmf3Pos = XMFLOAT3(0.0f, 100000.0f, 0.0f);
			Particle.m_xmf2DividedTexCoord.x = 1.f / szSprite.x;
			Particle.m_xmf2DividedTexCoord.y = 1.f / szSprite.y;
		}
		void SetTexCoord(const XMFLOAT2& xmf2Coord) { Particle.m_xmf2CurrTex = xmf2Coord; }
		void StartAnimate(
			const XMFLOAT3&	pos
			, XMFLOAT3&			dir
			, float				speed
			, float				animateTime
			, float				animateSpeed = 0.0f)
		{
			Particle.m_xmf3Pos = pos;
			xmf3Direction = Vector3::Normalize(dir);
			fAnimateTime = animateTime;
			fAnimateSpeed = animateSpeed;
			fSpeed = speed;
			bActive = true;
		}
		void Reset()
		{
			Particle.m_xmf2CurrTex = XMFLOAT2(0.0f, 0.0f);
			Particle.m_xmf3Pos = XMFLOAT3(0.0f, 100000.0f, 0.0f);
			xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
			fSpeed = 0.0f;
			fTimer = 0.0f;
			fAnimateTime = 0.0f;
			fAnimateSpeed = 0.0f;
			bActive = false;
		}
		void Animate(float fTimeElapsed)
		{
			if (!bActive) return;
			fTimer += fTimeElapsed;
			Particle.m_xmf2CurrTex.x += fAnimateSpeed * fTimeElapsed;
			if (Particle.m_xmf2CurrTex.x > szSprite.x)
			{
				Particle.m_xmf2CurrTex.x = 0;
				Particle.m_xmf2CurrTex.y += 1;
				if (Particle.m_xmf2CurrTex.y > szSprite.y)
					Particle.m_xmf2CurrTex = XMFLOAT2(0.0f, 0.0f);
			}
			Particle.m_xmf3Pos = Vector3::Add(
				Particle.m_xmf3Pos
				, Vector3::ScalarProduct(xmf3Direction, fSpeed * fTimeElapsed));
			if (fTimer > fAnimateTime) Reset();
		}
	};
public:
	ParticleControler * *					m_ppControlers;
	UINT								m_nControlers;

	UINT								m_iCurrControler;

public:
	CParticleBillboards(
		ID3D12Device*					pd3dDevice
		, ID3D12GraphicsCommandList*	pd3dCmdLst
		, eTex::Billboard				eTexture
		, XMFLOAT2						szSprite
		, UINT							nBillboards
		, XMFLOAT2						xmf2Size);
	virtual ~CParticleBillboards();

	virtual void Animate(float fTimeElapsed) override;

	void PopParticle(
		const XMFLOAT3&	pos
		, const XMFLOAT2&	coord
		, XMFLOAT3&			dir
		, float				speed
		, float				animateTime
		, float				animateSpeed = 0.0f);
};


class CAdvancedParticleBillBoards : public CGameObject
{
	struct AdvancedParticleControler {
		CAdvancedParticleBillboardVertex&		Particle;
		CParticleParamManager			ParamManager;
		float										m_fLaunchSpeed;

		AdvancedParticleControler(
			CAdvancedParticleBillboardVertex&		particle)
			: Particle(particle)
			, m_fLaunchSpeed(0)
		{
			ParamManager.ResetColoraAndAlphaAndSizeParam();
			Reset();
		};

		void StartAnimate(
			const XMFLOAT3		pos
			, XMFLOAT3			velocity
			, float				fLaunchSpeed
			, float				life
			, XMFLOAT3			option
			, XMFLOAT4			value
			, XMFLOAT4			CustomColor
			, XMFLOAT2			SizeMultiplier
		)
		{
			m_fLaunchSpeed = fLaunchSpeed;
			ParamManager.SetLife(life);
			ParamManager.Activate();

			Particle.m_xmf3Pos = pos;
			XMFLOAT3 velo = Vector3::Normalize(velocity);
			velo = Vector3::Multiply(m_fLaunchSpeed, velo);

			Particle.m_xmf3Velocity = velo;
			Particle.m_xmf4Color = ParamManager.GetColor();
			Particle.m_xmf2Size = ParamManager.GetSize();
			Particle.m_xmf3Opt = option;
			Particle.m_xmf4Value = value;
			Particle.m_xmf4CustomColor = CustomColor;
			Particle.m_xmf2SizeMultiplier = SizeMultiplier;
		}
		void AddColorParam(CParticleParamManager::ColorParam colorParam)
		{
			ParamManager.AddColorParam(colorParam);
		}
		void AddAlphaParam(CParticleParamManager::AlphaParam alphaParam)
		{
			ParamManager.AddAlphaParam(alphaParam);
		}
		void AddSizeParam(CParticleParamManager::SizeParam sizeParam)
		{
			ParamManager.AddSizeParam(sizeParam);
		}
		void Reset()
		{
			Particle.m_xmf3Pos = XMFLOAT3(0.0f, 100000.0f, 0.0f);
			Particle.m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			Particle.m_xmf4Color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			Particle.m_xmf2Size = XMFLOAT2(0.0f, 0.0f);
			Particle.m_xmf3Opt = XMFLOAT3(0, 0, 0);
			Particle.m_xmf4Value = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			Particle.m_xmf4CustomColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			Particle.m_xmf2SizeMultiplier = XMFLOAT2(1.0f, 1.0f);
			ParamManager.ResetTimeAndLife();
		}
		void Animate(float fTimeElapsed)
		{
			if (!ParamManager.GetIsActive()) return;
			ParamManager.Animate(fTimeElapsed);
			Particle.m_xmf4Color = ParamManager.GetColor();
			Particle.m_xmf2Size = ParamManager.GetSize();
			Particle.m_xmf4Value.z = ParamManager.GetCurrElapsedTime();

			if (!ParamManager.GetIsActive())
			{
				Reset();
			}
		}
	};
public:
	struct PopParticleEvent
	{
		float						time;
		XMFLOAT3					pos;
		XMFLOAT3					dir;
		float						speed;
		float						life;
		XMFLOAT3					option; // isForcedByGravity , isStretchedBB, isAlignedByMovDir 
		XMFLOAT4					value; // SpinRate, SeedValue, CurrLife, GravityForce
		XMFLOAT4					CustomColor;
		XMFLOAT2					SizeMultiplier;
		PopParticleEvent()
			:
			time(0)
			, pos(XMFLOAT3(0, 0, 0))
			, dir(XMFLOAT3(0, 0, 0))
			, speed(0)
			, life(0)
			, option(XMFLOAT3(0, 0, 0))
			, value(XMFLOAT4(0, 0, 0, 0))
			, CustomColor(XMFLOAT4(1, 1, 1, 1))
			, SizeMultiplier(XMFLOAT2(1, 1))
		{};

		PopParticleEvent(PopParticleEvent& new_event)
			:
			time(new_event.time)
			, pos(new_event.pos)
			, dir(new_event.dir)
			, speed(new_event.speed)
			, life(new_event.life)
			, option(new_event.option)
			, value(new_event.value)
			, CustomColor(new_event.CustomColor)
			, SizeMultiplier(new_event.SizeMultiplier)
		{};



	};

	struct Greater_prior {
		bool operator()(PopParticleEvent *a, PopParticleEvent *b) {
			return (a->time > b->time);
		}
	};


public:
	AdvancedParticleControler * *					m_ppControlers;
	UINT											m_nControlers;

	UINT											m_iCurrControler;
	std::priority_queue <PopParticleEvent *, vector<PopParticleEvent *>, Greater_prior >	m_particleEventQueue;
public:
	CAdvancedParticleBillBoards(
		ID3D12Device*					pd3dDevice
		, ID3D12GraphicsCommandList*	pd3dCmdLst
		, eTex::Billboard				eTexture
		, XMFLOAT2						szSprite
		, UINT							nBillboards
		, XMFLOAT2						xmf2Size
		, ePSO::Graphic					PSO
	);
	virtual ~CAdvancedParticleBillBoards();
	virtual void AddColorParam(CParticleParamManager::ColorParam colorParam);
	virtual void AddSizeParam(CParticleParamManager::SizeParam alphaParam);
	virtual void AddAlphaParam(CParticleParamManager::AlphaParam sizeParam);
	virtual void Animate(float fTimeElapsed) override;
	void AddParticleEvent(PopParticleEvent& _new_event);

};