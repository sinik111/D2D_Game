#pragma once

#include "Matrix3x2.h"

enum class RenderCommandType
{
	Bitmap,
	Text
};

class IRenderCommand
{
public:
	virtual ~IRenderCommand() = default;
	virtual RenderCommandType GetType() const = 0;
	virtual int GetSortOrder() const = 0;
};

struct BitmapRenderCommand :
	public IRenderCommand
{
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
	Matrix3x2 transform;
	float opacity;
	D2D1_RECT_F sourceRect;
	int sortOrder;

	BitmapRenderCommand(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap,
		const Matrix3x2& transform,
		const D2D1_RECT_F sourceRect,
		float opacity,
		int sortOrder)
		: bitmap{ std::move(bitmap) }, transform{ transform }, sortOrder{ sortOrder },
		sourceRect{ sourceRect }, opacity{ opacity }
	{

	}

	RenderCommandType GetType() const override
	{
		return RenderCommandType::Bitmap;
	}

	int GetSortOrder() const override
	{
		return sortOrder;
	}
};

struct TextRenderCommand :
	public IRenderCommand
{
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
	std::wstring text;
	Matrix3x2 transform;
	D2D1_POINT_2F point;
	D2D1_SIZE_F size;
	D2D1_COLOR_F color;
	int sortOrder = 0;

	TextRenderCommand(Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat, const std::wstring& text,
		const Matrix3x2& transform, const D2D1_POINT_2F& point, const D2D1_SIZE_F& size,
		const D2D1_COLOR_F color, int sortOrder = 0)
		: textFormat{ std::move(textFormat) }, transform{ transform }, text{ text }, point{ point }, size{ size }, color{ color },
		sortOrder{ sortOrder }
	{

	}

	RenderCommandType GetType() const override
	{
		return RenderCommandType::Text;
	}

	int GetSortOrder() const override
	{
		return sortOrder;
	}
};