#include "../D2DEngineLib/framework.h"
#include "MainMenuSettingController.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/SoundManager.h"

#include "MasterVolumeController.h"
#include "BGMVolumeController.h"
#include "SFXVolumeController.h"
#include "VolumeControllerBase.h"
#include "ScreenCurtainController.h"
#include "GameDataManager.h"

void MainMenuSettingController::Update()
{
	if (Input::IsKeyPressed(VK_UP))
	{
		--m_indicatingIndex;

		if (m_indicatingIndex < 0)
		{
			m_indicatingIndex = 0;
		}
		else
		{
			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
		}

		for (int i = 0; i < m_settingItems.size(); ++i)
		{
			if (i == m_indicatingIndex)
			{
				auto comp = m_settingItems[i]->GetComponent<VolumeControllerBase>();
				if (comp != nullptr)
				{
					comp->SetSelected(true);
				}
				else
				{
					auto br = m_settingItems[i]->GetComponent<BitmapRenderer>();

					if (i == 3)
					{
						Vector2 position = Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (m_settingItems.size() - 1));
						position.x += 2.0f;
						position.y -= 3.0f;
						m_settingItems[i]->GetTransform()->SetLocalPosition(position);
						br->SetBitmap(L"Image/UI/ICON_RESET_02.png");
					}
					else if (i == 4)
					{
						Vector2 position = Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (m_settingItems.size() - 1));
						position.x += 1.0f;
						position.y += 3.0f;
						m_settingItems[i]->GetTransform()->SetLocalPosition(position);
						br->SetBitmap(L"Image/UI/ICON_SAVE_02.png");
					}
				}
			}
			else
			{
				auto comp = m_settingItems[i]->GetComponent<VolumeControllerBase>();
				if (comp != nullptr)
				{
					comp->SetSelected(false);
				}
				else
				{
					auto br = m_settingItems[i]->GetComponent<BitmapRenderer>();

					if (i == 3)
					{
						Vector2 position = Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (m_settingItems.size() - 1));
						m_settingItems[i]->GetTransform()->SetLocalPosition(position);
						br->SetBitmap(L"Image/UI/ICON_RESET_01.png");
					}
					else if (i == 4)
					{
						Vector2 position = Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (m_settingItems.size() - 1));
						m_settingItems[i]->GetTransform()->SetLocalPosition(position);
						br->SetBitmap(L"Image/UI/ICON_SAVE_01.png");
					}
				}
			}
		}
	}
	else if (Input::IsKeyPressed(VK_DOWN))
	{
		++m_indicatingIndex;

		if (m_indicatingIndex > m_settingItems.size() - 1)
		{
			m_indicatingIndex = static_cast<int>(m_settingItems.size()) - 1;
		}
		else
		{
			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
		}

		for (int i = 0; i < m_settingItems.size(); ++i)
		{
			if (i == m_indicatingIndex)
			{
				auto comp = m_settingItems[i]->GetComponent<VolumeControllerBase>();
				if (comp != nullptr)
				{
					comp->SetSelected(true);
				}
				else
				{
					auto br = m_settingItems[i]->GetComponent<BitmapRenderer>();

					if (i == 3)
					{
						Vector2 position = Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (m_settingItems.size() - 1));
						position.x += 2.0f;
						position.y -= 3.0f;
						m_settingItems[i]->GetTransform()->SetLocalPosition(position);
						br->SetBitmap(L"Image/UI/ICON_RESET_02.png");
					}
					else if (i == 4)
					{
						Vector2 position = Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (m_settingItems.size() - 1));
						/*position.x += 1.0f;
						position.y += 3.0f;*/
						m_settingItems[i]->GetTransform()->SetLocalPosition(position);
						br->SetBitmap(L"Image/UI/ICON_SAVE_02.png");
					}
				}
			}
			else
			{
				auto comp = m_settingItems[i]->GetComponent<VolumeControllerBase>();
				if (comp != nullptr)
				{
					comp->SetSelected(false);
				}
				else
				{
					auto br = m_settingItems[i]->GetComponent<BitmapRenderer>();

					if (i == 3)
					{
						Vector2 position = Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (m_settingItems.size() - 1));
						m_settingItems[i]->GetTransform()->SetLocalPosition(position);
						br->SetBitmap(L"Image/UI/ICON_RESET_01.png");
					}
					else if (i == 4)
					{
						Vector2 position = Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (m_settingItems.size() - 1));
						m_settingItems[i]->GetTransform()->SetLocalPosition(position);
						br->SetBitmap(L"Image/UI/ICON_SAVE_01.png");
					}
				}
			}
		}
	}

	if (Input::IsKeyPressed(VK_RIGHT))
	{
		auto comp = m_settingItems[m_indicatingIndex]->GetComponent<VolumeControllerBase>();
		if (comp != nullptr)
		{
			comp->VolumeUp();

			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
		}
	}
	else if (Input::IsKeyPressed(VK_LEFT))
	{
		auto comp = m_settingItems[m_indicatingIndex]->GetComponent<VolumeControllerBase>();
		if (comp != nullptr)
		{
			comp->VolumeDown();

			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
		}
	}

	if (Input::IsKeyPressed('Z'))
	{
		const auto& name = m_settingItems[m_indicatingIndex]->GetName();

		SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

		if (name == L"Reset")
		{
			for (const auto& go : m_settingItems)
			{
				auto comp = go->GetComponent<VolumeControllerBase>();
				if (comp != nullptr)
				{
					comp->Reset();
				}
			}
		}
		else if (name == L"Save")
		{
			for (const auto& go : m_settingItems)
			{
				auto comp = go->GetComponent<VolumeControllerBase>();
				if (comp != nullptr)
				{
					comp->Save();
				}
			}
		}
	}

	if (m_screenCurtainController == nullptr && Input::IsKeyPressed('X'))
	{
		SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

		for (const auto& go : m_settingItems)
		{
			auto comp = go->GetComponent<VolumeControllerBase>();
			if (comp != nullptr)
			{
				comp->Cancel();
			}
		}

		GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
		m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, GameDataManager::Get().GetFadeDuration());	
	}

	if (m_screenCurtainController != nullptr)
	{
		if (m_screenCurtainController->IsFinished())
		{
			SceneManager::Get().ChangeScene(L"MainMenu");
		}
	}
}

void MainMenuSettingController::SetSettingItems(std::vector<GameObject*> settingItems)
{
	m_settingItems = settingItems;

	size_t count = m_settingItems.size();

	for (size_t i = 0; i < count; ++i)
	{
		m_settingItems[i]->GetTransform()->SetLocalPosition(
			Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (count - 1)));
	}
}