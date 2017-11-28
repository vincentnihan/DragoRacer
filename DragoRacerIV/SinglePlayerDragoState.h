#ifndef SINGLEPLAYERDRAGOSTATE_H
#define SINGLEPLAYERDRAGOSTATE_H

#include "GameState.h"
#include <string>

class GameManager;
class DragoGameManager;

class SinglePlayerDragoState : public GameState
{
public:
	SinglePlayerDragoState(GameManager *gameManager) : GameState(gameManager) {}
	virtual ~SinglePlayerDragoState();
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

private:
	DragoGameManager *m_DragoGameManager;
	float m_SoakInWinTime;
};

#endif
