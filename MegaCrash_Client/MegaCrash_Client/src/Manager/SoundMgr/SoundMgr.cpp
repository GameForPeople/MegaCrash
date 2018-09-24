#include "stdafx.h"
#include "SoundMgr.h"

CSoundMgr* const CSoundMgr::Instance()
{
	static CSoundMgr* sm{ nullptr };
	if (!sm) sm = new CSoundMgr();
	return sm;
}

CSoundMgr::CSoundMgr()
	: m_fmodSystem{ nullptr }
	, m_arrChannels{ nullptr, }
	, m_CurrNonLoopingChannelIdx{ NUM_LOOPING_CHANNELS }
{
	FMOD::System_Create(&m_fmodSystem);
	
	m_fmodSystem->getVersion(&m_Version);

	if (m_Version < FMOD_VERSION)
	{
		printf("FMOD lib version %08x doesn't match header version %08x", m_Version, FMOD_VERSION);
	}

	m_fmodSystem->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
	//m_fmodSystem->set3DSettings(1.0, DISTANCE_FACTOR, 1.0f);
	m_fmodSystem->set3DSettings(1.0f, DISTANCE_FACTOR, 1.0f);

	LoadSounds();
}
CSoundMgr::~CSoundMgr()
{
	for (auto& p : m_Sounds)
		p.second->release();
	m_Sounds.clear();
	m_fmodSystem->close();
	m_fmodSystem->release();
}

void CSoundMgr::LoadSounds()
{
	float sound_min_distance{ SOUND_MIN_DISTANCE };
	float sound_max_distance{ SOUND_MAX_DISTANCE };
	
	// Load Sound
	m_fmodSystem->createSound("./Assets/Sounds/spellFieldHited.wav", FMOD_3D, 0, &m_Sounds[SoundName::EFFECT_HIT]);
	m_Sounds[SoundName::EFFECT_HIT]->set3DMinMaxDistance(sound_min_distance, sound_max_distance);
	m_Sounds[SoundName::EFFECT_HIT]->setMode(FMOD_DEFAULT);

	m_fmodSystem->createSound("./Assets/Sounds/ShotBase.ogg", FMOD_3D, 0, &m_Sounds[SoundName::SHOT_BASE]);
	m_Sounds[SoundName::SHOT_BASE]->set3DMinMaxDistance(sound_min_distance, sound_max_distance);
	m_Sounds[SoundName::SHOT_BASE]->setMode(FMOD_DEFAULT);
	m_fmodSystem->createSound("./Assets/Sounds/Shot_ES.ogg", FMOD_3D, 0, &m_Sounds[SoundName::SHOT_ES]);
	m_Sounds[SoundName::SHOT_ES]->set3DMinMaxDistance(sound_min_distance, sound_max_distance);
	m_Sounds[SoundName::SHOT_ES]->setMode(FMOD_DEFAULT);
	m_fmodSystem->createSound("./Assets/Sounds/Shot_N.ogg", FMOD_3D, 0, &m_Sounds[SoundName::SHOT_N]);
	m_Sounds[SoundName::SHOT_N]->set3DMinMaxDistance(sound_min_distance, sound_max_distance);
	m_Sounds[SoundName::SHOT_N]->setMode(FMOD_DEFAULT);

	m_fmodSystem->createSound("./Assets/Sounds/Explosion.ogg", FMOD_3D, 0, &m_Sounds[SoundName::EXPLOSION]);
	m_Sounds[SoundName::EXPLOSION]->set3DMinMaxDistance(sound_min_distance, sound_max_distance);
	m_Sounds[SoundName::EXPLOSION]->setMode(FMOD_DEFAULT);

	m_fmodSystem->createSound("./Assets/Sounds/Dead1.ogg", FMOD_3D, 0, &m_Sounds[SoundName::DEAD_1]);
	m_Sounds[SoundName::DEAD_1]->set3DMinMaxDistance(sound_min_distance, sound_max_distance);
	m_Sounds[SoundName::DEAD_1]->setMode(FMOD_DEFAULT);
	m_fmodSystem->createSound("./Assets/Sounds/Dead2.ogg", FMOD_3D, 0, &m_Sounds[SoundName::DEAD_2]);
	m_Sounds[SoundName::DEAD_2]->set3DMinMaxDistance(sound_min_distance, sound_max_distance);
	m_Sounds[SoundName::DEAD_2]->setMode(FMOD_DEFAULT);

	// Load BGM
	m_fmodSystem->createSound("./Assets/Sounds/MainTheme.mp3", FMOD_2D, 0, &m_Sounds[SoundName::MainTheme]);
	m_Sounds[SoundName::MainTheme]->setMode(FMOD_LOOP_NORMAL);

	// UI SOUND
	m_fmodSystem->createSound("./Assets/Sounds/UION.ogg", FMOD_2D, 0, &m_Sounds[SoundName::UI_ON]);
	m_Sounds[SoundName::UI_ON]->setMode(FMOD_DEFAULT);
}

void CSoundMgr::Update(float fTimeElapsed)
{/*	
	vel = how far we moved last FRAME (meter/feet), then time compensate it to SECONDS (m/s).
	vel = (currpos - lastpos) * 1000 / ms														*/
	
	m_listener.vel.x = (m_listener.currpos.x - m_listener.lastpos.x) * fTimeElapsed;
	m_listener.vel.y = (m_listener.currpos.y - m_listener.lastpos.y) * fTimeElapsed;
	m_listener.vel.z = (m_listener.currpos.z - m_listener.lastpos.z) * fTimeElapsed;

	m_listener.lastpos = m_listener.currpos;

	m_fmodSystem->set3DListenerAttributes(0, &m_listener.currpos, &m_listener.vel, &m_listener.forward, &m_listener.up);

	m_fmodSystem->update();
}

void CSoundMgr::PlaySoundByName(SoundName sound_name, const XMFLOAT3& pos)
{
	static FMOD_VECTOR vel{ 0.0f, 0.0f, 0.0f };
	static FMOD_MODE mode;
	m_Sounds[sound_name]->getMode(&mode);
	if (mode & FMOD_3D)
	{
		FMOD_VECTOR fmod_pos{ pos.x*DISTANCE_FACTOR, pos.y*DISTANCE_FACTOR, pos.z*DISTANCE_FACTOR };
		m_fmodSystem->playSound(m_Sounds[sound_name], 0, false, &m_arrChannels[m_CurrNonLoopingChannelIdx]);
		m_arrChannels[m_CurrNonLoopingChannelIdx]->set3DAttributes(&fmod_pos, &vel);
		if (++m_CurrNonLoopingChannelIdx >= MAX_CHANNELS) m_CurrNonLoopingChannelIdx = NUM_LOOPING_CHANNELS;
	}
	else
	{
		m_fmodSystem->playSound(m_Sounds[sound_name], 0, false, &m_arrChannels[m_CurrLoopingChannelIdx]);
		if (++m_CurrLoopingChannelIdx >= NUM_LOOPING_CHANNELS) m_CurrLoopingChannelIdx = 0;
	}
}
