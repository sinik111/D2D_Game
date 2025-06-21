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
	const Matrix3x2& unityMatrix = m_d2dRenderer->GetUnityMatrix();
	const Matrix3x2& viewMatrix = Camera::s_mainCamera->GetViewMatrix();

	for (const auto& bitmapRenderer : m_bitmapRenderers)
	{
		D2D1_SIZE_F size = bitmapRenderer->GetBitmap()->GetSize();

		const Matrix3x2& renderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
			Matrix3x2::Translation(-size.width / 2, size.height / 2);

		m_d2dRenderer->AddRenderCommand(std::make_unique<BitmapRenderCommand>(
			bitmapRenderer->GetBitmap(),
			renderMatrix * bitmapRenderer->GetTransform()->GetWorldMatrix() * viewMatrix * unityMatrix,
			bitmapRenderer->GetSortOrder()
		));
	}
}