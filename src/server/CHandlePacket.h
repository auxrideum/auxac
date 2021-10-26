class CHandlePacket {
public:
	void Process(int iRpcId, RakNet::BitStream bsData, int iPlayerId);
	void SendServerConfig(int iPlayerId);
};

extern CHandlePacket* pHandlePacket;