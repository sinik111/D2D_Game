#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "Building03.h"

void Building03::Start()
{
	GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot({ 0.5f, 0.7f });

	CircleCollider* circlecollider;
	for (int i = 0; i < index; ++i)
	{
		circlecollider = GetGameObject()->AddComponent<CircleCollider>();
		circlecollider->SetRadius(100);
		circlecollider->SetLayer(CollisionLayer::Building);
		circlecollider->SetTrigger(true);

		switch (i)
		{
		case 0:
			circlecollider->SetOffset({ (-188.000f * scale), (219.000f * scale) });
			circlecollider->SetRadius(340.000f * scale);
			break;
		case 1:
			circlecollider->SetOffset({ (185.000f * scale), (215.000f * scale) });
			circlecollider->SetRadius(340.000f * scale);
			break;
		case 2:
			circlecollider->SetOffset({ (-118.200f * scale), (408.000f * scale) });
			circlecollider->SetRadius(200.300f * scale);
			break;
		case 3:
			circlecollider->SetOffset({ (-375.000f * scale), (400.000f * scale) });
			circlecollider->SetRadius(150.000f * scale);
			break;
		}

		m_Colliders.push_back(circlecollider);
	}
}

void Building03::ExporttoJson()
{
	jsonfilepath = "Resource/building_circles3.json";
	Structure::ExporttoJson();
}
