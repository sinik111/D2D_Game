#include "../D2DEngineLib/framework.h"
#include "SFXVolumeController.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/SoundManager.h"

void SFXVolumeController::Start()
{
	m_originalVolume = SoundManager::Get().GetSFXVolume();

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, m_originalVolume);


	GameObject* volumeSliderFrame = CreateGameObject(L"SFXVolumeSliderFrame");
	volumeSliderFrame->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	auto br = volumeSliderFrame->AddComponent<BitmapRenderer>(L"Image/VolumeSliderFrame.bmp");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot(m_bitmapRenderer->GetPivot());


	GameObject* sfxVolumeName = CreateGameObject(L"SFXVolumeName");
	sfxVolumeName->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() - Vector2(200.0f, 0.0f));
	br = sfxVolumeName->AddComponent<BitmapRenderer>(L"Image/SFXVolume.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });
}

void SFXVolumeController::VolumeUp()
{
	int volumeInteger = static_cast<int>(std::roundf(SoundManager::Get().GetSFXVolume() * 10.0f));
	if (volumeInteger == 10)
	{
		return;
	}

	++volumeInteger;

	float volumeFloat = volumeInteger * 0.1f;

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volumeFloat);

	SoundManager::Get().SetSFXVolume(volumeFloat);
}

void SFXVolumeController::VolumeDown()
{
	int volumeInteger = static_cast<int>(std::roundf(SoundManager::Get().GetSFXVolume() * 10.0f));
	if (volumeInteger == 0)
	{
		return;
	}

	--volumeInteger;

	float volumeFloat = volumeInteger * 0.1f;

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volumeFloat);

	SoundManager::Get().SetSFXVolume(volumeFloat);
}

void SFXVolumeController::SetVolume(float volume)
{
	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volume);

	SoundManager::Get().SetSFXVolume(volume);
}

void SFXVolumeController::Reset()
{
	float defaultVolume = SoundManager::Get().SetSFXDefaultVolume();

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, defaultVolume);
}

void SFXVolumeController::Cancel()
{
	SoundManager::Get().SetSFXVolume(m_originalVolume);
}

void SFXVolumeController::Save()
{
	m_originalVolume = SoundManager::Get().GetSFXVolume();
}