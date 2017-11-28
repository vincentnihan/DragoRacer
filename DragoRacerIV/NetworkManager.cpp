#include "NetworkManager.h"
#include "FileParser.h"
#include "GameManager.h"
#include "NetworkedGameState.h"
#include "BitStream.h"
#include "CoreMath.h"
#include <vector>
#include <chrono>

unsigned int NetworkManager::PORT = 60000;
NetworkManager::NetworkManager() : m_RakPeer(nullptr), m_runThread(false), playerNum(0)
{

}

void NetworkManager::Init()
{
	m_runThread = true;
	m_Thread = new std::thread(&NetworkManager::PollPackets,this);
}

void NetworkManager::InitHost()
{
	m_RakPeer = RakPeerInterface::GetInstance();

	m_IsHost = true;

	m_ReadyEventPlugin = new ReadyEvent();

	// These two plugins are just to automatically create a fully connected mesh so I don't have to call connect more than once
	m_FCM2 = new FullyConnectedMesh2();
	m_ConnectedGraph2 = new ConnectionGraph2();

	m_RakPeer->AttachPlugin(m_ReadyEventPlugin);
	m_RakPeer->AttachPlugin(m_FCM2);
	m_RakPeer->AttachPlugin(m_ConnectedGraph2);
	m_RakPeer->SetMaximumIncomingConnections(MAX_CONNECTIONS);
	
	m_FCM2->SetAutoparticipateConnections(true);
	m_FCM2->SetConnectOnNewRemoteConnection(true, "");
	m_ConnectedGraph2->SetAutoProcessNewConnections(true);

	SocketDescriptor sd(PORT,0);
	
	StartupResult sr = m_RakPeer->Startup(MAX_CONNECTIONS, &sd, 1);
	RakAssert(sr==RAKNET_STARTED);
}

void NetworkManager::InitClient()

{
	m_RakPeer = RakPeerInterface::GetInstance(); 
	m_IsHost = false; RakNet::SocketDescriptor sd(PORT, 0); 
	while (RakNet::IRNS2_Berkley::IsPortInUse(sd.port, sd.hostAddress, sd.socketFamily, SOCK_DGRAM) == true) 
	{ 
		sd.port++; 
	}  
	StartupResult sr = m_RakPeer->Startup(MAX_CONNECTIONS, &sd, 1); 
	RakAssert(sr == RAKNET_STARTED);

}

void NetworkManager::Shutdown()
{
	m_runThread = false;

	if(m_RakPeer)
		RakPeerInterface::DestroyInstance(m_RakPeer);

	if(m_Thread)
	{
		m_Thread->join();
		delete m_Thread;
		m_Thread = nullptr;
	}
}

bool NetworkManager::EstablishConnection()
{
	if(!m_IsHost)
	{
		FileParser::LINESARRAY lines;
		FileParser::ReadFile("HostIp.txt",lines);
		if(lines.size() > 0)
		{
			SystemAddress sysAddress;
			if(sysAddress.FromString(lines[0].c_str()))
			{
				ConnectionAttemptResult car = m_RakPeer->Connect(sysAddress.ToString(false),sysAddress.GetPort(), 0, 0, 0);
				RakAssert(car==CONNECTION_ATTEMPT_STARTED);
				return true;
			}
		}
	}
	return false;
}

void NetworkManager::AddPlayerToWaitList(RakNetGUID guid)
{
	m_ReadyEventPlugin->AddToWaitList(RE_STARTGAME, guid);
}

bool NetworkManager::GetNumPlayersInConnectionList(unsigned short &outNumPlayer)
{
	outNumPlayer = m_RakPeer->NumberOfConnections();
	return outNumPlayer > 0;
}

void NetworkManager::StartGame()
{
	m_ReadyEventPlugin->SetEvent(RE_STARTGAME,true);
}

NetworkID NetworkManager::GetRacerNetID(RakNet::Packet *packet) const
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(MessageID));
	NetworkID racerNetworkID;
	bsIn.Read(racerNetworkID);
	return racerNetworkID;
}

int NetworkManager::GetNum()
{
	int playerNumber = playerNum;
	return playerNumber;
}

void NetworkManager::increaseNum()
{
	playerNum = playerNum + 1;
}

void NetworkManager::BroadcastCreateRacer(NetworkID netID)
{
	RakNet::BitStream bsOut;
	bsOut.Write((MessageID)ID_CLIENT_CREATE_RACER);	
	bsOut.Write(netID);
	if(!m_RakPeer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,UNASSIGNED_SYSTEM_ADDRESS,true))
	{
		int problemSending = 0;
	}
}

void NetworkManager::SendPlayerNum()
{
	RakNet::BitStream bsOut;
	bsOut.Write((MessageID)ID_PLAYER_NUM);
	if (!m_RakPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
	{
		int problemSending = 0;
	}
}

void NetworkManager::BroadcastPosition(NetworkID netID, Vector2 position )
{
	RakNet::BitStream bsOut;
	bsOut.Write((MessageID)ID_POSITION_UPDATE);
	bsOut.Write(netID);
	bsOut.Write(position);
	if (!m_RakPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
	{
		int problemSending = 0;
	}
}

void NetworkManager::NetworkAccelerate(NetworkID netID)
{
	RakNet::BitStream bsOut;
	bsOut.Write((MessageID)ID_ACCELERATE);
	bsOut.Write(netID);
	if (!m_RakPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
	{
		int problemSending = 0;
	}
}

void NetworkManager::NetworkDecelerate(NetworkID netID)
{
	RakNet::BitStream bsOut;
	bsOut.Write((MessageID)ID_DECELERATE);
	bsOut.Write(netID);
	if (!m_RakPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
	{
		int problemSending = 0;
	}
}
void NetworkManager::PollPackets()
{
	while(m_runThread)
	{
		if(m_RakPeer)
		{
			NetGameState *netGameState = static_cast<NetGameState*>(m_GameManager->GetCurrentState());
			Packet *p; 
			for(p = m_RakPeer->Receive();p != NULL; m_RakPeer->DeallocatePacket(p), p=m_RakPeer->Receive())
			{
				if(netGameState)
					netGameState->HandlePacket(p);

				if(p->data[0] == NetworkManager::ID_CLIENT_CREATE_RACER)
				{
				
				}
			}	
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

//Notes
/*
listeners are registered through components
all custom enums are defined in this header file
the components do the hand shaking, register listeners for packets
those listeners have a call back function for that class
when packet is recieved, enum is index, array of callback functions get sent
*/