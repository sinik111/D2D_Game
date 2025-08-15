#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/ImGuiSystem.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/Component.h"

#include "StructureCreater.h"
#include "Objectimgui.h"
#include "Structure.h"

void StructureCreater::CreateBuilding(int number, float x, float y, int id)
{
	GameObject* structure = CreateGameObject(L"Building");

	std::string jsonfilepath;
	std::wstring wfilepath;

	if (number < 10)
	{
		jsonfilepath = "Resource/building_circles0" + std::to_string(number);
		wfilepath = L"Building0" + std::to_wstring(number);
	}
	else if (number >= 10)
	{
		jsonfilepath = "Resource/building_circles" + std::to_string(number);
		wfilepath = L"Building" + std::to_wstring(number);
	}

	if (number == 2 || number == 5 || number == 7 || number == 13 || number == 14)
	{
		wfilepath += L"_01";
		jsonfilepath += "_01";
	}
	wfilepath += L".png";
	jsonfilepath += ".json";

	structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
	structure->GetTransform()->SetLocalPosition(x, y);

	auto imguiObject = ImGuiSystem::Get().CreateImGui<Objectimgui>();

	Vector2 pos = structure->GetTransform()->GetLocalPosition();
	Structure* st = structure->AddComponent<Structure>(jsonfilepath);;
	switch (number)
	{
	case 2:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x + 1, pos.y + 265.0f);
		wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building3");
		structure->GetTransform()->SetLocalPosition(pos.x + 2, pos.y + 560.0f);
		wfilepath = L"Building0" + std::to_wstring(number) + L"_03.png";
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_03.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building4");
		wfilepath = L"Building0" + std::to_wstring(number) + L"_04.png";
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_04.json";
		structure->GetTransform()->SetLocalPosition(pos.x + 3, pos.y + 560.0f);
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 5:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x + 2, pos.y + 291);
		wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 7:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x, pos.y + 136.0f);
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_02.json";
		wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 13:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y + 285);
		wfilepath = L"Building" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = "Resource/building_circles" + std::to_string(number) + "_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 14:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y + 204);
		wfilepath = L"Building" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = "Resource/building_circles" + std::to_string(number) + "_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;
	}
	st->SetSize(4);
	imguiObject->SetObject(structure);
	imguiObject->SetName(jsonfilepath + std::to_string(id));
}
