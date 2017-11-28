#include "Sprite.h"
#include "RenderingManager.h"

Sprite::Sprite() : m_IsVisible(true)
{
	this->m_Image = NULL;

	Vector2 pos;
	pos.x = 0.0f;//RenderingManager::SCREENWIDTH;
	pos.y = 0.0f;//RenderingManager::SCREENHEIGHT;
	
	this->SetPosition(pos);
	
	this->m_Width = 1;
	this->m_Height = 1;
	
	m_ScaleRatio.x = 1.0f;
	m_ScaleRatio.y = 1.0f;

	
	m_Rotation.x = 0.0f;
	m_Rotation.y = 0.0f;

	this->m_Color = 0xFFFFFFFF;
	
}

Sprite::~Sprite()  {
	if (m_Image)
		delete m_Image;
}

bool Sprite::LoadImage(std::string filename, D3DCOLOR transcolor)
{
	//if image already exists, free it
	if (m_Image) 
		delete m_Image;

	//create texture and load image
	m_Image = new Texture();
	if (m_Image->Load(filename,transcolor))
	{
		this->SetSize(m_Image->getWidth(),m_Image->getHeight());
		
		return true;
	}
	else
		return false;
}

void Sprite::SetImage(Texture *image)
{
	this->m_Image = image;
	this->SetSize(image->getWidth(),image->getHeight());
}

void Sprite::GetTransform(D3DXMATRIX& mat)
{
	D3DXMatrixTransformation2D(&mat,nullptr,0.0f,&m_ScaleRatio,nullptr,0.0f,&m_ScreenPosition);
}

void Sprite::GetDrawInfo(RECT& outRect )
{
	outRect.top = 0.0;
	outRect.left = 0.0;
	outRect.right = this->m_Width;
	outRect.bottom = this->m_Height;
}

void Sprite::SetFullScreen()
{
	m_ScaleRatio.x = m_Width > 0.0f ? (float)RenderingManager::SCREENWIDTH/(float)m_Width : 1.0f;
	m_ScaleRatio.y = m_Width > 0.0f ? (float)RenderingManager::SCREENHEIGHT/(float)m_Height : 1.0f;
}

void Sprite::CentreImage()
{
	float xCentre = (float)RenderingManager::SCREENWIDTH / 2.0f;
	float yCentre = (float)RenderingManager::SCREENHEIGHT / 2.0f;
	SetPosition(Vector2(xCentre, yCentre));

}