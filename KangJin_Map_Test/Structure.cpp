#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Scene.h"
#include "Structure.h"

void Structure::Initialize()
{
    CircleCollider* circlecollider; 
    for (int i = 0; i < index; ++i)
    {
        circlecollider = GetGameObject()->AddComponent<CircleCollider>();
        m_Colliders.push_back(circlecollider);
        circlecollider->SetRadius(100);
    }
}

void Structure::OnTriggerStay(const Collision& collision)
{

}
