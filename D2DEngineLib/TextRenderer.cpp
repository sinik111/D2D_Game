#include "pch.h"
#include "TextRenderer.h"

#include "ComponentSystem.h"
#include "Transform.h"

TextRenderer::TextRenderer()
{
	ComponentSystem::Get().TextRenderer().Register(this);

	m_textFormat = ComponentSystem::Get().TextRenderer().CreateTextFormat(m_fontSize);
}

TextRenderer::~TextRenderer()
{
	ComponentSystem::Get().TextRenderer().Unregister(this);
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

	m_textFormat = ComponentSystem::Get().TextRenderer().CreateTextFormat(m_fontSize);

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

Microsoft::WRL::ComPtr<IDWriteTextFormat> TextRenderer::GetTextFormat() const
{
	return m_textFormat;
}

std::wstring TextRenderer::GetText() const
{
	return m_text;
}

D2D1_COLOR_F TextRenderer::GetColor() const
{
	return m_color;
}

void TextRenderer::Render(const RenderContext& context) const
{
	const Matrix3x2 worldMatrix = GetTransform()->GetWorldMatrix();
	const Matrix3x2 unityMatrix = context.unityMatrix;
	const Matrix3x2 viewUnityMatrix = context.viewUnityMatrix;

	Matrix3x2 finalMatrix;

	const D2D1_RECT_F layoutRect
	{
		m_point.x,
		-m_point.y,
		m_point.x + m_rectSize.width,
		-m_point.y + m_rectSize.height
	};

	switch (m_spaceType)
	{
	case SpaceType::Screen:
	{
		finalMatrix = Matrix3x2::Scale(1.0f, -1.0f) * unityMatrix;
	}
		break;
	case SpaceType::World:
	{
		finalMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
			Matrix3x2::Translation(-m_rectSize.width / 2.0f, m_rectSize.height / 2.0f) *
			GetTransform()->GetWorldMatrix() *
			viewUnityMatrix;
	}
		break;
	}

	context.solidBrush->SetColor(m_color);
	context.deviceContext->SetTransform(static_cast<D2D1_MATRIX_3X2_F>(finalMatrix));
	context.deviceContext->DrawTextW(
		m_text.c_str(),
		static_cast<UINT32>(m_text.size()),
		m_textFormat.Get(),
		layoutRect,
		context.solidBrush.Get());
}

SpaceType TextRenderer::GetSpaceType() const
{
	return m_spaceType;
}

int TextRenderer::GetSortOrder() const
{
	return m_sortOrder;
}

float TextRenderer::GetY() const
{
	return GetTransform()->GetWorldPosition().GetY();
}

D2D1_POINT_2F TextRenderer::GetPoint() const
{
	return m_point;
}

D2D1_SIZE_F TextRenderer::GetRectSize() const
{
	return m_rectSize;
}

float TextRenderer::GetFontSize() const
{
	return m_fontSize;
}
