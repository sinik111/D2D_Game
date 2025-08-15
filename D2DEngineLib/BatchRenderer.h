#pragma once

#include "Component.h"
#include "Enum.h"
#include "IRenderer.h"
#include "BitmapResource.h"
#include "SpriteData.h"

struct BatchUnit
{
	size_t index = 0;
	Vector2 position{ 0.0f, 0.0f };
	Vector2 scale{ 1.0f, 1.0f };
	float rotation = 0.0f;
	D2D1_COLOR_F color{ 1.0f, 1.0f, 1.0f, 1.0f };
	Matrix3x2 world;
	bool isFollowing = true;

	BatchUnit() = default;
	BatchUnit(size_t index, const Vector2& position, const Vector2& scale,
		float rotation, const D2D1_COLOR_F& color)
		: index{ index }, position{ position }, scale{ scale }, rotation{ rotation }, color{ color }
	{

	}

	static BatchUnit Lerp(const BatchUnit& b0, const BatchUnit& b1, float t)
	{
		BatchUnit result;

		result.position = Vector2::Lerp(b0.position, b1.position, t);
		result.scale = Vector2::Lerp(b0.scale, b1.scale, t);
		result.rotation = MyMath::Lerp(b0.rotation, b1.rotation, t);
		result.color.r = MyMath::Lerp(b0.color.r, b1.color.r, t);
		result.color.g = MyMath::Lerp(b0.color.g, b1.color.g, t);
		result.color.b = MyMath::Lerp(b0.color.b, b1.color.b, t);
		result.color.a = MyMath::Lerp(b0.color.a, b1.color.a, t);

		return result;
	}
};

class BatchRenderer :
	public Component, public IRenderer
{
private:
	std::shared_ptr<BitmapResource> m_bitmapResource;
	std::shared_ptr<SpriteSheet> m_spriteSheets;
	std::vector<BatchUnit> m_batchUnitList;

	D2D1_RECT_F m_localRect{ -50.0f, -50.0f, 50.0f, 50.0f };
	SpaceType m_spaceType = SpaceType::World;
	Bounds m_bounds;
	int m_sortOrder = 0;

public:
	BatchRenderer() = default;
	BatchRenderer(const std::wstring& bitmapPath, const std::wstring& sheetPath);

public:
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

public:
	void Update() override;
	void Render(const RenderContext& context) const override;
	int GetSortOrder() const override;
	SpaceType GetSpaceType() const override;
	float GetY() const override;
	const Bounds& GetBounds() const override;

	size_t GetBatchCount() const;

public:
	void SetBitmap(const std::wstring& filePath);
	void SetSpriteSheet(const std::wstring& filePath);
	void SetSpaceType(SpaceType spaceType);
	void SetLocalRect(const D2D1_RECT_F& rect);
	void SetSortOrder(int sortOrder);

	void AddBatchUnit(const BatchUnit& batchUnit);
	void ClearBatchUnitList();
};