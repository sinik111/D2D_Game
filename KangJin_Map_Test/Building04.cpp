#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "Building04.h"

void Building04::Start()
{
	GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot({ 0.5f, 0.725f });

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
			circlecollider->SetOffset({ (-110.000f * scale), (110.000f * scale) });
			circlecollider->SetRadius(173.000f * scale);
			break;
		case 1:
			circlecollider->SetOffset({ (160.000f * scale), (78.000f * scale) });
			circlecollider->SetRadius(120.000f * scale);
			break;
		case 2:
			circlecollider->SetOffset({ (10.800f * scale), (257.000f * scale) });
			circlecollider->SetRadius(97.300f * scale);
			break;
		case 3:
			circlecollider->SetOffset({ (90.400f * scale), (196.000f * scale) });
			circlecollider->SetRadius(108.500f * scale);
			break;
		}

		m_Colliders.push_back(circlecollider);
	}
}

void Building04::ExporttoJson()
{
	jsonfilepath = "Resource/building_circles4.json";
	Structure::ExporttoJson();
}
