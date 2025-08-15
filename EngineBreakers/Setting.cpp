#include "../D2DEngineLib/framework.h"
#include "Setting.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "ScreenCurtainController.h"
#include "MasterVolumeController.h"
#include "BGMVolumeController.h"
#include "SFXVolumeController.h"
#include "MainMenuSettingController.h"


void Setting::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();


	GameObject* mainTitleBackground = CreateGameObject(L"SettingBackground");
	auto br = mainTitleBackground->AddComponent<BitmapRenderer>(L"Image/UI/SETTING_BACKGROUND.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* masterVolumeController = CreateGameObject(L"MasterVolumeController");
	masterVolumeController->AddComponent<MasterVolumeController>();
	br = masterVolumeController->AddComponent<BitmapRenderer>(L"Image/UI/VOLUMEBAR_03.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });


	GameObject* bgmVolumeController = CreateGameObject(L"BGMVolumeController");
	bgmVolumeController->AddComponent<BGMVolumeController>();
	br = bgmVolumeController->AddComponent<BitmapRenderer>(L"Image/UI/VOLUMEBAR_03.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });


	GameObject* sfxVolumeController = CreateGameObject(L"SFXVolumeController");
	sfxVolumeController->AddComponent<SFXVolumeController>();
	br = sfxVolumeController->AddComponent<BitmapRenderer>(L"Image/UI/VOLUMEBAR_03.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });


	GameObject* resetButton = CreateGameObject(L"Reset");
	br = resetButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_RESET_01.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* saveButton = CreateGameObject(L"Save");
	br = saveButton->AddComponent<BitmapRenderer>(L"Image/UI/ICON_SAVE_01.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);

	GameObject* mainMenuSettingController = CreateGameObject(L"MainMenuSettingController");
	auto comp = mainMenuSettingController->AddComponent<MainMenuSettingController>();
	comp->SetSettingItems(
		{
			masterVolumeController,
			bgmVolumeController,
			sfxVolumeController,
			resetButton,
			saveButton
		}
	);


	GameObject* returnNotice = CreateGameObject(L"XReturn");
	returnNotice->GetTransform()->SetLocalPosition({ 800.0f, -325.0f });
	br = returnNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_X.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* selectNotice = CreateGameObject(L"ZSelect");
	selectNotice->GetTransform()->SetLocalPosition({ 800.0f, -300.0f });
	br = selectNotice->AddComponent<BitmapRenderer>(L"Image/UI/PRESS_Z.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* moveNotice = CreateGameObject(L"MoveNotice");
	moveNotice->GetTransform()->SetLocalPosition({ 800.0f, -270.0f });
	moveNotice->GetTransform()->SetLocalScale(0.65f, 0.65f);
	br = moveNotice->AddComponent<BitmapRenderer>(L"Image/UI/USE_A.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, 0.2f);
}