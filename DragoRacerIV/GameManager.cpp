#include "GameManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "RenderingManager.h"
#include "NetworkManager.h"
#include "GameState.h"
#include "DragoGameManager.h"
#include <strsafe.h>



GameManager::GameManager(void) : m_IsGameOver(false)
{
}


GameManager::~GameManager(void)
{
}

LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	
    switch( msg )
	{
		 case WM_QUIT:
		 case WM_CLOSE:
		 case WM_DESTROY:
			Singleton<DragoGameManager>::GetInstance()->SetIsGameOver(true);
			break;
	  
		default:
			break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void GameManager::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	DWORD dwStyle, dwExStyle;
	RECT windowRect;

	//set window dimensions
	windowRect.left = 0;
	windowRect.right = 640;
	windowRect.top = 0;
	windowRect.bottom = 480;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX); 

	//fill the struct with info
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WinProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = NULL;
	wc.hCursor	   = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "title";
	wc.hIconSm	   = NULL;

	//set up the window with the class info
	RegisterClassEx(&wc);

	dwStyle = WS_OVERLAPPEDWINDOW;
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	//adjust window to true requested size
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	//create the program window
	m_hWnd = CreateWindowEx( 0,
	   "title",								 //window class
	   "",								 //title bar
	   dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,	  
	   0, 0,											 //x,y coordinate
	   windowRect.right - windowRect.left,			   //width of the window
	   windowRect.bottom - windowRect.top,			   //height of the window
	   0,											 //parent window
	   0,											 //menu
	   hInstance,									  //application instance
	   0);											//window parameters

	//was there an error creating the window?
	if (!m_hWnd) 	
	{
		LPTSTR lpszFunction = TEXT("GetProcessId");
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError(); 

		FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

    // Display the error message and exit the process
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
		return; 
	}

	ShowWindow(m_hWnd, nCmdShow );
	UpdateWindow(m_hWnd);
}

void GameManager::Init()
{
	m_RenderingManager = Singleton<RenderingManager>::GetInstance();
	m_InputManager = Singleton<InputManager>::GetInstance(); 
	m_AudioManager = Singleton<AudioManager>::GetInstance(); 
	m_NetworkManager = Singleton<NetworkManager>::GetInstance(); 

	m_RenderingManager->SetWindowHandle(m_hWnd);
	m_RenderingManager->Init();
	m_InputManager->SetGameManager(this);
	m_InputManager->SetWindowsHandle(m_hWnd);
	m_InputManager->Init();
	m_AudioManager->Init();
	m_NetworkManager->SetGameManager(this);
	m_NetworkManager->Init();

	m_OldTime = timeGetTime();    
}

void GameManager::Shutdown()
{
	ChangeState(nullptr);

	if (m_RenderingManager)
	{
		m_RenderingManager->Shutdown();
		delete m_RenderingManager;
	}
		
	if (m_AudioManager)
	{
		m_AudioManager->Shutdown();
		delete m_AudioManager;
	}
		

	if (m_NetworkManager)
	{
		m_NetworkManager->Shutdown();
		delete m_NetworkManager;
	}

	if (m_InputManager)
	{
		m_InputManager->Shutdown();
		delete m_InputManager;
	}
		
}

void GameManager::Run(GameState *state)
{
	state->Init();
	m_GameStates.push_back(state);

	MSG msg;
	while(!IsGameOver())
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD NewTime = timeGetTime();    
		float dt = (float) (NewTime - m_OldTime)/1000; //delta time
		m_OldTime = NewTime;

		//keep it somewhat fixes
		if (dt > (0.016f)) dt = (0.016f);  
		if (dt < 0.001f) dt = 0.001f;

		Update(dt);
		m_RenderingManager->Update(dt);
		m_AudioManager->Update(dt);

		for(GAMESTATEITER it = m_GameStates.begin(); it != m_GameStates.end();++it)
		{
			//if true state has been changed
			if((*it)->Update(dt))
				break;
		}
	}

}

GameState* GameManager::GetCurrentState()
{
	GameState *currentGameState = nullptr;
	if(m_GameStates.size() > 0)
	{
		currentGameState = m_GameStates.back();	
	}
	return currentGameState;
}

void GameManager::PushState(GameState* state)
{
	//if there is a current state, put it in pause
	//push new state
	GAMESTATEITER it = m_GameStates.end();
	if(*it)
	{
		(*it)->Pause();
	}
	m_GameStates.push_back(state);
}

//can only be called if this state was pushed on, do a check, otherwise user should call changestate
bool GameManager::PopState()
{
	//clean up state
	//remove state
	GameState *currentGameState = nullptr;
	if(m_GameStates.size() > 0)
	{
		currentGameState = m_GameStates.back();	
		currentGameState->Shutdown();
	}

	m_GameStates.pop_back();
		
	return true;
}

//iterate through vector and clean up all states before changing to state
void GameManager::ChangeState(GameState* state)
{
	for(GAMESTATEITER it = m_GameStates.begin(); it != m_GameStates.end();++it)
	{
		(*it)->Shutdown();
	}
	
	m_GameStates.clear();
	if(state)
	{
		state->Init();
		m_GameStates.push_back(state);
	}
	
}

void GameManager::HandleKeyUp(int key)
{
	if(m_GameStates.size() > 0)
	{
		GameState *gameState = m_GameStates.back();
		if(gameState)
		{
			gameState->HandleKeyUp(key);
		}
	}
}

void GameManager::HandleKeyDown(int key)
{
	if(m_GameStates.size() > 0)
	{
		GameState *gameState = m_GameStates.back();
		if(gameState)
		{
			gameState->HandleKeyDown(key);
		}
	}
}

void GameManager::HandleMouseClick(int button)
{
	if(m_GameStates.size() > 0)
	{
		GameState *gameState = m_GameStates.back();
		if(gameState)
		{
			gameState->HandleMouseClick(button);
		}
	}
}

void GameManager::HandleMouseMotionAbs(int x,int y)
{
	if(m_GameStates.size() > 0)
	{
		GameState *gameState = m_GameStates.back();
		if(gameState)
		{
			gameState->HandleMouseMotionAbs(x,y);
		}
	}
}

void GameManager::HandleMouseMotionRel(int deltax,int deltay)
{
	if(m_GameStates.size() > 0)
	{
		GameState *gameState = m_GameStates.back();
		if(gameState)
		{
			gameState->HandleMouseMotionRel(deltax,deltay);
		}
	}
}

void GameManager::HandleMouseScroll(int scroll)
{
	if(m_GameStates.size() > 0)
	{
		GameState *gameState = m_GameStates.back();
		if(gameState)
		{
			gameState->HandleMouseScroll(scroll);
		}
	}
}