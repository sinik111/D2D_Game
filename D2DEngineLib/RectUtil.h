#pragma once

#include <d2d1.h>

#include "Vector2.h"

static bool IsContain(const D2D1_RECT_F& rect, const Vector2& point)
{
	return point.x >= rect.left && point.x <= rect.right &&
		point.y >= rect.top && point.y <= rect.bottom;
}

static bool IsContain(const D2D1_RECT_F& bigger, const D2D1_RECT_F& smaller)
{
	return bigger.left <= smaller.left && bigger.right >= smaller.right &&
		bigger.top <= smaller.top && bigger.bottom >= smaller.bottom;
}

static bool IsOverlap(const D2D1_RECT_F& a, const D2D1_RECT_F& b)
{
	return a.left < b.right && a.right > b.left &&
		a.top < b.bottom && a.bottom > b.top;
}
