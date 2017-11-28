#ifndef MANAGER_H
#define MANAGER_H

#include "Singleton.h"

class IManager
{
public:

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

};

#endif