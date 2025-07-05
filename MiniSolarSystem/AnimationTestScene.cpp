#include "../D2DEngineLib/framework.h"
#include "AnimationTestScene.h"

#include <sstream>

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/MyTime.h"

#include "Ken.h"
#include "HpViewer.h"
#include "Health.h"

void AnimationTestScene::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* go = CreateGameObject(L"Ken");
	go->AddComponent<BitmapRenderer>();
	go->AddComponent<Animator>();
	go->AddComponent<PlayerInput>();
	go->AddComponent<Ken>();
	Health* health = go->AddComponent<Health>();
	

	go = CreateGameObject(L"KenKeyInfo");
	TextRenderer* textRenderer = go->AddComponent<TextRenderer>();
	textRenderer->SetSpaceType(SpaceType::Screen);
	textRenderer->SetFontSize(15.0f);
	textRenderer->SetPivot({ 0.5f, 0.0f });
	//textRenderer->SetPoint({ 100.0f, 0.0f });
	textRenderer->SetRectSize({ 300.0f, 400.0f });
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	textRenderer->SetSortOrder(3);
	textRenderer->SetVerticalAlignment(VerticalAlignment::Top);
	textRenderer->SetHorizontalAlignment(HorizontlAlignment::Left);

	go->GetTransform()->SetLocalPosition({ 100.0f, 0.0f });
	
	std::wostringstream oss;
	oss << L"[ 1 ]: TitleScene\n\n[ ¡ç ¡æ ]: Back/FrontDash\n[ Q ]: Jump\n"
		<< L"[ W ]: SpinningKick - FireEarth\n[ E ]: Ken takes damage 10\n"
		<< L"[ R ]: Ken revive";

	textRenderer->SetText(oss.str());



	go = CreateGameObject(L"KenHpViewer");

	HpViewer* hpViewer = go->AddComponent<HpViewer>();
	hpViewer->SetTargetName(L"Ken");

	textRenderer = go->AddComponent<TextRenderer>();
	go->GetTransform()->SetLocalPosition({ 0.0f, 300.0f });
	//textRenderer->SetPoint({ 0.0f, 300.0f });

	health->GetOnChangeHp().Add(hpViewer, &HpViewer::ChangeHpText);
}

void AnimationTestScene::Exit()
{
	Scene::Exit();

	MyTime::SetTimeScale(1.0f);
}