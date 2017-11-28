#include "DragoGameManager.h"
#include "IntroState.h"

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	DragoGameManager* gameManager = Singleton<DragoGameManager>::GetInstance();
	gameManager->InitWindow(hInstance,nCmdShow);
	gameManager->Init();
	gameManager->Run(new IntroState(gameManager));
	gameManager->Shutdown();
	return 0;
}
