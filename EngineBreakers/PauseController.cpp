#include "../D2DEngineLib/framework.h"
#include "PauseController.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"

#include "ScreenCurtainController.h"
#include "GameDataManager.h"
#include "MasterVolumeController.h"
#include "BGMVolumeController.h"
#include "SFXVolumeController.h"
#include "VolumeControllerBase.h"

void PauseController::Start()
{
}

void PauseController::Update()
{
	if (m_screenCurtainController == nullptr)
	{
		if (!GameDataManager::Get().GetIsGameFinished() && 
			GameDataManager::Get().GetPlayerHp() > 0 &&
			Input::IsKeyPressed(VK_ESCAPE))
		{
			if (m_currentMenuState == MenuState::Off)
			{
				SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

				m_pauseBackground = CreateGameObject(L"PauseBackground");
				auto br = m_pauseBackground->AddComponent<BitmapRenderer>(L"Image/UI/PAUSED/PAUSED_OPACITY_01.png");
				br->SetSortOrder(10);
				br->SetSpaceType(SpaceType::Screen);

				m_pausePanel = CreateGameObject(L"PausePanel");
				br = m_pausePanel->AddComponent<BitmapRenderer>(L"Image/UI/PAUSED/PAUSED_OPACITY_02.png");
				br->SetSortOrder(11);
				br->SetSpaceType(SpaceType::Screen);

				m_currentMenuState = MenuState::Main;
				CreateMenuButtons();

				m_selectNotice = CreateGameObject(L"ZSelect");
				m_selectNotice->GetTransform()->SetLocalPosition({ 800.0f, -300.0f });
				br = m_selectNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_Z.png");
				br->SetSortOrder(12);
				br->SetSpaceType(SpaceType::Screen);
				br->SetPivot({ 1.0f, 0.5f });


				m_arrowNotice = CreateGameObject(L"ArrowNotice");
				m_arrowNotice->GetTransform()->SetLocalPosition({ 800.0f, -270.0f });
				m_arrowNotice->GetTransform()->SetLocalScale(0.65f, 0.65f);
				br = m_arrowNotice->AddComponent<BitmapRenderer>(L"Image/UI/USE_A.png");
				br->SetSortOrder(12);
				br->SetSpaceType(SpaceType::Screen);
				br->SetPivot({ 1.0f, 0.5f });

				ComponentSystem::Get().PlayerInput().SetIsStopped(true);
				MyTime::SetTimeScale(0.0f);
			}
			else
			{
				SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

				m_currentMenuState = MenuState::Off;
				m_menuButtonIndex = 0;

				ClearGameObjects();

				ComponentSystem::Get().PlayerInput().SetIsStopped(false);
				MyTime::SetTimeScale(1.0f);
			}
		}

		if (m_currentMenuState == MenuState::Main)
		{
			if (Input::IsKeyPressed(VK_UP))
			{
				if (m_menuButtonIndex != 0)
				{
					--m_menuButtonIndex;

					SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
				}

				switch (m_menuButtonIndex)
				{
				case 0:
					m_menuButtons[0]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_RESUME_02.png");
					m_menuButtons[1]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_GUIDE_01.png");
					m_menuButtons[2]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_OPTIONS_01.png");
					m_menuButtons[3]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_QUIT_01.png");
					break;

				case 1:
					m_menuButtons[0]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_RESUME_01.png");
					m_menuButtons[1]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_GUIDE_02.png");
					m_menuButtons[2]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_OPTIONS_01.png");
					m_menuButtons[3]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_QUIT_01.png");
					break;

				case 2:
					m_menuButtons[0]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_RESUME_01.png");
					m_menuButtons[1]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_GUIDE_01.png");
					m_menuButtons[2]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_OPTIONS_02.png");
					m_menuButtons[3]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_QUIT_01.png");
					break;
				}
			}
			else if (Input::IsKeyPressed(VK_DOWN))
			{
				if (m_menuButtonIndex != 3)
				{
					++m_menuButtonIndex;
					SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
				}

				switch (m_menuButtonIndex)
				{
				case 1:
					m_menuButtons[0]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_RESUME_01.png");
					m_menuButtons[1]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_GUIDE_02.png");
					m_menuButtons[2]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_OPTIONS_01.png");
					m_menuButtons[3]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_QUIT_01.png");
					break;

				case 2:
					m_menuButtons[0]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_RESUME_01.png");
					m_menuButtons[1]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_GUIDE_01.png");
					m_menuButtons[2]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_OPTIONS_02.png");
					m_menuButtons[3]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_QUIT_01.png");
					break;

				case 3:
					m_menuButtons[0]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_RESUME_01.png");
					m_menuButtons[1]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_GUIDE_01.png");
					m_menuButtons[2]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_OPTIONS_01.png");
					m_menuButtons[3]->GetComponent<BitmapRenderer>()->SetBitmap(L"Image/UI/PAUSED/ICON_QUIT_02.png");
					break;
				}
			}

			if (Input::IsKeyPressed('Z'))
			{
				SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

				switch (m_menuButtonIndex)
				{
				case 0:
				{
					ClearGameObjects();
					m_currentMenuState = MenuState::Off;
					m_menuButtonIndex = 0;

					ComponentSystem::Get().PlayerInput().SetIsStopped(false);
					MyTime::SetTimeScale(1.0f);
				}
				break;

				case 1:
				{
					ClearNotices();
					DestroyMenuButtons();
					m_currentMenuState = MenuState::Guide;

					m_returnNotice = CreateGameObject(L"XReturn");
					m_returnNotice->GetTransform()->SetLocalPosition({ 800.0f, -300.0f });
					auto br = m_returnNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_X.png");
					br->SetSortOrder(12);
					br->SetSpaceType(SpaceType::Screen);
					br->SetPivot({ 1.0f, 0.5f });

					m_guide = CreateGameObject(L"GuideImage");
					br = m_guide->AddComponent<BitmapRenderer>(L"Image/UI/ACTION_KEYS_ICON.png");
					br->SetSortOrder(12);
					br->SetSpaceType(SpaceType::Screen);
				}
				break;

				case 2:
				{
					ClearNotices();
					DestroyMenuButtons();
					m_currentMenuState = MenuState::Setting;
					m_settingIndicatingIndex = 0;

					GameObject* masterVolumeController = CreateGameObject(L"MasterVolumeController");
					masterVolumeController->AddComponent<MasterVolumeController>();
					auto br = masterVolumeController->AddComponent<BitmapRenderer>(L"Image/UI/VOLUMEBAR_03.png");
					br->SetSortOrder(13);
					br->SetSpaceType(SpaceType::Screen);
					br->SetPivot({ 0.0f, 0.5f });


					GameObject* bgmVolumeController = CreateGameObject(L"BGMVolumeController");
					bgmVolumeController->AddComponent<BGMVolumeController>();
					br = bgmVolumeController->AddComponent<BitmapRenderer>(L"Image/UI/VOLUMEBAR_03.png");
					br->SetSortOrder(13);
					br->SetSpaceType(SpaceType::Screen);
					br->SetPivot({ 0.0f, 0.5f });


					GameObject* sfxVolumeController = CreateGameObject(L"SFXVolumeController");
					sfxVolumeController->AddComponent<SFXVolumeController>();
					br = sfxVolumeController->AddComponent<BitmapRenderer>(L"Image/UI/VOLUMEBAR_03.png");
					br->SetSortOrder(13);
					br->SetSpaceType(SpaceType::Screen);
					br->SetPivot({ 0.0f, 0.5f });


					GameObject* resetButton = CreateGameObject(L"Reset");
					br = resetButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_RESET_01.png");
					br->SetSortOrder(13);
					br->SetSpaceType(SpaceType::Screen);


					GameObject* saveButton = CreateGameObject(L"Save");
					br = saveButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_SAVE_01.png");
					br->SetSortOrder(13);
					br->SetSpaceType(SpaceType::Screen);

					m_returnNotice = CreateGameObject(L"XReturn");
					m_returnNotice->GetTransform()->SetLocalPosition({ 800.0f, -300.0f });
					br = m_returnNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_X.png");
					br->SetSortOrder(12);
					br->SetSpaceType(SpaceType::Screen);
					br->SetPivot({ 1.0f, 0.5f });


					m_selectNotice = CreateGameObject(L"ZSelect");
					m_selectNotice->GetTransform()->SetLocalPosition({ 800.0f, -275.0f });
					br = m_selectNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_Z.png");
					br->SetSortOrder(12);
					br->SetSpaceType(SpaceType::Screen);
					br->SetPivot({ 1.0f, 0.5f });


					m_arrowNotice = CreateGameObject(L"ArrowNotice");
					m_arrowNotice->GetTransform()->SetLocalPosition({ 800.0f, -245.0f });
					m_arrowNotice->GetTransform()->SetLocalScale(0.65f, 0.65f);
					br = m_arrowNotice->AddComponent<BitmapRenderer>(L"Image/UI/USE_A.png");
					br->SetSortOrder(12);
					br->SetSpaceType(SpaceType::Screen);
					br->SetPivot({ 1.0f, 0.5f });

					m_settingButtons.push_back(masterVolumeController);
					m_settingButtons.push_back(bgmVolumeController);
					m_settingButtons.push_back(sfxVolumeController);
					m_settingButtons.push_back(resetButton);
					m_settingButtons.push_back(saveButton);

					for (size_t i = 0; i < m_settingButtons.size(); ++i)
					{
						m_settingButtons[i]->GetTransform()->SetLocalPosition(
							Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1)));
					}
				}
				break;

				case 3:
				{
					GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
					m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, 0.5f);
				}
				break;
				}
			}
		}
		else if (m_currentMenuState == MenuState::Guide)
		{
			if (Input::IsKeyPressed('X'))
			{
				SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

				if (m_guide)
				{
					Destroy(m_guide);
					m_guide = nullptr;
				}

				ClearNotices();

				m_selectNotice = CreateGameObject(L"ZSelect");
				m_selectNotice->GetTransform()->SetLocalPosition({ 800.0f, -300.0f });
				auto br = m_selectNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_Z.png");
				br->SetSortOrder(12);
				br->SetSpaceType(SpaceType::Screen);
				br->SetPivot({ 1.0f, 0.5f });


				m_arrowNotice = CreateGameObject(L"ArrowNotice");
				m_arrowNotice->GetTransform()->SetLocalPosition({ 800.0f, -270.0f });
				m_arrowNotice->GetTransform()->SetLocalScale(0.65f, 0.65f);
				br = m_arrowNotice->AddComponent<BitmapRenderer>(L"Image/UI/USE_A.png");
				br->SetSortOrder(12);
				br->SetSpaceType(SpaceType::Screen);
				br->SetPivot({ 1.0f, 0.5f });

				CreateMenuButtons();
				m_currentMenuState = MenuState::Main;
			}
		}
		else if (m_currentMenuState == MenuState::Setting)
		{
			if (Input::IsKeyPressed(VK_UP))
			{
				--m_settingIndicatingIndex;

				if (m_settingIndicatingIndex < 0)
				{
					m_settingIndicatingIndex = 0;
				}
				else
				{
					SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
				}

				for (int i = 0; i < m_settingButtons.size(); ++i)
				{
					if (i == m_settingIndicatingIndex)
					{
						auto comp = m_settingButtons[i]->GetComponent<VolumeControllerBase>();
						if (comp != nullptr)
						{
							comp->SetSelected(true);
						}
						else
						{
							auto br = m_settingButtons[i]->GetComponent<BitmapRenderer>();

							if (i == 3)
							{
								Vector2 position = Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1));
								position.x += 2.0f;
								position.y -= 3.0f;
								m_settingButtons[i]->GetTransform()->SetLocalPosition(position);
								br->SetBitmap(L"Image/UI/ICON_RESET_02.png");
							}
							else if (i == 4)
							{
								Vector2 position = Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1));
								position.x += 1.0f;
								position.y += 3.0f;
								m_settingButtons[i]->GetTransform()->SetLocalPosition(position);
								br->SetBitmap(L"Image/UI/ICON_SAVE_02.png");
							}
						}
					}
					else
					{
						auto comp = m_settingButtons[i]->GetComponent<VolumeControllerBase>();
						if (comp != nullptr)
						{
							comp->SetSelected(false);
						}
						else
						{
							auto br = m_settingButtons[i]->GetComponent<BitmapRenderer>();

							if (i == 3)
							{
								Vector2 position = Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1));
								m_settingButtons[i]->GetTransform()->SetLocalPosition(position);
								br->SetBitmap(L"Image/UI/ICON_RESET_01.png");
							}
							else if (i == 4)
							{
								Vector2 position = Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1));
								m_settingButtons[i]->GetTransform()->SetLocalPosition(position);
								br->SetBitmap(L"Image/UI/ICON_SAVE_01.png");
							}
						}
					}
				}
			}
			else if (Input::IsKeyPressed(VK_DOWN))
			{
				++m_settingIndicatingIndex;

				if (m_settingIndicatingIndex > m_settingButtons.size() - 1)
				{
					m_settingIndicatingIndex = static_cast<int>(m_settingButtons.size()) - 1;
				}
				else
				{
					SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);
				}

				for (int i = 0; i < m_settingButtons.size(); ++i)
				{
					if (i == m_settingIndicatingIndex)
					{
						auto comp = m_settingButtons[i]->GetComponent<VolumeControllerBase>();
						if (comp != nullptr)
						{
							comp->SetSelected(true);
						}
						else
						{
							auto br = m_settingButtons[i]->GetComponent<BitmapRenderer>();

							if (i == 3)
							{
								Vector2 position = Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1));
								position.x += 2.0f;
								position.y -= 3.0f;
								m_settingButtons[i]->GetTransform()->SetLocalPosition(position);
								br->SetBitmap(L"Image/UI/ICON_RESET_02.png");
							}
							else if (i == 4)
							{
								Vector2 position = Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1));
								position.x += 1.0f;
								position.y += 3.0f;
								m_settingButtons[i]->GetTransform()->SetLocalPosition(position);
								br->SetBitmap(L"Image/UI/ICON_SAVE_02.png");
							}
						}
					}
					else
					{
						auto comp = m_settingButtons[i]->GetComponent<VolumeControllerBase>();
						if (comp != nullptr)
						{
							comp->SetSelected(false);
						}
						else
						{
							auto br = m_settingButtons[i]->GetComponent<BitmapRenderer>();

							if (i == 3)
							{
								Vector2 position = Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1));
								m_settingButtons[i]->GetTransform()->SetLocalPosition(position);
								br->SetBitmap(L"Image/UI/ICON_RESET_01.png");
							}
							else if (i == 4)
							{
								Vector2 position = Vector2::Lerp(m_settingStartPosition, m_settingEndPosition, static_cast<float>(i) / (m_settingButtons.size() - 1));
								m_settingButtons[i]->GetTransform()->SetLocalPosition(position);
								br->SetBitmap(L"Image/UI/ICON_SAVE_01.png");
							}
						}
					}
				}
			}

			if (Input::IsKeyPressed(VK_RIGHT))
			{
				auto comp = m_settingButtons[m_settingIndicatingIndex]->GetComponent<VolumeControllerBase>();
				if (comp != nullptr)
				{
					SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);

					comp->VolumeUp();
				}
			}
			else if (Input::IsKeyPressed(VK_LEFT))
			{
				auto comp = m_settingButtons[m_settingIndicatingIndex]->GetComponent<VolumeControllerBase>();
				if (comp != nullptr)
				{
					SoundManager::Get().PlayGlobalSound(L"ChangeButton", SoundType::SFX);

					comp->VolumeDown();
				}
			}

			if (Input::IsKeyPressed('Z'))
			{
				SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

				const auto& name = m_settingButtons[m_settingIndicatingIndex]->GetName();

				if (name == L"Reset")
				{
					for (const auto& go : m_settingButtons)
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
					for (const auto& go : m_settingButtons)
					{
						auto comp = go->GetComponent<VolumeControllerBase>();
						if (comp != nullptr)
						{
							comp->Save();
						}
					}
				}
			}

			if (Input::IsKeyPressed('X'))
			{
				SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

				if (!m_settingButtons.empty())
				{
					for (auto go : m_settingButtons)
					{
						auto comp = go->GetComponent<VolumeControllerBase>();
						if (comp != nullptr)
						{
							comp->Cancel(true);
						}

						Destroy(go);
					}

					m_settingButtons.clear();
				}

				ClearNotices();

				m_selectNotice = CreateGameObject(L"ZSelect");
				m_selectNotice->GetTransform()->SetLocalPosition({ 800.0f, -300.0f });
				auto br = m_selectNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_Z.png");
				br->SetSortOrder(12);
				br->SetSpaceType(SpaceType::Screen);
				br->SetPivot({ 1.0f, 0.5f });


				m_arrowNotice = CreateGameObject(L"ArrowNotice");
				m_arrowNotice->GetTransform()->SetLocalPosition({ 800.0f, -270.0f });
				m_arrowNotice->GetTransform()->SetLocalScale(0.65f, 0.65f);
				br = m_arrowNotice->AddComponent<BitmapRenderer>(L"Image/UI/USE_A.png");
				br->SetSortOrder(12);
				br->SetSpaceType(SpaceType::Screen);
				br->SetPivot({ 1.0f, 0.5f });

				CreateMenuButtons();
				m_currentMenuState = MenuState::Main;
			}
		}
	}
	else
	{
		if (m_screenCurtainController->IsFinished())
		{
			SoundManager::Get().StopSound(L"GamePlayBGM");

			ComponentSystem::Get().PlayerInput().SetIsStopped(false);
			MyTime::SetTimeScale(1.0f);

			SceneManager::Get().ChangeScene(L"MainTitle");
		}
	}
}

void PauseController::CreateMenuButtons()
{
	GameObject* resumeButton = CreateGameObject(L"Resume");
	resumeButton->GetTransform()->SetLocalPosition({ 0.0f, 100.0f });
	auto br = resumeButton->AddComponent<BitmapRenderer>(L"Image/UI/PAUSED/ICON_RESUME_01.png");
	if (m_menuButtonIndex == 0)
	{
		br->SetBitmap(L"Image/UI/PAUSED/ICON_RESUME_02.png");
	}
	br->SetSortOrder(12);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* guideButton = CreateGameObject(L"Guide");
	guideButton->GetTransform()->SetLocalPosition({ 0.0f, 0.0f });
	br = guideButton->AddComponent<BitmapRenderer>(L"Image/UI/PAUSED/ICON_GUIDE_01.png");
	if (m_menuButtonIndex == 1)
	{
		br->SetBitmap(L"Image/UI/PAUSED/ICON_GUIDE_02.png");
	}
	br->SetSortOrder(12);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* optionButton = CreateGameObject(L"Option");
	optionButton->GetTransform()->SetLocalPosition({ 0.0f, -100.0f });
	br = optionButton->AddComponent<BitmapRenderer>(L"Image/UI/PAUSED/ICON_OPTIONS_01.png");
	if (m_menuButtonIndex == 2)
	{
		br->SetBitmap(L"Image/UI/PAUSED/ICON_OPTIONS_02.png");
	}
	br->SetSortOrder(12);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* quitButton = CreateGameObject(L"Quit");
	quitButton->GetTransform()->SetLocalPosition({ 0.0f, -200.0f });
	br = quitButton->AddComponent<BitmapRenderer>(L"Image/UI/PAUSED/ICON_QUIT_01.png");
	br->SetSortOrder(12);
	br->SetSpaceType(SpaceType::Screen);

	m_menuButtons.push_back(resumeButton);
	m_menuButtons.push_back(guideButton);
	m_menuButtons.push_back(optionButton);
	m_menuButtons.push_back(quitButton);
}

void PauseController::DestroyMenuButtons()
{
	if (!m_menuButtons.empty())
	{
		for (auto go : m_menuButtons)
		{
			Destroy(go);
		}

		m_menuButtons.clear();
	}
}

void PauseController::ClearGameObjects()
{
	ClearNotices();

	if (m_pauseBackground)
	{
		Destroy(m_pauseBackground);
		m_pauseBackground = nullptr;
	}

	if (m_pausePanel)
	{
		Destroy(m_pausePanel);
		m_pausePanel = nullptr;
	}

	if (!m_menuButtons.empty())
	{
		for (auto go : m_menuButtons)
		{
			Destroy(go);
		}

		m_menuButtons.clear();
	}

	if (m_guide)
	{
		Destroy(m_guide);
		m_guide = nullptr;
	}

	if (!m_settingButtons.empty())
	{
		for (auto go : m_settingButtons)
		{
			auto comp = go->GetComponent<VolumeControllerBase>();
			if (comp != nullptr)
			{
				comp->Cancel(true);
			}

			Destroy(go);
		}

		m_settingButtons.clear();
	}
}

void PauseController::ClearNotices()
{
	if (m_arrowNotice)
	{
		Destroy(m_arrowNotice);
		m_arrowNotice = nullptr;
	}

	if (m_selectNotice)
	{
		Destroy(m_selectNotice);
		m_selectNotice = nullptr;
	}

	if (m_returnNotice)
	{
		Destroy(m_returnNotice);
		m_returnNotice = nullptr;
	}
}