#include "../D2DEngineLib/framework.h"
#include "MasterVolumeController.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/SoundManager.h"

void MasterVolumeController::Start()
{
	m_originalVolume = SoundManager::Get().GetMasterVolume();

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, m_originalVolume);


	GameObject* volumeSliderFrame = CreateGameObject(L"MasterVolumeSliderFrame");
	volumeSliderFrame->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	auto br = volumeSliderFrame->AddComponent<BitmapRenderer>(L"Image/VolumeSliderFrame.bmp");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot(m_bitmapRenderer->GetPivot());


	GameObject* masterVolumeName = CreateGameObject(L"MasterVolumeName");
	masterVolumeName->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() - Vector2(200.0f, 0.0f));
	br = masterVolumeName->AddComponent<BitmapRenderer>(L"Image/MasterVolume.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });
}

void MasterVolumeController::VolumeUp()
{
	int volumeInteger = static_cast<int>(std::roundf(SoundManager::Get().GetMasterVolume() * 10.0f));
	if (volumeInteger == 10)
	{
		return;
	}

	++volumeInteger;

	float volumeFloat = volumeInteger * 0.1f;

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volumeFloat);

	SoundManager::Get().SetMasterVolume(volumeFloat);
}

void MasterVolumeController::VolumeDown()
{
	int volumeInteger = static_cast<int>(std::roundf(SoundManager::Get().GetMasterVolume() * 10.0f));
	if (volumeInteger == 0)
	{
		return;
	}

	--volumeInteger;

	float volumeFloat = volumeInteger * 0.1f;

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volumeFloat);

	SoundManager::Get().SetMasterVolume(volumeFloat);
}

void MasterVolumeController::SetVolume(float volume)
{
	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volume);

	SoundManager::Get().SetMasterVolume(volume);
}

void MasterVolumeController::Reset()
{
	float defaultVolume = SoundManager::Get().SetMasterDefaultVolume();

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, defaultVolume);
}

void MasterVolumeController::Cancel()
{
	SoundManager::Get().SetMasterVolume(m_originalVolume);
}

void MasterVolumeController::Save()
{
	m_originalVolume = SoundManager::Get().GetMasterVolume();
}