#include "HostState.h"
#include "DragoGameManager.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "Sprite.h"
#include "NetworkManager.h"
#include "NetDragoState.h"


void HostState::Init()
{
	//m_GameManager = Singleton<DragoGameManager>::GetInstance();

	m_SplashScreen = new Sprite();
	m_SplashScreen->LoadImage("Lundgren_Ivan_DragoHost.jpg");
	m_SplashScreen->SetFullScreen();
	m_GameManager->GetRenderingManager()->AddSprite(m_SplashScreen);

	m_StartText = new Sprite();
	m_StartText->LoadImage("StartText.png");
	m_StartText->SetVisible(false);
	m_StartText->SetPosition(Vector2(100.0f,300.0f));
	m_GameManager->GetRenderingManager()->AddSprite(m_StartText);

	NetworkManager *netManager = m_GameManager->GetNetworkManager();
	netManager->InitHost();
}

void HostState::Shutdown()
{
	m_GameManager->GetRenderingManager()->RemoveSprite(m_SplashScreen);
	m_GameManager->GetRenderingManager()->RemoveSprite(m_StartText);
}

void HostState::HandleKeyUp(int key)
{
	if(key == DIK_ESCAPE)
		m_GameManager->SetIsGameOver(true);

	if(key == DIK_S && CanStartGame())
	{
		m_DesiredState = GameManager::GS_NetDragoState;
	}
}

bool HostState::Update(float deltaTime)
{
	if(m_DesiredState == GameManager::GS_NetDragoState)
	{
		m_GameManager->GetNetworkManager()->StartGame();
		ChangeState(new NetDragoState(m_GameManager));
		return true;
	}
	return false;
}

void HostState::HandleRequestAccepted(RakNet::Packet *packet)
{
	m_GameManager->GetNetworkManager()->AddPlayerToWaitList(packet->guid);
	if(CanStartGame() && !m_StartText->GetVisible())
	{
		m_StartText->SetVisible(true);
	}
}

void HostState::PlayerNumIncrease(RakNet::Packet *packet)
{
	m_GameManager->GetNetworkManager()->SendPlayerNum();
}

bool HostState::CanStartGame()
{
	unsigned short numPlayers;
	m_GameManager->GetNetworkManager()->GetNumPlayersInConnectionList(numPlayers);
	return numPlayers > 0;
}

void HostState::HandlePacket( RakNet::Packet *packet)
{
	switch (packet->data[0])
	{
		case ID_CONNECTION_REQUEST_ACCEPTED:
				break;
		case ID_NEW_INCOMING_CONNECTION:
				PlayerNumIncrease(packet);
				HandleRequestAccepted(packet);
				break;
		case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				//printf("ID_DISCONNECTION_NOTIFICATION\n");
			break;
		default:
			break;
	}
}
