#include "pch.h"
#include "BitmapRenderer.h"

#include "ComponentSystem.h"
#include "ResourceManager.h"
#include "Transform.h"

BitmapRenderer::BitmapRenderer(const std::wstring& filePath)
	: m_filePath{ filePath }
{
	m_bitmapResource = ResourceManager::Get().CreateBitmapResource(filePath);

	m_sourceRect = {
		0.0f,
		0.0f,
		m_bitmapResource->GetSize().width,
		m_bitmapResource->GetSize().height
	};
}

void BitmapRenderer::RegisterToSystem()
{
	ComponentSystem::Get().Renderer().Register(this);
}

void BitmapRenderer::UnregisterFromSystem()
{
	ComponentSystem::Get().Renderer().Unregister(this);
}

const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& BitmapRenderer::GetBitmap() const
{
	return m_bitmapResource->GetBitmap();
}

void BitmapRenderer::Update()
{
	if (m_isBitmapDirty || GetTransform()->IsDirtyThisFrame())
	{
		if (m_isBitmapDirty)
		{
			MakeRenderMatrix();
		}

		const Matrix3x2& worldMatrix = GetTransform()->GetWorldMatrix();

		m_cachedRenderWorldMatrix = m_cachedRenderMatrix * worldMatrix;

		const float bitmapWidth = m_sourceRect.right - m_sourceRect.left;
		const float bitmapHeight = m_sourceRect.bottom - m_sourceRect.top;

		D2D1_RECT_F localRect{
			-bitmapWidth * m_pivot.x,
			bitmapHeight * m_pivot.y,
			bitmapWidth * (1.0f - m_pivot.x),
			-bitmapHeight * (1.0f - m_pivot.y)
		};

		m_bounds = Bounds::RectToWorldBounds(localRect, worldMatrix);

		m_isBitmapDirty = false;
	}
}

void BitmapRenderer::Render(const RenderContext& context) const // todo : renderFlag 처리 함수로 빼기
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

	D2D1_RECT_F destinationRect{
		0.0f,
		0.0f,
		m_sourceRect.right - m_sourceRect.left,
		m_sourceRect.bottom - m_sourceRect.top
	};

	D2D1_RECT_F sourceRect = m_sourceRect;

	if (m_renderFlag & RenderFlag::FLOATING)
	{
		D2D1_RECT_F firstSourceRect = sourceRect;
		D2D1_RECT_F secondSourceRect = sourceRect;
		D2D1_RECT_F firstDestinationRect = destinationRect;
		D2D1_RECT_F secondDestinationRect = destinationRect;

		if (m_renderFlag & RenderFlag::FILLING)
		{
			switch (m_floatingDirectionType)
			{
			case BitmapDirectionType::RightToLeft:
			{
				float width = m_sourceRect.right - m_sourceRect.left;
				float floatingWidth = width * m_floatingPosition;

				firstSourceRect.left = firstSourceRect.left + floatingWidth;
				firstDestinationRect.right = firstDestinationRect.right - floatingWidth;
				secondSourceRect.right = secondSourceRect.right - (width - floatingWidth);
				secondDestinationRect.left = secondDestinationRect.left + (width - floatingWidth);
			}
				break;

			case BitmapDirectionType::LeftToRight:
				// todo : 나머지 방향들도 구현 필요
				break;

			case BitmapDirectionType::BottomToTop:
			{
				float height = m_sourceRect.bottom - m_sourceRect.top;
				float floatingHeight = height * m_floatingPosition;

				firstSourceRect.top = firstSourceRect.top + floatingHeight;
				firstDestinationRect.bottom = firstDestinationRect.bottom - floatingHeight;
				secondSourceRect.bottom = secondSourceRect.bottom - (height - floatingHeight);
				secondDestinationRect.top = secondDestinationRect.top + (height - floatingHeight);
			}
				break;

			case BitmapDirectionType::TopToBottom:
				break;
			}

			switch (m_fillDirectionType)
			{
			case BitmapDirectionType::RightToLeft:
			{
				float width = m_sourceRect.right - m_sourceRect.left;
				float fillWidth = width * m_fillRatio;
				float floatingWidth = width * m_floatingPosition;

				if (secondDestinationRect.left <= fillWidth)
				{
					secondDestinationRect.right -= (width - fillWidth);
					secondSourceRect.right -= (width - fillWidth);
				}
				else if (secondDestinationRect.left > fillWidth)
				{
					secondDestinationRect.left = 0.0f;
					secondDestinationRect.right = 0.0f;
					secondSourceRect.left = 0.0f;
					secondSourceRect.right = 0.0f;

					firstDestinationRect.right = fillWidth;
					firstSourceRect.right -= ((width - floatingWidth) - fillWidth);
				}
			}
				break;

			case BitmapDirectionType::LeftToRight:
				
				break;

			case BitmapDirectionType::BottomToTop:
			{
				float height = m_sourceRect.bottom - m_sourceRect.top;
				float fillheight = height * m_fillRatio;
				float floatingHeight = height * m_floatingPosition;

				if (secondDestinationRect.top <= fillheight)
				{
					secondDestinationRect.bottom -= (height - fillheight);
					secondSourceRect.bottom -= (height - fillheight);
				}
				else if (secondDestinationRect.top > fillheight)
				{
					secondDestinationRect.top = 0.0f;
					secondDestinationRect.bottom = 0.0f;
					secondSourceRect.top = 0.0f;
					secondSourceRect.bottom = 0.0f;

					firstDestinationRect.bottom = fillheight;
					firstSourceRect.bottom -= ((height - floatingHeight) - fillheight);
				}
			}
				break;

			case BitmapDirectionType::TopToBottom:
				
				break;
			}
		}
		else
		{
			switch (m_floatingDirectionType)
			{
			case BitmapDirectionType::RightToLeft:
			{
				float width = m_sourceRect.right - m_sourceRect.left;
				float floatingWidth = width * m_floatingPosition;

				firstSourceRect.left = firstSourceRect.left + floatingWidth;
				firstDestinationRect.right = firstDestinationRect.right - floatingWidth;
				secondSourceRect.right = secondSourceRect.right - (width - floatingWidth);
				secondDestinationRect.left = secondDestinationRect.left + (width - floatingWidth);
			}
				break;

			case BitmapDirectionType::LeftToRight:
				break;

			case BitmapDirectionType::BottomToTop:
			{
				float height = m_sourceRect.bottom - m_sourceRect.top;
				float floatingHeight = height * m_floatingPosition;

				firstSourceRect.top = firstSourceRect.top + floatingHeight;
				firstDestinationRect.bottom = firstDestinationRect.bottom - floatingHeight;
				secondSourceRect.bottom = secondSourceRect.bottom - (height - floatingHeight);
				secondDestinationRect.top = secondDestinationRect.top + (height - floatingHeight);
			}
				break;

			case BitmapDirectionType::TopToBottom:
				break;
			}
		}

		context.deviceContext->SetTransform(Matrix3x2::Identity());
		context.deviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		context.spriteBatch->Clear();

		D2D1_RECT_U firstSourceRectU{
			static_cast<UINT32>(firstSourceRect.left),
			static_cast<UINT32>(firstSourceRect.top),
			static_cast<UINT32>(firstSourceRect.right),
			static_cast<UINT32>(firstSourceRect.bottom),
		};

		D2D1_RECT_U secondSourceRectU{
			static_cast<UINT32>(secondSourceRect.left),
			static_cast<UINT32>(secondSourceRect.top),
			static_cast<UINT32>(secondSourceRect.right),
			static_cast<UINT32>(secondSourceRect.bottom),
		};

		context.spriteBatch->AddSprites(1, &firstDestinationRect, &firstSourceRectU, &m_color, &finalMatrix);
		context.spriteBatch->AddSprites(1, &secondDestinationRect, &secondSourceRectU, &m_color, &finalMatrix);
		context.deviceContext->DrawSpriteBatch(
			context.spriteBatch.Get(),
			m_bitmapResource->GetBitmap().Get()
		);

		context.deviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
	else if (m_renderFlag & RenderFlag::FILLING)
	{
		switch (m_fillDirectionType)
		{
		case BitmapDirectionType::RightToLeft:
			destinationRect.right *= m_fillRatio;
			sourceRect.right *= m_fillRatio;
			break;

		case BitmapDirectionType::LeftToRight:
			destinationRect.left = destinationRect.right * (1.0f - m_fillRatio);
			sourceRect.left = sourceRect.right * (1.0f - m_fillRatio);
			break;

		case BitmapDirectionType::BottomToTop:
			destinationRect.bottom *= m_fillRatio;
			sourceRect.bottom *= m_fillRatio;
			break;

		case BitmapDirectionType::TopToBottom:
			destinationRect.top = destinationRect.bottom * (1.0f - m_fillRatio);
			sourceRect.top = sourceRect.bottom * (1.0f - m_fillRatio);
			break;
		}
	}

	if ((m_renderFlag & (RenderFlag::COLORED | RenderFlag::FLOATING)) == 0)
	{
		context.deviceContext->SetTransform(finalMatrix);
		context.deviceContext->DrawBitmap(
			m_bitmapResource->GetBitmap().Get(),
			&destinationRect,
			m_opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			&sourceRect
		);
	}
	else if ((m_renderFlag & RenderFlag::FLOATING) == 0)
	{
		context.deviceContext->SetTransform(Matrix3x2::Identity());
		context.deviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		context.spriteBatch->Clear();

		D2D1_RECT_U sourceRectU{
			static_cast<UINT32>(sourceRect.left),
			static_cast<UINT32>(sourceRect.top),
			static_cast<UINT32>(sourceRect.right),
			static_cast<UINT32>(sourceRect.bottom),
		};

		context.spriteBatch->AddSprites(1, &destinationRect, &sourceRectU, &m_color, &finalMatrix);
		context.deviceContext->DrawSpriteBatch(
			context.spriteBatch.Get(),
			m_bitmapResource->GetBitmap().Get()
		);

		context.deviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
}

int BitmapRenderer::GetSortOrder() const
{
	return m_sortOrder;
}

bool BitmapRenderer::GetFlipX() const
{
	return m_doFlipX;
}

SpaceType BitmapRenderer::GetSpaceType() const
{
	return m_spaceType;
}

float BitmapRenderer::GetY() const
{
	return GetTransform()->GetWorldPosition().y;
}

Vector2 BitmapRenderer::GetPivot() const
{
	return m_pivot;
}

const Bounds& BitmapRenderer::GetBounds() const
{
	return m_bounds;
}

D2D1_RECT_F BitmapRenderer::GetSourceRect() const
{
	return m_sourceRect;
}

float BitmapRenderer::GetOpacity() const
{
	return m_opacity;
}

void BitmapRenderer::SetBitmap(const std::wstring& filePath)
{
	if (m_filePath == filePath)
	{
		return;
	}

	m_filePath = filePath;

	m_bitmapResource = ResourceManager::Get().CreateBitmapResource(filePath);

	m_sourceRect = { 0.0f, 0.0f, m_bitmapResource->GetSize().width, m_bitmapResource->GetSize().height };

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetBitmap(std::shared_ptr<BitmapResource> bitmapResource)
{
	if (m_bitmapResource == bitmapResource)
	{
		return;
	}

	m_bitmapResource = bitmapResource;

	m_sourceRect = { 0.0f, 0.0f, m_bitmapResource->GetSize().width, m_bitmapResource->GetSize().height };

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetSortOrder(int sortOrder)
{
	m_sortOrder = sortOrder;
}

void BitmapRenderer::SetFlipX(bool doFlip)
{
	m_doFlipX = doFlip;

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetSpaceType(SpaceType spaceType)
{
	m_spaceType = spaceType;
}

void BitmapRenderer::SetSourceRect(const D2D1_RECT_F& sourceRect)
{
	m_sourceRect = sourceRect;

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetPivot(const Vector2& pivot)
{
	m_pivot = pivot;

	m_isBitmapDirty = true;
}

void BitmapRenderer::SetOpacity(float opacity)
{
	m_opacity = opacity;
}

void BitmapRenderer::SetColor(const D2D1_COLOR_F& color)
{
	m_color = color;

	m_renderFlag |= RenderFlag::COLORED;
}

void BitmapRenderer::SetFill(BitmapDirectionType type, float ratio)
{
	m_fillDirectionType = type;
	m_fillRatio = MyMath::Clamp(ratio, 0.0f, 1.0f);

	m_renderFlag |= RenderFlag::FILLING;
}

void BitmapRenderer::SetFloating(BitmapDirectionType type, float position)
{
	m_floatingDirectionType = type;
	m_floatingPosition = MyMath::Clamp(position, 0.0f, 1.0f);

	m_renderFlag |= RenderFlag::FLOATING;
}

void BitmapRenderer::ResetColor()
{
	m_color = D2D1::ColorF(D2D1::ColorF::White);

	m_renderFlag &= ~RenderFlag::COLORED;
}

void BitmapRenderer::MakeRenderMatrix()
{
	if (m_doFlipX)
	{
		m_cachedRenderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
			Matrix3x2::Translation(-(m_sourceRect.right - m_sourceRect.left) * m_pivot.x,
				(m_sourceRect.bottom - m_sourceRect.top) * m_pivot.y) *
			Matrix3x2::Scale(-1.0f, 1.0f);
	}
	else
	{
		m_cachedRenderMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
			Matrix3x2::Translation(-(m_sourceRect.right - m_sourceRect.left) * m_pivot.x,
				(m_sourceRect.bottom - m_sourceRect.top) * m_pivot.y);
	}
}