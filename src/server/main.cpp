#include "main.h"

extern void* pAMXFunctions;
void** ppPluginData = nullptr;

using sampgdk::logprintf;

bool bIsPluginInitialized = false;
bool bIsPluginTerminated = false;

CCRakServer* pRakServer = nullptr;

AMX_NATIVE_INFO PluginNatives[] =
{
	{"IsPlayerUsingAux", Natives::IsPlayerUsingAux},
	{"GetPlayerHWID", Natives::GetPlayerHWID},
	{ 0, 0 }
};

int main() {
	//FindGetPacketIdAddress();

	pVerifiedPlayer = new CVerifiedPlayer();
	pHandlePacket = new CHandlePacket();
	pHookServerFunctions = new CHookServerFunctions();
	
	while (!bIsPluginInitialized) {
#ifdef WIN32
		Sleep(15);
#else
		usleep(1);
#endif
	}

	return EXIT_SUCCESS;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void** ppData) {

	ppPluginData = ppData;
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];

	bool ret = sampgdk::Load(ppData);

#ifdef WIN32
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&main, 0, 0, 0);
#else
	pthread_t thread0;
	pthread_create(&thread0, NULL, main, NULL);
#endif

	logprintf("[AUXAC] Plugin has been loaded correctly.");

	return ret;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	sampgdk::Unload();
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
	Callback::GetAMXList().push_back(amx);

	if (!bIsPluginInitialized) {
		int(*pfn_GetRakServer)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_RAKSERVER];
		pRakServer = (CCRakServer*)pfn_GetRakServer();

		int (*pfn_GetConsole)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_CONSOLE];
		void* pConsole = (void*)pfn_GetConsole();

		if (pRakServer && pConsole) {

			sampgdk_logprintf("%p", pRakServer);

			if (!pHookServerFunctions->Install())
				sampgdk_logprintf("[AUXAC] Couldn't load correctly. %p", pRakServer);

			// Aggiungiamo il nostro AC alle rules, i players lo vedranno
#ifdef _WIN32
			typedef void(__thiscall* AddStringVariableFn)(void*, const char*, int, const char*, void*);
			AddStringVariableFn pfnAddStringVariable = (AddStringVariableFn)0x48C570;
#else
			typedef void(*AddStringVariableFn)(void*, const char*, int, const char*, void*);
			AddStringVariableFn pfnAddStringVariable = (AddStringVariableFn)0x80a08f0;
#endif

			char szBuf[5];
			snprintf(szBuf, sizeof szBuf, "%s", PLUGIN_VERSION);

			pfnAddStringVariable(pConsole, "auxac", 4, szBuf, 0);

			bIsPluginInitialized = true;
		}
		else {
			logprintf("[AUXAC] Plugin could not be initialized.");
		}

	}

	return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
	Callback::GetAMXList().remove(amx);
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
	sampgdk::ProcessTick();
}