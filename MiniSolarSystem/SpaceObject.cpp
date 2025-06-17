#include "../D2DEngineLib/framework.h"
#include "SpaceObject.h"

#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"

#include "Sun.h"

SpaceObject::SpaceObject(float speed)
	: m_speed{ speed }
{

}

void SpaceObject::Start()
{
	BitmapRenderer* bitmapRenderer = m_owner->AddComponent<BitmapRenderer>();

	if (m_owner->GetName() == L"Sun")
	{
		bitmapRenderer->SetBitmap(ResourceManager::GetBitmap(L"Sun"));
		GetTransform()->SetPosition(Vector2(0.0f, 0.0f));
		GetTransform()->SetScale(Vector2(0.2f, 0.2f));

		m_owner->AddComponent<Sun>();
	}
	else if (m_owner->GetName() == L"Earth")
	{
		bitmapRenderer->SetBitmap(ResourceManager::GetBitmap(L"Earth"));
		GetTransform()->SetPosition(Vector2(800.0f, 0.0f));
		GetTransform()->SetScale(Vector2(1.0f, 1.0f));
	}
	else if (m_owner->GetName() == L"Moon")
	{
		bitmapRenderer->SetBitmap(ResourceManager::GetBitmap(L"Moon"));
		GetTransform()->SetPosition(Vector2(400.0f, 0.0f));
		GetTransform()->SetScale(Vector2(0.5f, 0.5f));
	}
}

void SpaceObject::Update()
{
	GetTransform()->Rotate(m_speed);
}