#include "RenderingManager.h"
#include "Sprite.h"

RenderingManager::RenderingManager() : 
	m_isFullScreen(false)
{
	m_AmbientColor = D3DCOLOR_XRGB(255,255,255);
}

void RenderingManager::Init()
{
	//initialize Direct3D
    this->m_D3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (this->m_D3d == NULL) {
		//assert
        return;
    }
    
    //get system desktop color depth
    D3DDISPLAYMODE dm;
    this->m_D3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
    
    //set configuration options for Direct3D
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = (!m_isFullScreen);
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.BackBufferFormat = dm.Format;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = SCREENWIDTH;
    d3dpp.BackBufferHeight = SCREENHEIGHT;
    d3dpp.hDeviceWindow = m_WindowHandle;
    
    //create Direct3D device
    this->m_D3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_REF,
        this->m_WindowHandle,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &this->m_Device);
    
    if (this->m_Device == NULL) 
	{
		//assert
			return;
	}
    
    //clear the backbuffer to black
    this->ClearScene(D3DCOLOR_XRGB(0,0,0));
    
    //create pointer to the back buffer
    this->m_Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->m_BackBuffer);
    
    //use ambient lighting and z-buffering
    this->m_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    this->m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    this->m_Device->SetRenderState(D3DRS_LIGHTING, true);
	this->SetAmbient(this->m_AmbientColor);
    
    //initialize 2D renderer
    HRESULT result = D3DXCreateSprite(this->m_Device, &this->m_SpriteHandler);
    if (result != D3D_OK) 
	{
		//assert
		return;
	}

	D3DXMATRIX matrixWorld;
    D3DXMatrixIdentity(&matrixWorld);
    m_Device->SetTransform(D3DTS_WORLD, &matrixWorld);
}

void RenderingManager::SetAmbient(D3DCOLOR colorvalue)
{
    this->m_AmbientColor = colorvalue;
    this->m_Device->SetRenderState(D3DRS_AMBIENT, this->m_AmbientColor);
}

void RenderingManager::Shutdown()
{

}

void RenderingManager::ClearScene(D3DCOLOR color)
{
	m_Device->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

void RenderingManager::Update(float dt)
{
	RenderStart();
	Render2D_Start();
	DrawSprites();
	Render2D_Stop();
	RenderStop();
}

void RenderingManager::Render2D_Start()
{
   if( m_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK)
	   return;

}
    
void RenderingManager::Render2D_Stop()
{
    if(m_SpriteHandler->End()!= D3D_OK)
		return;
}

bool RenderingManager::RenderStart()
{
    if (!this->m_Device) 
		return 0;
    if (this->m_Device->BeginScene() != D3D_OK) 
		return 0;

    return 1;
}
    
bool RenderingManager::RenderStop()
{
    if (!this->m_Device) 
		return 0;

    if (this->m_Device->EndScene() != D3D_OK) 
		return 0;

    if (m_Device->Present(NULL, NULL, NULL, NULL) != D3D_OK) 
		return 0;

    return 1;
}

void RenderingManager::DrawSprites()
{
	Vector3 center;
	Vector3 position;
	D3DXMATRIX mat;
	D3DCOLOR color;
	for(SPRITESITER it = m_Sprites.begin(); it != m_Sprites.end(); ++it)
	{
		Sprite *sprite = (*it);
		if(sprite->GetVisible())
		{
			//matrix will contain scale and rotation info
			sprite->GetTransform(mat);
			sprite->GetColor(color);

			RECT drawRect;
			sprite->GetDrawInfo(drawRect);

			GetSpriteHandler()->SetTransform(&mat);
			GetSpriteHandler()->Draw(sprite->GetImage()->GetTexture(),&drawRect,nullptr,nullptr,color);
		}
	}
}

void  RenderingManager::AddSprite(Sprite* sprite)
{
	if (sprite == nullptr)
	{
		return;
	}

	m_Sprites.push_back(sprite);
	
}

void  RenderingManager::RemoveSprite(Sprite* sprite)
{
	if (sprite == nullptr)
	{
		return;
	}

	for(SPRITESITER it = m_Sprites.begin(); it != m_Sprites.end(); ++it)
	{
		if( (*it) == sprite)
		{
			m_Sprites.erase(it);
			break;
		}
	}
}

void  RenderingManager::EmptySprites()
{
	m_Sprites.empty();
}