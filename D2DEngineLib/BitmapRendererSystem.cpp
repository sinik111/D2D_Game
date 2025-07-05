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

	const float viewLeft = cameraPosition.x - halfScreenWidth;
	const float viewRight = cameraPosition.x + halfScreenWidth;
	const float viewBottom = cameraPosition.y - halfScreenHeight;
	const float viewTop = cameraPosition.y + halfScreenHeight;

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
			const Vector2 pivot = renderer->GetPivot();

			const D2D1_RECT_F sourceRect = renderer->GetSourceRect();

			const float bitmapWidth = (sourceRect.right - sourceRect.left) * worldScale.x;
			const float bitmapHeight = (std::fabsf(sourceRect.bottom - sourceRect.top)) * worldScale.y;

			const float bitmapLeft = worldPosition.x - bitmapWidth * pivot.x;
			const float bitmapRight = worldPosition.x + bitmapWidth * (1.0f - pivot.x);
			const float bitmapBottom = worldPosition.y - bitmapHeight * pivot.y;
			const float bitmapTop = worldPosition.y + bitmapHeight * (1.0f - pivot.y);

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
