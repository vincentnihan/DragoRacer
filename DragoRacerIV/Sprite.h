#ifndef SPRITE_H
#define SPRITE_H

#include "Texture.h"
#include "CoreMath.h"

class Sprite {

	public:
		Sprite();
		virtual ~Sprite();
		bool LoadImage(std::string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(255,0,255));
		void SetImage(Texture *texture);
		void GetDrawInfo(RECT& outRect);

		//screen position
		Vector3 GetPosition() { return m_ScreenPosition; }
		void SetPosition(Vector2 position) { this->m_ScreenPosition = position; }
	
		//image size
		void SetSize(int width, int height) { this->m_Width = width; this->m_Height = height; }
		int GetWidth() { return this->m_Width; }
		void SetWidth(int value) { this->m_Width = value; }
		int GetHeight() { return this->m_Height; }
		void SetHeight(int value) { this->m_Height = value; }

		bool GetVisible() { return m_IsVisible && m_Image; }
		void SetVisible(bool value) { m_IsVisible = value; }

		void SetColor(D3DCOLOR col) { m_Color = col; }
		void GetColor(D3DCOLOR& color) { color = m_Color; }
		
		void GetTransform(D3DXMATRIX& mat);

		Texture *GetImage() {return m_Image;}

		void SetFullScreen(); 
		void CentreImage();

	private:
		bool m_IsVisible;
		Vector2 m_ScreenPosition;

		Texture *m_Image;
		unsigned int m_Width;
		unsigned int m_Height;
		Vector2 m_ScaleRatio;
		Vector2 m_Rotation;
		D3DCOLOR m_Color;
	}; 

#endif