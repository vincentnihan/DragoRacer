#ifndef NETDRAGOSTATE_H
#define NETDRAGOSTATE_H

#include "NetworkedGameState.h"
#include <string>

class GameManager;
class DragoGameManager;

class NetDragoState : public NetGameState
{
public:
	NetDragoState(GameManager *gameManager) : NetGameState(gameManager), m_IsClientReady(false) {}
	virtual ~NetDragoState();
	virtual void Init();
	virtual void Shutdown();

	virtual void Pause() {}
	virtual void Resume() {}

	virtual void HandleKeyUp(int key);
	virtual void HandleKeyDown(int key);
	virtual void HandleMouseClick(int button){}

	virtual void HandleMouseMotionAbs(int x,int y) {}
	virtual void HandleMouseMotionRel(int deltax,int deltay){}
	virtual void HandleMouseScroll(int scroll) {}

	virtual bool Update(float deltaTime);

	virtual void HandlePacket( RakNet::Packet *packet);
	void HandleClientCreateRacer(RakNet::Packet *packet);
	void HandlePosition(RakNet::Packet *packet);
	void HandleAccelerate(RakNet::Packet *packet);
	void HandleDecelerate(RakNet::Packet *packet);

private:
	DragoGameManager *m_DragoGameManager;
	bool m_IsClientReady;
	float m_SoakInWinTime;
};

#endif
