#include "pch.h"
#include "Image.h"

Image::Image()
    :m_bitmap{ nullptr }
{
}

ID2D1Bitmap1** Image::GetBitmap()
{
    return m_bitmap.GetAddressOf();
}

void Image::SetBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap)
{
    m_bitmap = bitmap;
}

D2D1_SIZE_F Image::GetSize() const
{
    return m_bitmap.Get()->GetSize();
}
