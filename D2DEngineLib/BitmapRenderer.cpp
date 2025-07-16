#include "pch.h"
#include "BitmapRenderer.h"

#include "ComponentSystem.h"
#include "ResourceManager.h"
#include "Transform.h"

BitmapRenderer::BitmapRenderer(const std::wstring& filePath)
	: m_filePath{ filePath }
{
	m_bitmapResource = ResourceManager::Get().CreateBitmapResource(filePath);

	m_sourceRect = {
		0.0f,
		0.0f,
		m_bitmapResource->GetSize().width,
		m_bitmapResource->GetSize().height
	};
}

void BitmapRenderer::RegisterToSystem()
{
	ComponentSystem::Get().Renderer().Register(this);
}

void BitmapRenderer::UnregisterFromSystem()
{
	ComponentSystem::Get().Renderer().Unregister(this);
}

const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& BitmapRenderer::GetBitmap() const
{
	return m_bitmapResource->GetBitmap();
}

void BitmapRenderer::Update()
{
	if (m_isBitmapDirty || GetTransform()->GetIsDirtyThisFrame())
	{
		if (m_isBitmapDirty)
		{
			MakeRenderMatrix();
		}

		const Matrix3x2& worldMatrix = GetTransform()->GetWorldMatrix();

		m_cachedRenderWorldMatrix = m_cachedRenderMatrix * worldMatrix;

		const float bitmapWidth = m_sourceRect.right - m_sourceRect.left;
		const float bitmapHeight = std::fabsf(m_sourceRect.bottom - m_sourceRect.top);

		D2D1_RECT_F localRect{
			-bitmapWidth * m_pivot.x,
			bitmapHeight * (1.0f - m_pivot.y),
			bitmapWidth * (1.0f - m_pivot.x),
			-bitmapHeight * m_pivot.y
		};

		m_bounds = Bounds::RectToWorldBounds(localRect, worldMatrix);

		m_isBitmapDirty = false;
	}
}

void BitmapRenderer::Render(const RenderContext& context) const
{
	Matrix3x2 finalMatrix;

	switch (m_spaceType)
	{
	case SpaceType::Screen:
		finalMatrix = m_cachedRenderWorldMatrix * context.unityMatrix;

		break;
	case SpaceType::World:
		finalMatrix = m_cachedRenderWorldMatrix * context.viewUnityMatrix;

		break;
	}

	context.deviceContext->SetTransform(finalMatrix);
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
	return GetTransform()->GetWorldPosition().y;
}

Vector2 BitmapRenderer::GetPivot() const
{
	return m_pivot;
}

const Bounds& BitmapRenderer::GetBounds() const
{
	return m_bounds;
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
	if (m_filePath == filePath)
	{
		return;
	}

	m_filePath = filePath;

	m_bitmapResource = ResourceManager::Get().CreateBitmapResource(filePath);

	m_sourceRect = { 0.0f, 0.0f, m_bitmapResource->GetSize().width, m_bitmapResource->GetSize().height };

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetSortOrder(int sortOrder)
{
	m_sortOrder = sortOrder;
}

void BitmapRenderer::SetFlipX(bool doFlip)
{
	m_doFlipX = doFlip;

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetSpaceType(SpaceType spaceType)
{
	m_spaceType = spaceType;
}

void BitmapRenderer::SetSourceRect(const D2D1_RECT_F& sourceRect)
{
	m_sourceRect = sourceRect;

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetPivot(const Vector2& pivot)
{
	m_pivot = pivot;

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetOpacity(float opacity)
{
	m_opacity = opacity;
}

void BitmapRenderer::MakeRenderMatrix()
{
	if (m_doFlipX)
	{
		m_cachedRenderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
			Matrix3x2::Translation(-(m_sourceRect.right - m_sourceRect.left) * m_pivot.x,
				(std::abs(m_sourceRect.bottom - m_sourceRect.top)) * (1 - m_pivot.y)) *
			Matrix3x2::Scale(-1.0f, 1.0f);
	}
	else
	{
		m_cachedRenderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
			Matrix3x2::Translation(-(m_sourceRect.right - m_sourceRect.left) * m_pivot.x,
				(std::abs(m_sourceRect.bottom - m_sourceRect.top)) * (1 - m_pivot.y));
	}
}