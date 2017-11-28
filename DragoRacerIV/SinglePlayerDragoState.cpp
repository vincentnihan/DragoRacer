#include "SinglePlayerDragoState.h"
#include "DragoGameManager.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "NetworkManager.h"
#include "AudioManager.h"
#include "Sprite.h"

const float SOAKTIME = 1.0f;

SinglePlayerDragoState::~SinglePlayerDragoState()
{

}

void SinglePlayerDragoState::Init()
{
	m_DragoGameManager = static_cast<DragoGameManager*>(m_GameManager);
	m_DragoGameManager->DrawGameBG();
	m_DragoGameManager->CreateRacer();
	m_SoakInWinTime = 0.0f;

}

bool SinglePlayerDragoState::Update(float deltaTime)
{
	//check to see if there is a winner, only if a winner hasn't been 
	//found yet
	if (m_SoakInWinTime < FLT_EPSILON)
	{
		CDragoRacer* winner = m_DragoGameManager->CheckForWinner();
		if (winner)
		{
			m_DragoGameManager->DrawResult(true);
			m_SoakInWinTime = SOAKTIME;
		}
	}

	if (m_SoakInWinTime > FLT_EPSILON)
	{
		m_SoakInWinTime -= deltaTime;
		if (m_SoakInWinTime < FLT_EPSILON)
		{
			m_DragoGameManager->SetIsGameOver(true);
		}
	}

	return false;
}

void SinglePlayerDragoState::Shutdown()
{
}

void SinglePlayerDragoState::HandleKeyUp(int key)
{
	if(key == DIK_ESCAPE)
		m_DragoGameManager->SetIsGameOver(true);

	m_DragoGameManager->TestAccelerate(true);
}


void SinglePlayerDragoState::HandleKeyDown(int key)
{
	m_DragoGameManager->TestAccelerate(key);
}
