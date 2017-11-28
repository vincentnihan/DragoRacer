#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Manager.h"
#include <dinput.h>
#include <thread>

class GameManager;

class InputManager : IManager
{
public:
	InputManager();
	~InputManager();

	void Init();
	void Shutdown();

	void SetWindowsHandle(HWND window) {m_Window = window;}
	void SetGameManager(GameManager *gameManager){m_GameManager = gameManager;}

	void PollInput();
	//void Update(float dt);
	bool GetMouseButton( char button );

	char GetKeyState(int key) { return m_KeyState[key]; }
	long GetPosX() { return m_Position.x; }
	long GetPosY() { return m_Position.y; }
	long GetDeltaX() { return m_MouseState.lX; }
	long GetDeltaY() { return m_MouseState.lY; }
	long GetDeltaWheel() { return m_MouseState.lZ; }
private:
	HWND m_Window; 
	IDirectInput8 *m_Di; 
	IDirectInputDevice8 *m_Keyboard;
	char m_KeyState[256]; 
	IDirectInputDevice8 *m_Mouse; 
	DIMOUSESTATE m_MouseState; 
	POINT m_Position; 
	GameManager *m_GameManager;
	std::thread *m_Thread;
	bool m_runThread;
protected:
	void PollMouse();
	void PollKeyboard();
};

#endif