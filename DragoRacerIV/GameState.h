#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameManager;

class GameState
{
public:
	GameState(GameManager* gameManager) { m_GameManager = gameManager; }
	~GameState(){}

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

	void PushState(GameState* newState);
	//can only be called if this state was pushed on, do a check, otherwise user should call changestate
	bool PopState();

	//iterate through vector and clean up all states before changing to state
	void ChangeState(GameState* state);
	void SetGameManager(GameManager* gameManager) {m_GameManager = gameManager;}
protected:
	GameManager *m_GameManager;
	int m_DesiredState;

};

#endif