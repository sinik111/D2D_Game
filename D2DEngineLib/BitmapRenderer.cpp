#include "pch.h"
#include "BitmapRenderer.h"

#include "ComponentSystem.h"

BitmapRenderer::BitmapRenderer()
{
	ComponentSystem::Get().BitmapRenderer().Register(this);
}

BitmapRenderer::~BitmapRenderer()
{
	ComponentSystem::Get().BitmapRenderer().Unregister(this);
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> BitmapRenderer::GetBitmap() const
{
	return m_bitmap;
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

D2D1_RECT_F BitmapRenderer::GetSourceRect() const
{
	return m_sourceRect;
}

float BitmapRenderer::GetOpacity() const
{
	return m_opacity;
}

void BitmapRenderer::SetBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap)
{
	m_bitmap = std::move(bitmap);

	m_sourceRect = { 0.0f, 0.0f, m_bitmap->GetSize().width, m_bitmap->GetSize().height };
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
