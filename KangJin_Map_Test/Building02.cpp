#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "Building02.h"

void Building02::Start()
{
	GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot({ 0.5f, 0.8f });

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
			circlecollider->SetOffset({ (0.0f * scale), (125.0f * scale) });
			circlecollider->SetRadius(260.000f * scale);
			break;
		
		case 1:
			circlecollider->SetOffset({ (-20.000f * scale), (380.000f * scale) });
			circlecollider->SetRadius(245.0f * scale);
			break;
		
		case 2:
			circlecollider->SetOffset({ (-164.200f * scale), (260.000f * scale) });
			circlecollider->SetRadius(97.300f * scale);
			break;
		
		case 3:
			circlecollider->SetOffset({ (154.400f * scale), (232.000f * scale) });
			circlecollider->SetRadius(108.500f * scale);
			break;
		}

		m_Colliders.push_back(circlecollider);
	}


}

void Building02::ExporttoJson()
{
	jsonfilepath = "Resource/building_circles2.json";
	Structure::ExporttoJson();
}
