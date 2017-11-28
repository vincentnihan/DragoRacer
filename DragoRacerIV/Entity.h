#ifndef ENTITY_H
#define ENTITY_H

#include "CoreMath.h"
#include <vector>

class CComponent;

class CEntity
{
public:
	CEntity();
	~CEntity();

	unsigned int GetID() const {return m_ID;}
	Vector2 GetPosition() const {return m_Position;}
	void SetPosition(const Vector2& pos) {m_Position = pos;}
	void Translate(const Vector2& pos) {m_Position += pos;}
	virtual void Update(float dt);

	void AddComponent(CComponent* component);

private:
	unsigned int m_ID;
	Vector2 m_Position;
	typedef std::vector<CComponent*>::iterator COMPONENTITERATOR;
	std::vector<CComponent*> m_Components;

protected:

};

#endif