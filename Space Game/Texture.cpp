#include "Texture.h"
#include "Game.h"
#include <d2d1_1.h>
#include <d2d1svg.h>

Texture::Texture(const wchar_t* filename)
{
	

	iBitmap = nullptr;

	IWICImagingFactory* iWicFactory = nullptr; //Used for windows imaging components
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&iWicFactory);
	assert(SUCCEEDED(hr));

	IWICBitmapDecoder* iWicDecoder = nullptr; //Create bitmap decoder
	hr = iWicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &iWicDecoder);
	assert(SUCCEEDED(hr));

	IWICBitmapFrameDecode* iWicFrame = nullptr; //Get WIC frame from image
	hr = iWicDecoder->GetFrame(0, &iWicFrame);
	assert(SUCCEEDED(hr));
	
	/*if (fTextureDrawnWidth != 0 && fTextureDrawnHeight != 0)
	{
		IWICBitmapScaler* iWicScaler;
		iWicFactory->CreateBitmapScaler(&iWicScaler);

		iWicScaler->Initialize(iWicFrame, fTextureDrawnWidth, fTextureDrawnHeight, WICBitmapInterpolationModeHighQualityCubic);
	}*/

	IWICFormatConverter* iWicConverter = nullptr; //Create converter to convert image to bitmap
	hr = iWicFactory->CreateFormatConverter(&iWicConverter);
	assert(SUCCEEDED(hr));
	
	//Initialise WIC converter
	hr = iWicConverter->Initialize(iWicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	assert(SUCCEEDED(hr));

	hr = Graphics::iRenderTarget->CreateBitmapFromWicBitmap(iWicConverter, NULL, &iBitmap);
	assert(SUCCEEDED(hr));

	if (iWicFactory) iWicFactory->Release();
	if (iWicDecoder) iWicDecoder->Release();
	if (iWicFrame) iWicFrame->Release();
	if (iWicConverter) iWicConverter->Release();

	nTextureWidth = iBitmap->GetSize().width;
	nTextureHeight = iBitmap->GetSize().height;
	fTextureDrawnHeight = nTextureHeight;
	fTextureDrawnWidth = nTextureWidth;
	nTexturesAcross = 0;
}
Texture::Texture(const wchar_t* filename, int nTextureWidth, int nTextureHeight, float fTextureDrawnWidth, float fTextureDrawnHeight)
	: Texture(filename)
{
	this->nTextureHeight = nTextureHeight;
	this->nTextureWidth = nTextureWidth;
	this->fTextureDrawnWidth = fTextureDrawnWidth;
	this->fTextureDrawnHeight = fTextureDrawnHeight;

	nTexturesAcross = iBitmap->GetSize().width / nTextureWidth;
}
Texture::~Texture()
{
	if (iBitmap) iBitmap->Release();
}

void Texture::Draw()
{
	Graphics::iRenderTarget->DrawBitmap(
		iBitmap,
		D2D1::RectF(0.0f, 0.0f, iBitmap->GetSize().width, iBitmap->GetSize().height),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(0.0f, 0.0f, iBitmap->GetSize().width, iBitmap->GetSize().height)
	);
}
void Texture::Draw(int index, float x, float y, bool bRealCoordinates)
{
	D2D_RECT_F rSrc;
	if (nTexturesAcross != 0) //We are using multiple textures in one file
	{
		rSrc = D2D1::RectF(
			(float)((index % nTexturesAcross) * nTextureWidth),
			(float)((index / nTexturesAcross) * nTextureHeight),
			(float)((index % nTexturesAcross) * nTextureWidth) + nTextureWidth,
			(float)((index / nTexturesAcross) * nTextureHeight) + nTextureHeight);
	}
	else
		rSrc = D2D1::RectF(0.0f, 0.0f, nTextureWidth, nTextureHeight);

	D2D_RECT_F rDest = D2D1::RectF(x, y, (x + fTextureDrawnWidth), (y + fTextureDrawnHeight)); //Calculate destination coordinates
	if (!bRealCoordinates)
	{
		rDest.top *= fScaleV;
		rDest.bottom *= fScaleV;
		rDest.left *= fScaleH;
		rDest.right *= fScaleH;
	}

	Graphics::iRenderTarget->DrawBitmap(iBitmap, rDest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rSrc); //Display bitmap
}
void Texture::DrawPanorama(int x)
{
	RECT rClientRect;
	GetClientRect(Graphics::hWindow, &rClientRect); //Get screen area

	D2D_RECT_F rSrc = D2D1::RectF(x, 0.0f, x + 1280, iBitmap->GetSize().height); //Get source coordinates
	D2D_RECT_F rDest = D2D1::RectF(rClientRect.left, rClientRect.top, rClientRect.right, rClientRect.bottom); //Get destination coordinates

	Graphics::iRenderTarget->DrawBitmap(iBitmap, rDest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rSrc);//Display bitmap
}