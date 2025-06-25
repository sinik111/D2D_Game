#pragma once

#include "Component.h"
#include "Enum.h"

class BitmapRenderer :
	public Component
{
private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_bitmap;
	SpaceType m_spaceType{ SpaceType::World };
	D2D1_RECT_F m_sourceRect{};
	int m_sortOrder{ 0 };
	float m_opacity{ 1.0f };
	bool m_doFlipX{ false };

public:
	BitmapRenderer();
	~BitmapRenderer() override;

public:
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> GetBitmap() const;
	int GetSortOrder() const;
	bool GetFlipX() const;
	SpaceType GetSpaceType() const;
	D2D1_RECT_F GetSourceRect() const;
	float GetOpacity() const;

public:
	void SetBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap);
	void SetSortOrder(int sortOrder);
	void SetFlipX(bool doFlip);
	void SetSpaceType(SpaceType spaceType);
	void SetSourceRect(const D2D1_RECT_F& sourceRect);
	void SetOpacity(float opacity);
};