#ifndef CHECK_PARAMS
#define CHECK_PARAMS(m,n)                                                                                           \
		do                                                                                                              \
				{                                                                                                               \
			if (pParams[0] != (m * 4))                                                                                   \
						{                                                                                                           \
				sampgdk_logprintf(" Error: Incorrect parameter count on \"" n "\", %d != %d\n", m, pParams[0] / 4); \
				return 0;                                                                                               \
						}                                                                                                           \
				}                                                                                                               \
						while (0)
#endif

namespace Natives
{
	cell AMX_NATIVE_CALL IsPlayerUsingAux(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL GetPlayerHWID(AMX* amx, cell* params);
}