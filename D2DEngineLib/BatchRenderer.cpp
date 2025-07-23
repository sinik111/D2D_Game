#include "pch.h"
#include "BatchRenderer.h"

#include "ComponentSystem.h"
#include "ResourceManager.h"
#include "Transform.h"

BatchRenderer::BatchRenderer(const std::wstring& bitmapPath, const std::wstring& sheetPath)
{
    m_bitmapResource = ResourceManager::Get().CreateBitmapResource(bitmapPath);
    m_spriteSheet = ResourceManager::Get().CreateSpriteSheet(sheetPath);
}

void BatchRenderer::RegisterToSystem()
{
    ComponentSystem::Get().Renderer().Register(this);
}

void BatchRenderer::UnregisterFromSystem()
{
    ComponentSystem::Get().Renderer().Unregister(this);
}

void BatchRenderer::Update()
{
    if (GetTransform()->IsDirtyThisFrame())
    {
        m_bounds = Bounds::RectToWorldBounds(m_localRect, GetTransform()->GetWorldMatrix());
    }
}

void BatchRenderer::Render(const RenderContext& context) const
{
	if (!m_batchUnitList.empty())
	{
		const Matrix3x2& worldMatrix = GetTransform()->GetWorldMatrix();
		const Matrix3x2 worldUnityMatrix = worldMatrix * context.unityMatrix;
		const Matrix3x2 worldViewUnityMatrix = worldMatrix * context.viewUnityMatrix;

		context.deviceContext->SetTransform(Matrix3x2::Identity());
		context.deviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		context.spriteBatch->Clear();

		for (const auto& batchUnit : m_batchUnitList)
		{
			const Sprite& sprite = m_spriteSheet->sprites[batchUnit.index];

			D2D1_RECT_U sourceRectU{
				static_cast<UINT32>(sprite.x),
				static_cast<UINT32>(sprite.y),
				static_cast<UINT32>(sprite.x + sprite.width),
				static_cast<UINT32>(sprite.y + sprite.height),
			};

			D2D1_RECT_F destinationRect{
				0.0f,
				0.0f,
				sprite.width,
				sprite.width
			};

			Matrix3x2 finalMatrix;

			switch (m_spaceType)
			{
			case SpaceType::Screen:
				if (!batchUnit.isFollowing)
				{
					finalMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
						Matrix3x2::Translation(-sprite.width * sprite.pivotX, sprite.height * sprite.pivotY) *
						Matrix3x2::Scale(batchUnit.scale) *
						Matrix3x2::Rotation(batchUnit.rotation) *
						Matrix3x2::Translation(batchUnit.position) *
						batchUnit.world * context.unityMatrix;
				}
				else
				{
					finalMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
						Matrix3x2::Translation(-sprite.width * sprite.pivotX, sprite.height * sprite.pivotY) *
						Matrix3x2::Scale(batchUnit.scale) *
						Matrix3x2::Rotation(batchUnit.rotation) *
						Matrix3x2::Translation(batchUnit.position) *
						worldUnityMatrix;
				}
				break;

			case SpaceType::World:
				if (!batchUnit.isFollowing)
				{
					finalMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
						Matrix3x2::Translation(-sprite.width * sprite.pivotX, sprite.height * sprite.pivotY) *
						Matrix3x2::Scale(batchUnit.scale) *
						Matrix3x2::Rotation(batchUnit.rotation) *
						Matrix3x2::Translation(batchUnit.position) *
						batchUnit.world * context.viewUnityMatrix;
				}
				else
				{
					finalMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
						Matrix3x2::Translation(-sprite.width * sprite.pivotX, sprite.height * sprite.pivotY) *
						Matrix3x2::Scale(batchUnit.scale) *
						Matrix3x2::Rotation(batchUnit.rotation) *
						Matrix3x2::Translation(batchUnit.position) *
						worldViewUnityMatrix;
				}
				break;
			}

			context.spriteBatch->AddSprites(1, &destinationRect, &sourceRectU, &batchUnit.color, &finalMatrix);
		}

		context.deviceContext->DrawSpriteBatch(
			context.spriteBatch.Get(),
			m_bitmapResource->GetBitmap().Get()
		);

		context.deviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
}

int BatchRenderer::GetSortOrder() const
{
    return m_sortOrder;
}

SpaceType BatchRenderer::GetSpaceType() const
{
    return m_spaceType;
}

float BatchRenderer::GetY() const
{
    return GetTransform()->GetWorldPosition().y;
}

const Bounds& BatchRenderer::GetBounds() const
{
    return m_bounds;
}

size_t BatchRenderer::GetBatchCount() const
{
	return m_batchUnitList.size();
}

void BatchRenderer::SetBitmap(const std::wstring& filePath)
{
    m_bitmapResource = ResourceManager::Get().CreateBitmapResource(filePath);
}

void BatchRenderer::SetSpriteSheet(const std::wstring& filePath)
{

    m_spriteSheet = ResourceManager::Get().CreateSpriteSheet(filePath);
}

void BatchRenderer::SetSpaceType(SpaceType spaceType)
{
    m_spaceType = spaceType;
}

void BatchRenderer::SetLocalRect(const D2D1_RECT_F& rect)
{
    m_localRect = rect;

    m_bounds = Bounds::RectToWorldBounds(m_localRect, GetTransform()->GetWorldMatrix());
}

void BatchRenderer::SetSortOrder(int sortOrder)
{
	m_sortOrder = sortOrder;
}

void BatchRenderer::AddBatchUnit(const BatchUnit& batchUnit)
{
	m_batchUnitList.push_back(batchUnit);
}

void BatchRenderer::ClearBatchUnitList()
{
	m_batchUnitList.clear();
}