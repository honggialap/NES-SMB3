#pragma once
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#pragma region INCLUDE
#include <d3d10.h>
#include <d3dx10.h>
#include <unordered_map>
#pragma endregion

struct CTexture
{
	ID3D10Texture2D* _texture;
	ID3D10ShaderResourceView* _rsview;

	int _width;
	int _height;

	CTexture(ID3D10Texture2D* texture, ID3D10ShaderResourceView* rsview);
	~CTexture();
};
typedef CTexture* pTexture;

class CGraphics
{
	/* Direct 3D */
private:
	ID3D10Device* _device = NULL;
	IDXGISwapChain* _swapChain = NULL;
	ID3D10RenderTargetView* _renderTargetView = NULL;
	ID3D10BlendState* _blendStateAlpha = NULL;
	ID3D10SamplerState* _pointSamplerState = NULL;
	ID3DX10Sprite* _spriteHandler = NULL;

	unsigned int _backBufferWidth = 0;
	unsigned int _backBufferHeight = 0;

public:
	ID3D10Device* GetDevice() { return _device; }
	IDXGISwapChain* GetSwapChain() { return _swapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return _renderTargetView; }
	ID3D10BlendState* GetBlendStateAlpha() { return _blendStateAlpha; }
	ID3DX10Sprite* GetSpriteHandler() { return _spriteHandler; }

	unsigned int GetBackBufferWidth() { return _backBufferWidth; }
	unsigned int GetBackBufferHeight() { return _backBufferHeight; }

	void Initialize(HWND hWnd);
	void Shutdown();

	/* Texture Database */
private:
	std::unordered_map<unsigned int, pTexture> _textures;

public:
	void LoadTexture(unsigned int id, std::wstring texturePath);
	pTexture GetTexture(unsigned int id);
};
typedef CGraphics* pGraphics;

#endif // !__GRAPHICS_H__