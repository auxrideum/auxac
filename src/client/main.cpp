/*

	TO DO:
		- MD5 checksum di GTA


*/

#include "main.h"

#include "CModulesList.h"
#include "CHookFunctions.h"

HMODULE g_hDllModule = NULL;
FILE* g_flLog = NULL;

char g_szWorkingDirectory[MAX_PATH];
char g_szFileName[MAX_PATH];

BOOL bIsApplicationClosed = false;
BOOL bIsSampInitialized = false;

int mainThread() {
#if DEVELOPER_VERSION == 1
	Log("mainThread reached. Version: %s WorkingDir: %s", VERSION_BUILD, g_szWorkingDirectory);
#endif
	pModulesList = new CModulesList();
	pCore = new CCore();
	pD3D9Hook = new CD3D9Hook();

	// Controlliamo i moduli del processo
	pModulesList->Update();
	
	// Installiamo dei call hooks per LoadLibrary e CreateThread
	pHookFunctions->Install();
	
	// Abilitiamo delle patch per GTA:SA
	pPatchGameSA->Apply();
	
	// Controlliamo la v-table di D3D9 e installiamo l'hook
	pD3D9Hook->Install();

	// Inizilizziamo SAMP
	pMultiplayerSA = new CMultiplayerSA();
	
	while (!bIsApplicationClosed) {
		pModulesList->Update();

		if (GetAsyncKeyState(VK_F3) & 1) {
		
			char** videoModeList = (char**)0xC920D0;

			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					pMultiplayerSA->SendChatMessage("test: %s\n", videoModeList[i][j]);
				}
			}

			pMultiplayerSA->SendChatMessage("0x%p", videoModeList);
		}
		Sleep(100);
	}

	return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	switch (dwCallReason)
	{
		case DLL_PROCESS_ATTACH:
		{
#if DEVELOPER_VERSION == 0
			// Abilitiamo la console
			AllocConsole();
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
#endif
			// Il nostro modulo
			g_hDllModule = hModule;

			// Non dobbiamo essere injectati, dobbiamo partire all'inizio
			if (*(int*)0xC8D4C0 >= 6)
				FreeLibraryAndExitThread(g_hDllModule, 0);

			// Registriamo il nostro file
			GetModuleFileName(g_hDllModule, g_szFileName, sizeof(g_szFileName));

			// Settiamo la directory in cui lavoriamo
			pMiscFunctions->SetWorkingDirectory(g_szWorkingDirectory);
			
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&mainThread, 0, 0, 0);
			break;
		}
		case DLL_PROCESS_DETACH:
		{
		#if DEVELOPER_VERSION == 1
			Log("DLL_PROCESS_DETACH reached. Exiting...");
		#endif

			delete pModulesList;
			delete pCore;
			delete pMultiplayerSA;

			fclose(g_flLog);
		}
	}

	return 1;
}

void Log(const char* fmt, ...)
{
	SYSTEMTIME time;
	va_list ap;

	if (g_flLog == NULL)
	{
		// char filename[512];
		// snprintf(filename, sizeof(filename), "%s\\%s", g_szWorkingDirectory, "auxac.log");

		g_flLog = fopen("auxac.log", "w");
		if (g_flLog == NULL)
			return;
	}

	GetLocalTime(&time);

	//fprintf(g_flLog, "[%02d:%02d:%02d.%03d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	va_start(ap, fmt);
	vfprintf(g_flLog, fmt, ap);
	va_end(ap);
	fprintf(g_flLog, "\n");
	fflush(g_flLog);
}