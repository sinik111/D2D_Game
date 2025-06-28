#pragma once

#include "Component.h"
#include "Enum.h"
#include "BitmapResource.h"
#include "IRenderer.h"

class BitmapRenderer :
	public Component, public IRenderer
{
private:
	std::shared_ptr<BitmapResource> m_bitmapResource;
	SpaceType m_spaceType = SpaceType::World;
	D2D1_RECT_F m_sourceRect{};
	int m_sortOrder = 0;
	float m_opacity = 1.0f;
	bool m_doFlipX = false;

public:
	BitmapRenderer();
	BitmapRenderer(const std::wstring& filePath);
	~BitmapRenderer() override;

public:
	const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& GetBitmap() const;
	bool GetFlipX() const;
	D2D1_RECT_F GetSourceRect() const;
	float GetOpacity() const;

	int GetSortOrder() const override;
	SpaceType GetSpaceType() const override;

public:
	void SetBitmap(const std::wstring& filePath);
	void SetSortOrder(int sortOrder);
	void SetFlipX(bool doFlip);
	void SetSpaceType(SpaceType spaceType);
	void SetSourceRect(const D2D1_RECT_F& sourceRect);
	void SetOpacity(float opacity);
};