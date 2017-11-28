#ifndef HOSTSTATE_H
#define HOSTSTATE_H

#include "NetworkedGameState.h"
//press space to start, only listen when another racer is on screen
class Sprite;
class GameManager;

class HostState : public NetGameState
{
public:
	HostState(GameManager *gameManager) : NetGameState(gameManager) {}
	~HostState() {}

	virtual void Init();
	virtual void Shutdown();

	virtual void Pause(){}
	virtual void Resume(){};

	virtual void HandleKeyUp(int key);
	virtual void HandleKeyDown(int key){};
	virtual void HandleMouseClick(int button){};

	virtual void HandleMouseMotionAbs(int x,int y){};
	virtual void HandleMouseMotionRel(int deltax,int deltay){};
	virtual void HandleMouseScroll(int scroll) {};

	virtual bool Update(float deltaTime);

	bool CanStartGame();

	virtual void HandlePacket( RakNet::Packet *packet);
	void HandleRequestAccepted(RakNet::Packet *packet);
	void PlayerNumIncrease(RakNet::Packet *packet);
private:
	Sprite *m_SplashScreen;
	Sprite *m_StartText;

};

#endif 