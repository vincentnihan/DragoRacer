#include "IntroState.h"
#include "DragoGameManager.h"
#include "InputManager.h"
#include "RenderingManager.h"
#include "NetworkManager.h"
#include "AudioManager.h"
#include "Sprite.h"
#include "HostState.h"
#include "JoinState.h"
#include "SinglePlayerDragoState.h"

//only need a splash screen here and listen in on input for 1 2 or 3
	//this will take you to reg game, host, or join screens depending on your 
	//option
	//read from a file the ipaddress and port

IntroState::~IntroState()
{

}

void IntroState::Init()
{
	m_DesiredState = GameManager::GS_Intro;
	//m_GameManager = Singleton<DragoGameManager>::GetInstance();
	
	m_SplashScreen = new Sprite();
	m_SplashScreen->LoadImage("Lundgren_Ivan_DragoRacer.jpg");
	m_SplashScreen->SetFullScreen();
	m_GameManager->GetRenderingManager()->AddSprite(m_SplashScreen);

	m_AudioSample = m_GameManager->GetAudioManager()->Load("Rocky 8bit.mp3");
	if(m_AudioSample)
	{
		m_GameManager->GetAudioManager()->Play(m_AudioSample);
	}
}

bool IntroState::Update(float deltaTime)
{
	if(m_DesiredState == GameManager::GS_SinglePlayer)
	{
		ChangeState(new SinglePlayerDragoState(m_GameManager));
		return true;
	}
	else if(m_DesiredState == GameManager::GS_Join)
	{
		ChangeState(new JoinState(m_GameManager));
		return true;
	}
	else if(m_DesiredState == GameManager::GS_Host)
	{
		ChangeState(new HostState(m_GameManager));
		return true;
	}

	return false;
}

void IntroState::Shutdown()
{
	m_GameManager->GetRenderingManager()->RemoveSprite(m_SplashScreen);
	m_GameManager->GetAudioManager()->StopAll();
	if(m_AudioSample)
	{
		delete m_AudioSample;
	}
	if (m_SplashScreen)
	{
		delete m_SplashScreen;
	}
}

void IntroState::HandleKeyUp(int key)
{
	if(key == DIK_ESCAPE)
		m_GameManager->SetIsGameOver(true);

	if(key == DIK_1)
	{
		m_DesiredState = GameManager::GS_SinglePlayer;
	}
		

	if(key == DIK_2)
	{
		//host game
		m_DesiredState = GameManager::GS_Host;
	}

	if(key == DIK_3)
	{
		//join game
		m_DesiredState = GameManager::GS_Join;
	}
}
