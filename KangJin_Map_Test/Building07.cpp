#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "Building07.h"

void Building07::Start()
{
	GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot({ 0.5f, 0.73f });

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
			circlecollider->SetOffset({ (-123.000f * scale), (153.000f * scale) });
			circlecollider->SetRadius(250.000f * scale);
			break;
		case 1:
			circlecollider->SetOffset({ (110.000f * scale), (110.000f * scale) });
			circlecollider->SetRadius(250.000f * scale);
			break;
		case 2:
			circlecollider->SetOffset({ (-2.200f * scale), (279.000f * scale) });
			circlecollider->SetRadius(205.300f * scale);
			break;
		case 3:
			circlecollider->SetOffset({ (220.400f * scale), (333.000f * scale) });
			circlecollider->SetRadius(138.500f * scale);
			break;
		}

		m_Colliders.push_back(circlecollider);
	}
}

void Building07::ExporttoJson()
{
	jsonfilepath = "Resource/building_circles7.json";
	Structure::ExporttoJson();
}
