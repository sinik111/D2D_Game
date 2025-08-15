#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "Building02.h"

void Building02::Start()
{
	jsonfilepath = "Resource/building_circles02_01.json";
	ImportfromJson();
	
	jsonfilepath = "Resource/building_circles02_02.json";
	ImportfromJson();
	
	jsonfilepath = "Resource/building_circles02_03.json";
	ImportfromJson();
	
	jsonfilepath = "Resource/building_circles02_04.json";
	ImportfromJson();
}
