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
	const Bounds& visibleBounds = Camera::s_mainCamera->GetVisibleBounds();

	for (const auto& renderer : m_bitmapRenderers)
	{
		renderer->Update();

		switch (renderer->GetSpaceType())
		{
		case SpaceType::Screen:
			m_d2dRenderer->RegisterRendererToQueue(renderer);
			break;

		case SpaceType::World:
		{
			const Bounds& bounds = renderer->GetBounds();

			if (!Bounds::IsOverlap(visibleBounds, bounds))
			{
				continue;
			}

			m_d2dRenderer->RegisterRendererToQueue(renderer);
		}
			break;
		}
	}
}
