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
	auto br = mainTitleBackground->AddComponent<BitmapRenderer>(L"Image/SettingBackground.bmp");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* masterVolumeController = CreateGameObject(L"MasterVolumeController");
	masterVolumeController->AddComponent<MasterVolumeController>();
	br = masterVolumeController->AddComponent<BitmapRenderer>(L"Image/VolumeSliderBar.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.3f, 0.5f });


	GameObject* bgmVolumeController = CreateGameObject(L"BGMVolumeController");
	bgmVolumeController->AddComponent<BGMVolumeController>();
	br = bgmVolumeController->AddComponent<BitmapRenderer>(L"Image/VolumeSliderBar.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.3f, 0.5f });


	GameObject* sfxVolumeController = CreateGameObject(L"SFXVolumeController");
	sfxVolumeController->AddComponent<SFXVolumeController>();
	br = sfxVolumeController->AddComponent<BitmapRenderer>(L"Image/VolumeSliderBar.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.3f, 0.5f });


	GameObject* resetButton = CreateGameObject(L"Reset");
	br = resetButton->AddComponent<BitmapRenderer>(L"Image/ResetSettingButton.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* saveButton = CreateGameObject(L"Save");
	br = saveButton->AddComponent<BitmapRenderer>(L"Image/SaveSettingButton.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* indicator = CreateGameObject(L"Indicator");
	br = indicator->AddComponent<BitmapRenderer>(L"Image/MainMenuArrow.bmp");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 0.5f });


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
	comp->SetIndicator(indicator);


	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, 0.2f);
}