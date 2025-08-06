#include "pch.h"
#include "SoundManager.h"

#include "../Vendor/FMOD/fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")

static const int MASTER = 0;
static const int BGM = 1;
static const int SFX = 2;

SoundManager& SoundManager::Get()
{
	static SoundManager s_instance;

	return s_instance;
}

void SoundManager::Initialize(const std::string& resourcePath)
{
	m_resourcePathA = resourcePath;

	FMOD::System_Create(&m_system);
	m_system->init(512, FMOD_INIT_NORMAL, 0);

	m_system->getMasterChannelGroup(&m_channelGroups[MASTER]);
	m_system->createChannelGroup("BGM_Group", &m_channelGroups[BGM]);
	m_system->createChannelGroup("SFX_Group", &m_channelGroups[SFX]);

	m_channelGroups[MASTER]->addGroup(m_channelGroups[BGM]);
	m_channelGroups[MASTER]->addGroup(m_channelGroups[SFX]);
}

void SoundManager::Shutdown()
{
	ReleaseSounds();
	ReleaseGlobalSounds();

	if (m_channelGroups[BGM] != nullptr)
	{
		m_channelGroups[BGM]->release();
	}

	if (m_channelGroups[SFX] != nullptr)
	{
		m_channelGroups[SFX]->release();
	}

	if (m_system != nullptr)
	{
		m_system->close();
		m_system->release();
	}
}

void SoundManager::Update()
{
	m_system->update();

	for (auto iter = m_managedChannel.begin(); iter != m_managedChannel.end(); )
	{
		bool isPlaying;
		FMOD_RESULT result = iter->second->isPlaying(&isPlaying);

		if (result != FMOD_OK || (result == FMOD_OK && !isPlaying))
		{
			iter = m_managedChannel.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void SoundManager::ReleaseSounds()
{
	for (auto pair : m_sounds)
	{
		pair.second->release();
	}

	m_sounds.clear();
}

void SoundManager::ReleaseGlobalSounds()
{
	for (auto pair : m_globalSounds)
	{
		pair.second->release();
	}

	m_globalSounds.clear();
}

void SoundManager::LoadSound(const std::string& fileName, const std::wstring& soundName)
{
	if (m_sounds.find(soundName) != m_sounds.end())
	{
		assert(false && L"이미 있는 soundName");
	}

	FMOD::Sound* sound = nullptr;

	std::string filePath = m_resourcePathA + fileName;

	FMOD_RESULT fr = m_system->createSound(filePath.c_str(), FMOD_DEFAULT | FMOD_CREATESAMPLE, nullptr, &sound);
	
	assert(fr == FMOD_OK && L"LoadSound 실패");

	FMOD_OPENSTATE fos;

	while (true)
	{
		sound->getOpenState(&fos, nullptr, nullptr, nullptr);

		if (fos == FMOD_OPENSTATE_READY)
		{
			break;
		}
	}

	m_sounds.emplace(soundName, sound);
}

void SoundManager::LoadGlobalSound(const std::string& fileName, const std::wstring& soundName)
{
	if (m_globalSounds.find(soundName) != m_globalSounds.end())
	{
		assert(false && L"이미 있는 soundName");
	}

	FMOD::Sound* sound = nullptr;

	std::string filePath = m_resourcePathA + fileName;

	FMOD_RESULT fr = m_system->createSound(filePath.c_str(), FMOD_DEFAULT | FMOD_CREATESAMPLE, nullptr, &sound);

	assert(fr == FMOD_OK && L"LoadSound 실패");

	FMOD_OPENSTATE fos;

	while (true)
	{
		sound->getOpenState(&fos, nullptr, nullptr, nullptr);

		if (fos == FMOD_OPENSTATE_READY)
		{
			break;
		}
	}

	m_globalSounds.emplace(soundName, sound);
}

void SoundManager::PlaySound(const std::wstring& soundName, SoundType type, const std::wstring& managedName, bool isLoop)
{
	if (m_sounds.find(soundName) == m_sounds.end())
	{
		assert(false && L"없는 soundName");

		return;
	}

	FMOD::Sound* sound = m_sounds[soundName];

	if (isLoop)
	{
		sound->setMode(FMOD_LOOP_NORMAL);
	}
	else
	{
		sound->setMode(FMOD_LOOP_OFF);
	}

	FMOD::Channel* channel = nullptr;

	m_system->playSound(sound, m_channelGroups[static_cast<int>(type)], false, &channel);

	if (!managedName.empty())
	{
		if (m_managedChannel.find(managedName) != m_managedChannel.end())
		{
			assert(false && L"managedName은 고유해야 합니다");

			return;
		}

		m_managedChannel.emplace(managedName, channel);
	}
}

void SoundManager::PlayGlobalSound(const std::wstring& soundName, SoundType type, const std::wstring& managedName, bool isLoop)
{
	if (m_globalSounds.find(soundName) == m_globalSounds.end())
	{
		assert(false && L"없는 soundName");

		return;
	}

	FMOD::Sound* sound = m_globalSounds[soundName];

	if (isLoop)
	{
		sound->setMode(FMOD_LOOP_NORMAL);
	}
	else
	{
		sound->setMode(FMOD_LOOP_OFF);
	}

	FMOD::Channel* channel = nullptr;

	m_system->playSound(sound, m_channelGroups[static_cast<int>(type)], false, &channel);

	if (!managedName.empty())
	{
		if (m_managedChannel.find(managedName) != m_managedChannel.end())
		{
			assert(false && L"managedName은 고유해야 합니다");

			return;
		}

		m_managedChannel.emplace(managedName, channel);
	}
}

void SoundManager::StopSound(const std::wstring& managedName)
{
	const auto& iter = m_managedChannel.find(managedName);
	if (iter != m_managedChannel.end())
	{
		iter->second->stop();
		m_managedChannel.erase(iter);
	}
}

void SoundManager::PauseSound(const std::wstring& managedName)
{
	const auto& iter = m_managedChannel.find(managedName);
	if (iter != m_managedChannel.end())
	{
		iter->second->setPaused(true);
	}
}

void SoundManager::ResumeSound(const std::wstring& managedName)
{
	const auto& iter = m_managedChannel.find(managedName);
	if (iter != m_managedChannel.end())
	{
		iter->second->setPaused(false);
	}
}

bool SoundManager::IsPlaying(const std::wstring& managedName)
{
	return m_managedChannel.find(managedName) != m_managedChannel.end();
}

void SoundManager::SetMasterVolume(float volume)
{
	m_channelGroups[MASTER]->setVolume(volume);
}

void SoundManager::SetBGMVolume(float volume)
{
	m_channelGroups[BGM]->setVolume(volume);
}

void SoundManager::SetSFXVolume(float volume)
{
	m_channelGroups[SFX]->setVolume(volume);
}