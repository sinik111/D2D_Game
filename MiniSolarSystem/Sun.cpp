#include "../D2DEngineLib/framework.h"
#include "Sun.h"

#include "../D2DEngineLib/GameObject.h"
#include "SpaceObject.h"

Sun::Sun()
{
}

void Sun::Start()
{
}

void Sun::Update()
{
    if (GetAsyncKeyState('A') & 0x8000)
    {
        GameObject* newEarth = CreateGameObject(L"Earth");
        //newEarth->GetTransform()->SetParent(GetTransform());
        newEarth->AddComponent<SpaceObject>(0.6f);

        m_earths.push_back(newEarth);
    }

    if (GetAsyncKeyState('S') & 0x8000)
    {
        if (!m_earths.empty())
        {
            m_earths.front()->Destroy();
            m_earths.pop_front();
        }
    }
}
