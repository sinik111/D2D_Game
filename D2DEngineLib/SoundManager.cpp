#include "pch.h"
#include "SoundManager.h"

#include <fstream>

#include "../Vendor/FMOD/fmod.hpp"

#include "json.hpp"
#include "ResourceManager.h"
#include "Camera.h"
#include "Transform.h"

#pragma comment(lib, "fmod_vc.lib")

using nlohmann::json;

static const int MASTER = 0;
static const int BGM = 1;
static const int SFX = 2;

static void from_json(const json& j, VolumeSettingData& volumeSettingData)
{
	j.at("DefaultMasterVolume").get_to(volumeSettingData.defaultMasterVolume);
	j.at("DefaultBGMVolume").get_to(volumeSettingData.defaultBGMVolume);
	j.at("DefaultSFXVolume").get_to(volumeSettingData.defaultSFXVolume);
	j.at("CurrentMasterVolume").get_to(volumeSettingData.currentMasterVolume);
	j.at("CurrentBGMVolume").get_to(volumeSettingData.currentBGMVolume);
	j.at("CurrentSFXVolume").get_to(volumeSettingData.currentSFXVolume);
}

static void to_json(json& j, const VolumeSettingData& volumeSettingData)
{
	j = json{
		{ "DefaultMasterVolume", volumeSettingData.defaultMasterVolume },
		{ "DefaultBGMVolume", volumeSettingData.defaultBGMVolume },
		{ "DefaultSFXVolume", volumeSettingData.defaultSFXVolume },
		{ "CurrentMasterVolume", volumeSettingData.currentMasterVolume },
		{ "CurrentBGMVolume", volumeSettingData.currentBGMVolume },
		{ "CurrentSFXVolume", volumeSettingData.currentSFXVolume }
	};
}

SoundManager& SoundManager::Get()
{
	static SoundManager s_instance;

	return s_instance;
}

void SoundManager::Initialize()
{
	m_resourcePathA = ResourceManager::Get().GetResourcePathA();

	FMOD::System_Create(&m_system);
	m_system->init(512, FMOD_INIT_NORMAL, 0);

	m_system->getMasterChannelGroup(&m_channelGroups[MASTER]);
	m_system->createChannelGroup("BGM_Group", &m_channelGroups[BGM]);
	m_system->createChannelGroup("SFX_Group", &m_channelGroups[SFX]);

	m_channelGroups[MASTER]->addGroup(m_channelGroups[BGM]);
	m_channelGroups[MASTER]->addGroup(m_channelGroups[SFX]);

	FMOD_RESULT fr = m_system->set3DSettings(1.0f, 1.0f, 1.0f);

	LoadVolumeSettingData();
}

void SoundManager::Shutdown()
{
	SaveSoundSettingData();

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

	Vector2 cameraPosition = Camera::s_mainCamera->GetTransform()->GetLocalPosition();

	FMOD_VECTOR listenerPos = { cameraPosition.x, cameraPosition.y, 0.0f };
	FMOD_VECTOR listenerVel = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR listenerForward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR listenerUp = { 0.0f, 1.0f, 0.0f };

	FMOD_RESULT fr = m_system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &listenerForward, &listenerUp);
	int a = 0;


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

void SoundManager::LoadSound(const std::string& fileName, const std::wstring& soundName, bool is2D)
{
	if (m_sounds.find(soundName) != m_sounds.end())
	{
		assert(false && L"이미 있는 soundName");
	}

	FMOD::Sound* sound = nullptr;

	std::string filePath = m_resourcePathA + fileName;

	int flag = is2D ? FMOD_2D | FMOD_DEFAULT | FMOD_CREATESAMPLE : FMOD_3D | FMOD_DEFAULT | FMOD_3D_LINEARROLLOFF | FMOD_CREATESAMPLE;

	FMOD_RESULT fr = m_system->createSound(filePath.c_str(), flag, nullptr, &sound);
	
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

	fr = sound->set3DMinMaxDistance(400.0f, 1000.0f);

	m_sounds.emplace(soundName, sound);
}

void SoundManager::LoadGlobalSound(const std::string& fileName, const std::wstring& soundName, bool is2D)
{
	if (m_globalSounds.find(soundName) != m_globalSounds.end())
	{
		assert(false && L"이미 있는 soundName");
	}

	FMOD::Sound* sound = nullptr;

	std::string filePath = m_resourcePathA + fileName;

	int flag = is2D ? FMOD_2D | FMOD_DEFAULT | FMOD_CREATESAMPLE : FMOD_3D | FMOD_DEFAULT | FMOD_3D_LINEARROLLOFF | FMOD_CREATESAMPLE;

	FMOD_RESULT fr = m_system->createSound(filePath.c_str(), flag, nullptr, &sound);

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

void SoundManager::PlaySound(const std::wstring& soundName, SoundType type, const Vector2* position, const std::wstring& managedName, bool isLoop)
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

	if (position != nullptr)
	{
		FMOD_VECTOR soundPos = { position->x, position->y, 0.0f };
		FMOD_VECTOR soundVel = { 0.0f, 0.0f, 0.0f };

		FMOD_RESULT fr = channel->set3DAttributes(&soundPos, &soundVel);
		int a = 0;
	}
	else
	{
		Vector2 cameraPosition = Camera::s_mainCamera->GetTransform()->GetLocalPosition();

		FMOD_VECTOR soundPos = { cameraPosition.x, cameraPosition.y, 0.0f };
		FMOD_VECTOR soundVel = { 0.0f, 0.0f, 0.0f };

		channel->set3DAttributes(&soundPos, &soundVel);
	}

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

void SoundManager::PlayGlobalSound(const std::wstring& soundName, SoundType type, const Vector2* position, const std::wstring& managedName, bool isLoop)
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

	if (position != nullptr)
	{
		FMOD_VECTOR soundPos = { position->x, position->y, 0.0f };
		FMOD_VECTOR soundVel = { 0.0f, 0.0f, 0.0f };

		channel->set3DAttributes(&soundPos, &soundVel);
	}
	else
	{
		Vector2 cameraPosition = Camera::s_mainCamera->GetTransform()->GetLocalPosition();

		FMOD_VECTOR soundPos = { cameraPosition.x, cameraPosition.y, 0.0f };
		FMOD_VECTOR soundVel = { 0.0f, 0.0f, 0.0f };

		channel->set3DAttributes(&soundPos, &soundVel);
	}

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

void SoundManager::Update3DAttribute(const std::wstring& managedName, const Vector2& position)
{
	const auto& iter = m_managedChannel.find(managedName);
	if (iter != m_managedChannel.end())
	{
		FMOD_VECTOR soundPos = { position.x, position.y, 0.0f };
		FMOD_VECTOR soundVel = { 0.0f, 0.0f, 0.0f };

		FMOD_RESULT fr = iter->second->set3DAttributes(&soundPos, &soundVel);
	}
}

float SoundManager::GetMasterVolume() const
{
	return m_volumeSettingData.currentMasterVolume;
}

float SoundManager::GetBGMVolume() const
{
	return m_volumeSettingData.currentBGMVolume;
}

float SoundManager::GetSFXVolume() const
{
	return m_volumeSettingData.currentSFXVolume;
}

void SoundManager::SetMasterVolume(float volume)
{
	m_channelGroups[MASTER]->setVolume(volume);

	m_volumeSettingData.currentMasterVolume = volume;
}

void SoundManager::SetBGMVolume(float volume)
{
	m_channelGroups[BGM]->setVolume(volume);

	m_volumeSettingData.currentBGMVolume = volume;
}

void SoundManager::SetSFXVolume(float volume)
{
	m_channelGroups[SFX]->setVolume(volume);

	m_volumeSettingData.currentSFXVolume = volume;
}

float SoundManager::SetMasterDefaultVolume()
{
	m_channelGroups[MASTER]->setVolume(m_volumeSettingData.defaultMasterVolume);

	m_volumeSettingData.currentMasterVolume = m_volumeSettingData.defaultMasterVolume;

	return m_volumeSettingData.currentMasterVolume;
}

float SoundManager::SetBGMDefaultVolume()
{
	m_channelGroups[BGM]->setVolume(m_volumeSettingData.defaultBGMVolume);

	m_volumeSettingData.currentBGMVolume = m_volumeSettingData.defaultBGMVolume;

	return m_volumeSettingData.currentBGMVolume;
}

float SoundManager::SetSFXDefaultVolume()
{
	m_channelGroups[SFX]->setVolume(m_volumeSettingData.defaultSFXVolume);

	m_volumeSettingData.currentSFXVolume = m_volumeSettingData.defaultSFXVolume;

	return m_volumeSettingData.currentSFXVolume;
}

void SoundManager::LoadVolumeSettingData()
{
	std::ifstream inFile(ResourceManager::Get().GetResourcePath() + L"Data/VolumeSettingData.json");
	if (inFile.is_open())
	{
		json j;
		inFile >> j;
		inFile.close();

		m_volumeSettingData.defaultMasterVolume = j["DefaultMasterVolume"];
		m_volumeSettingData.defaultBGMVolume = j["DefaultBGMVolume"];
		m_volumeSettingData.defaultSFXVolume = j["DefaultSFXVolume"];
		m_volumeSettingData.currentMasterVolume = j["CurrentMasterVolume"];
		m_volumeSettingData.currentBGMVolume = j["CurrentBGMVolume"];
		m_volumeSettingData.currentSFXVolume = j["CurrentSFXVolume"];
	}

	m_channelGroups[MASTER]->setVolume(m_volumeSettingData.currentMasterVolume);
	m_channelGroups[BGM]->setVolume(m_volumeSettingData.currentBGMVolume);
	m_channelGroups[SFX]->setVolume(m_volumeSettingData.currentSFXVolume);
}

void SoundManager::SaveSoundSettingData()
{
	json j;
	to_json(j, m_volumeSettingData);

	std::ofstream outputFile(ResourceManager::Get().GetResourcePath() + L"Data/VolumeSettingData.json");
	if (outputFile.is_open())
	{
		outputFile << j.dump(4);
		outputFile.close();
	}
}