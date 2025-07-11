#include "../D2DEngineLib/framework.h"
#include "AnimationTestScene.h"

#include <sstream>

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "Ken.h"
#include "HpViewer.h"
#include "Health.h"

void AnimationTestScene::Enter()
{
	GameObject* camera = CreateGameObjectWithComponents(L"Camera",
		camera->AddComponent<Camera>();
		);

	Health* health;

	GameObject* go = CreateGameObjectWithComponents(L"Ken",
		go->AddComponent<BitmapRenderer>();
		go->AddComponent<Animator>();
		go->AddComponent<PlayerInput>();
		go->AddComponent<Ken>();
		health = go->AddComponent<Health>();
		);
	

	go = CreateGameObjectWithComponents(L"KenKeyInfo",
		TextRenderer* textRenderer = go->AddComponent<TextRenderer>();
		textRenderer->SetSpaceType(SpaceType::Screen);
		textRenderer->SetFontSize(15.0f);
		textRenderer->SetPivot({ 0.5f, 1.0f });
		textRenderer->SetRectSize({ 300.0f, 400.0f });
		textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		textRenderer->SetSortOrder(3);
		textRenderer->SetVerticalAlignment(VerticalAlignment::Top);
		textRenderer->SetHorizontalAlignment(HorizontlAlignment::Left);

		go->GetTransform()->SetLocalPosition({ 100.0f, 0.0f });

		std::wostringstream oss;
		oss << L"[ 1 ]: TitleScene\n\n[ ¡ç ¡æ ]: Back/FrontDash\n[ Q ]: Jump\n"
			<< L"[ W ]: SpinningKick - FireEarth\n[ E ]: Take 10 damage\n"
			<< L"[ R ]: Revive";

		textRenderer->SetText(oss.str());
		);
	
	go = CreateGameObjectWithComponents(L"KenHpViewer",
		HpViewer* hpViewer = go->AddComponent<HpViewer>();
		hpViewer->SetTargetName(L"Ken");
		go->GetTransform()->SetLocalPosition({ 0.0f, 300.0f });

		TextRenderer* textRenderer = go->AddComponent<TextRenderer>();

		health->GetOnChangeHp().Add(hpViewer, &HpViewer::ChangeHpText);
		);
}

void AnimationTestScene::Exit()
{
	Scene::Exit();

	MyTime::SetTimeScale(1.0f);
}