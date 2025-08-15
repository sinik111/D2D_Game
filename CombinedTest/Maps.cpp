#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/D2DRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInputSystem.h"
#include "../D2DEngineLib/Input.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/json.hpp"
#include "../D2DEngineLib/Screen.h"
#include "../D2DEngineLib/LineSegment.h"
#include "../D2DEngineLib/LineCollider.h"
#include <fstream>
#include <iostream>
#include "CameraController.h"
#include "Maps.h"

using json = nlohmann::json;

void Maps::Initialize()
{
}

void Maps::Start()
{
	CreateLineCollider();
}

void Maps::FixedUpdate()
{
}

void Maps::Update()
{
	// 마우스 우클릭으로 추가
	float width = Screen::Get().GetWidth() * 0.5f;
	float height = Screen::Get().GetHeight() * 0.5f;

	Transform* camera = GetGameObject()->GetComponent<CameraController>()->GetTransform();

	float pointX = (float)Input::GetCursorPoint().x - width;
	float pointY = (float)Input::GetCursorPoint().y - height;
	Vector2 clickPoint{ pointX, pointY };
	Vector2 startPos = { 0,0 };
	Vector2 halfpoint;
	LineCollider* prevLine;

	if (Input::IsKeyPressed(VK_RBUTTON))
	{
		GetMouseCursorPosition();
		if (m_ClickPositions.size() >= 1)
		{
			prevLine = linecollider;
			Line = CreateGameObject(L"LineCollider");
			linecollider = Line->AddComponent<LineCollider>();
			GameObject* tempGameObject = Line;
			m_Lines.push_back(tempGameObject);
		}
	}
	if (m_ClickPositions.size() >= 1)
	{
		Matrix3x2 renderMatrix = Matrix3x2::Scale(1.0f, -1.0f);
		Matrix3x2 viewMatrix = Camera::s_mainCamera->GetViewMatrix().Inverse();
		Matrix3x2 finalMatrix = renderMatrix * viewMatrix;
		clickPoint = finalMatrix.TransformPoint(clickPoint);

		startPos = m_ClickPositions.back();
		Vector2 halfvec = (clickPoint - startPos) / 2;
		halfpoint = startPos + halfvec;
		Line->GetTransform()->SetLocalPosition(halfpoint);

		if (startPos != clickPoint)
		{
			linecollider->SetLine(startPos - halfpoint, clickPoint - halfpoint);
		}
		//std::cout <<"x :" << linecollider->GetLineSegment().endPoint.x <<", y : " << linecollider->GetLineSegment().endPoint.y << std::endl;
	}

	//1번 키로 원하는 도형 콜라이더 생성
	if (Input::IsKeyPressed('1'))
	{
		if (!m_ClickPositions.empty())
		{
			Vector2 firstStart = m_ClickPositions[0];

			for (int i = 0; i < m_ClickPositions.size(); ++i)
			{
				if (i + 1 < m_ClickPositions.size())
				{
					Vector2 vecdistance = (m_ClickPositions[i + 1] - m_ClickPositions[i]) / 2;
					Vector2 centerPos = m_ClickPositions[i] + vecdistance;
					Vector2 relativeStart = -vecdistance;
					Vector2 relativeEnd = vecdistance;

					m_lineDatas.push_back(LineData{
						vecdistance,
						centerPos,
						relativeStart,
						relativeEnd
						});
				}
			}

			Vector2 lastPoint = m_ClickPositions[m_ClickPositions.size() - 1];
			Vector2 vecdistance = (firstStart - lastPoint) / 2;
			Vector2 centerPos = lastPoint + vecdistance;
			Vector2 relativeStart = -vecdistance;
			Vector2 relativeEnd = vecdistance;

			m_lineDatas.push_back(LineData{
				vecdistance,
				centerPos,
				relativeStart,
				relativeEnd
				});

			m_ClickPositions.clear();
		}
	}

	//2번키로 json파일 export
	if (Input::IsKeyPressed('2'))
	{
		/*json jarr = json::array();
		for (auto data : m_lineDatas)
		{
			json j;
			j["center"] = { {"x",data.centerPos.x},{"y", data.centerPos.y} };
			j["start"] = { {"x", data.relativeStart.x} ,{"y", data.relativeStart.y} };
			j["end"] = { {"x", data.relativeEnd.x}, {"y", data.relativeEnd.y} };
			jarr.push_back(j);
		}
		json j;
		j["lines"] = jarr;
		std::string jsonstring = j.dump(4);

		std::ofstream outfile("Resource/points.json");
		if (outfile.is_open())
		{
			outfile << jsonstring;
			outfile.close();
		}*/
	}

	//엔터 눌렀을 때에 라인콜라이더 생성
	if (Input::IsKeyPressed(VK_RETURN))
	{
		CreateLineCollider();
	}

	//백 스페이스로 삭제
	if (Input::IsKeyPressed(VK_BACK))
	{
		if (m_ClickPositions.size() >= 1) { m_ClickPositions.pop_back(); }
		if (m_Lines.size() >= 1)
		{
			Destroy(m_Lines.back());
			m_Lines.pop_back();
			Line = m_Lines.back();
			linecollider = Line->GetComponent<LineCollider>();
			linecollider->SetLine(m_ClickPositions.back(), clickPoint - halfpoint);
		}
	}

	//Delete 키로 도형 하나 삭제
	if (Input::IsKeyPressed(VK_DELETE))
	{
	}
	if (Input::IsKeyPressed('R'))
	{
		//ExportObjectPositionstoPath("Resource/Structure_Positions.json");
	}
	if (Input::IsKeyPressed('T'))
	{
		//ExportEnemyPositions("Resource/Enemy_Positions.json");
	}
}

void Maps::LateUpdate()
{
}

void Maps::OnDestroy()
{
}

void Maps::GetMouseCursorPosition()
{
	float width = Screen::Get().GetWidth() * 0.5f;
	float height = Screen::Get().GetHeight() * 0.5f;

	Transform* camera = GetGameObject()->GetComponent<CameraController>()->GetTransform();

	float pointX = (float)Input::GetCursorPoint().x - width;
	float pointY = (float)Input::GetCursorPoint().y - height;

	Vector2 clickPoint{ pointX, pointY };

	Matrix3x2 renderMatrix = Matrix3x2::Scale(1.0f, -1.0f);
	Matrix3x2 viewMatrix = Camera::s_mainCamera->GetViewMatrix().Inverse();
	Matrix3x2 finalMatrix = renderMatrix * viewMatrix;

	clickPoint = finalMatrix.TransformPoint(clickPoint);

	m_ClickPositions.push_back(clickPoint);

	std::cout << "x : " << clickPoint.x << ", y : " << clickPoint.y << std::endl;
}

std::string Maps::ConvertPositiontoJSON()
{
	json jarr = json::array();
	for (int i = 0; i < m_ClickPositions.size(); ++i)
	{
		if (i + 1 < m_ClickPositions.size())
		{
			Vector2 vecdistance = (m_ClickPositions[i + 1] - m_ClickPositions[i]) / 2;
			Vector2 centerPos = m_ClickPositions[i] + vecdistance;
			Vector2 relativeStart = -vecdistance;
			Vector2 relativeEnd = vecdistance;

			json j;
			j["center"] = { {"x",centerPos.x},{"y", centerPos.y} };
			j["start"] = { {"x", relativeStart.x} ,{"y", relativeStart.y} };
			j["end"] = { {"x", relativeEnd.x}, {"y", relativeEnd.y} };
			jarr.push_back(j);
		}
	}
	json j;
	j["lines"] = jarr;
	std::string jsonstring = j.dump(4);
	return jsonstring;
}

void Maps::ExportJsontoPath(std::string filepath)
{
	std::ofstream outfile(filepath);
	if (outfile.is_open())
	{
		std::string str = ConvertPositiontoJSON();
		outfile << str;
		outfile.close();
	}
}

void Maps::CreateLineCollider()
{
	std::ifstream infile("Resource/points.json");
	if (infile.is_open())
	{
		json j;
		infile >> j;
		infile.close();

		//json파일의 라인의 개수만큼 크기 조정
		for (auto& line : j["lines"])
		{
			GameObject* gameobject = CreateGameObject(L"LineCollider");

			Vector2 position;
			position.x = line["center"]["x"];
			position.y = line["center"]["y"];
			gameobject->GetTransform()->SetLocalPosition(position);

			LineCollider* linecollider = gameobject->AddComponent<LineCollider>();
			linecollider->SetLayer(CollisionLayer::Wall);
			Vector2 startPoint;
			startPoint.x = line["start"]["x"];
			startPoint.y = line["start"]["y"];

			Vector2 endPoint;
			endPoint.x = line["end"]["x"];
			endPoint.y = line["end"]["y"];
			linecollider->SetLine(startPoint, endPoint);

			std::cout << "x : " << gameobject->GetTransform()->GetLocalPosition().x << ", y :" << gameobject->GetTransform()->GetLocalPosition().y << std::endl;
		}
	}
}

void Maps::ExportObjectPositionstoPath(std::string filepath)
{

	json jarr = json::array();
	for (auto& structure : m_Structrues)
	{
		json j;
		j["type"] = structure.type;
		j["position"] = { {"x", structure.gameObject->GetTransform()->GetLocalPosition().x}, {"y", structure.gameObject->GetTransform()->GetLocalPosition().y} };
		j["id"] = structure.id;

		jarr.push_back(j);
	}

	json j;
	j["Buildings"] = jarr;
	std::string file = j.dump(4);

	std::ofstream outfile(filepath);
	outfile << file;
	outfile.close();
}

void Maps::InsertStructure(int type, GameObject* position, int id)
{
	StructureData sd;
	sd.type = type;
	sd.gameObject = position;
	sd.id = id;
	m_Structrues.push_back(sd);
}

std::vector<ImportStructData> Maps::ImportStructure()
{
	std::vector<ImportStructData> ImportedData;
	std::ifstream infile("Resource/Structure_Positions.json");
	if (infile.is_open())
	{
		json j;
		infile >> j;
		infile.close();

		for (auto& structure : j["Buildings"])
		{
			ImportStructData isd;
			isd.type = structure["type"];
			isd.position = Vector2{ structure["position"]["x"], structure["position"]["y"] };
			isd.id = structure["id"];

			ImportedData.push_back(isd);
		}
	}
	return ImportedData;
}

void Maps::PushBackEnemyPosition(Vector2 enemypos)
{
	m_EnmeyPositions.push_back(enemypos);
}

void Maps::ExportEnemyPositions(std::string filepath)
{
	json jarr = json::array();
	for (auto& pos : m_EnmeyPositions)
	{
		json j;
		j["position"] = { {"x", pos.x},{"y", pos.y} };

		jarr.push_back(j);
	}
	json j;
	j["normalEnemy"] = jarr;
	std::string file = j.dump(4);
	std::ofstream outfile(filepath);
	outfile << file;
	outfile.close();
}
