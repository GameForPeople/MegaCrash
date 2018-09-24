#pragma once
#include <fmod.hpp>
#pragma comment(lib, "fmodL64_vc.lib")

class CSoundMgr
{
public:
	static constexpr UINT	MAX_CHANNELS			{ 1000 };
	static constexpr UINT	NUM_LOOPING_CHANNELS	{ 1 };
	static constexpr float	DISTANCE_FACTOR			{ 0.000001f };
	static constexpr float	SOUND_MIN_DISTANCE		{ 0.01f };
	static constexpr float	SOUND_MAX_DISTANCE		{ 5000.0f };
	enum class SoundName
	{
		  BGM_NAME
		, MainTheme
		, EFFECT_HIT
		, EXPLOSION
		, DEAD_1
		, DEAD_2
		, SHOT_BASE
		, SHOT_N
		, SHOT_ES
		, UI_ON
	};

	struct Listener
	{
		FMOD_VECTOR		lastpos;
		FMOD_VECTOR		currpos;
		FMOD_VECTOR		forward;
		FMOD_VECTOR		up;
		FMOD_VECTOR		vel;

		void SetParameter(const XMFLOAT3& pos, const XMFLOAT3& _forward, const XMFLOAT3& _up) {
			currpos = { pos.x*DISTANCE_FACTOR, pos.y*DISTANCE_FACTOR, pos.z*DISTANCE_FACTOR };
			forward = { _forward.x, _forward.y, _forward.z };
			up = { _up.x, _up.y, _up.z };
		}
	};

private:
	CSoundMgr();

public:
	static CSoundMgr* const Instance();
	~CSoundMgr();

	void LoadSounds();
	void Update(float fTimeElapsed);
	void PlaySoundByName(SoundName sound_name, const XMFLOAT3& pos = XMFLOAT3());
	FMOD::Sound* GetSoundByName(SoundName sound_name) { return m_Sounds[sound_name]; }
	FMOD::Channel** GetChannel() { return m_arrChannels; }
	Listener& GetListenerHandle() { return m_listener; }

private:
	FMOD::System*								m_fmodSystem;
	UINT										m_Version;
	unordered_map<SoundName, FMOD::Sound*>		m_Sounds;
	FMOD::Channel*								m_arrChannels[MAX_CHANNELS];
	UINT										m_CurrNonLoopingChannelIdx;
	UINT										m_CurrLoopingChannelIdx;

	Listener									m_listener;
};