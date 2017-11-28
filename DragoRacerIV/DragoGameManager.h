#ifndef DRAGOGAMEMANAGER_H
#define DRAGOGAMEMANAGER_H

#include "GameManager.h"
#include <vector>
#include "CoreMath.h"

namespace RakNet
{
	class Packet;
}

class CDragoRacer;
class Sprite;

class DragoGameManager : public GameManager
{
public:
	struct DragoInputKeys
	{
		//customize input to test networking easier
		int m_AccelerationKey;
		int m_NitroKey;
	};

	DragoGameManager();
	virtual ~DragoGameManager();
	void Init();
	void Update(float dt);
	
	void DrawGameBG();
	void DrawResult(bool isWin);

	//used for online
	void HostCreatePlayer();
	void ClientCreatePlayer(RakNet::Packet *packet);
	void HandleAccelerate(RakNet::Packet *packet);
	void HandlePosition(RakNet::Packet *packet);
	void HandleDecelerate(RakNet::Packet *packet);
	//used for sp 
	void SetNumPlayers(unsigned int numPlayers) {}
	void CreateRacer(int racerNum = 1);
	CDragoRacer* CheckForWinner();

	bool TestAccelerate(bool testDecelerate = false);
	bool TestNitro(int key);

	void InitInputKeys();
	void InitStartingPositions();
	void HandleUpdate();
	void UpdateCountdownTimer(float dt);
	void HandlePlayerNum(RakNet::Packet *packet);

	static const unsigned int MAXPLAYERS = 4;
	
	int m_NumPlayers;
	int m_playerLabel;

private:
	static const float COUNTDOWNTIME;

	std::vector<DragoInputKeys> m_DragRacerInputKeys;
	std::vector<Vector2> m_StartingPositions;
	typedef std::vector<CDragoRacer*>::iterator DRAGRACERSITERATOR;
	//typedef std::vector<CNetDragoRacer*>::iterator NETDRAGRACERSITERATOR;
	std::vector<CDragoRacer*> m_DragRacers;
	Sprite* m_Background;
	Sprite* m_CountdownSprite;
	Sprite* m_RaceResultSprite;
	float m_CountdownTimer;
	bool m_IsActive;

};

#endif