#pragma once

#include "Component.h"

class BitmapRenderer :
	public Component
{
private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_bitmap;
	int m_sortOrder = 0;

public:
	BitmapRenderer();
	~BitmapRenderer() override;

public:
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> GetBitmap();
	int GetSortOrder();

public:
	void SetBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap);
	void SetSortOrder(int sortOrder);
};