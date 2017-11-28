#include "DragoGameManager.h"
#include "Sprite.h"
#include "DragoRacer.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "NetDragoRacer.h"
//#include "NetworkManager.h"

const float DragoGameManager::COUNTDOWNTIME = 2.0f;

DragoGameManager::DragoGameManager() : GameManager()
	,m_IsActive(true)
	,m_NumPlayers(0)
	,m_playerLabel(1)
	,m_CountdownTimer(COUNTDOWNTIME)
	,m_Background(nullptr)
	,m_CountdownSprite(nullptr)
	,m_RaceResultSprite(nullptr)
{
}

DragoGameManager::~DragoGameManager()
{
	if(m_Background)
		delete m_Background;

	m_DragRacers.clear();
}

void DragoGameManager::Init()
{
	__super::Init();
	InitStartingPositions();
	InitInputKeys();
}

void DragoGameManager::Update(float dt)
{
	if(m_NumPlayers > 0 && m_CountdownTimer > FLT_EPSILON)
	{
		UpdateCountdownTimer(dt);
	}

	for(DRAGRACERSITERATOR it = m_DragRacers.begin();it!=m_DragRacers.end();++it)
	{
		(*it)->Update(dt);
	}
	HandleUpdate();

}

void DragoGameManager::CreateRacer(int racerNum /* = 1 */)
{
	CDragoRacer *racer = new CDragoRacer();
	racer->Init(racerNum);
	racer->SetPosition(m_StartingPositions[m_NumPlayers]);
	m_DragRacers.push_back(racer);
	m_NumPlayers++;
}

CDragoRacer* DragoGameManager::CheckForWinner()
{
	for (DRAGRACERSITERATOR it = m_DragRacers.begin(); it != m_DragRacers.end(); ++it)
	{
		if ((*it)->GetPosition().x > RenderingManager::SCREENWIDTH)
			return (*it);
	}

	return nullptr;
}

//MultiPlayer
void DragoGameManager::HostCreatePlayer()
{
	if(m_NumPlayers < MAXPLAYERS)
	{
		CNetDragoRacer *racer = new CNetDragoRacer();
		racer->Init(m_NumPlayers);
		if (m_NumPlayers == 0)
		{
			racer->isMaster = true;
		}
		racer->SetNetworkIDManager(GetNetworkManager()->GetNetworkIDManager());
		racer->SetPosition(m_StartingPositions[m_NumPlayers]);
		m_DragRacers.push_back(racer);
		m_NumPlayers++;
		GetNetworkManager()->BroadcastCreateRacer(racer->GetNetworkID());
	}
}

void DragoGameManager::ClientCreatePlayer(RakNet::Packet *packet)
{
	m_NumPlayers += 1;
	RakNet::NetworkID netID = GetNetworkManager()->GetRacerNetID(packet);
	CNetDragoRacer *racer = new CNetDragoRacer();
	racer->Init(m_NumPlayers - 1);
	int temp = m_NumPlayers - 1;
	m_playerLabel = GetNetworkManager()->GetNum();
	if (temp == m_playerLabel)
	{
		racer->isMaster = true;
	}

	racer->SetPosition(m_StartingPositions[m_NumPlayers-1]);
	racer->SetNetworkIDManager(GetNetworkManager()->GetNetworkIDManager());
	racer->SetNetworkID(netID);
	m_DragRacers.push_back(racer);
	
}

void DragoGameManager::DrawGameBG()
{
	m_Background = new Sprite();
	m_Background->LoadImage("dragtrack.png");
	m_Background->SetFullScreen();
	GetRenderingManager()->AddSprite(m_Background);
}

void DragoGameManager::DrawResult(bool isWin)
{
	Sprite* resultSprite = new Sprite();
	std::string imageName = isWin ? "YouWin.png" : "YouLose.png";
	resultSprite->LoadImage(imageName.c_str());
	resultSprite->CentreImage();
	GetRenderingManager()->AddSprite(resultSprite);
}

void DragoGameManager::InitStartingPositions()
{
	//take the screen height, divide by max number players
	//find the height of the image and leave that as a buffer
	float buffer = RenderingManager::SCREENHEIGHT/MAXPLAYERS + 25.0f;
	int i =0;
	for(int i = 0;i < MAXPLAYERS;++i)
	{
		m_StartingPositions.push_back(Vector2(0.0f,i*buffer));
	}
}

void DragoGameManager::InitInputKeys()
{
	static int keys[MAXPLAYERS*2] =
	{
		DIK_A,DIK_Z,
		DIK_G,DIK_B,
		DIK_J,DIK_M,
		DIK_L,DIK_PERIOD,
	};

	for(int i = 0;i < MAXPLAYERS;++i)
	{
		DragoInputKeys inputs;
		inputs.m_AccelerationKey = keys[i];
		inputs.m_NitroKey = keys[i+1];
		m_DragRacerInputKeys.push_back(inputs);
		i += 2;
	}
}

void DragoGameManager::UpdateCountdownTimer(float dt)
{
	//do a countdown
}

void DragoGameManager::HandleAccelerate(RakNet::Packet *packet)
{
	RakNet::NetworkID netID = GetNetworkManager()->GetRacerNetID(packet);
	int i = 0;
	for (DRAGRACERSITERATOR it = m_DragRacers.begin(); it != m_DragRacers.end(); ++it, ++i)
	{
		CNetDragoRacer *temp = static_cast<CNetDragoRacer*>((*it));
		if (temp->GetNetworkID() == netID)
		{
			m_DragRacers[i]->Accelerate();
		}

	}
}

void DragoGameManager::HandlePosition(RakNet::Packet *packet)
{
	RakNet::NetworkID netID = GetNetworkManager()->GetRacerNetID(packet);
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(MessageID));
	bsIn.IgnoreBytes(sizeof(netID));
	Vector2 tempPosition;
	bsIn.Read(tempPosition);

	int i = 0;
	for (DRAGRACERSITERATOR it = m_DragRacers.begin(); it != m_DragRacers.end(); ++it, ++i)
	{
		CNetDragoRacer *temp = static_cast<CNetDragoRacer*>((*it));
		if (temp->GetNetworkID() == netID)
		{
			m_DragRacers[i]->SetPosition(tempPosition);
		}

	}
}

void DragoGameManager::HandleUpdate()
{
	int i = 0;
	for (DRAGRACERSITERATOR it = m_DragRacers.begin(); it != m_DragRacers.end(); ++it, ++i)
	{
		CNetDragoRacer *temp = static_cast<CNetDragoRacer*>((*it));
		if (i != 0)
		{
			GetNetworkManager()->BroadcastPosition(temp->GetNetworkID(),temp->GetPosition());
		}

	}
}

void DragoGameManager::HandleDecelerate(RakNet::Packet *packet)
{
	RakNet::NetworkID netID = GetNetworkManager()->GetRacerNetID(packet);
	int i = 0;
	for (DRAGRACERSITERATOR it = m_DragRacers.begin(); it != m_DragRacers.end(); ++it, ++i)
	{
		CNetDragoRacer *temp = static_cast<CNetDragoRacer*>((*it));
		if (temp->GetNetworkID() == netID)
		{
			m_DragRacers[i]->Decelerate();
		}

	}
}

bool DragoGameManager::TestAccelerate(bool testDecelerate /* = false*/ )
{
	if(m_IsActive)
	{
		for(DRAGRACERSITERATOR it = m_DragRacers.begin();it!=m_DragRacers.end();++it)
		{
			if(testDecelerate == true)
			{
				CNetDragoRacer *temp = static_cast<CNetDragoRacer*>((*it));
				if (temp && temp->isMaster == true)
				{
					(*it)->Decelerate();
					GetNetworkManager()->NetworkDecelerate(temp->GetNetworkID());
				}
			}
			else
			{
				CNetDragoRacer *temp = static_cast<CNetDragoRacer*>((*it));
				if (temp && temp->isMaster == true)
				{
					(*it)->Accelerate();
					GetNetworkManager()->NetworkAccelerate(temp->GetNetworkID());
				}
			}
		}
	}
	return false;
}

bool DragoGameManager::TestNitro(int key)
{
	return false;
}