#ifndef NETWORKEDGAMESTATE_H
#define NETWORKEDGAMESTATE_H

#include "GameState.h"
class GameManager;

namespace RakNet
{
	class Packet;
}

class NetGameState : public GameState
{
public:
	NetGameState(GameManager *gameManager) : GameState(gameManager) {}
	~NetGameState(){}

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleKeyUp(int key) = 0;
	virtual void HandleKeyDown(int key) = 0;
	virtual void HandleMouseClick(int button) = 0;

	virtual void HandleMouseMotionAbs(int x,int y) = 0;
	virtual void HandleMouseMotionRel(int deltax,int deltay) = 0;
	virtual void HandleMouseScroll(int scroll) = 0;

	virtual bool Update(float deltaTime) = 0;

	virtual void HandlePacket(RakNet::Packet *packet) = 0;

};

#endif