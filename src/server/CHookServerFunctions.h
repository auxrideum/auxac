typedef BYTE(*GetPacketID_t)(Packet* p);
typedef int(*ProcessQueryPacket_t)(struct in_addr in, u_short hostshort, char* buf, int len, SOCKET s);

class CHookServerFunctions {
public:
	CHookServerFunctions();

	bool Install();

private:
	static SubHook sh_GetPacketID;

	static BYTE GetPacketID_HOOK(Packet* p);

	unsigned long FUNC_GetPacketID = NULL;
};

extern CHookServerFunctions* pHookServerFunctions;