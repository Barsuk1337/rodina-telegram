#include "Hooks.h"
#include "Utility.h"
#include "plugin.h"

#ifdef _WIN32
	#include <Psapi.h>
#endif

extern void *pAMXFunctions;

static SubHook amx_Register_hook;

bool bHooksInstalled = false;

AMX_NATIVE GetGravityFunc = NULL;

cell AMX_NATIVE_CALL SecretProc(AMX* pAmx, cell* pParams)
{
	// If the number of parameters is greater than 0
	if (pParams[0] > 0)
	{
		// Get a list of all natives the plugin registered
		AMX_NATIVE_INFO* natives = GetPluginNatives();

		// gateway function sends the function name as a string as the first parameter always, so get the function we're trying to call
		char* func;
		amx_StrParam(pAmx, pParams[1], func);

		// Remove the first parameter string from the params
		pParams[0] -= 4;
		pParams[1] = pParams[0];

		// Loop through all natives registered
		int i = 0;
		while (natives[i].func > 0)
		{
			// If a native matches the function we're trying to call
			if (!strcmp(natives[i].name, func))
			{
				// Call it, and return 
				return natives[i].func(pAmx, &pParams[1]);
				break;
			}
			i++;
		}
	}

	// If we couldn't find the function, or no parameters were sent, call GetGravity as usual.
	return GetGravityFunc(pAmx, pParams);
}

int AMXAPI HOOK_amx_Register(AMX *amx, AMX_NATIVE_INFO *nativelist, int number)
{
	SubHook::ScopedRemove remove(&amx_Register_hook);

	int i = 0;

	// Loop through all natives being registered
	while (!bHooksInstalled && nativelist[i].func > 0)
	{
		// If one matches GetGravity
		if (!strcmp(nativelist[i].name, "GetGravity"))
		{
			// Hook it
			bHooksInstalled = true;
			GetGravityFunc = nativelist[i].func;
			nativelist[i].func = SecretProc;
			break;
		}
		i++;
	}

	// note: will not allow YSF redirected natives...
	return amx_Register(amx, nativelist, number);
}


void InstallAmxHooks()
{
	amx_Register_hook.Install((void*)*(unsigned long*)((unsigned long)pAMXFunctions + (PLUGIN_AMX_EXPORT_Register * 4)), (void*)HOOK_amx_Register);
}

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask)
{
	for (; *mask; ++mask, ++data, ++pattern)
	{
		if (*mask == 'x' && *data != *pattern)
			return false;
	}
	return (*mask) == NULL;
}
