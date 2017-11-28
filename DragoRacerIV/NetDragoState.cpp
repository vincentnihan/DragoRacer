#include "NetDragoState.h"
#include "DragoGameManager.h"
#include "InputManager.h"
#include "JoinState.h"
#include "HostState.h"
#include <stdlib.h> 
#include "RenderingManager.h"
#include "NetDragoRacer.h"
#include "AudioManager.h"
#include "Sprite.h"

const float SOAKTIME = 1.0f;

NetDragoState::~NetDragoState()
{

}

void NetDragoState::Init()
{
	m_DragoGameManager = static_cast<DragoGameManager*>(m_GameManager);
	m_DragoGameManager->Init();

	m_SoakInWinTime = 0.0f;

	bool drawBG = m_GameManager->GetNetworkManager()->IsHost() || m_IsClientReady;

	if(drawBG)
		m_DragoGameManager->DrawGameBG();

	//if you are host create the other players locally and send a packet to inform others
	if(m_GameManager->GetNetworkManager()->IsHost())
	{
		//host creates himself
		m_DragoGameManager->HostCreatePlayer();
		unsigned short numPlayers = 0;
		m_GameManager->GetNetworkManager()->GetNumPlayersInConnectionList(numPlayers);
		for(int i = 0; i < numPlayers; ++i)
		{
			Sleep(500);
			//create the other players
			//host creates all players so that netid's will be controlled that way
			m_DragoGameManager->HostCreatePlayer();
		}
	}
}

bool NetDragoState::Update(float deltaTime)
{
	if (m_SoakInWinTime < FLT_EPSILON)
	{
		CDragoRacer* winner = m_DragoGameManager->CheckForWinner();
		CNetDragoRacer* temp = static_cast<CNetDragoRacer*>((winner));
		if (temp)
		{
			if (temp->isMaster == true)
			{
				m_DragoGameManager->DrawResult(true);
			}
			else if (temp->isMaster == false)
			{
				m_DragoGameManager->DrawResult(false);
			}
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

void NetDragoState::Shutdown()
{
	//m_GameManager->GetRenderingManager()->RemoveSprite(m_SplashScreen);
	//m_GameManager->GetAudioManager()->StopAll();
}

void NetDragoState::HandleKeyUp(int key)
{
	if(key == DIK_ESCAPE)
		m_DragoGameManager->SetIsGameOver(true);

	if (key == DIK_Z)
	{
		m_DragoGameManager->TestAccelerate(true);
	}

	//m_DragoGameManager->TestAccelerate(key,true);
}


void NetDragoState::HandleKeyDown(int key)
{
	if (key == DIK_Z)
	{
		m_DragoGameManager->TestAccelerate(false);
	}
}

void NetDragoState::HandlePacket( RakNet::Packet *packet)
{
	switch (packet->data[0])
	{
	case NetworkManager::ID_CLIENT_CREATE_RACER:
		HandleClientCreateRacer(packet);
		break;
	case NetworkManager::ID_PLAYER_NUM:
		m_GameManager->GetNetworkManager()->increaseNum();
	case NetworkManager::ID_ACCELERATE:
		HandleAccelerate(packet);
		break;
	case NetworkManager::ID_POSITION_UPDATE:
		HandlePosition(packet);
		break;
	case NetworkManager::ID_DECELERATE:
		HandleDecelerate(packet);
		break;
	case ID_READY_EVENT_ALL_SET:
		m_IsClientReady = true;
		m_DragoGameManager->DrawGameBG();
		//HandleEventReady();
		break;
	case ID_READY_EVENT_SET:
		//m_IsClientReady = true;
		//m_DragoGameManager->DrawGameBG();
		//HandleEventReady();
		break;
	default:
		break;
	}
}

void NetDragoState::HandlePosition(RakNet::Packet *packet)
{
	m_DragoGameManager->HandlePosition(packet);
}

void NetDragoState::HandleClientCreateRacer(RakNet::Packet *packet)
{
	m_DragoGameManager->ClientCreatePlayer(packet);
}

void NetDragoState::HandleAccelerate(RakNet::Packet *packet)
{
	m_DragoGameManager->HandleAccelerate(packet);
}

void NetDragoState::HandleDecelerate(RakNet::Packet *packet)
{
	m_DragoGameManager->HandleDecelerate(packet);
}
