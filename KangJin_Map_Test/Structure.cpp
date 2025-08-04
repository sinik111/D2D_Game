#include "../D2DEngineLib/framework.h"
#include "Structure.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Input.h"
#include <fstream>
using json = nlohmann::json;

void Structure::Start()
{

}

void Structure::Update()
{
	//3번 키로 collider.json 파일 export
	if (Input::IsKeyPressed('3'))
	{
		this->ExporttoJson();
	}
}

void Structure::FixedUpdate()
{
	if (isTransparent)
	{
		GetGameObject()->GetComponent<BitmapRenderer>()->SetColor({ 1,1,1,0.5f });
	}
	else
	{
		GetGameObject()->GetComponent<BitmapRenderer>()->SetColor({ 1,1,1,1.0f });
	}

	isTransparent = false;
}

void Structure::OnTriggerStay(const Collision& collision)
{
	isTransparent = true;
}

void Structure::ImportfromJson()
{
	//std::ifstream infile(jsonfilepath);
	//if (infile.is_open())
	//{
	//	json j;
	//	infile >> j;
	//	infile.close();

	//	for (auto& circle : j["circles"])
	//	{
	//		
	//	}
	//}
}

void Structure::ExporttoJson()
{
	json arr = json::array();
	for (auto& circle : m_Colliders)
	{
		json j;
		j["offset"] = { {"x", circle->GetOffset().x}, {"y", circle->GetOffset().y} };
		j["radius"] = circle->GetCircle().radius;
		arr.push_back(j);
	}

	json jarr;
	jarr["circles"] = arr;
	std::string jsonstring = jarr.dump(4);

	std::ofstream outfile(jsonfilepath);
	if (outfile.is_open())
	{
		outfile << jsonstring;
		outfile.close();
	}
}
