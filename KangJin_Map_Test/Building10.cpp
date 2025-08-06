#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "Building10.h"

void Building10::Start()
{
	GetGameObject()->GetComponent<BitmapRenderer>()->SetPivot({ 0.5f, 0.77f });

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
			circlecollider->SetOffset({ (-105.000f* scale), (130.000f* scale) });
			circlecollider->SetRadius(185.000f * scale);
			break;
		case 1:
			circlecollider->SetOffset({ (85.000f * scale), (137.000f * scale) });
			circlecollider->SetRadius(200.000f * scale);
			break;
		case 2:
			circlecollider->SetOffset({ (-194.000f * scale), (246.000f * scale) });
			circlecollider->SetRadius(97.000f * scale);
			break;
		case 3:
			circlecollider->SetOffset({ (185.400f * scale), (270.000f * scale) });
			circlecollider->SetRadius(103.500f * scale);
			break;
		}

		m_Colliders.push_back(circlecollider);
	}
}
