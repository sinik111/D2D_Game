#include "../D2DEngineLib/framework.h"
#include "BGMVolumeController.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/SoundManager.h"

void BGMVolumeController::Start()
{
	m_originalVolume = SoundManager::Get().GetBGMVolume();

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, m_originalVolume);


	GameObject* volumeSliderFrame = CreateGameObject(L"BGMVolumeSliderFrame");
	volumeSliderFrame->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	auto br = volumeSliderFrame->AddComponent<BitmapRenderer>(L"Image/VolumeSliderFrame.bmp");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot(m_bitmapRenderer->GetPivot());


	GameObject* bgmVolumeName = CreateGameObject(L"BGMVolumeName");
	bgmVolumeName->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() - Vector2(200.0f, 0.0f));
	br = bgmVolumeName->AddComponent<BitmapRenderer>(L"Image/BGMVolume.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });
}

void BGMVolumeController::VolumeUp()
{
	int volumeInteger = static_cast<int>(std::roundf(SoundManager::Get().GetBGMVolume() * 10.0f));
	if (volumeInteger == 10)
	{
		return;
	}

	++volumeInteger;

	float volumeFloat = volumeInteger * 0.1f;

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volumeFloat);

	SoundManager::Get().SetBGMVolume(volumeFloat);
}

void BGMVolumeController::VolumeDown()
{
	int volumeInteger = static_cast<int>(std::roundf(SoundManager::Get().GetBGMVolume() * 10.0f));
	if (volumeInteger == 0)
	{
		return;
	}

	--volumeInteger;

	float volumeFloat = volumeInteger * 0.1f;

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volumeFloat);

	SoundManager::Get().SetBGMVolume(volumeFloat);
}

void BGMVolumeController::SetVolume(float volume)
{
	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volume);

	SoundManager::Get().SetBGMVolume(volume);
}

void BGMVolumeController::Reset()
{
	float defaultVolume = SoundManager::Get().SetBGMDefaultVolume();

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, defaultVolume);
}

void BGMVolumeController::Cancel()
{
	SoundManager::Get().SetBGMVolume(m_originalVolume);
}

void BGMVolumeController::Save()
{
	m_originalVolume = SoundManager::Get().GetBGMVolume();
}