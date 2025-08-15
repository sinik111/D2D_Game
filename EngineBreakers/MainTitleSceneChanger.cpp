#include "../D2DEngineLib/framework.h"
#include "MainTitleSceneChanger.h"

#include "../D2DEngineLib/SceneManager.h"

#include "ScreenCurtainController.h"

void MainTitleSceneChanger::Update()
{
	if (m_screenCurtainController == nullptr && (Input::IsKeyPressed('Z') || Input::IsKeyPressed('X')))
	{
		GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
		m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, 1.0f);
	}

	if (m_screenCurtainController != nullptr)
	{
		if (m_screenCurtainController->IsFinished())
		{
			SceneManager::Get().ChangeScene(L"MainMenu");
		}
	}
}
