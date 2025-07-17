#include "pch.h"
#include "TextRenderer.h"

#include "ComponentSystem.h"
#include "Transform.h"

TextRenderer::TextRenderer()
{
	m_textFormat = ComponentSystem::Get().Renderer().CreateTextFormat(m_fontSize);
}

void TextRenderer::RegisterToSystem()
{
	ComponentSystem::Get().Renderer().Register(this);
}

void TextRenderer::UnregisterFromSystem()
{
	ComponentSystem::Get().Renderer().Unregister(this);
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

	m_isTextDirty = true;
}

void TextRenderer::SetFontSize(float size)
{
	m_fontSize = size;

	m_textFormat = ComponentSystem::Get().Renderer().CreateTextFormat(m_fontSize);

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

void TextRenderer::SetHorizontalAlignment(HorizontalAlignment align)
{
	m_horizontalAlignment = align;

	switch (align)
	{
	case HorizontalAlignment::Left:
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;

	case HorizontalAlignment::Right:
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;

	case HorizontalAlignment::Center:
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;

	case HorizontalAlignment::Justified:
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		break;

	default:
		break;
	}
	
}

void TextRenderer::SetPivot(const Vector2& pivot)
{
	m_pivot = pivot;

	m_isTextDirty = true;
}

void TextRenderer::MakeRenderMatrix()
{
	m_cachedRenderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
		Matrix3x2::Translation(-m_rectSize.width * m_pivot.x,
			m_rectSize.height * (1.0f - m_pivot.y));
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

void TextRenderer::Update()
{
	if (m_isTextDirty || GetTransform()->GetIsDirtyThisFrame())
	{
		if (m_isTextDirty)
		{
			MakeRenderMatrix();
		}

		const Matrix3x2& worldMatrix = GetTransform()->GetWorldMatrix();

		m_cachedRenderWorldMatrix = m_cachedRenderMatrix * worldMatrix;

		D2D1_RECT_F localRect{
			-m_rectSize.width * m_pivot.x,
			m_rectSize.height * (1.0f - m_pivot.y),
			m_rectSize.width * (1.0f - m_pivot.x),
			-m_rectSize.height * m_pivot.y
		};

		m_bounds = Bounds::RectToWorldBounds(localRect, worldMatrix);

		m_isTextDirty = false;
	}
}

void TextRenderer::Render(const RenderContext& context) const
{
	Matrix3x2 finalMatrix;

	switch (m_spaceType)
	{
	case SpaceType::Screen:
		finalMatrix = m_cachedRenderWorldMatrix * context.unityMatrix;
		break;

	case SpaceType::World:
		finalMatrix = m_cachedRenderWorldMatrix * context.viewUnityMatrix;
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

Vector2 TextRenderer::GetPivot() const
{
	return m_pivot;
}

const Bounds& TextRenderer::GetBounds() const
{
	return m_bounds;
}