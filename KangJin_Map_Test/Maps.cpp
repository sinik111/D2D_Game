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
}

void Maps::FixedUpdate()
{
}

void Maps::Update()
{
	// 마우스 우클릭으로 추가
	if (Input::IsKeyPressed(VK_RBUTTON))
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

		m_Positions.push_back(clickPoint);

		std::cout << "x : " << clickPoint.x << ", y : " << clickPoint.y << std::endl;
	}

	//1번 키로 원하는 도형 콜라이더 생성
	if (Input::IsKeyPressed('1'))
	{
		if (!m_Positions.empty())
		{
			Vector2 firstStart = m_Positions[0];

			for (int i = 0; i < m_Positions.size(); ++i)
			{
				if (i + 1 < m_Positions.size())
				{
					Vector2 vecdistance = (m_Positions[i + 1] - m_Positions[i]) / 2;
					Vector2 centerPos = m_Positions[i] + vecdistance;
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

			Vector2 lastPoint = m_Positions[m_Positions.size() - 1];
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

			m_Positions.clear();
		}
	}

	//2번키로 json파일 export
	if (Input::IsKeyPressed('2'))
	{
		json jarr = json::array();
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
		}
	}

	//엔터 눌렀을 때에 라인콜라이더 생성
	if (Input::IsKeyPressed(VK_RETURN))
	{
		CreateLineCollider();
	}

	//백스페이스로 삭제
	if (Input::IsKeyPressed(VK_BACK))
	{
		if (m_Positions.size() >= 1) { m_Positions.pop_back(); }

		ExportJsontoPath("Resource/points.json");
	}

	//Delete 키로 도형 하나 삭제
	if (Input::IsKeyPressed(VK_DELETE))
	{
		if (!m_lineDatas.empty())
		{
			m_lineDatas.pop_back();
		}
	}
}

void Maps::LateUpdate()
{
}

void Maps::OnDestroy()
{
}

//void Maps::OnCollisionEnter(const Collision& collision)
//{
//}
//
//void Maps::OnCollisionStay(const Collision& collision)
//{
//}
//
//void Maps::OnCollisionExit(const Collision& collision)
//{
//}
//
//void Maps::OnTriggerEnter(const Collision& collision)
//{
//}
//
//void Maps::OnTriggerStay(const Collision& collision)
//{
//}
//
//void Maps::OnTriggerExit(const Collision& collision)
//{
//}

std::string Maps::ConvertPositiontoJSON()
{
	json jarr = json::array();
	for (int i = 0; i < m_Positions.size(); ++i)
	{
		if (i + 1 < m_Positions.size())
		{
			Vector2 vecdistance = (m_Positions[i + 1] - m_Positions[i]) / 2;
			Vector2 centerPos = m_Positions[i] + vecdistance;
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
