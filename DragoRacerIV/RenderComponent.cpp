#include "RenderComponent.h"
#include "Sprite.h"
#include "RenderingManager.h"
#include "Entity.h"

CRenderComponent::CRenderComponent() : m_ActiveSprite(nullptr)
{
}

CRenderComponent::~CRenderComponent()
{
	if (m_ActiveSprite != nullptr)
	{
		Singleton<RenderingManager>::GetInstance()->RemoveSprite(m_ActiveSprite);
	}

	m_SpriteMap.clear();
}

void CRenderComponent::AddSprite(std::string imageName, std::string imageFileName)
{
	if(m_SpriteMap.find(imageName) == m_SpriteMap.end())
	{ 
		Sprite* addSprite = new Sprite();
		addSprite->LoadImage(imageFileName);
		m_SpriteMap[imageName] = addSprite;
	}
}

void  CRenderComponent::RemoveSprite(std::string imageName)
{
	std::map<std::string, Sprite*>::iterator removeSprite = m_SpriteMap.find(imageName);
	if (removeSprite != m_SpriteMap.end())
	{
		if (removeSprite->second == m_ActiveSprite)
		{
			Singleton<RenderingManager>::GetInstance()->RemoveSprite(m_ActiveSprite);
			m_ActiveSprite = nullptr;
		}
		m_SpriteMap.erase(removeSprite);
	}
}

void CRenderComponent::SetActiveSprite(std::string imageName)
{
	std::map<std::string, Sprite*>::iterator activeSprite = m_SpriteMap.find(imageName);
	if (activeSprite != m_SpriteMap.end())
	{
		Singleton<RenderingManager>::GetInstance()->RemoveSprite(m_ActiveSprite);
		m_ActiveSprite = activeSprite->second;
		Singleton<RenderingManager>::GetInstance()->AddSprite(m_ActiveSprite);
	}
	else
	{
		Singleton<RenderingManager>::GetInstance()->RemoveSprite(m_ActiveSprite);
		m_ActiveSprite = nullptr;
	}
}

void CRenderComponent::Update(float dt)
{
	if(m_Owner && m_ActiveSprite)
	{
		m_ActiveSprite->SetPosition(m_Owner->GetPosition());
	}
}
