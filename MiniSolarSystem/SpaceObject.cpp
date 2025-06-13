#include "../D2DEngineLib/framework.h"
#include "SpaceObject.h"

#include "../D2DEngineLib/Transform.h"
#include "../D2DEngineLib/D2DRenderer.h"
#include "Camera.h"

SpaceObject::SpaceObject(Camera* camera, D2DRenderer* renderer, float speed)
	: m_image{ new Image() }, m_camera{ camera }, m_renderer{ renderer },
	m_speed{ speed }
{
}

SpaceObject::~SpaceObject()
{
	delete m_image;
}

Image* SpaceObject::GetImage()
{
	return m_image;
}

void SpaceObject::Update()
{
	m_transform->Rotate(m_speed);
}

void SpaceObject::Render()
{
	D2D1_SIZE_F size = m_image->GetSize();

	D2D1::Matrix3x2F renderMatrix = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
		D2D1::Matrix3x2F::Translation(-size.width / 2, size.height / 2);

	D2D1::Matrix3x2F finalMatrix = renderMatrix * m_transform->GetMatrix() *
		m_camera->GetInvertedMatrix() * m_renderer->GetUnityMatrix();

	m_renderer->DrawBitmap(*m_image->GetBitmap(), finalMatrix);
}