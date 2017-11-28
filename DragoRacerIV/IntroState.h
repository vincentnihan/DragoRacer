#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "GameState.h"
#include <string>

class GameManager;

class IntroState : public GameState
{
public:
	IntroState(GameManager *gameManager) : GameState(gameManager) {}
	virtual ~IntroState();
	virtual void Init();
	virtual void Shutdown();

	virtual void Pause() {}
	virtual void Resume() {}

	virtual void HandleKeyUp(int key);
	virtual void HandleKeyDown(int key) {}
	virtual void HandleMouseClick(int button){}

	virtual void HandleMouseMotionAbs(int x,int y) {}
	virtual void HandleMouseMotionRel(int deltax,int deltay){}
	virtual void HandleMouseScroll(int scroll) {}

	virtual bool Update(float deltaTime);

private:
	class Sprite *m_SplashScreen;
	std::string m_BGMusic;
	class AudioSample *m_AudioSample;

};

#endif
