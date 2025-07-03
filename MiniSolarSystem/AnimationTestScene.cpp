#include "../D2DEngineLib/framework.h"
#include "AnimationTestScene.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "Ken.h"

void AnimationTestScene::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* go = CreateGameObject(L"Ken");
	go->AddComponent<BitmapRenderer>();
	go->AddComponent<Animator>();
	go->AddComponent<PlayerInput>();
	go->AddComponent<Ken>();

	go = CreateGameObject(L"KenKeyInfo");
	TextRenderer* textRenderer = go->AddComponent<TextRenderer>();
	textRenderer->SetSpaceType(SpaceType::Screen);
	textRenderer->SetFontSize(15.0f);
	textRenderer->SetPivot({ 0.5f, 0.0f });
	textRenderer->SetPoint({ 100.0f, 0.0f });
	textRenderer->SetRectSize({ 300.0f, 400.0f });
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	textRenderer->SetSortOrder(3);
	textRenderer->SetVerticalAlignment(VerticalAlignment::Top);
	textRenderer->SetHorizontalAlignment(HorizontlAlignment::Left);
	textRenderer->SetText(L"[ 1 ]: TitleScene\n\n[ ¡ç ¡æ ]: Back/FrontDash\n[ A ]: Roll\n[ S ]: SpinningKick - FireEarth");
}