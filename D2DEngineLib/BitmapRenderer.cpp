#include "pch.h"
#include "BitmapRenderer.h"

#include "ComponentSystem.h"
#include "ResourceManager.h"
#include "Transform.h"

BitmapRenderer::BitmapRenderer()
{
	ComponentSystem::Get().BitmapRenderer().Register(this);
}

BitmapRenderer::BitmapRenderer(const std::wstring& filePath)
{
	ComponentSystem::Get().BitmapRenderer().Register(this);

	m_bitmapResource = ResourceManager::Get().CreateBitmapResource(filePath);

	m_sourceRect = {
		0.0f,
		0.0f,
		m_bitmapResource->GetSize().width,
		m_bitmapResource->GetSize().height
	};
}

BitmapRenderer::~BitmapRenderer()
{
	ComponentSystem::Get().BitmapRenderer().Unregister(this);
}

const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& BitmapRenderer::GetBitmap() const
{
	return m_bitmapResource->GetBitmap();
}

void BitmapRenderer::Render(const RenderContext& context) const
{
	const Matrix3x2 worldMatrix = GetTransform()->GetWorldMatrix();
	const Matrix3x2 unityMatrix = context.unityMatrix;
	const Matrix3x2 viewUnityMatrix = context.viewUnityMatrix;

	const float bitmapHalfWidth = (m_sourceRect.right - m_sourceRect.left) / 2.0f;
	const float bitmapHalfHeight = (m_sourceRect.bottom - m_sourceRect.top) / 2.0f;

	const Matrix3x2 renderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
		Matrix3x2::Translation(-bitmapHalfWidth, bitmapHalfHeight);

	Matrix3x2 finalMatrix;

	switch (m_spaceType)
	{
	case SpaceType::Screen: // 수정 필요. 방향 이상함
	{
		if (m_doFlipX)
		{
			finalMatrix = Matrix3x2::Scale(-1.0f, -1.0f) * worldMatrix * unityMatrix;
		}
		else
		{
			finalMatrix = Matrix3x2::Scale(1.0f, -1.0f) * worldMatrix * unityMatrix;
		}
	}
		break;
	case SpaceType::World:
	{
		if (m_doFlipX)
		{
			finalMatrix = renderMatrix * Matrix3x2::Scale(-1.0f, 1.0f) * worldMatrix * viewUnityMatrix;
		}
		else
		{
			finalMatrix = renderMatrix * worldMatrix * viewUnityMatrix;
		}
	}
		break;
	}

	context.deviceContext->SetTransform(static_cast<D2D1_MATRIX_3X2_F>(finalMatrix));
	context.deviceContext->DrawBitmap(
		m_bitmapResource->GetBitmap().Get(),
		nullptr,
		m_opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		&m_sourceRect
	);
}

int BitmapRenderer::GetSortOrder() const
{
	return m_sortOrder;
}

bool BitmapRenderer::GetFlipX() const
{
	return m_doFlipX;
}

SpaceType BitmapRenderer::GetSpaceType() const
{
	return m_spaceType;
}

float BitmapRenderer::GetY() const
{
	return GetTransform()->GetWorldPosition().GetY();
}

D2D1_RECT_F BitmapRenderer::GetSourceRect() const
{
	return m_sourceRect;
}

float BitmapRenderer::GetOpacity() const
{
	return m_opacity;
}

void BitmapRenderer::SetBitmap(const std::wstring& filePath)
{
	m_bitmapResource = ResourceManager::Get().CreateBitmapResource(filePath);

	m_sourceRect = { 0.0f, 0.0f, m_bitmapResource->GetSize().width, m_bitmapResource->GetSize().height };
}

void BitmapRenderer::SetSortOrder(int sortOrder)
{
	m_sortOrder = sortOrder;
}

void BitmapRenderer::SetFlipX(bool doFlip)
{
	m_doFlipX = doFlip;
}

void BitmapRenderer::SetSpaceType(SpaceType spaceType)
{
	m_spaceType = spaceType;
}

void BitmapRenderer::SetSourceRect(const D2D1_RECT_F& sourceRect)
{
	m_sourceRect = sourceRect;
}

void BitmapRenderer::SetOpacity(float opacity)
{
	m_opacity = opacity;
}
