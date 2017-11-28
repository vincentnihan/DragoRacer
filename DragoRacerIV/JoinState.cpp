#include "JoinState.h"

#include "DragoGameManager.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "Sprite.h"
#include "NetworkManager.h"
#include "AudioManager.h"
#include "IntroState.h"
#include "NetDragoState.h"

void JoinState::Init()
{
	//m_GameManager = Singleton<DragoGameManager>::GetInstance();
	m_IsConnectionAccepted = false;

	LoadAssets();

	m_GameManager->GetRenderingManager()->AddSprite(m_SplashScreen);

	NetworkManager *netManager = m_GameManager->GetNetworkManager();
	netManager->InitClient();
}

void JoinState::Shutdown()
{
	m_GameManager->GetRenderingManager()->RemoveSprite(m_SplashScreen);
}

void JoinState::LoadAssets()
{
	m_SplashScreen = new Sprite();
	m_SplashScreen->LoadImage("Lundgren_Ivan_DragoJoin.jpg");
	m_SplashScreen->SetFullScreen();
	m_ConnectionSound = m_GameManager->GetAudioManager()->Load("boxing2rings.mp3");

}


void JoinState::HandleKeyUp(int key)
{
	if(key == DIK_ESCAPE)
		m_GameManager->SetIsGameOver(true);

	if(key == DIK_SPACE && !m_IsConnectionAccepted)
	{
		 if(m_GameManager->GetNetworkManager()->EstablishConnection())
			 m_DesiredState = GameManager::GS_NetDragoState;
	}
}

bool JoinState::Update(float deltaTime)
{
	if(m_DesiredState == GameManager::GS_NetDragoState)
	{
		ChangeState(new NetDragoState(m_GameManager));
		return true;
	}
	return false;
}

void JoinState::HandleConnectionAccepted(RakNet::Packet *packet)
{
	m_GameManager->GetAudioManager()->Play(m_ConnectionSound);
	m_GameManager->GetNetworkManager()->SetHostAddress(packet->systemAddress);
	m_IsConnectionAccepted = true;
}

void JoinState::HandlePacket( RakNet::Packet *packet)
{
	int x,y;
	switch (packet->data[0])
	{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			HandleConnectionAccepted(packet);
			break;
		case NetworkManager::ID_PLAYER_NUM:
			m_GameManager->GetNetworkManager()->increaseNum();
			break;
		default:
			break;
	}
}


