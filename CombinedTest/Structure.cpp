#include "../D2DEngineLib/framework.h"
#include "Structure.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Input.h"
#include <fstream>
using json = nlohmann::json;

void Structure::Start()
{
	ImportfromJson();
}

void Structure::Update()
{
	//3번 키로 collider.json 파일 export
	if (Input::IsKeyPressed('E'))
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
	CircleCollider* circlecollider;
	std::ifstream infile(jsonfilepath);
	if (infile.is_open())
	{
		json j;
		infile >> j;
		infile.close();

		Vector2 pivot = { j["pivot"]["x"], j["pivot"]["y"] };
		GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot(pivot);

		for (auto& circle : j["circles"])
		{
			Vector2 circleoffset = { circle["offset"]["x"], circle["offset"]["y"] };
			float circleradius = circle["radius"];
			circlecollider = GetGameObject()->AddComponent<CircleCollider>();
			circlecollider->SetLayer(CollisionLayer::Building);
			circlecollider->SetTrigger(true);
			circlecollider->SetOffset(circleoffset);
			circlecollider->SetRadius(circleradius);

			m_Colliders.push_back(circlecollider);
		}
	}
	else
	{
		for (int i = 0; i < index; ++i)
		{
			GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot({ 0.5,0.5 });
			circlecollider = GetGameObject()->AddComponent<CircleCollider>();
			circlecollider->SetLayer(CollisionLayer::Building);
			circlecollider->SetTrigger(true);
			circlecollider->SetOffset({ 0.5f, 0.5f });

			m_Colliders.push_back(circlecollider);
		}
	}
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
	Vector2 pivot = GetGameObject()->GetComponent<BitmapRenderer>()->GetPivot();
	jarr["pivot"] = { {"x", pivot.x}, {"y", pivot.y} };
	jarr["circles"] = arr;
	std::string jsonstring = jarr.dump(4);

	std::ofstream outfile(jsonfilepath);
	if (outfile.is_open())
	{
		outfile << jsonstring;
		outfile.close();
	}
}
