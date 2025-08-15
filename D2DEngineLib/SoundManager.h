#pragma once

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
	class ChannelGroup;
}

enum class SoundType
{
	BGM = 1,
	SFX = 2
};

struct VolumeSettingData
{
	float defaultMasterVolume = 1.0f;
	float defaultBGMVolume = 1.0f;
	float defaultSFXVolume = 1.0f;
	float currentMasterVolume = 1.0f;
	float currentBGMVolume = 1.0f;
	float currentSFXVolume = 1.0f;
};

class SoundManager
{
private:
	FMOD::System* m_system = nullptr;
	std::unordered_map<std::wstring, FMOD::Sound*> m_sounds;
	std::unordered_map<std::wstring, FMOD::Sound*> m_globalSounds;

	FMOD::ChannelGroup* m_channelGroups[3]{}; // Master, BGM, SFX
	std::unordered_map<std::wstring, FMOD::Channel*> m_managedChannel;

	std::string m_resourcePathA;
	VolumeSettingData m_volumeSettingData;

private:
	SoundManager() = default;
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;
	~SoundManager() = default;

public:
	static SoundManager& Get();

	void Initialize();
	void Shutdown();
	void Update();
	void ReleaseSounds();
	void ReleaseGlobalSounds();

public:
	void LoadSound(const std::string& fileName, const std::wstring& soundName, bool is2D = false);
	void LoadGlobalSound(const std::string& fileName, const std::wstring& soundName, bool is2D = false);
	void PlaySound(const std::wstring& soundName, SoundType type, const Vector2* position = nullptr, const std::wstring& managedName = L"", bool isLoop = false);
	void PlayGlobalSound(const std::wstring& soundName, SoundType type, const Vector2* position = nullptr, const std::wstring& managedName = L"", bool isLoop = false);
	void StopSound(const std::wstring& managedName);
	void PauseSound(const std::wstring& managedName);
	void ResumeSound(const std::wstring& managedName);
	bool IsPlaying(const std::wstring& managedName);

	void Update3DAttribute(const std::wstring& managedName, const Vector2& position);

	float GetMasterVolume() const;
	float GetBGMVolume() const;
	float GetSFXVolume() const;

	void SetMasterVolume(float volume);
	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume);

	float SetMasterDefaultVolume();
	float SetBGMDefaultVolume();
	float SetSFXDefaultVolume();

private:
	void LoadVolumeSettingData();
	void SaveSoundSettingData();
};