class CHandlePacket {
public:
	void Process(int iRpcId, BitStream bsData);
};

extern CHandlePacket* pHandlePacket;