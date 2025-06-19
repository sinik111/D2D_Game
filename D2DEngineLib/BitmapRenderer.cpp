#include "pch.h"
#include "BitmapRenderer.h"

#include "ComponentSystem.h"

BitmapRenderer::BitmapRenderer()
	: m_sortOrder{ 0 }
{
	ComponentSystem::Get().BitmapRenderer().Register(this);
}

BitmapRenderer::~BitmapRenderer()
{
	ComponentSystem::Get().BitmapRenderer().Unregister(this);
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> BitmapRenderer::GetBitmap()
{
	return m_bitmap;
}

int BitmapRenderer::GetSortOrder()
{
	return m_sortOrder;
}

void BitmapRenderer::SetBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap)
{
	m_bitmap = std::move(bitmap);
}

void BitmapRenderer::SetSortOrder(int sortOrder)
{
	m_sortOrder = sortOrder;
}
