#pragma once

#include "Component.h"
#include "Enum.h"
#include "IRenderer.h"

enum class VerticalAlignment
{
	Top,
	Bottom,
	Center
};

enum class HorizontalAlignment
{
	Left,
	Right,
	Center,
	Justified
};

class TextRenderer :
	public Component, public IRenderer
{
private:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;

	std::wstring m_text = L"text"; // 출력할 텍스트

	Matrix3x2 m_cachedRenderMatrix;
	Matrix3x2 m_cachedRenderWorldMatrix;
	D2D1_COLOR_F m_color{ 0.0f, 0.0f, 0.0f, 1.0f }; // 텍스트 색 (RGBA)
	Vector2 m_pivot{ 0.5f, 0.5f };
	D2D1_SIZE_F m_rectSize{ 250.0f, 50.0f }; // 텍스트를 출력할 사각형 사이즈
	Bounds m_bounds;
	float m_fontSize = 15.0f; // 텍스트 폰트 크기

	SpaceType m_spaceType = SpaceType::Screen; // 출력 타입

	VerticalAlignment m_verticalAlignment = VerticalAlignment::Top; // 세로 정렬
	HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left; // 가로 정렬

	int m_sortOrder{}; // 정렬 순서

	bool m_isTextDirty = true;

public:
	TextRenderer();

public:
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

public:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> GetTextFormat() const;
	std::wstring GetText() const;
	D2D1_COLOR_F GetColor() const;
	D2D1_SIZE_F GetRectSize() const;
	float GetFontSize() const;
	Vector2 GetPivot() const;

	void Update() override;
	void Render(const RenderContext& context) const override;
	SpaceType GetSpaceType() const override;
	int GetSortOrder() const override;
	float GetY() const override;
	const Bounds& GetBounds() const override;

public:
	void SetText(const std::wstring& text);
	void SetColor(const D2D1_COLOR_F& color);
	void SetSpaceType(SpaceType spaceType);
	void SetSortOrder(int sortOrder);
	void SetRectSize(const D2D1_SIZE_F& size);
	void SetFontSize(float size);
	void SetVerticalAlignment(VerticalAlignment align);
	void SetHorizontalAlignment(HorizontalAlignment align);
	void SetPivot(const Vector2& pivot);

private:
	void MakeRenderMatrix();
};