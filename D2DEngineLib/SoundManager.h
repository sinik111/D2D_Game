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

class SoundManager
{
private:
	FMOD::System* m_system = nullptr;
	std::unordered_map<std::wstring, FMOD::Sound*> m_sounds;
	std::unordered_map<std::wstring, FMOD::Sound*> m_globalSounds;

	FMOD::ChannelGroup* m_channelGroups[3]{}; // Master, BGM, SFX
	std::unordered_map<std::wstring, FMOD::Channel*> m_managedChannel;

	std::string m_resourcePathA;

private:
	SoundManager() = default;
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;
	~SoundManager() = default;

public:
	static SoundManager& Get();

	void Initialize(const std::string& resourcePath);
	void Shutdown();
	void Update();
	void ReleaseSounds();
	void ReleaseGlobalSounds();

public:
	void LoadSound(const std::string& fileName, const std::wstring& soundName);
	void LoadGlobalSound(const std::string& fileName, const std::wstring& soundName);
	void PlaySound(const std::wstring& soundName, SoundType type, const std::wstring& managedName = L"", bool isLoop = false);
	void PlayGlobalSound(const std::wstring& soundName, SoundType type, const std::wstring& managedName = L"", bool isLoop = false);
	void StopSound(const std::wstring& managedName);
	void PauseSound(const std::wstring& managedName);
	void ResumeSound(const std::wstring& managedName);
	bool IsPlaying(const std::wstring& managedName);

	void SetMasterVolume(float volume);
	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume);
};