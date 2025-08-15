#include "../D2DEngineLib/framework.h"
#include "MainMenuSettingController.h"

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

		float posX = m_indicator->GetTransform()->GetLocalPosition().x;
		float posY = m_settingItems[m_indicatingIndex]->GetTransform()->GetLocalPosition().y;

		m_indicator->GetTransform()->SetLocalPosition(posX, posY);
	}
	else if (Input::IsKeyPressed(VK_DOWN))
	{
		++m_indicatingIndex;

		if (m_indicatingIndex > m_settingItems.size() - 1)
		{
			m_indicatingIndex = static_cast<int>(m_settingItems.size()) - 1;
		}

		float posX = m_indicator->GetTransform()->GetLocalPosition().x;
		float posY = m_settingItems[m_indicatingIndex]->GetTransform()->GetLocalPosition().y;

		m_indicator->GetTransform()->SetLocalPosition(posX, posY);
	}

	if (Input::IsKeyPressed(VK_RIGHT))
	{
		auto comp = m_settingItems[m_indicatingIndex]->GetComponent<VolumeControllerBase>();
		if (comp != nullptr)
		{
			comp->VolumeUp();
		}
	}
	else if (Input::IsKeyPressed(VK_LEFT))
	{
		auto comp = m_settingItems[m_indicatingIndex]->GetComponent<VolumeControllerBase>();
		if (comp != nullptr)
		{
			comp->VolumeDown();
		}
	}

	if (Input::IsKeyPressed('Z'))
	{
		const auto& name = m_settingItems[m_indicatingIndex]->GetName();

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

void MainMenuSettingController::SetIndicator(GameObject* indicator)
{
	m_indicator = indicator;

	float posY = m_settingItems[0]->GetTransform()->GetLocalPosition().y;
	float posX = -840.0f;

	m_indicator->GetTransform()->SetLocalPosition(posX, posY);
}