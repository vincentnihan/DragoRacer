#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H
//get a hold of the device
//wrapper for direct x textures etc...
#include "Manager.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

class Sprite;

class RenderingManager : public IManager
{
public:
	RenderingManager();
	~RenderingManager(){}

	void Init();
	void Shutdown();

	void SetIsFullScreen(bool isFullScreen) {m_isFullScreen = isFullScreen;}
	//void SetAmbientColor(D3DCOLOR ambientColor) {m_AmbientColor = ambientColor;}

	void Update(float dt);
	void ClearScene(D3DCOLOR color);

	void SetIdentity(){}	
	bool RenderStart();
	bool RenderStop();
	void Render2D_Start();
	void Render2D_Stop();
	int Release(){}

	void DrawSprites();

	void AddSprite(Sprite* sprite);
	void RemoveSprite(Sprite* sprite);
	void EmptySprites();

		LPDIRECT3DDEVICE9 GetDevice() { return this->m_Device; }

	LPDIRECT3DSURFACE9 GetBackBuffer() { return this->m_BackBuffer; }

	LPD3DXSPRITE GetSpriteHandler() { return this->m_SpriteHandler; }

	void SetWindowHandle(HWND hwnd) { this->m_WindowHandle = hwnd; }
	HWND GetWindowHandle() { return this->m_WindowHandle; }

	void SetAmbient(D3DCOLOR colorvalue);

	static const unsigned int SCREENWIDTH = 640;
	static const unsigned int SCREENHEIGHT = 480;
    
private:

	HWND m_WindowHandle;
	LPDIRECT3D9 m_D3d;
	LPDIRECT3DDEVICE9 m_Device;
	LPDIRECT3DSURFACE9 m_BackBuffer;
	LPD3DXSPRITE m_SpriteHandler;
	bool m_isFullScreen;
	D3DCOLOR m_AmbientColor;
	typedef std::vector<Sprite*>::iterator SPRITESITER;
	std::vector<Sprite*> m_Sprites;
	
};

#endif