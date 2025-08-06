#include "../D2DEngineLib/framework.h"
#include "SoundPlayer.h"

#include "../D2DEngineLib/SoundManager.h"

float masterVolume = 1.0f;
float bgmVolume = 1.0f;
float sfxVolume = 1.0f;

void SoundPlayer::Start()
{
	SoundManager::Get().PlaySound(L"MainBGM", SoundType::BGM, L"MainBGM", true);
}

void SoundPlayer::Update()
{
	if (Input::IsKeyPressed('1'))
	{
		SoundManager::Get().PlaySound(L"MainBGM", SoundType::BGM, L"MainBGM", true);
	}

	if (Input::IsKeyPressed('2'))
	{
		masterVolume -= 0.1f;
		masterVolume = MyMath::Clamp(masterVolume, 0.0f, 1.0f);

		SoundManager::Get().SetMasterVolume(masterVolume);
	}

	if (Input::IsKeyPressed('3'))
	{
		masterVolume += 0.1f;
		masterVolume = MyMath::Clamp(masterVolume, 0.0f, 1.0f);

		SoundManager::Get().SetMasterVolume(masterVolume);
	}

	if (Input::IsKeyPressed('4'))
	{
		bgmVolume -= 0.1f;
		bgmVolume = MyMath::Clamp(bgmVolume, 0.0f, 1.0f);

		SoundManager::Get().SetBGMVolume(bgmVolume);
	}

	if (Input::IsKeyPressed('5'))
	{
		bgmVolume += 0.1f;
		bgmVolume = MyMath::Clamp(bgmVolume, 0.0f, 1.0f);

		SoundManager::Get().SetBGMVolume(bgmVolume);
	}

	if (Input::IsKeyPressed('6'))
	{
		sfxVolume -= 0.1f;
		sfxVolume = MyMath::Clamp(sfxVolume, 0.0f, 1.0f);

		SoundManager::Get().SetSFXVolume(sfxVolume);
	}

	if (Input::IsKeyPressed('7'))
	{
		sfxVolume += 0.1f;
		sfxVolume = MyMath::Clamp(sfxVolume, 0.0f, 1.0f);

		SoundManager::Get().SetSFXVolume(sfxVolume);
	}

	if (Input::IsKeyPressed('8'))
	{
		SoundManager::Get().PauseSound(L"MainBGM");
	}

	if (Input::IsKeyPressed('9'))
	{
		SoundManager::Get().ResumeSound(L"MainBGM");
	}

	if (Input::IsKeyPressed('0'))
	{
		SoundManager::Get().StopSound(L"MainBGM");
	}

	if (Input::IsKeyPressed('Q'))
	{
		SoundManager::Get().PlaySound(L"LaserSound", SoundType::SFX);
	}
}