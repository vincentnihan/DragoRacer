#include "InputManager.h"
#include "GameManager.h"
#include <chrono>


InputManager::InputManager() : m_Thread(nullptr), m_runThread(false)
{
}

void InputManager::Init()
{
	//create DirectInput object
	DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&m_Di, NULL );

	//initialize keyboard
	m_Di->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL);
	m_Keyboard->SetDataFormat( &c_dfDIKeyboard );
	m_Keyboard->SetCooperativeLevel( m_Window, 
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	m_Keyboard->Acquire();

	//initialize mouse
	m_Di->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
	m_Mouse->SetDataFormat(&c_dfDIMouse);
	m_Mouse->SetCooperativeLevel(m_Window,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
	m_Mouse->Acquire();

	m_runThread = true;
	m_Thread = new std::thread(&InputManager::PollInput,this);
}

InputManager::~InputManager()
{
}

void InputManager::Shutdown()
{
	m_runThread = false;
	if (m_Thread)
	{
		m_Thread->join();
		delete m_Thread;
		m_Thread = nullptr;
	}

	m_Di->Release();
	m_Di = nullptr;
	m_Keyboard->Release();
	m_Keyboard = nullptr;
	m_Mouse->Release();
	m_Mouse = nullptr;

}

void InputManager::PollInput()
{
	while(m_runThread)
	{
		//poll state of the keyboard
		if (m_Keyboard)
		{
			m_Keyboard->Poll();
			if (!SUCCEEDED(m_Keyboard->GetDeviceState(256, (LPVOID)&m_KeyState)))
			{
				//keyboard device lost, try to re-acquire
				m_Keyboard->Acquire();
			}
		}
		
		//poll state of the mouse
		if (m_Mouse)
		{
			m_Mouse->Poll();
			if (!SUCCEEDED(m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
			{
				//mouse device lose, try to re-acquire
				m_Mouse->Acquire();
			}
		}
		

		//get mouse position on screen
		GetCursorPos(&m_Position);
		ScreenToClient(m_Window, &m_Position);

		PollMouse();
		PollKeyboard();

		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

void InputManager::PollMouse()
{
    static int oldPosX = 0;
    static int oldPosY = 0;
    int deltax = GetDeltaX();
    int deltay = GetDeltaY();
    
	//GameManager *gameManager = Singleton<GameManager>::GetInstance();

    //check mouse buttons 1-3
    for (int n=0; n<4; n++) {
        if (GetMouseButton(n))
            m_GameManager->HandleMouseClick(n);
    }
    
    //check mouse position
    if (GetPosX() != oldPosX || GetPosY() != oldPosY) {
        m_GameManager->HandleMouseMotionAbs(GetPosX(), GetPosY() );
        oldPosX = GetPosX();
        oldPosY = GetPosY();
    }
    
    //check mouse motion
    if (deltax != 0 || deltay ) {
        m_GameManager->HandleMouseMotionRel(deltax,deltay);
    }
    
    //check mouse wheel
    int wheel = GetDeltaWheel();
    if (wheel != 0) 
        m_GameManager->HandleMouseScroll(wheel);
    
}
    
void InputManager::PollKeyboard()
{

    static char old_keys[256];
    for (int n=0; n<255; n++) 
    {
        //check for key press
        if (GetKeyState(n) & 0x80) {
            m_GameManager->HandleKeyDown(n);
            old_keys[n] = GetKeyState(n);
        } 
        //check for release
        else if (old_keys[n] & 0x80) {
            m_GameManager->HandleKeyUp(n);
            old_keys[n] = GetKeyState(n);
        }
    }
}

bool InputManager::GetMouseButton( char button )
{
	return ( m_MouseState.rgbButtons[button] & 0x80 );
}