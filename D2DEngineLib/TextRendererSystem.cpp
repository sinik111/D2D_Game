#include "pch.h"
#include "TextRendererSystem.h"

#include "ContainerUtility.h"
#include "D2DRenderer.h"
#include "TextRenderer.h"
#include "Transform.h"
#include "Camera.h"

void TextRendererSystem::Register(TextRenderer* textRenderer)
{
	m_textRenderers.push_back(textRenderer);
}

void TextRendererSystem::Unregister(TextRenderer* textRenderer)
{
	Util::OptimizedErase(m_textRenderers, textRenderer);
}

void TextRendererSystem::SetD2DRenderer(D2DRenderer* d2dRenderer)
{
	m_d2dRenderer = d2dRenderer;
}

void TextRendererSystem::Update()
{
	const Bounds& visibleBounds = Camera::s_mainCamera->GetVisibleBounds();

	for (const auto& renderer : m_textRenderers)
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

Microsoft::WRL::ComPtr<IDWriteTextFormat> TextRendererSystem::CreateTextFormat(float fontSize)
{
	return m_d2dRenderer->CreateTextFormat(fontSize);
}