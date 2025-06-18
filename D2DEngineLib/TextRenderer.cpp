#include "pch.h"
#include "TextRenderer.h"

#include "ComponentSystem.h"

TextRenderer::TextRenderer()
	: m_text{ L"text" }, m_point{}, m_color{ 0.0f, 0.0f, 0.0f, 1.0f },
	m_rectSize{ 50.0f, 20.0f }, m_spaceType{ SpaceType::Screen },
	m_sortOrder{}, m_fontSize{ 10.0f }, m_horizontalAlignment{ HorizontlAlignment::Left },
	m_verticalAlignment{ VerticalAlignment::Top }
{
	ComponentSystem::TextRenderer().Register(this);

	m_textFormat = ComponentSystem::TextRenderer().CreateTextFormat(m_fontSize);
}

TextRenderer::~TextRenderer()
{
	ComponentSystem::TextRenderer().Unregister(this);
}

void TextRenderer::SetText(const std::wstring& text)
{
	m_text = text;
}

void TextRenderer::SetColor(const D2D1_COLOR_F& color)
{
	m_color = color;
}

void TextRenderer::SetSpaceType(SpaceType spaceType)
{
	m_spaceType = spaceType;
}

void TextRenderer::SetSortOrder(int sortOrder)
{
	m_sortOrder = sortOrder;
}

void TextRenderer::SetPoint(const D2D1_POINT_2F& point)
{
	m_point = point;
}

void TextRenderer::SetRectSize(const D2D1_SIZE_F& size)
{
	m_rectSize = size;
}

void TextRenderer::SetFontSize(float size)
{
	m_fontSize = size;

	m_textFormat = ComponentSystem::TextRenderer().CreateTextFormat(m_fontSize);

	SetVerticalAlignment(m_verticalAlignment);
	SetHorizontalAlignment(m_horizontalAlignment);
}

void TextRenderer::SetVerticalAlignment(VerticalAlignment align)
{	
	m_verticalAlignment = align;

	switch (align)
	{
	case VerticalAlignment::Top:
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case VerticalAlignment::Bottom:
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case VerticalAlignment::Center:
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	default:
		break;
	}
}

void TextRenderer::SetHorizontalAlignment(HorizontlAlignment align)
{
	m_horizontalAlignment = align;

	switch (align)
	{
	case HorizontlAlignment::Left:
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;

	case HorizontlAlignment::Right:
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;

	case HorizontlAlignment::Center:
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;

	case HorizontlAlignment::Justified:
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		break;

	default:
		break;
	}
	
}

Microsoft::WRL::ComPtr<IDWriteTextFormat> TextRenderer::GetTextFormat()
{
	return m_textFormat;
}

std::wstring TextRenderer::GetText()
{
	return m_text;
}

D2D1_COLOR_F TextRenderer::GetColor()
{
	return m_color;
}

TextRenderer::SpaceType TextRenderer::GetSpaceType()
{
	return m_spaceType;
}

int TextRenderer::GetSortOrder()
{
	return m_sortOrder;
}

D2D1_POINT_2F TextRenderer::GetPoint()
{
	return m_point;
}

D2D1_SIZE_F TextRenderer::GetRectSize()
{
	return m_rectSize;
}

float TextRenderer::GetFontSize()
{
	return 0.0f;
}
