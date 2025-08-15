#include "../D2DEngineLib/framework.h"
#include "MainMenuSceneController.h"

#include "../D2DEngineLib/SceneManager.h"

#include "ScreenCurtainController.h"
#include "GameDataManager.h"

void MainMenuSceneController::Start()
{
	m_indicatingIndex = GameDataManager::Get().GetSelectedMenuIndex();
}

void MainMenuSceneController::Update()
{
	if (Input::IsKeyPressed(VK_UP))
	{
		--m_indicatingIndex;

		if (m_indicatingIndex < 0)
		{
			m_indicatingIndex = 0;
		}

		float posX = m_indicator->GetTransform()->GetLocalPosition().x;
		float posY = m_menuButtons[m_indicatingIndex]->GetTransform()->GetLocalPosition().y;

		m_indicator->GetTransform()->SetLocalPosition(posX, posY);
	}
	else if (Input::IsKeyPressed(VK_DOWN))
	{
		++m_indicatingIndex;

		if (m_indicatingIndex > m_menuButtons.size() - 1)
		{
			m_indicatingIndex = static_cast<int>(m_menuButtons.size()) - 1;
		}

		float posX = m_indicator->GetTransform()->GetLocalPosition().x;
		float posY = m_menuButtons[m_indicatingIndex]->GetTransform()->GetLocalPosition().y;

		m_indicator->GetTransform()->SetLocalPosition(posX, posY);
	}
	else if (m_screenCurtainController == nullptr && Input::IsKeyPressed('Z'))
	{
		GameDataManager::Get().SetSelectedMenuIndex(m_indicatingIndex);

		m_nextSceneName = m_menuButtons[m_indicatingIndex]->GetName();

		GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
		m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, GameDataManager::Get().GetFadeDuration());
	}

	if (m_screenCurtainController != nullptr)
	{
		if (m_screenCurtainController->IsFinished())
		{
			if (m_nextSceneName == L"Exit")
			{
				PostQuitMessage(0);

				return;
			}

			SceneManager::Get().ChangeScene(m_nextSceneName);
		}
	}
}

void MainMenuSceneController::SetMenuButtons(const std::vector<GameObject*>& menuButtons)
{
	m_menuButtons = menuButtons;

	size_t count = m_menuButtons.size();

	for (size_t i = 0; i < count; ++i)
	{
		m_menuButtons[i]->GetTransform()->SetLocalPosition(
			Vector2::Lerp(m_startPosition, m_endPosition, static_cast<float>(i) / (count - 1)));
	}
}

void MainMenuSceneController::SetIndicator(GameObject* indicator)
{
	m_indicator = indicator;

	float posY = m_menuButtons[GameDataManager::Get().GetSelectedMenuIndex()]->GetTransform()->GetLocalPosition().y;
	float posX = -900.0f;

	m_indicator->GetTransform()->SetLocalPosition(-840.0f, posY);
}
