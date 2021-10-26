#include "main.h"

namespace Callback
{
	static AMX* amx_allowed = NULL;
	static list<AMX*> amxPointers;

	std::list<AMX*>& GetAMXList()
	{
		return amxPointers;
	}

	cell OnLoadLibrary(int iPlayerId, const char *szPath)
	{
		int idx = 0;
		for (std::list<AMX*>::iterator it = amxPointers.begin(); it != amxPointers.end(); ++it, ++idx)
		{
			AMX* pAmx = *it;
			cell ret, amx_Address, * phys_addr;	
			int amxerr = amx_FindPublic(pAmx, "OnLoadLibrary", &idx);

			if (amxerr == AMX_ERR_NONE)
			{
				amx_PushString(pAmx, &amx_Address, &phys_addr, szPath, 0, 0);
				amx_Push(pAmx, iPlayerId);
				amx_Exec(pAmx, &ret, idx);
				return 1;
			}
		}
		return 0;
	}

	void SAMPGDK_CALL KickPlayer(int timerid, void* data) {
		int playerid = (int)data;
		Kick(playerid);
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
		if (pVerifiedPlayer->m_Player[playerid].bIsPlayerUsingClient) {
			char szBuf[128];
			char szPlayerName[MAX_PLAYER_NAME];

			GetPlayerName(playerid, szPlayerName, sizeof(szPlayerName));
			snprintf(szBuf, sizeof(szBuf), "{FF0000}[AUXAC]{FFFFFF} %s connected - %s.", szPlayerName, pVerifiedPlayer->m_Player[playerid].sVersion.c_str());

			SendClientMessageToAll(-1, szBuf);

			if (pVerifiedPlayer->m_Player[playerid].sVersion != PLUGIN_SUPPORTED_CLIENT) {
				snprintf(szBuf, sizeof(szBuf), "{FF0000}[AUXAC]{FFFFFF} You're running an outdated version [%s]. Please update at %s", pVerifiedPlayer->m_Player[playerid].sVersion.c_str(), WEBSITE_AUXAC);

				SendClientMessage(playerid, -1, szBuf);
				SetTimer(50, false, KickPlayer, (void*)playerid);
			}
		}
		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason) {
		if (pVerifiedPlayer->m_Player[playerid].bIsPlayerUsingClient) {
			sampgdk_logprintf("[AUXAC] PlayerID %d [%s] has left the server.", playerid, pVerifiedPlayer->m_Player[playerid].sVersion.c_str());

			pVerifiedPlayer->m_Player[playerid].bIsPlayerUsingClient = false;
			pVerifiedPlayer->m_Player[playerid].sPlayerHWID = "";
			pVerifiedPlayer->m_Player[playerid].sVersion = "";
		}

		return true;
	}

	PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char* params)
	{
	

		return false;
	}

}