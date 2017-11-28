#include "GameState.h"
#include "GameManager.h"

void GameState::PushState(GameState* newState)
{
	m_GameManager->PushState(newState);
}
	
bool GameState::PopState()
{
	return m_GameManager->PopState();
}

void GameState::ChangeState(GameState* state)
{
	m_GameManager->ChangeState(state);
}