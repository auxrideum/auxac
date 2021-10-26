#include "main.h"

#include "lib/imgui/imgui.h"

CCore* pCore = NULL;

CCore::CCore() {

}

CCore::~CCore() {

}

void CCore::HandleLibraryLoad(string moduleName) {

	if (m_bIsConnected) {
		BitStream bsSend;

		bsSend.Write((BYTE)iPacketRPC);
		bsSend.Write(RPC_OnLoadLibrary);

		// Facciamo sapere al server la path del modulo
		bsSend.Write((unsigned short)strlen(moduleName.c_str()));
		bsSend.Write(moduleName.c_str(), strlen(moduleName.c_str()));

		g_RakClient->Send(&bsSend, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
	}

#if DEVELOPER_VERSION == 1
	Log("%s should not be loaded.", moduleName.c_str());
#endif
}

void CCore::HandleMenu() {
	if (GetAsyncKeyState(VK_F2) & 1) {
		if (m_bIsGraphicUserInterfaceShown) {
			pMultiplayerSA->SetCursorStatus(false);
			m_bIsGraphicUserInterfaceShown = false;
		}
		else {
			pMultiplayerSA->SetCursorStatus(true);
			m_bIsGraphicUserInterfaceShown = true;
		}
	}
}