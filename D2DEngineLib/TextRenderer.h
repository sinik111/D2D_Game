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

	std::wstring m_text{ L"text" }; // 출력할 텍스트

	D2D1_COLOR_F m_color{ 0.0f, 0.0f, 0.0f, 1.0f }; // 텍스트 색 (RGBA)

	// 텍스트를 출력할 사각형 위치.
	// m_spaceType이 Screen이면 왼쪽 위 꼭지점의 위치,
	// m_spaceType이 World면 중심의 위치
	D2D1_POINT_2F m_point{};

	D2D1_SIZE_F m_rectSize{ 50.0f, 20.0f }; // 텍스트를 출력할 사각형 사이즈
	float m_fontSize{ 10.0f }; // 텍스트 폰트 크기

	SpaceType m_spaceType{ SpaceType::Screen }; // 출력 타입

	VerticalAlignment m_verticalAlignment{ VerticalAlignment::Top }; // 세로 정렬
	HorizontlAlignment m_horizontalAlignment{ HorizontlAlignment::Left }; // 가로 정렬

	int m_sortOrder{}; // 정렬 순서

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