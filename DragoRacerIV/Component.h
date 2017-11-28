#ifndef COMPONENT_H
#define COMPONENT_H

class CEntity;

class CComponent 
{
public:
	CComponent() {}
	~CComponent() {}

	virtual void Update(float dt){}

	CEntity *GetOwner() const {return m_Owner;}
	void SetOwner(CEntity *owner) {m_Owner = owner;}
private:
protected:
	CEntity *m_Owner;
};

#endif