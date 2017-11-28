#ifndef DRAGORACER_H
#define DRAGORACER_H

#include "CoreMath.h"
#include "Entity.h"

class Sprite;
class CRenderComponent;
class AudioSample;

class CDragoRacer : public CEntity
{
public:
	CDragoRacer();
	virtual ~CDragoRacer();

	void Init(int racerNum = 1);

	bool UseNitro();
	virtual void Accelerate();
	virtual void Decelerate();

	bool IsNitroActive() const {return m_CurrentNitroTime > FLT_EPSILON;}

	void Update(float dt);

	void CalculateAcceleration(float dt);
	
private:
	float m_MaxSpeed;
	float m_NitroMaxSpeed;
	float m_CurrentSpeed;
	float m_AccelerationRate;
	float m_DecelerationRate;
	float m_CurrentNitroTime;
	float m_MaxNitroTime;
	bool m_IsAccelerating;
	CRenderComponent *m_RenderComponent;
	AudioSample	*m_AccelerationAudio;
	AudioSample *m_NitroAudio;

protected:

};

#endif