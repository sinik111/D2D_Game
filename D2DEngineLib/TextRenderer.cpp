#include "pch.h"
#include "TextRenderer.h"

#include "ComponentSystem.h"
#include "Transform.h"

TextRenderer::TextRenderer()
{
	ComponentSystem::Get().TextRenderer().Register(this);

	m_textFormat = ComponentSystem::Get().TextRenderer().CreateTextFormat(m_fontSize);

	m_cachedRenderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
		Matrix3x2::Translation(-m_rectSize.width * m_pivot.x, m_rectSize.height * m_pivot.y);
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

void TextRenderer::SetRectSize(const D2D1_SIZE_F& size)
{
	m_rectSize = size;

	m_cachedRenderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
		Matrix3x2::Translation(-m_rectSize.width * m_pivot.x, m_rectSize.height * m_pivot.y);	
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

void TextRenderer::SetPivot(const Vector2& pivot)
{
	m_pivot = pivot;

	m_cachedRenderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
		Matrix3x2::Translation(-m_rectSize.width * m_pivot.x, m_rectSize.height * m_pivot.y);
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
	Vector2 position = GetTransform()->GetWorldPosition();

	Matrix3x2 finalMatrix;

	switch (m_spaceType)
	{
	case SpaceType::Screen:
		finalMatrix = m_cachedRenderMatrix *
			GetTransform()->GetWorldMatrix() *
			context.unityMatrix;
		break;

	case SpaceType::World:
		finalMatrix = m_cachedRenderMatrix *
			GetTransform()->GetWorldMatrix() *
			context.viewUnityMatrix;
		break;
	}

	context.solidBrush->SetColor(m_color);
	context.deviceContext->SetTransform(finalMatrix);
	context.deviceContext->DrawTextW(
		m_text.c_str(),
		static_cast<UINT32>(m_text.size()),
		m_textFormat.Get(),
		{ 0.0f, 0.0f, m_rectSize.width, m_rectSize.height },
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
	return GetTransform()->GetWorldPosition().y;
}

D2D1_SIZE_F TextRenderer::GetRectSize() const
{
	return m_rectSize;
}

float TextRenderer::GetFontSize() const
{
	return m_fontSize;
}
