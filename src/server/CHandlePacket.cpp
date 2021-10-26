#include "main.h"

CHandlePacket* pHandlePacket;

void CHandlePacket::Process(int iRpcId, RakNet::BitStream bsData, int iPlayerId) {
	switch (iRpcId) {
		case RPC_OnFirstConnection:
		{
			char szHWID[256];
			char szVersion[16];

			bsData.ReadString((unsigned char*)szVersion);
			bsData.ReadString((unsigned char*)szHWID);

			pVerifiedPlayer->m_Player[iPlayerId].bIsPlayerUsingClient = true;
			pVerifiedPlayer->m_Player[iPlayerId].sPlayerHWID = szHWID;
			pVerifiedPlayer->m_Player[iPlayerId].sVersion = szVersion;

			// ** aggiungere un controllo prima di mandare la config
			SendServerConfig(iPlayerId);

			sampgdk_logprintf("[AUXAC] %s (%d) has joined the server.", pVerifiedPlayer->m_Player[iPlayerId].sVersion.c_str(), iPlayerId);

			break;
		}
		case RPC_OnLoadLibrary: {
			char szPathLibrary[MAX_PATH];

			bsData.ReadString((unsigned char*)szPathLibrary);
			std::string pathLibrary(reinterpret_cast<char*>(szPathLibrary));

			sampgdk_logprintf("Received: %d PlayerId: %d Path: %s", RPC_OnLoadLibrary, iPlayerId, pathLibrary.c_str());

			Callback::OnLoadLibrary(iPlayerId, pathLibrary.c_str());

			break;
		}
		case RPC_OnTakeScreenImage: {
			char szBuf[128];
			snprintf(szBuf, 128, "PlayerId: %d took a screenshot.", iPlayerId);
			sampgdk_SendClientMessageToAll(-1, szBuf);
			break;
		}
		case RPC_OnModulesUpdate: {
			char szPathLibrary[128];
			bsData.ReadString((unsigned char*)szPathLibrary);

			if (std::find(pVerifiedPlayer->m_Player[iPlayerId].modules.begin(), pVerifiedPlayer->m_Player[iPlayerId].modules.end(), szPathLibrary) == pVerifiedPlayer->m_Player[iPlayerId].modules.end()) {
				pVerifiedPlayer->m_Player[iPlayerId].modules.push_back(szPathLibrary);
			}

			sampgdk_logprintf("%s", szPathLibrary);

			break;
		}
	}
}

void CHandlePacket::SendServerConfig(int iPlayerId) {
	RakNet::BitStream bsData;

	// Otteniamo le config iniziali
	bool bMenuEnabled = true;
	bool bSprintEnable = false;
	bool bCrouchBugEnabled = true;

	bsData.Write((BYTE)iPacketRPC);
	bsData.Write(RPC_OnSendServerConfig);

	// Scriviamo le config
	bsData.Write(bMenuEnabled);
	bsData.Write(bSprintEnable);
	bsData.Write(bCrouchBugEnabled);

	// Mandiamo il pacchetto
	pRakServer->Send(&bsData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakServer->GetPlayerIDFromIndex(iPlayerId), false);
}