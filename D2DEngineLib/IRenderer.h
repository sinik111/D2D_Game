#pragma once

#include "Enum.h"

struct RenderContext
{
	Matrix3x2 unityMatrix;
	Matrix3x2 viewUnityMatrix;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> deviceContext;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidBrush;
};

class IRenderer
{
public:
	virtual void Render(const RenderContext& context) const = 0;
	virtual SpaceType GetSpaceType() const = 0;
	virtual int GetSortOrder() const = 0;
	virtual float GetY() const = 0;
};