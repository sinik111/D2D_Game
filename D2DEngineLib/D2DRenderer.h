#pragma once

#include "Matrix3x2.h"
#include "Enum.h"

class IRenderer;
class MyImGui;
class D2DRenderer
{
	//싱글톤 전용 인스턴스입니다.
	static D2DRenderer* m_Instance;
private:
	HWND m_hWnd;
	
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3d11Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;
	Microsoft::WRL::ComPtr<IDXGIDevice3> m_dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter3> m_dxgiAdapter;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// Batch
	Microsoft::WRL::ComPtr<ID2D1SpriteBatch> m_d2dSpriteBatch;

	// DWrite
	Microsoft::WRL::ComPtr<IDWriteFactory> m_dWriteFactory;

	// Brush
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_d2dSolidColorBrush;
	std::vector<std::vector<std::vector<IRenderer*>>> m_renderQueues;

	//ImGui
	std::vector<MyImGui*> m_ImGuiVector;

	UINT m_width;
	UINT m_height;

	Matrix3x2 m_unityMatrix;

public:
	HRESULT Initialize(HWND hWnd, UINT width, UINT height);
	void ShutDown();
	UINT GetWidth() const;
	UINT GetHeight() const;

public:
	void BeginDraw(const D2D1_COLOR_F& color) const;
	void EndDraw() const;

public:
	const Microsoft::WRL::ComPtr<ID2D1DeviceContext7>& GetDeviceContext() const;
	Matrix3x2 GetUnityMatrix() const;

public:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat(float fontSize);

public:
	void RegisterRendererToQueue(IRenderer* renderer);
	void ExecuteRenderQueue();

	void Trim();
	void DrawRect(const D2D1_RECT_F& rect, const D2D1_COLOR_F& color = { 1.0f, 0.0f, 0.0f, 1.0f });
	void DrawEllipse(const D2D1_ELLIPSE& ellipse, const D2D1_COLOR_F& color = { 1.0f, 0.0f, 0.0f, 1.0f });
	void DrawLine(const D2D1_POINT_2F& startPoint, const D2D1_POINT_2F& endPoint,
		const D2D1_COLOR_F& color = { 1.0f, 0.0f, 0.0f, 1.0f });
	void DrawTriangle(const D2D1_POINT_2F& p1, const D2D1_POINT_2F& p2, const D2D1_POINT_2F& p3,
		const D2D1_COLOR_F& color = { 1.0f, 0.0f, 0.0f, 1.0f });

private:
	void PrepareRenderQueue();
	void ClearQueue();

	//ImGui 그리기 시작, 그리기, 
public:
	void InitImGui();
	void UnInitImGui();

	static D2DRenderer* Get() { return m_Instance; };
	static void Set(D2DRenderer* instance) { m_Instance = instance; }
};