#pragma once

#include "Matrix3x2.h"

class D2DRenderer
{
public:
	enum class RenderCommandType
	{
		Bitmap,
		Text
	};

	class IRenderCommand
	{
	public:
		virtual ~IRenderCommand() = default;
		virtual RenderCommandType GetType() const = 0;
		virtual int GetSortOrder() const = 0;
	};

	struct BitmapRenderCommand :
		public IRenderCommand
	{
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
		Matrix3x2 transform;
		// 필요시 추가
		// float opacity;
		// D2D1_RECT_F sourceRect;
		// D2D1_RECT_F destinationRect;
		int sortOrder;

		BitmapRenderCommand(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap,
			const Matrix3x2& transform,
			int sortOrder = 0)
			: bitmap{ std::move(bitmap) }, transform{ transform }, sortOrder{ sortOrder }
		{

		}

		RenderCommandType GetType() const override
		{
			return RenderCommandType::Bitmap;
		}

		int GetSortOrder() const override
		{
			return sortOrder;
		}
	};

	struct TextRenderCommand :
		public IRenderCommand
	{
		int sortOrder = 0;

		RenderCommandType GetType() const override
		{
			return RenderCommandType::Bitmap;
		}

		int GetSortOrder() const override
		{
			return sortOrder;
		}
	};

private:
	HWND m_hWnd;
	
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3d11Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	std::vector<std::unique_ptr<IRenderCommand>> m_renderCommands;

	UINT m_width;
	UINT m_height;

	Matrix3x2 m_unityMatrix;

public:
	D2DRenderer(HWND hWnd, UINT width, UINT height);
	~D2DRenderer();

public:
	void Initialize();
	void Shutdown();

public:
	void BeginDraw(const D2D1::ColorF& color) const;
	void EndDraw() const;

public:
	const Microsoft::WRL::ComPtr<ID2D1DeviceContext7>& GetDeviceContext() const;
	Matrix3x2 GetUnityMatrix() const;

	void AddRenderCommand(std::unique_ptr<IRenderCommand> renderCommand);
	void ExecuteRenderCommands();

private:
	void PrepareRenderCommands();
	void ClearCommands();
};