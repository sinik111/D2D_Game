#include "../D2DEngineLib/framework.h"
#include "CreditSceneController.h"

#include "../D2DEngineLib/SoundManager.h"

#include "ScreenCurtainController.h"

void CreditSceneController::Start()
{
}

void CreditSceneController::Update()
{
	if (m_screenCurtainController == nullptr && Input::IsKeyPressed('X'))
	{
		SoundManager::Get().PlayGlobalSound(L"SelectButton", SoundType::SFX);

		GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
		m_screenCurtainController = screenCurtainController->AddComponent<ScreenCurtainController>(false, 0.5f);
	}

	if (m_screenCurtainController != nullptr)
	{
		if (m_screenCurtainController->IsFinished())
		{
			SceneManager::Get().ChangeScene(L"MainMenu");
		}
	}
}