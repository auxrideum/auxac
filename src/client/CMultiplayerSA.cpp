#include "main.h"

struct stSAMP* g_SAMP = 0x0;

DWORD dwSAMPAddr = NULL;
CMultiplayerSA* pMultiplayerSA = NULL;

ScreenShotFn CMultiplayerSA::pfnScreenShot = NULL;

CMultiplayerSA::CMultiplayerSA() {
	// Aspettiamo l'handle di SAMP
	while (dwSAMPAddr == NULL)
		dwSAMPAddr = (DWORD)GetModuleHandleA("samp.dll");

	// Controllo versione di SAMP
	CheckVersion();

	// SAMP deve essere totalmente caricato, dwInfoAddr != NULL
	DWORD* dwInfoAddr = (DWORD*)(dwSAMPAddr + CNetGame_Addr);

	while (*dwInfoAddr == NULL)
		Sleep(150);

	// Struttura di SAMP
	g_SAMP = *(stSAMP**)(dwSAMPAddr + CNetGame_Addr);

	// Hook all'interfaccia RakNet di SAMP
	g_RakClient = new RakClient(g_SAMP->pRakClientInterface);
	g_SAMP->pRakClientInterface = new HookedRakClientInterface();

	InstallPatches();
	InstallHooks();

	SendChatMessage("{FF0000}[AUXAC]{FFFFFF} AUXAC has been started. %s.", VERSION_BUILD);

	m_bIsInitializedMultiplayerSA = true;
}

CMultiplayerSA::~CMultiplayerSA() {
	g_RakClient = 0x00;
	g_SAMP = 0x00;
}

void CMultiplayerSA::InstallHooks() {
	pfnScreenShot = (ScreenShotFn)DetourFunction((PBYTE)(dwSAMPAddr + 0x755F0), (BYTE*)OnTakeScreen);
};

void CMultiplayerSA::InstallPatches() {
	// Connettiamoci subito al server
	mem::set((void*)(dwSAMPAddr + 0x8942), 0x90, 5);

	// Rimuoviamo il delay quando quittiamo
	mem::set((void*)(dwSAMPAddr + 0xC5077), 0x90, 6);

};

void CMultiplayerSA::CheckVersion() {
	DWORD dwCode = *(DWORD*)(dwSAMPAddr + 0xBABE);

	if (dwCode != NULL) {
		if (dwCode == eVersionMultiplayerSA::SAMP037_R4) {
			Log("%s detected.", SAMP_VERSION);
		}
		else {
			char szErrorMessage[128];
			sprintf_s(szErrorMessage, sizeof szErrorMessage, "AUXAC can't recognise your current SA-MP version. %s is the only version supported.", SAMP_VERSION);

			MessageBox(NULL, szErrorMessage, "Unknown SAMP version", MB_ICONERROR);

			FreeLibrary(g_hDllModule);
		}
	}
	else {
		Log("Couldn't recognise SA-MP version. Aborting...");
		FreeLibrary(g_hDllModule);
	}
};

void CMultiplayerSA::SendChatMessage(const char* text, ...)
{
	va_list ap;

	if (text == NULL)
		return;

	char tmp[512];
	memset(tmp, 0, 512);

	va_start(ap, text);
	vsnprintf_s(tmp, sizeof(tmp) - 1, text, ap);
	va_end(ap);

	typedef int(__cdecl* __AddChatMessage)(void* g_Chat, char* szBuf, ...);
	__AddChatMessage AddChatMessage = (__AddChatMessage)(dwSAMPAddr + CChatWindow_Print);

	DWORD g_Chat = *(DWORD*)(dwSAMPAddr + CChatWindowAddr);

	if(g_Chat != NULL)
		AddChatMessage((void*)g_Chat, tmp);
}

void CMultiplayerSA::ShowDialog(int send, int dialogID, int typedialog, char* caption, char* text, char* button1, char* button2)
{
	uint32_t func = dwSAMPAddr + 0x6FFE0;
	uint32_t data = dwSAMPAddr + 0x26E9C8;

	__asm mov eax, dword ptr[data]
	__asm mov ecx, dword ptr[eax]
	__asm push send
	__asm push button2
	__asm push button1
	__asm push text
	__asm push caption
	__asm push typedialog
	__asm push dialogID
	__asm call func

	return;
}

void CMultiplayerSA::SetCursorStatus(bool bEnabled)
{
	void* pGame = (void*)(dwSAMPAddr + 0x26EA24);

	auto pfnSetCursorMode = (int(__thiscall*)(void*, int, int))(dwSAMPAddr + 0xA0720);
	auto pfnProcessInputEnabling = (int(__thiscall*)(void*))(dwSAMPAddr + 0xA0600);

	if(bEnabled) pfnSetCursorMode(pGame, 3, 0);
	else {
		pfnSetCursorMode(pGame, 0, 1);
		pfnProcessInputEnabling(pGame);
	}

}

void CMultiplayerSA::OnTakeScreen() {
	if (g_RakClient->IsPlayerConnected()) {
		BitStream bsSend;

		bsSend.Write((BYTE)iPacketRPC);
		bsSend.Write(RPC_OnTakeScreenImage);

		g_RakClient->Send(&bsSend, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
	}

	pfnScreenShot();
}