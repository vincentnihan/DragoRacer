#include "Entity.h"
#include "Component.h"

CEntity::CEntity() : 
	m_ID(0)
{

}

CEntity::~CEntity()
{

}

void CEntity::Update(float dt)
{
	for(COMPONENTITERATOR it = m_Components.begin();it!=m_Components.end();++it)
	{
		(*it)->Update(dt);
	}
}

void CEntity::AddComponent(CComponent* component)
{
	m_Components.push_back(component);
}
