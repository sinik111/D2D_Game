#pragma once

#include "Component.h"

class TextRenderer :
	public Component
{
public:
	enum class SpaceType
	{
		Screen,
		World
	};

	enum class VerticalAlignment
	{
		Top,
		Bottom,
		Center
	};

	enum class HorizontlAlignment
	{
		Left,
		Right,
		Center,
		Justified
	};

private:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	std::wstring m_text;
	D2D1_COLOR_F m_color;
	D2D1_POINT_2F m_point;
	D2D1_SIZE_F m_rectSize; // rect size. font size 구현 필요
	float m_fontSize;
	SpaceType m_spaceType;
	VerticalAlignment m_verticalAlignment;
	HorizontlAlignment m_horizontalAlignment;
	int m_sortOrder;

public:
	TextRenderer();
	~TextRenderer() override;

public:
	void SetText(const std::wstring& text);
	void SetColor(const D2D1_COLOR_F& color);
	void SetSpaceType(SpaceType spaceType);
	void SetSortOrder(int sortOrder);
	void SetPoint(const D2D1_POINT_2F& point);
	void SetRectSize(const D2D1_SIZE_F& size);
	void SetFontSize(float size);
	void SetVerticalAlignment(VerticalAlignment align);
	void SetHorizontalAlignment(HorizontlAlignment align);

	Microsoft::WRL::ComPtr<IDWriteTextFormat> GetTextFormat();
	std::wstring GetText();
	D2D1_COLOR_F GetColor();
	SpaceType GetSpaceType();
	int GetSortOrder();
	D2D1_POINT_2F GetPoint();
	D2D1_SIZE_F GetRectSize();
	float GetFontSize();
};