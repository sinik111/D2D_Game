#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "Building09.h"

void Building09::Start()
{
	GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot({ 0.5f, 0.84f });

	CircleCollider* circlecollider;
	for (int i = 0; i < index; ++i)
	{
		circlecollider = GetGameObject()->AddComponent<CircleCollider>();
		//circlecollider->SetRadius(100);
		circlecollider->SetLayer(CollisionLayer::Building);
		circlecollider->SetTrigger(true);

		switch (i)
		{
		case 0:
			circlecollider->SetOffset({ (0.0f * scale), (100.000f * scale) });
			circlecollider->SetRadius(142.000f * scale);
			break;
		case 1:
			circlecollider->SetOffset({ (0.000f * scale), (202.000f * scale) });
			circlecollider->SetRadius(142.000f * scale);
			break;
		case 2:
			circlecollider->SetOffset({ (0.000f * scale), (262.000f * scale) });
			circlecollider->SetRadius(142.000f * scale);
			break;
		case 3:
			circlecollider->SetOffset({ (75.000f * scale), (348.000f * scale) });
			circlecollider->SetRadius(72.000f * scale);
			break;
		}

		m_Colliders.push_back(circlecollider);
	}
}

void Building09::ExporttoJson()
{
	jsonfilepath = "Resource/building_circles9.json";
	Structure::ExporttoJson();
}
