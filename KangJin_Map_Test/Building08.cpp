#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Scene.h"
#include "../D2DEngineLib/LineCollider.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "Building08.h"
#include <iostream>

void Building08::Start()
{
	Vector2 pivot{ 0.35f, 0.65f };
	GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot(pivot);

	Vector2 pos = GetGameObject()->GetTransform()->GetLocalPosition();
	scale = 1.3f;

	//TODO : 나중에 LINECOLLIDER 그릴 때 한꺼번에 하기
	
	//GameObject* Line1 = CreateGameObject(L"Line1");
	//LineCollider* linecollider1 = Line1->AddComponent<LineCollider>();
	//linecollider1->SetLayer(CollisionLayer::Wall);
	//linecollider1->GetTransform()->SetLocalPosition({pos.x -(7.0f*scale), pos.y-(115.5f*scale)});
	//linecollider1->SetLine({ (-246.0f *scale),(123.5f * scale) }, { (246.0f * scale), (-123.5f * scale) });
	//
	//GameObject* Line2 = CreateGameObject(L"Line2");
	//LineCollider* linecollider2 = Line2->AddComponent<LineCollider>();
	//linecollider2->SetLayer(CollisionLayer::Wall);
	//linecollider2->GetTransform()->SetLocalPosition({pos.x + (367.5f * scale), pos.y - (173.5f * scale) });
	//linecollider2->SetLine({ ( - 128.5f * scale),( - 65.5f * scale) }, {(128.5f * scale), (65.5f * scale) });
	//
	//GameObject* Line3 = CreateGameObject(L"Line3");
	//LineCollider* linecollider3 = Line3->AddComponent<LineCollider>();
	//linecollider3->SetLayer(CollisionLayer::Wall);
	//linecollider3->GetTransform()->SetLocalPosition({pos.x + (494.5f * scale), pos.y - (60.5f * scale) });
	//linecollider3->SetLine({ (1.5f * scale),-(47.5f * scale) }, { -(1.5f * scale), (47.5f * scale) });
	//
	//GameObject* Line4 = CreateGameObject(L"Line4");
	//LineCollider* linecollider4 = Line4->AddComponent<LineCollider>();
	//linecollider4->SetLayer(CollisionLayer::Wall);
	//linecollider4->GetTransform()->SetLocalPosition({pos.x + (239.0f * scale), pos.y + (106.5f * scale) });
	//linecollider4->SetLine({ (254.0f * scale), (-119.5f * scale) }, { (- 254.0f * scale), (119.5f * scale) });

	//GameObject* Line5 = CreateGameObject(L"Line5");
	//LineCollider* linecollider5 = Line5->AddComponent<LineCollider>();
	//linecollider5->SetLayer(CollisionLayer::Wall);
	//linecollider5->GetTransform()->SetLocalPosition({pos.x - (133.0f * scale), pos.y + (171.5f * scale) });
	//linecollider5->SetLine({ (118.0f * scale), (54.5f * scale) }, { ( - 118.0f * scale), ( - 54.5f * scale) });
 //
	//GameObject* Line6 = CreateGameObject(L"Line6");
	//LineCollider* linecollider6 = Line6->AddComponent<LineCollider>();
	//linecollider6->SetLayer(CollisionLayer::Wall);
	//linecollider6->GetTransform()->SetLocalPosition({pos.x - (252.0f * scale), pos.y + (62.5f * scale) });
	//linecollider6->SetLine({ (1.0f * scale), (54.5f * scale) }, { (- 1.0f * scale), (-54.5f * scale) });
    
	CircleCollider* circlecollider; 
    for (int i = 0; i < index; ++i)
    {
        circlecollider = GetGameObject()->AddComponent<CircleCollider>();
        m_Colliders.push_back(circlecollider);
        circlecollider->SetRadius(100);
		circlecollider->SetLayer(CollisionLayer::Building);
		circlecollider->SetTrigger(true);
		switch (i)
		{
		case 0 :
			circlecollider->SetOffset({ (-112.0f * scale), (254.0f * scale) });
			circlecollider->SetRadius(135.0f * scale);
			break;
		case 1 :
			circlecollider->SetOffset({ (16.0f * scale), (320.0f * scale) });
			circlecollider->SetRadius(130.0f * scale);
			break;
		case 2 :
			circlecollider->SetOffset({ (174.0f * scale), (223.0f * scale) });
			circlecollider->SetRadius(139.0f * scale);
			break;
		case 3 :
			circlecollider->SetOffset({ (342.0f * scale), (119.0f * scale) });
			circlecollider->SetRadius(155.0f * scale);
			break;
		}
    }
}



