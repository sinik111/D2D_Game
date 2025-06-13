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

	Matrix3x2 renderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
		Matrix3x2::Translation(-size.width / 2, size.height / 2);

	Matrix3x2 finalMatrix = renderMatrix * m_transform->GetWorldMatrix() *
		m_camera->GetInvertedMatrix() * m_renderer->GetUnityMatrix();

	m_renderer->DrawBitmap(*m_image->GetBitmap(), finalMatrix);
}