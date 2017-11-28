#ifndef JOINSTATE_H
#define JOINSTATE_H

#include "NetworkedGameState.h"

class Sprite;
class AudioSample;
class GameManager;

class JoinState : public NetGameState
{
public:

	JoinState(GameManager *gameManager) : NetGameState(gameManager) {}
	~JoinState(){}

	virtual void Init();
	virtual void Shutdown();

	void LoadAssets();

	virtual void Pause(){}
	virtual void Resume(){};

	virtual void HandleKeyUp(int key);
	virtual void HandleKeyDown(int key){};
	virtual void HandleMouseClick(int button){};

	virtual void HandleMouseMotionAbs(int x,int y){};
	virtual void HandleMouseMotionRel(int deltax,int deltay){};
	virtual void HandleMouseScroll(int scroll) {};

	virtual bool Update(float deltaTime);

	virtual void HandlePacket( RakNet::Packet *packet);

	void HandleConnectionAccepted(RakNet::Packet *packet);
private:
	Sprite *m_SplashScreen;
	AudioSample *m_ConnectionSound;
	bool m_IsConnectionAccepted;
};

#endif