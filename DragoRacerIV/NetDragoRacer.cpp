#include "NetDragoRacer.h"

CNetDragoRacer::CNetDragoRacer()
{
	//SetNetworkIDManager(Singleton<NetworkManager>::GetInstance()->GetNetworkIDManager());
}

void CNetDragoRacer::Accelerate()
{
	if (isMaster == true)
	{
		//__super::Accelerate();
	}
	__super::Accelerate();



}

void CNetDragoRacer::Decelerate()
{
	if (isMaster == true)
	{
		//__super::Decelerate();
	}
	__super::Decelerate();

}