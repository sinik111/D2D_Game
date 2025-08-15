#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "InGameSettingController.h"

void InGameSettingController::Start()
{
	isOn = false;
	bitmap = GetGameObject()->GetComponent<BitmapRenderer>();
}

void InGameSettingController::Update()
{
	if (Input::IsKeyPressed('I'))
	{
		if (isOn == true)	
		{
			isOn = false;
			//bitmap->SetColor()
		}
		else 
		{
			isOn = true; 
		}
	}
}
