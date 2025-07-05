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
	const Vector2 cameraPosition = Camera::s_mainCamera->GetTransform()->GetWorldPosition();
	const float zoomFactor = Camera::s_mainCamera->GetZoom();

	const float halfScreenWidth = m_d2dRenderer->GetWidth() / 2.0f * zoomFactor;
	const float halfScreenHeight = m_d2dRenderer->GetHeight() / 2.0f * zoomFactor;

	const float viewLeft = cameraPosition.x - halfScreenWidth;
	const float viewRight = cameraPosition.x + halfScreenWidth;
	const float viewBottom = cameraPosition.y - halfScreenHeight;
	const float viewTop = cameraPosition.y + halfScreenHeight;

	for (const auto& renderer : m_textRenderers)
	{
		switch (renderer->GetSpaceType())
		{
		case SpaceType::Screen:
		{
			m_d2dRenderer->RegisterRendererToQueue(renderer);
			break;
		}
		case SpaceType::World:
		{
			const Vector2 worldPosition = renderer->GetTransform()->GetWorldPosition();
			const Vector2 worldScale = renderer->GetTransform()->GetWorldScale();
			const Vector2 pivot = renderer->GetPivot();

			const D2D1_SIZE_F rectSize = renderer->GetRectSize();

			const float rectHalfWidth = rectSize.width * worldScale.x;
			const float rectHalfHeight = rectSize.height * worldScale.y;

			const float rectLeft = worldPosition.x - rectHalfWidth * pivot.x;
			const float rectRight = worldPosition.x + rectHalfWidth * (1.0f - pivot.x);
			const float rectBottom = worldPosition.y - rectHalfHeight * pivot.y;
			const float rectTop = worldPosition.y + rectHalfHeight * (1.0f - pivot.y);

			if (rectRight < viewLeft || rectLeft > viewRight ||
				rectTop < viewBottom || rectBottom > viewTop)
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