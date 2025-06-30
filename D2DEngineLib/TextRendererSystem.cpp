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

	const float viewLeft = cameraPosition.GetX() - halfScreenWidth;
	const float viewRight = cameraPosition.GetX() + halfScreenWidth;
	const float viewBottom = cameraPosition.GetY() - halfScreenHeight;
	const float viewTop = cameraPosition.GetY() + halfScreenHeight;

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

			const D2D1_SIZE_F rectSize = renderer->GetRectSize();

			const float rectHalfWidth = rectSize.width * worldScale.GetX() / 2.0f;
			const float rectHalfHeight = rectSize.height * worldScale.GetY() / 2.0f;

			const float rectLeft = worldPosition.GetX() - rectHalfWidth;
			const float rectRight = worldPosition.GetX() + rectHalfWidth;
			const float rectBottom = worldPosition.GetY() - rectHalfHeight;
			const float rectTop = worldPosition.GetY() + rectHalfHeight;

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