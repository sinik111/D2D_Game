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

void TextRendererSystem::MakeRenderCommands()
{
	const Matrix3x2 unityMatrix = m_d2dRenderer->GetUnityMatrix();
	const Matrix3x2 viewMatrix = Camera::s_mainCamera->GetViewMatrix();
	D2D1_POINT_2F point;

	for (const auto& textRenderer : m_textRenderers)
	{
		Matrix3x2 renderMatrix;
		Matrix3x2 finalMatrix;

		switch (textRenderer->GetSpaceType())
		{
		case TextRenderer::SpaceType::Screen:
		{
			renderMatrix = Matrix3x2::Scale(1.0f, -1.0f);

			finalMatrix = renderMatrix * unityMatrix;

			point = textRenderer->GetPoint();
			point.y = -point.y;

			break;
		}
		case TextRenderer::SpaceType::World:
		{
			D2D1_SIZE_F size = textRenderer->GetRectSize();

			renderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
				Matrix3x2::Translation(-size.width / 2, size.height / 2);

			finalMatrix = renderMatrix * textRenderer->GetTransform()->GetWorldMatrix() *
				viewMatrix * unityMatrix;

			point = textRenderer->GetPoint();

			break;
		}
		}

		m_d2dRenderer->AddRenderCommand(std::make_unique<TextRenderCommand>(
			textRenderer->GetTextFormat(),
			textRenderer->GetText(),
			finalMatrix,
			point,
			textRenderer->GetRectSize(),
			textRenderer->GetColor(),
			textRenderer->GetSortOrder()
		));
	}
}

Microsoft::WRL::ComPtr<IDWriteTextFormat> TextRendererSystem::CreateTextFormat(float fontSize)
{
	return m_d2dRenderer->CreateTextFormat(fontSize);
}