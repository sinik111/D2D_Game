#include "pch.h"
#include "BitmapRendererSystem.h"

#include "ContainerUtility.h"
#include "D2DRenderer.h"
#include "BitmapRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Debug.h"

void BitmapRendererSystem::Register(BitmapRenderer* bitmapRenderer)
{
	m_bitmapRenderers.push_back(bitmapRenderer);
}

void BitmapRendererSystem::Unregister(BitmapRenderer* bitmapRenderer)
{
	Util::OptimizedErase(m_bitmapRenderers, bitmapRenderer);
}

void BitmapRendererSystem::SetD2DRenderer(D2DRenderer* d2dRenderer)
{
	m_d2dRenderer = d2dRenderer;
}

void BitmapRendererSystem::Update()
{
	const Vector2 cameraPosition = Camera::s_mainCamera->GetTransform()->GetWorldPosition();
	const float zoomFactor = Camera::s_mainCamera->GetZoom();

	const float halfScreenWidth = m_d2dRenderer->GetWidth() / 2.0f * zoomFactor;
	const float halfScreenHeight = m_d2dRenderer->GetHeight() / 2.0f * zoomFactor;

	const float viewLeft = cameraPosition.GetX() - halfScreenWidth;
	const float viewRight = cameraPosition.GetX() + halfScreenWidth;
	const float viewBottom = cameraPosition.GetY() - halfScreenHeight;
	const float viewTop = cameraPosition.GetY() + halfScreenHeight;

	for (const auto& renderer : m_bitmapRenderers)
	{
		switch (renderer->GetSpaceType())
		{
		case SpaceType::Screen:
			m_d2dRenderer->RegisterRendererToQueue(renderer);
			break;

		case SpaceType::World:
		{
			const Vector2 worldPosition = renderer->GetTransform()->GetWorldPosition();
			const Vector2 worldScale = renderer->GetTransform()->GetWorldScale();

			const D2D1_RECT_F sourceRect = renderer->GetSourceRect();

			const float bitmapHalfWidth = (sourceRect.right - sourceRect.left) * worldScale.GetX() / 2.0f;
			const float bitmapHalfHeight = (sourceRect.bottom - sourceRect.top) * worldScale.GetY() / 2.0f;

			const float bitmapLeft = worldPosition.GetX() - bitmapHalfWidth;
			const float bitmapRight = worldPosition.GetX() + bitmapHalfWidth;
			const float bitmapBottom = worldPosition.GetY() - bitmapHalfHeight;
			const float bitmapTop = worldPosition.GetY() + bitmapHalfHeight;

			if (bitmapRight < viewLeft || bitmapLeft > viewRight ||
				bitmapTop < viewBottom || bitmapBottom > viewTop)
			{
				continue;
			}

			m_d2dRenderer->RegisterRendererToQueue(renderer);
		}
			break;
		}
	}
}
