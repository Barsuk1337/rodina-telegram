#include "plugin.h"
#include "Utility.h"
#include "GDK/sampgdk.h"
#include "Callback.h"
#include "SDK/samp-sdk/amx/amx.h"
#include "Hooks.h"
#include "Shared/curl/curl.h"

#include <stdio.h>
#include <Shared/tgbot/tgbot.h>

cell AMX_NATIVE_CALL SendTelegramProtectMessageProc(AMX* pAmx, cell* pParams)
{
	// Make sure the parameter count is correct.
	CHECK_PARAMS(3, "SendTelegramProtectMessage");

	// Make sure the player is connected
	if (!sampgdk::IsPlayerConnected(pParams[1])) return 0;

	CURL* curl; //our curl object
	curl = curl_easy_init();

	if (curl) 
	{
		const char* botToken = "6777919855:AAEw3Z-9AvNC3PEQWpMYKFwUJTWqB2n8O8k";

		std::string url = "https://api.telegram.org/bot" + std::string(botToken) + "/sendMessage"; 
		std::string message = "Ваш код подтверждения: " + std::string((char*)pParams[3]); 

		// Формируем JSON-строку с данными для отправки 
		std::string json = "{\"chat_id\":\"" + std::string((char*)pParams[2]) + "\",\"text\":\"" + message + "\"}"; 

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str()); 
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Content-Type: application/json"); 

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	return 1;
}

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

	// Initialize cURL
	curl_global_init(CURL_GLOBAL_ALL);

	// hook amx_register
	InstallAmxHooks();

	TgBot::Bot bot("6777919855:AAEw3Z-9AvNC3PEQWpMYKFwUJTWqB2n8O8k");
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });

    try 
	{
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);

        while (true) 
		{
            printf("Long poll started\n");
            longPoll.start();
        }
    } 
	catch (TgBot::TgException& e) 
	{
        printf("error: %s\n", e.what());
    }

	// Print out that we've loaded successfully.
	Utility::Printf("Telegram Protect has loaded successfully");

	// return SampGDK load value
	return load;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	// The plugin has been unloaded, let them know we're leaving :(
	Utility::Printf("Unloaded Telegram Protect");

	// Clean up
	// Not really necessary
	// curl_global_cleanup();
	
	return sampgdk::Unload();
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{ "SendTelegramProtectMessage", SendTelegramProtectMessageProc },
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