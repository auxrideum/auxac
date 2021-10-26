#include "main.h"

cell AMX_NATIVE_CALL Natives::IsPlayerUsingAux(AMX* amx, cell* pParams)
{
	CHECK_PARAMS(1, "IsPlayerUsingAux");

	if (!sampgdk_IsPlayerConnected(pParams[1])) return 0;

	return pVerifiedPlayer->m_Player[pParams[1]].bIsPlayerUsingClient;
}

cell AMX_NATIVE_CALL Natives::GetPlayerHWID(AMX* amx, cell* pParams)
{
	CHECK_PARAMS(3, "GetPlayerHWID");

	if (!sampgdk_IsPlayerConnected(pParams[1])) return 0;
	if (!pVerifiedPlayer->m_Player[pParams[1]].bIsPlayerUsingClient) return 0;

	cell* cVarAddress = NULL;
	amx_GetAddr(amx, pParams[2], &cVarAddress);

	// Return the result of setting the 2nd parameter to the hardware ID string.
	return amx_SetString(cVarAddress, pVerifiedPlayer->m_Player[pParams[1]].sPlayerHWID.c_str(), 0, 0, pParams[3]);
}

