#include "pch.h"
#include "BitmapRendererSystem.h"

#include "ContainerUtility.h"
#include "D2DRenderer.h"
#include "BitmapRenderer.h"
#include "Transform.h"
#include "Camera.h"

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

void BitmapRendererSystem::MakeRenderCommands()
{
	const Matrix3x2& unityMatrix{ m_d2dRenderer->GetUnityMatrix() };
	const Matrix3x2& viewMatrix{ Camera::s_mainCamera->GetViewMatrix() };
	const Matrix3x2& unityMulViewMatrix{ viewMatrix * unityMatrix };

	const Vector2& cameraPosition{ Camera::s_mainCamera->GetTransform()->GetPosition() };
	const float zoomFactor{ Camera::s_mainCamera->GetZoom() };

	const float halfScreenWidth{ m_d2dRenderer->GetWidth() / 2.0f * zoomFactor };
	const float halfScreenHeight{ m_d2dRenderer->GetHeight() / 2.0f * zoomFactor };

	const float viewLeft{ cameraPosition.GetX() - halfScreenWidth };
	const float viewRight{ cameraPosition.GetX() + halfScreenWidth };
	const float viewBottom{ cameraPosition.GetY() - halfScreenHeight };
	const float viewTop{ cameraPosition.GetY() + halfScreenHeight };

	for (const auto& bitmapRenderer : m_bitmapRenderers)
	{
		const D2D1_RECT_F& sourceRect{ bitmapRenderer->GetSourceRect() };
		const D2D1_SIZE_F& size{ sourceRect.right - sourceRect.left, sourceRect.bottom - sourceRect.top };
		const Vector2& position{ bitmapRenderer->GetTransform()->GetPosition() };

		const float bitmapHalfWidth{ size.width / 2.0f };
		const float bitmapHalfHeight{ size.height / 2.0f };

		const float bitmapLeft{ position.GetX() - bitmapHalfWidth };
		const float bitmapRight{ position.GetX() + bitmapHalfWidth };
		const float bitmapBottom{ position.GetY() - bitmapHalfHeight };
		const float bitmapTop{ position.GetY() + bitmapHalfHeight };

		if (bitmapRight < viewLeft || bitmapLeft > viewRight ||
			bitmapTop < viewBottom || bitmapBottom > viewTop)
		{
			continue;
		}

		const Matrix3x2& renderMatrix{ Matrix3x2::Scale(1.0f, -1.0f) *
			Matrix3x2::Translation(-bitmapHalfWidth, bitmapHalfHeight) };

		Matrix3x2 finalMatrix{ renderMatrix * bitmapRenderer->GetTransform()->GetWorldMatrix() };

		if (bitmapRenderer->GetFlipX())
		{
			finalMatrix *= Matrix3x2::Scale(-1.0f, 1.0f);
		}

		finalMatrix *= unityMulViewMatrix;

		m_d2dRenderer->AddRenderCommand(std::make_unique<BitmapRenderCommand>(
			bitmapRenderer->GetBitmap(),
			finalMatrix,
			sourceRect,
			bitmapRenderer->GetOpacity(),
			bitmapRenderer->GetSortOrder()
		));
	}
}