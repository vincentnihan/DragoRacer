#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#define _WINSOCKAPI_ 
#include <Windows.h>
#include "Manager.h"

#include <vector>

class GameState;
class InputManager;
class RenderingManager;
class AudioManager;
class NetworkManager;

class GameManager : IManager
{
public:

	enum GameStates
	{
		GS_Intro = 0,
		GS_Join,
		GS_Host,
		GS_SinglePlayer,
		GS_NetDragoState,
		GS_SIZE
	};

	GameManager(void);
	~GameManager(void);

	virtual void Init();

	virtual void Shutdown();

	virtual void InitWindow(HINSTANCE hInstance, int nCmdShow);

	virtual void Run(GameState *state);

	virtual void Update(float dt){}

	GameState* GetCurrentState();

	void PushState(GameState* state);
	//can only be called if this state was pushed on, do a check, otherwise user should call changestate
	bool PopState();

	//iterate through vector and clean up all states before changing to state
	void ChangeState(GameState* state);

	virtual void HandleKeyUp(int key);
	virtual void HandleKeyDown(int key);
	virtual void HandleMouseClick(int button);

	void HandleMouseMotionAbs(int x,int y);
	void HandleMouseMotionRel(int deltax,int deltay);
	void HandleMouseScroll(int scroll);

	bool IsGameOver() const {return m_IsGameOver;}
	void SetIsGameOver(bool isGameOver) {m_IsGameOver = isGameOver;}

	RenderingManager *GetRenderingManager() const { return m_RenderingManager;}
	InputManager *GetInputManager() const {return m_InputManager; }
	AudioManager *GetAudioManager() const {return m_AudioManager; }
	NetworkManager *GetNetworkManager() const {return m_NetworkManager;}

private:
	HWND m_hWnd;

	RenderingManager *m_RenderingManager;
	InputManager *m_InputManager;
	AudioManager *m_AudioManager;
	NetworkManager *m_NetworkManager;

	typedef std::vector<GameState*>::iterator GAMESTATEITER;
	std::vector<GameState*> m_GameStates;

	DWORD m_OldTime;
	bool m_IsGameOver;

protected:
};

#endif
