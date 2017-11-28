#ifndef NETDRAGORACER_H
#define NETDRAGORACER_H

#include "NetworkManager.h"
#include "DragoRacer.h"


class CNetDragoRacer : public CDragoRacer, public RakNet::NetworkIDObject
{
public:
	CNetDragoRacer();
	~CNetDragoRacer(){}

	void Accelerate();
	void Decelerate();

	bool isMaster = false;
};

#endif