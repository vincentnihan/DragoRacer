#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component.h"
#include <string>
#include <map>

class Sprite;

class CRenderComponent : public CComponent
{
public:
	CRenderComponent();
	~CRenderComponent();

	void AddSprite(std::string imageName, std::string imageFileName);
	void RemoveSprite(std::string imageName);

	void SetActiveSprite(std::string imageName);

	void Update(float dt);

private:
	Sprite* m_ActiveSprite;
	std::map<std::string, Sprite*> m_SpriteMap;
protected:

};


#endif