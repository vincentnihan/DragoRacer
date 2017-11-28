#ifndef NETWORKMANAGER_H
#define NETWROKMANAGER_H

#include "Manager.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "ReadyEvent.h"
#include "SocketLayer.h"
#include "FullyConnectedMesh2.h"
#include "ConnectionGraph2.h"
#include "NetworkIDObject.h"
#include "CoreMath.h"
#include <vector>
#include "NetworkIDManager.h"
#include <thread>

using namespace RakNet;
class GameManager;

class NetworkManager : public IManager
{
public:

	enum ReadyEvents
	{
		RE_STARTGAME = 0,
	};

	enum CustomPacketHeaders
	{
		ID_CLIENT_CREATE_RACER = ID_USER_PACKET_ENUM,
		ID_PLAYER_NUM,
		ID_POSITION_UPDATE,
		ID_ACCELERATE,
		ID_DECELERATE,
	};


	NetworkManager(void);
	~NetworkManager(void) {}

	void Init();
	void Shutdown();

	void SetGameManager(GameManager *gameManager){m_GameManager = gameManager;}

	void InitHost();
	void InitClient();

	void SetHostAddress(SystemAddress hostAddress) {m_HostAddress = hostAddress;}
	SystemAddress GetHostAddress() const {return m_HostAddress;}

	bool EstablishConnection();
	void PollPackets();

	void SetIsHost(bool isHost) { m_IsHost = isHost; }
	bool IsHost() { return m_IsHost;}

	bool GetNumPlayersInConnectionList(unsigned short &outNumPlayer);
	void AddPlayerToWaitList(RakNetGUID guid);

	void increaseNum();

	int GetNum();
	void StartGame();
	void BroadcastCreateRacer(NetworkID netID);
	NetworkID GetRacerNetID(RakNet::Packet *packet) const;

	void BroadcastPosition(NetworkID netID, Vector2 position);

	void NetworkAccelerate(NetworkID netID);
	void NetworkDecelerate(NetworkID netID);

	void SendPlayerNum();

	NetworkIDManager* GetNetworkIDManager() {return &m_NetworkIDManager;}

	int playerNum;

private:

	static const unsigned int MAX_CONNECTIONS = 8;
	static unsigned int PORT;

	bool m_IsHost;
	SystemAddress m_HostAddress;

	RakPeerInterface *m_RakPeer;
	ReadyEvent *m_ReadyEventPlugin;

	// These two plugins are just to automatically create a fully connected mesh so I don't have to call connect more than once
	FullyConnectedMesh2 *m_FCM2;
	ConnectionGraph2 *m_ConnectedGraph2;

	NetworkIDManager m_NetworkIDManager;

	GameManager *m_GameManager;
	std::thread *m_Thread;
	bool m_runThread;
};

#endif
