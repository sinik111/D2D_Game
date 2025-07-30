#include "../D2DEngineLib/framework.h"
#include "KinematicTest.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "KinematicSun.h"
#include "StaticMoon.h"
#include "Maps.h"
#include "CameraController.h"
#include "../D2DEngineLib/imgui.h"
#include "../D2DEngineLib/imgui_impl_win32.h"
#include "../D2DEngineLib/imgui_impl_dx11.h"

void KinematicTest::Enter()
{
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 

	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraController>();
	Maps* pmap = camera->AddComponent<Maps>();

	GameObject* go = CreateGameObject(L"KinematicSun");
	go->AddComponent<KinematicSun>();
	auto rigidbody = go->AddComponent<RigidBody2D>();
	rigidbody->SetBodyType(RigidBody2D::BodyType::Kinematic);
	BoxCollider2D* sunCollider = go->AddComponent<BoxCollider2D>();
	sunCollider->SetSize({ 50.0f, 50.f });
	go->AddComponent<BitmapRenderer>(L"Sun.png")->SetSortOrder(1);
	

	pmap->Initialize();

	for (size_t i = 0; i < 50; ++i)
	{
		go = CreateGameObject(L"StaticMoon");
		go->AddComponent<StaticMoon>();
		BoxCollider2D* moonCollider = go->AddComponent<BoxCollider2D>();
		moonCollider->SetTrigger(true);
		moonCollider->SetSize({ 20.0f, 20.f });
		go->AddComponent<BitmapRenderer>(L"Moon.png")->SetSortOrder(1);
		go->GetTransform()->SetLocalPosition(Random::Float(-500, 500), Random::Float(-400, 400));
	}

	go = CreateGameObject();
	auto textRenderer = go->AddComponent<TextRenderer>();
	textRenderer->SetText(L"Press 1 to go main");
	textRenderer->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	textRenderer->SetFontSize(20.0f);

}