#include "../D2DEngineLib/framework.h"
#include "MasterVolumeController.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/SoundManager.h"

void MasterVolumeController::Start()
{
	m_originalVolume = SoundManager::Get().GetMasterVolume();

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, m_originalVolume);


	m_volumeSliderFrame = CreateGameObject(L"MasterVolumeSliderFrame");
	m_volumeSliderFrame->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	auto br = m_volumeSliderFrame->AddComponent<BitmapRenderer>(L"Image/UI/VOLUMEBAR_01.png");
	br->SetSortOrder(12);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot(m_bitmapRenderer->GetPivot());

	D2D1_RECT_F rect = br->GetSourceRect();
	m_barWidth = rect.right - rect.left;

	m_masterVolumeName = CreateGameObject(L"MasterVolumeName");
	m_masterVolumeName->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() - Vector2(450.0f, 0.0f));
	br = m_masterVolumeName->AddComponent<BitmapRenderer>(L"Image/UI/ICON_MASTER_02.png");
	br->SetSortOrder(12);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });

	m_volumeIndicator = CreateGameObject(L"VolumeIndicator");
	br = m_volumeIndicator->AddComponent<BitmapRenderer>(L"Image/UI/VOLUMEBAR_02.png");
	br->SetSortOrder(14);
	br->SetSpaceType(SpaceType::Screen);

	float position = m_barWidth * m_originalVolume;
	m_volumeIndicator->GetTransform()->SetLocalPosition({ position, GetTransform()->GetLocalPosition().y });
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

	float position = m_barWidth * volumeFloat;
	m_volumeIndicator->GetTransform()->SetLocalPosition({ position, GetTransform()->GetLocalPosition().y });

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

	float position = m_barWidth * volumeFloat;
	m_volumeIndicator->GetTransform()->SetLocalPosition({ position, GetTransform()->GetLocalPosition().y });

	SoundManager::Get().SetMasterVolume(volumeFloat);
}

void MasterVolumeController::SetVolume(float volume)
{
	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, volume);

	float position = m_barWidth * volume;
	m_volumeIndicator->GetTransform()->SetLocalPosition({ position, GetTransform()->GetLocalPosition().y });

	SoundManager::Get().SetMasterVolume(volume);
}

void MasterVolumeController::Reset()
{
	float defaultVolume = SoundManager::Get().SetMasterDefaultVolume();

	float position = m_barWidth * defaultVolume;
	m_volumeIndicator->GetTransform()->SetLocalPosition({ position, GetTransform()->GetLocalPosition().y });

	m_bitmapRenderer->SetFill(BitmapDirectionType::LeftToRight, defaultVolume);
}

void MasterVolumeController::Cancel(bool destroy)
{
	SoundManager::Get().SetMasterVolume(m_originalVolume);

	if (destroy)
	{
		Destroy(m_volumeSliderFrame);
		Destroy(m_masterVolumeName);
		Destroy(m_volumeIndicator);
	}
}

void MasterVolumeController::Save()
{
	m_originalVolume = SoundManager::Get().GetMasterVolume();
}

void MasterVolumeController::SetSelected(bool selected)
{
	if (selected)
	{
		m_masterVolumeName->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/ICON_MASTER_02.png");
		m_masterVolumeName->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() - Vector2(450.0f, 0.0f));
	}
	else
	{
		m_masterVolumeName->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/ICON_MASTER_01.png");
		m_masterVolumeName->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() - Vector2(424.0f, 0.0f));
	}
}