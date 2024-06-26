#pragma once

#include <list>
#include "SDK/samp-sdk/amx/amx.h"
#include <string>

struct PAWNArray
{
	cell* address;
	size_t length;
};

namespace Callback
{
	// PURPOSE: Execute a PAWN callback.
	// REQUIRES: A public function name, and whatever arguments.
	// PROMISE: That public function will be called in all PAWN scripts.
	// NOTES: Arguments pushed in reverse order.
	cell Execute(const char* szFunction, const char* szFormat, ...);

	// PURPOSE: Get a list of loaded AMX scripts (all gamemodes and filterscripts)
	// REQUIRES: an std::list to hold it all.
	std::list<AMX*>& GetAMXList();
}