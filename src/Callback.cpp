#include "Callback.h"
#include "Utility.h"
#include "GDK/sampgdk.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>

namespace Callback
{
	static AMX* amx_allowed = NULL;
	static std::list<AMX*> amxPointers;

	std::list<AMX*>& GetAMXList()
	{
		return amxPointers;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
	{


		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
	{

		return true;
	}

	bool onGameModeInitCalled = false; // if OnGameModeInit has been called at least once
	PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit()
	{
		if (onGameModeInitCalled)
			return true;

		// Seed for rand()
		srand((unsigned int)time(NULL));

		onGameModeInitCalled = true;
		return true;
	}
}