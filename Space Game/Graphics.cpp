#include <Windows.h>
#include <d2d1_1.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <cassert>
#include <dwrite.h>
#include <wrl/client.h>
#include <mutex>
#include <wincodec.h>
#include "Game.h"
#include "Graphics.h"

using namespace Microsoft::WRL;

namespace Graphics {

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch Win32 API errors.
			ExitProcess(0);
		}
	}

	ComPtr<ID2D1Factory1> m_d2dFactory;
	D3D_FEATURE_LEVEL m_featureLevel;
	ComPtr<ID2D1Device> m_d2dDevice;
	ComPtr<ID2D1DeviceContext> m_d2dContext;

	ComPtr<IDXGISwapChain1> m_swapChain;
	ComPtr<ID2D1Bitmap1> m_d2dTargetBitmap;

	static ComPtr<IDWriteFactory> iWriteFactory;
	static ComPtr<ID2D1SolidColorBrush> iSolidColourBrush;

	std::mutex mGraphics;

	bool init = false;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	ID2D1Bitmap1* iLightingBitmap;
	ID2D1DeviceContext* iLightingDeviceContext;

	HWND hWindow;

	D2D1::ColorF clrWhite = D2D1::ColorF(1.0f, 1.0f, 1.0f);

	bool Initialise(HWND window)
	{
		mGraphics.lock();
		hWindow = window;

		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), (void**)&m_d2dFactory);

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		ThrowIfFailed(
			D3D11CreateDevice(
				nullptr,                    // specify null to use the default adapter
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				creationFlags,              // optionally set debug and Direct2D compatibility flags
				featureLevels,              // list of feature levels this app can support
				ARRAYSIZE(featureLevels),   // number of possible feature levels
				D3D11_SDK_VERSION,
				&device,                    // returns the Direct3D device created
				&m_featureLevel,            // returns feature level of device created
				&context                    // returns the device immediate context
			)
		);

		ComPtr<IDXGIDevice> dxgiDevice;

		ThrowIfFailed(
			device.As(&dxgiDevice)
		);

		ThrowIfFailed(m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice));

		ThrowIfFailed(
			m_d2dDevice->CreateDeviceContext(
				D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				&m_d2dContext
			)
		);

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		swapChainDesc.Width = 0;                           // use automatic sizing
		swapChainDesc.Height = 0;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
		swapChainDesc.Flags = 0;

		ComPtr<IDXGIAdapter> dxgiAdapter;
		ThrowIfFailed(
			dxgiDevice->GetAdapter(&dxgiAdapter)
		);

		// Get the factory object that created the DXGI device.
		ComPtr<IDXGIFactory2> dxgiFactory;

		ThrowIfFailed(
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
		);

		// Get the final swap chain for this window from the DXGI factory.
		ThrowIfFailed(
			dxgiFactory->CreateSwapChainForHwnd(
				device.Get(),
				hWindow,
				&swapChainDesc,
				nullptr,
				nullptr,
				&m_swapChain
			)
		);

		//dxgiFactory->MakeWindowAssociation(hWindow, 0);

		// Get the backbuffer for this window which is be the final 3D render target.
		ComPtr<ID3D11Texture2D> backBuffer;

		ThrowIfFailed(
			m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
		);

		FLOAT m_dpi = GetDpiForWindow(hWindow);

		// Now we set up the Direct2D render target bitmap linked to the swapchain. 
		// Whenever we render to this bitmap, it is directly rendered to the 
		// swap chain associated with the window.
		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED/*D2D1_ALPHA_MODE_IGNORE*/),
				m_dpi,
				m_dpi
			);

		// Direct2D needs the dxgi version of the backbuffer surface pointer.

		ComPtr<IDXGISurface> dxgiBackBuffer;

		ThrowIfFailed(
			m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
		);

		// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
		ThrowIfFailed(
			m_d2dContext->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer.Get(),
				&bitmapProperties,
				&m_d2dTargetBitmap
			)
		);

		// Now we can set the Direct2D render target.
		m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

		m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &iSolidColourBrush);

		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&iWriteFactory);

		RECT rClientRect;
		GetClientRect(hWindow, &rClientRect);

		HRESULT hr = m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &iLightingDeviceContext);

		D2D1_SIZE_U size;
		size.width = rClientRect.right;
		size.height = rClientRect.bottom;

		D2D1_BITMAP_PROPERTIES1 properties =
			D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED/*D2D1_ALPHA_MODE_IGNORE*/), m_dpi, m_dpi);

		hr = iLightingDeviceContext->CreateBitmap(size, nullptr, 0, properties, &iLightingBitmap);

		iLightingDeviceContext->SetTarget(iLightingBitmap);

		iLightingDeviceContext->BeginDraw();
		iLightingDeviceContext->Clear(); //TODO check if automatically clears
		hr = iLightingDeviceContext->EndDraw();

		init = true;
		mGraphics.unlock();

		return true;
	}

	void Close()
	{
		//TODO cleanup
	}

	void BeginDraw()
	{
		mGraphics.lock();
		m_d2dContext->BeginDraw();
		
		//iLightingDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	}

	void EndDraw()
	{
		m_d2dContext->EndDraw();
		m_swapChain->Present(1, 0); //m_swapChain->Present(1, 0);
		mGraphics.unlock();
	}

	void Clear(D2D1_COLOR_F& colour)
	{
		m_d2dContext->Clear(colour);
	}

	void WriteText(const wchar_t* text, int x, int y, float size, const D2D1::ColorF& clrColour, float fOpacity, const wchar_t* font)
	{
		if (size == 0.0f) return;
		if (font == nullptr) font = L"Bahnschrift";
		IDWriteTextFormat* iTextFormat;
		HRESULT hr = iWriteFactory->CreateTextFormat(font, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,  size, L"en-US", &iTextFormat);
		assert(SUCCEEDED(hr));

		iSolidColourBrush->SetColor(clrColour);
		iSolidColourBrush->SetOpacity(fOpacity);

		RECT rClientRect;
		GetClientRect(hWindow, &rClientRect);

		m_d2dContext->DrawTextW(text, lstrlenW(text), iTextFormat, D2D1::RectF( x,  y,  rClientRect.right,  rClientRect.bottom), iSolidColourBrush.Get());
		iSolidColourBrush->SetOpacity(fOpacity);
		iTextFormat->Release();
	}

	void TextMetrics(const wchar_t* text, float size, DWRITE_TEXT_METRICS& textmetrics, const wchar_t* font)
	{
		if (size == 0.0f) return;
		if (font == nullptr) font = L"Bahnschrift";
		IDWriteTextFormat* iTextFormat;
		HRESULT hr = iWriteFactory->CreateTextFormat(font, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, size, L"en-US", &iTextFormat);
		assert(SUCCEEDED(hr));

		RECT rClientRect;
		bool b = GetClientRect(hWindow, &rClientRect);
		assert(b);

		IDWriteTextLayout* iTextLayout;
		hr = iWriteFactory->CreateTextLayout(text, lstrlen(text), iTextFormat, rClientRect.right, rClientRect.bottom, &iTextLayout);
		assert(SUCCEEDED(hr));

		hr = iTextLayout->GetMetrics(&textmetrics);
		assert(SUCCEEDED(hr));

		iTextLayout->Release();
		iTextFormat->Release();
	}
	
	void DrawRectangle(float fX, float fY, float fW, float fH, D2D1::ColorF& cColour, float fOpacity, float thickness)
	{
		iSolidColourBrush->SetColor(cColour);
		iSolidColourBrush->SetOpacity(fOpacity);
		D2D1_RECT_F rRect = D2D1::RectF( fX,  fY,  (fX + fW),  (fY + fH));
		m_d2dContext->DrawRectangle(rRect, iSolidColourBrush.Get(), thickness);
		iSolidColourBrush->SetOpacity(1.0f);
	}

	void FillRectangle(float fX, float fY, float fW, float fH, D2D1::ColorF& cColour, float fOpacity)
	{
		iSolidColourBrush->SetColor(cColour);
		iSolidColourBrush->SetOpacity(fOpacity);
		D2D1_RECT_F rRect = D2D1::RectF( fX,  fY,  (fX + fW),  (fY + fH));
		m_d2dContext->FillRectangle(rRect, iSolidColourBrush.Get());
		iSolidColourBrush->SetOpacity(1.0f);
	}

	void Resize()
	{
		if (init)
		{
			RECT rcClientRect;
			GetClientRect(hWindow, &rcClientRect);
			nScreenWidth = rcClientRect.right;
			nScreenHeight = rcClientRect.bottom;

			m_d2dTargetBitmap.Reset();
			m_d2dContext.Reset();

			context->ClearState();

			HRESULT hr = m_swapChain->ResizeBuffers(0, nScreenWidth, nScreenHeight, DXGI_FORMAT_UNKNOWN, 0);

			m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext);

			ComPtr<IDXGISurface> dxgiBackBuffer;
			hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

			float m_dpi = GetDpiForWindow(hWindow);

			D2D1_BITMAP_PROPERTIES1 bitmapProperties =
				D2D1::BitmapProperties1(
					D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
					m_dpi,
					m_dpi
				);

			ComPtr<ID3D11Texture2D> backBuffer;

			hr = m_d2dContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(), bitmapProperties, &m_d2dTargetBitmap);

			m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

			fScaleV = (float)nScreenHeight / 720.0f;
			fScaleH = (float)nScreenWidth / 1280.0f;


			RECT rClientRect;
			GetClientRect(hWindow, &rClientRect);

			D2D1_SIZE_U size;
			size.width = rClientRect.right;
			size.height = rClientRect.bottom;

			D2D1_BITMAP_PROPERTIES1 properties =
				D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED/*D2D1_ALPHA_MODE_IGNORE*/), m_dpi, m_dpi);
			iLightingBitmap->Release();
			hr = iLightingDeviceContext->CreateBitmap(size, nullptr, 0, properties, &iLightingBitmap);
			iLightingDeviceContext->SetTarget(iLightingBitmap);		
		}
	}

	void DrawLighting()
	{
		
		ComPtr<ID2D1Effect> effect;
		HRESULT hr = m_d2dContext->CreateEffect(CLSID_D2D1Flood, &effect);
		hr = effect->SetValue(D2D1_FLOOD_PROP_COLOR, D2D1::Vector4F(0.0f, 0.0f, 0.0f, 1.0f));
		iLightingDeviceContext->DrawImage(effect.Get(), D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1_COMPOSITE_MODE_XOR);

		hr = iLightingDeviceContext->EndDraw();

		m_d2dContext->DrawBitmap(iLightingBitmap);
	}

}