#include "../D2DEngineLib/framework.h"
#include "GuideSceneController.h"

#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "ScreenCurtainController.h"
#include "GameDataManager.h"

void GuideSceneController::Start()
{
	GameObject* guideImage = CreateGameObject(L"GuideImage");
	m_guideImageRenderer = guideImage->AddComponent<BitmapRenderer>(L"Image/Guide1.bmp");
	m_guideImageRenderer->SetSortOrder(1);
	m_guideImageRenderer->SetSpaceType(SpaceType::Screen);
	m_guideImageRenderer->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_currentGuidePage = 0;
}

void GuideSceneController::Update()
{
	if (Input::IsKeyPressed(VK_LEFT))
	{
		if (m_currentGuidePage != 0)
		{
			--m_currentGuidePage;
		}

		switch (m_currentGuidePage)
		{
		case 0:
			m_guideImageRenderer->SetBitmap(L"Image/Guide1.bmp");
			break;

		case 1:
			m_guideImageRenderer->SetBitmap(L"Image/Guide2.bmp");
			break;
		}

	}
	else if (Input::IsKeyPressed(VK_RIGHT))
	{
		if (m_currentGuidePage != m_totalGuidePage - 1)
		{
			++m_currentGuidePage;
		}

		switch (m_currentGuidePage)
		{
		case 0:
			m_guideImageRenderer->SetBitmap(L"Image/Guide1.bmp");
			break;

		case 1:
			m_guideImageRenderer->SetBitmap(L"Image/Guide2.bmp");
			break;
		}
	}

	if (m_screenCurtainController == nullptr && Input::IsKeyPressed('X'))
	{
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
