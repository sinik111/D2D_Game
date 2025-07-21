#pragma once

#include "Component.h"
#include "Enum.h"
#include "BitmapResource.h"
#include "IRenderer.h"

class BitmapRenderer :
	public Component, public IRenderer
{
private:
	std::shared_ptr<BitmapResource> m_bitmapResource;
	std::wstring m_filePath;
	Matrix3x2 m_cachedRenderMatrix;
	Matrix3x2 m_cachedRenderWorldMatrix;
	SpaceType m_spaceType = SpaceType::World;
	D2D1_RECT_F m_sourceRect{};
	D2D1_COLOR_F m_color{ D2D1::ColorF::White };
	Bounds m_bounds;
	Vector2 m_pivot{ 0.5f, 0.5f };
	int m_sortOrder = 0;
	float m_opacity = 1.0f;
	bool m_doFlipX = false;
	bool m_isBitmapDirty = true;
	bool m_isColored = false;

public:
	BitmapRenderer() = default;
	BitmapRenderer(const std::wstring& filePath);

public:
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

public:
	const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& GetBitmap() const;
	bool GetFlipX() const;
	D2D1_RECT_F GetSourceRect() const;
	float GetOpacity() const;
	Vector2 GetPivot() const;

	void Update() override;
	void Render(const RenderContext& context) const override;
	int GetSortOrder() const override;
	SpaceType GetSpaceType() const override;
	float GetY() const override;
	const Bounds& GetBounds() const override;

public:
	void SetBitmap(const std::wstring& filePath);
	void SetSortOrder(int sortOrder);
	void SetFlipX(bool doFlip);
	void SetSpaceType(SpaceType spaceType);
	void SetSourceRect(const D2D1_RECT_F& sourceRect);
	void SetPivot(const Vector2& pivot);
	void SetOpacity(float opacity);
	void SetColor(const D2D1_COLOR_F& color);
	void ResetColor();

private:
	void MakeRenderMatrix();
};