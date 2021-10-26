#include "main.h"
#include "utils.h"

CHookServerFunctions* pHookServerFunctions = NULL;

SubHook CHookServerFunctions::sh_GetPacketID;

CHookServerFunctions::CHookServerFunctions() {
#ifdef _WIN32
	FUNC_GetPacketID = FindPattern("\x8B\x44\x24\x04\x85\xC0\x75\x03\x0C\xFF\xC3", "xxxxxxx???x");
#else
	FUNC_GetPacketID = FindPattern("\x55\xB8\x00\x00\x00\x00\x89\xE5\x8B\x55\x00\x85\xD2", "xx????xxxx?xx");
#endif

	sampgdk_logprintf("%p", FUNC_GetPacketID);
}

bool CHookServerFunctions::Install() {
	if (FUNC_GetPacketID) {
		sh_GetPacketID.Install((void*)FUNC_GetPacketID, (void*)GetPacketID_HOOK);
		return true;
	}
	else {
		sampgdk_logprintf("I couldn't find correct addresses.");
		return false;
	}
}

BYTE CHookServerFunctions::GetPacketID_HOOK(Packet* p) {
	BYTE packetId = GetPacketID(p);

	if (packetId == 0xFF) {
		return 0xFF;
	}

	sampgdk_logprintf("%d.", packetId);

	if (packetId == iPacketRPC) {

		RakNet::BitStream bsData(&p->data[1], p->length - 1, false);
		int iRpcId;

		bsData.Read(iRpcId);

		pHandlePacket->Process(iRpcId, bsData, p->playerIndex);

		if (sh_GetPacketID.GetTrampoline() == 0)
		{
			return 0xFF;
		}

	}

	if (sh_GetPacketID.GetTrampoline() == 0) {
		return packetId;
	}
	else {
		return ((GetPacketID_t)sh_GetPacketID.GetTrampoline())(p);
	}
}