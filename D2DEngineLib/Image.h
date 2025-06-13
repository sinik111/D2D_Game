#pragma once

class Image
{
private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_bitmap;

public:
	Image();

	ID2D1Bitmap1** GetBitmap();
	void SetBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap);
	D2D1_SIZE_F GetSize() const;
};