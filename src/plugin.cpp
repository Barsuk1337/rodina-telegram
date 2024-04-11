#include "plugin.h"
#include "Utility.h"
#include "GDK/sampgdk.h"
#include "Callback.h"
#include "SDK/samp-sdk/amx/amx.h"

#include <stdio.h>
#include "tgbot/tgbot/Bot.h"
#include "tgbot/tgbot/net/TgLongPoll.h"

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	// Handle sampGDK ticking.
	return sampgdk::ProcessTick();
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	// Load SampGDK
	bool load = sampgdk::Load(ppData);

	// Initialize
	Utility::Initialize(ppData);

	TgBot::Bot bot("6777919855:AAEw3Z-9AvNC3PEQWpMYKFwUJTWqB2n8O8k");

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {

        bot.getApi().sendMessage(message->chat->id, "Hi");
    });

	bot.getEvents().onCommand("reg", [&bot](TgBot::Message::Ptr message) {

        bot.getApi().sendMessage(message->chat->id, "Your code: " + std::string((char*)message->chat->id));
    });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {

        printf("User wrote %s\n", message->text.c_str());
        bot.getApi().sendMessage(message->chat->id, "Bot message: " + message->text);
    });

    try 
	{
        Utility::Printf("Telegram Protect has loaded, bot name: %s", bot.getApi().getMe()->username.c_str());

        TgBot::TgLongPoll longPoll(bot);

        while (true) 
		{
            longPoll.start();

			usleep(2000);
        }
    } 
	catch (TgBot::TgException& e) 
	{
        Utility::Printf("error: %s | %s", e.what(), e);
    }

	// return SampGDK load value
	return load;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	// The plugin has been unloaded, let them know we're leaving :(
	Utility::Printf("Unloaded Telegram Protect");

	return sampgdk::Unload();
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{ 0, 0 }
};

AMX_NATIVE_INFO* GetPluginNatives()
{
	return PluginNatives;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *pAmx)
{
	// Add a new AMX script to our list of running AMX files.
	Callback::GetAMXList().push_back(pAmx);
	
	// Allow the new loaded script to use our plugin's functions.
	return amx_Register(pAmx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *pAmx)
{
	// Remove the AMX scripts from our list of loaded AMX files.
	Callback::GetAMXList().remove(pAmx);
	return AMX_ERR_NONE;
}