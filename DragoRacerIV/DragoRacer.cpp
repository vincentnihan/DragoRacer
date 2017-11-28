#include "DragoRacer.h"
#include "RenderComponent.h"

CDragoRacer::CDragoRacer() :
	m_MaxSpeed(60.0f)
	,m_NitroMaxSpeed(120.0f)
	,m_CurrentSpeed(0.0f)
	,m_AccelerationRate(33.0f)
	,m_DecelerationRate(15.0f)
	,m_CurrentNitroTime(0.0f)
	,m_MaxNitroTime(1.0f)
	,m_IsAccelerating(false)
	,m_AccelerationAudio(nullptr)
	,m_NitroAudio(nullptr)
{
	m_RenderComponent = new CRenderComponent();
	m_RenderComponent->SetOwner(this);
	AddComponent(m_RenderComponent);
	
}

CDragoRacer::~CDragoRacer()
{
	if(m_RenderComponent)
		delete m_RenderComponent;
}

void CDragoRacer::Init(int racerNum)
{
	std::string imageName = "DragRacerPlayer"; 
	imageName += std::to_string(racerNum+1);
	//imageName += ".png";
	m_RenderComponent->AddSprite("idle", imageName + ".png");
	m_RenderComponent->AddSprite("accelerating", imageName + "Moving.png");
	m_RenderComponent->SetActiveSprite("idle");
}

bool CDragoRacer::UseNitro()
{
	return true;
}

void CDragoRacer::Accelerate()
{
	if (!m_IsAccelerating)
	{
		m_IsAccelerating = true;
		m_RenderComponent->SetActiveSprite("accelerating");
	}
	
}

void CDragoRacer::Decelerate()
{
	if (m_IsAccelerating)
	{
		m_IsAccelerating = false;
		m_RenderComponent->SetActiveSprite("idle");
	}
}

void CDragoRacer::CalculateAcceleration(float dt) 
{
	if(m_IsAccelerating)
	{
		if(m_CurrentSpeed < m_MaxSpeed)
		{
			m_CurrentSpeed += m_AccelerationRate * dt;
		}
	}
	else
	{
		if(m_CurrentSpeed > FLT_EPSILON)
		{
			m_CurrentSpeed -= m_DecelerationRate * dt;
			if(m_CurrentSpeed < 0.0f)
				m_CurrentSpeed = 0.0f;
		}
	}
}

void CDragoRacer::Update(float dt) 
{
	CalculateAcceleration(dt);
	Translate(Vector2(m_CurrentSpeed, 0.0f));
	__super::Update(dt);
}