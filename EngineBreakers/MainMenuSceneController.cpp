#include "../D2DEngineLib/framework.h"
#include "MainMenuSceneController.h"

#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/SoundManager.h"

#include "ScreenCurtainController.h"
#include "GameDataManager.h"

void MainMenuSceneController::Update()
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

		size_t count = m_menuButtons.size();

		for (size_t i = 0; i < count; ++i)
		{
			float positionY = MyMath::Lerp(m_startY, m_endY, static_cast<float>(i) / (count - 1));

			if (i == m_indicatingIndex)
			{
				m_menuButtons[i]->GetTransform()->SetLocalPosition({ m_selectedX, positionY });
				auto br = m_menuButtons[i]->GetComponent<BitmapRenderer>();
				switch (m_indicatingIndex)
				{
				case 0:
					br->SetBitmap(L"Image/UI/ICON_START_02.png");
					break;

				case 1:
					br->SetBitmap(L"Image/UI/ICON_RANKING_02.png");
					break;

				case 2:
					br->SetBitmap(L"Image/UI/ICON_GUIDE_02.png");
					break;

				case 3:
					br->SetBitmap(L"Image/UI/ICON_Option_02.png");
					break;

				case 4:
					br->SetBitmap(L"Image/UI/ICON_CREDIT_02.png");
					break;

				case 5:
					br->SetBitmap(L"Image/UI/ICON_QUITGAME_02.png");
					break;
				}
				
			}
			else
			{
				m_menuButtons[i]->GetTransform()->SetLocalPosition({ m_normalX, positionY });
				auto br = m_menuButtons[i]->GetComponent<BitmapRenderer>();
				switch (i)
				{
				case 0:
					br->SetBitmap(L"Image/UI/ICON_START_01.png");
					break;

				case 1:
					br->SetBitmap(L"Image/UI/ICON_RANKING_01.png");
					break;

				case 2:
					br->SetBitmap(L"Image/UI/ICON_GUIDE_01.png");
					break;

				case 3:
					br->SetBitmap(L"Image/UI/ICON_Option_01.png");
					break;

				case 4:
					br->SetBitmap(L"Image/UI/ICON_CREDIT_01.png");
					break;

				case 5:
					br->SetBitmap(L"Image/UI/ICON_QUITGAME_01.png");
					break;
				}
			}
		}
	}
	else if (Input::IsKeyPressed(VK_DOWN))
	{
		++m_indicatingIndex;

		if (m_indicatingIndex > m_menuButtons.size() - 1)
		{
			m_indicatingIndex = static_cast<int>(m_menuButtons.size()) - 1;
		}
		else
		{
			SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
		}

		size_t count = m_menuButtons.size();

		for (size_t i = 0; i < count; ++i)
		{
			float positionY = MyMath::Lerp(m_startY, m_endY, static_cast<float>(i) / (count - 1));

			if (i == m_indicatingIndex)
			{
				m_menuButtons[i]->GetTransform()->SetLocalPosition({ m_selectedX, positionY });
				auto br = m_menuButtons[i]->GetComponent<BitmapRenderer>();
				switch (m_indicatingIndex)
				{
				case 0:
					br->SetBitmap(L"Image/UI/ICON_START_02.png");
					break;

				case 1:
					br->SetBitmap(L"Image/UI/ICON_RANKING_02.png");
					break;

				case 2:
					br->SetBitmap(L"Image/UI/ICON_GUIDE_02.png");
					break;

				case 3:
					br->SetBitmap(L"Image/UI/ICON_Option_02.png");
					break;

				case 4:
					br->SetBitmap(L"Image/UI/ICON_CREDIT_02.png");
					break;

				case 5:
					br->SetBitmap(L"Image/UI/ICON_QUITGAME_02.png");
					break;
				}

			}
			else
			{
				m_menuButtons[i]->GetTransform()->SetLocalPosition({ m_normalX, positionY });
				auto br = m_menuButtons[i]->GetComponent<BitmapRenderer>();
				switch (i)
				{
				case 0:
					br->SetBitmap(L"Image/UI/ICON_START_01.png");
					break;

				case 1:
					br->SetBitmap(L"Image/UI/ICON_RANKING_01.png");
					break;

				case 2:
					br->SetBitmap(L"Image/UI/ICON_GUIDE_01.png");
					break;

				case 3:
					br->SetBitmap(L"Image/UI/ICON_Option_01.png");
					break;

				case 4:
					br->SetBitmap(L"Image/UI/ICON_CREDIT_01.png");
					break;

				case 5:
					br->SetBitmap(L"Image/UI/ICON_QUITGAME_01.png");
					break;
				}
			}
		}
	}
	else if (m_screenCurtainController == nullptr && Input::IsKeyPressed('Z'))
	{
		SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

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
	m_indicatingIndex = GameDataManager::Get().GetSelectedMenuIndex();
	m_menuButtons = menuButtons;

	size_t count = m_menuButtons.size();

	for (size_t i = 0; i < count; ++i)
	{
		float positionY = MyMath::Lerp(m_startY, m_endY, static_cast<float>(i) / (count - 1));

		if (i == m_indicatingIndex)
		{
			m_menuButtons[i]->GetTransform()->SetLocalPosition({ m_selectedX, positionY });
			auto br = m_menuButtons[i]->GetComponent<BitmapRenderer>();
			switch (m_indicatingIndex)
			{
			case 0:
				br->SetBitmap(L"Image/UI/ICON_START_02.png");
				break;

			case 1:
				br->SetBitmap(L"Image/UI/ICON_RANKING_02.png");
				break;

			case 2:
				br->SetBitmap(L"Image/UI/ICON_GUIDE_02.png");
				break;

			case 3:
				br->SetBitmap(L"Image/UI/ICON_Option_02.png");
				break;

			case 4:
				br->SetBitmap(L"Image/UI/ICON_CREDIT_02.png");
				break;

			case 5:
				br->SetBitmap(L"Image/UI/ICON_QUITGAME_02.png");
				break;
			}

		}
		else
		{
			m_menuButtons[i]->GetTransform()->SetLocalPosition({ m_normalX, positionY });
			auto br = m_menuButtons[i]->GetComponent<BitmapRenderer>();
			switch (i)
			{
			case 0:
				br->SetBitmap(L"Image/UI/ICON_START_01.png");
				break;

			case 1:
				br->SetBitmap(L"Image/UI/ICON_RANKING_01.png");
				break;

			case 2:
				br->SetBitmap(L"Image/UI/ICON_GUIDE_01.png");
				break;

			case 3:
				br->SetBitmap(L"Image/UI/ICON_Option_01.png");
				break;

			case 4:
				br->SetBitmap(L"Image/UI/ICON_CREDIT_01.png");
				break;

			case 5:
				br->SetBitmap(L"Image/UI/ICON_QUITGAME_01.png");
				break;
			}
		}
	}
}