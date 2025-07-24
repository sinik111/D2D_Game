#include "../D2DEngineLib/framework.h"
#include "MainTitle.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Button.h"

#include "TitleSceneChange.h"
#include "MainSceneChangeButton.h"

void MainTitle::Enter()
{
	// [Tip] Scene ������ Camera�� ������ �ϳ��� �����������.
	// Camera Component�� ������ GameObject�� Camera�� ������ �ϰԵ�.
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	

	// [Tip] GameObject �����ÿ� �ִ� ���ڰ� GameObject�� �̸��� ��.
	// ������ tag�� ���� �ٸ� ���� ��Ұ� ���� ������ �̸����� ������ �ϸ� ��.
	// �ʿ�� tag �߰� ���� �ʿ�.
	// 
	// GameObject::Find �Լ��� ����ϸ� �̸����� GameObject�� ã�� �� ����.
	// ���� �̸��� �������� ��ȸ �� ���� ó�� �߰��� GameObject ��ȯ.
	
	GameObject* go = CreateGameObject(L"SceneChange");
	go->AddComponent<TitleSceneChange>();


	// [Tip] Button�� ������� GameObject�� Button Component�� �����ϸ� ��.
	// Button Component�� BitmapRenderer�� �ʼ������� �ʿ���.
	// Button�� ũ��� BitmapRenderer�� �׸��� �̹����� ũ��� ����.
	// ����� Ŭ�� �� ������ { 0.97f, 0.97f, 0.97f, 1.0f }(���� ��Ӱ� ��)�� �����ε�
	// �ʿ�� �� ���� �����ϰ� ���� �ʿ�.

	GameObject* buttonGo = CreateGameObject(L"Button");
	buttonGo->AddComponent<BitmapRenderer>(L"ChangeSceneButton.png");
	buttonGo->AddComponent<Button>();
	buttonGo->AddComponent<MainSceneChangeButton>();
}