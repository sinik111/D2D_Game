#include "pch.h"
#include "RendererSystem.h"

#include "ContainerUtility.h"
#include "D2DRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "IRenderer.h"

void RendererSystem::Register(IRenderer* renderer)
{
	m_renderers.push_back(renderer);
}

void RendererSystem::Unregister(IRenderer* renderer)
{
	Util::OptimizedErase(m_renderers, renderer);
}

void RendererSystem::SetD2DRenderer(D2DRenderer* d2dRenderer)
{
	m_d2dRenderer = d2dRenderer;
}

void RendererSystem::RegisterRendererToRenderQueue()
{
	const Bounds& visibleBounds = Camera::s_mainCamera->GetVisibleBounds();

	for (const auto& renderer : m_renderers)
	{
		renderer->Update();

		switch (renderer->GetSpaceType())
		{
		case SpaceType::Screen:
		{
			m_d2dRenderer->RegisterRendererToQueue(renderer);
			break;
		}
		case SpaceType::World:
		{
			const Bounds& bounds = renderer->GetBounds();

			if (!Bounds::IsOverlap(visibleBounds, bounds))
			{
				continue;
			}

			m_d2dRenderer->RegisterRendererToQueue(renderer);

			break;
		}
		}
	}
}

Microsoft::WRL::ComPtr<IDWriteTextFormat> RendererSystem::CreateTextFormat(float fontSize)
{
	return m_d2dRenderer->CreateTextFormat(fontSize);
}