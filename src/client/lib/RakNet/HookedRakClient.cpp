#include "main.h"

void InitialWriteToBitStream(int iRpcId, BitStream* bOutPut) {
	bOutPut->Write((BYTE)iPacketRPC);
	bOutPut->Write(iRpcId);
}

bool HookedRakClientInterface::RPC(int* uniqueID, BitStream *parameters, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp)
{
	return g_RakClient->GetInterface()->RPC(uniqueID, parameters, priority, reliability, orderingChannel, shiftTimestamp);
}

int icount = 0;

bool HookedRakClientInterface::Send(BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	BYTE packetId;
	bitStream->Read(packetId);

	if (packetId == ID_AIM_SYNC) {
		stAimData OutgoingAimData;
		bitStream->Read((PCHAR)&OutgoingAimData, sizeof(stAimData));

		stCameraAim* pcaInternalAim = (stCameraAim*)0xB6F32C;
		BYTE* pbyteCameraMode = (BYTE*)0xB6F1A8;

		float fVecAimPos[3] = { pcaInternalAim->pos1x, pcaInternalAim->pos1y, pcaInternalAim->pos1z };

		if (*pbyteCameraMode != OutgoingAimData.byteCamMode) {
			// Ripristiniamo
			OutgoingAimData.byteCamMode = *pbyteCameraMode;

			// Facciamolo sapere al server
			BitStream bsSend;

			bsSend.Write((BYTE)iPacketRPC);
			bsSend.Write(RPC_OnUnknownAimData);

			g_RakClient->Send(&bsSend, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		}

		for (int i = 0; i < 3; i++) {
			if (OutgoingAimData.vecAimPos[i] != fVecAimPos[i]) {
				// Ripristiniamo
				OutgoingAimData.vecAimPos[i] = fVecAimPos[i];

				// Facciamolo sapere al server
				BitStream bsSend;

				bsSend.Write((BYTE)iPacketRPC);
				bsSend.Write(RPC_OnUnknownAimData);

				g_RakClient->Send(&bsSend, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			}
		}

		bitStream->Reset();
		bitStream->Write((BYTE)ID_AIM_SYNC);
		bitStream->Write((PCHAR)&OutgoingAimData, sizeof(stAimData));

	}
	else if (packetId == ID_PLAYER_SYNC) {
		stOnFootData OutgoingOnFootData;
		bitStream->Read((PCHAR)&OutgoingOnFootData, sizeof(stOnFootData));

		for (int i = 0; i < 3; i++) {
			OutgoingOnFootData.fMoveSpeed[i] = OutgoingOnFootData.fMoveSpeed[i] / 2;
		}

		bitStream->Reset();
		bitStream->Write((BYTE)ID_PLAYER_SYNC);
		bitStream->Write((PCHAR)&OutgoingOnFootData, sizeof(stOnFootData));
	}

	return g_RakClient->GetInterface()->Send(bitStream, priority, reliability, orderingChannel);
}

Packet *HookedRakClientInterface::Receive(void)
{
	Packet* ReceivedPacket = g_RakClient->GetInterface()->Receive();
	BYTE ReceivedPacketID = GetPacketID(ReceivedPacket);

	switch (ReceivedPacketID) {
	case ID_CONNECTION_REQUEST_ACCEPTED: {

		string sHardwareId = pMiscFunctions->GetHardwareID();
		BitStream bsSend;

		bsSend.Write((BYTE)iPacketRPC);
		bsSend.Write(RPC_OnFirstConnection);

		// Scriviamo la versione da mandare
		string sVersion = VERSION_BUILD;

		bsSend.Write((unsigned short)strlen(sVersion.c_str()));
		bsSend.Write(sVersion.c_str(), strlen(sVersion.c_str()));

		// Scriviamo l'HWID in BitStream
		bsSend.Write((unsigned short)strlen(sHardwareId.c_str()));
		bsSend.Write(sHardwareId.c_str(), strlen(sHardwareId.c_str()));

		g_RakClient->Send(&bsSend, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		break;
	}
	case iPacketRPC: {
		BitStream bsData((unsigned char*)ReceivedPacket->data, ReceivedPacket->length, false);

		int iRpcId;

		bsData.IgnoreBits(8);
		bsData.Read(iRpcId);

		pHandlePacket->Process(iRpcId, bsData);
	}
	}

	return ReceivedPacket;
}

bool HookedRakClientInterface::Connect(const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer)
{
	return g_RakClient->GetInterface()->Connect(host, serverPort, clientPort, depreciated, threadSleepTimer);
}

void HookedRakClientInterface::Disconnect(unsigned int blockDuration, unsigned char orderingChannel)
{
	g_RakClient->GetInterface()->Disconnect(blockDuration, orderingChannel);
}

void HookedRakClientInterface::InitializeSecurity(const char *privKeyP, const char *privKeyQ)
{

	g_RakClient->GetInterface()->InitializeSecurity(privKeyP, privKeyQ);
}

void HookedRakClientInterface::SetPassword(const char *_password)
{

	g_RakClient->GetInterface()->SetPassword(_password);
}

bool HookedRakClientInterface::HasPassword(void) const
{

	return g_RakClient->GetInterface()->HasPassword();
}

bool HookedRakClientInterface::Send(const char *data, const int length, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{

	return g_RakClient->GetInterface()->Send(data, length, priority, reliability, orderingChannel);
}

void HookedRakClientInterface::DeallocatePacket(Packet *packet)
{

	g_RakClient->GetInterface()->DeallocatePacket(packet);
}

void HookedRakClientInterface::PingServer(void)
{

	g_RakClient->GetInterface()->PingServer();
}

void HookedRakClientInterface::PingServer(const char* host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections)
{

	g_RakClient->GetInterface()->PingServer(host, serverPort, clientPort, onlyReplyOnAcceptingConnections);
}

int HookedRakClientInterface::GetAveragePing(void)
{

	return g_RakClient->GetInterface()->GetAveragePing();
}

int HookedRakClientInterface::GetLastPing(void) const
{

	return g_RakClient->GetInterface()->GetLastPing();
}

int HookedRakClientInterface::GetLowestPing(void) const
{

	return g_RakClient->GetInterface()->GetLowestPing();
}

int HookedRakClientInterface::GetPlayerPing(const PlayerID playerId)
{

	return g_RakClient->GetInterface()->GetPlayerPing(playerId);
}

void HookedRakClientInterface::StartOccasionalPing(void)
{

	g_RakClient->GetInterface()->StartOccasionalPing();
}

void HookedRakClientInterface::StopOccasionalPing(void)
{

	g_RakClient->GetInterface()->StopOccasionalPing();
}

bool HookedRakClientInterface::IsConnected(void) const
{

	return g_RakClient->GetInterface()->IsConnected();
}

unsigned int HookedRakClientInterface::GetSynchronizedRandomInteger(void) const
{

	return g_RakClient->GetInterface()->GetSynchronizedRandomInteger();
}

bool HookedRakClientInterface::GenerateCompressionLayer(unsigned int inputFrequencyTable[256], bool inputLayer)
{

	return g_RakClient->GetInterface()->GenerateCompressionLayer(inputFrequencyTable, inputLayer);
}

bool HookedRakClientInterface::DeleteCompressionLayer(bool inputLayer)
{

	return g_RakClient->GetInterface()->DeleteCompressionLayer(inputLayer);
}

void HookedRakClientInterface::RegisterAsRemoteProcedureCall(int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms))
{
	g_RakClient->GetInterface()->RegisterAsRemoteProcedureCall(uniqueID, functionPointer);
}

void HookedRakClientInterface::RegisterClassMemberRPC(int* uniqueID, void *functionPointer)
{

	g_RakClient->GetInterface()->RegisterClassMemberRPC(uniqueID, functionPointer);
}

void HookedRakClientInterface::UnregisterAsRemoteProcedureCall(int* uniqueID)
{
	g_SAMP->pRakClientInterface = g_RakClient->GetInterface();

	g_RakClient->GetInterface()->UnregisterAsRemoteProcedureCall(uniqueID);
}

bool HookedRakClientInterface::RPC(int* uniqueID, const char *data, unsigned int bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp)
{

	return g_RakClient->GetInterface()->RPC(uniqueID, data, bitLength, priority, reliability, orderingChannel, shiftTimestamp);
}

void HookedRakClientInterface::SetTrackFrequencyTable(bool b)
{

	g_RakClient->GetInterface()->SetTrackFrequencyTable(b);
}

bool HookedRakClientInterface::GetSendFrequencyTable(unsigned int outputFrequencyTable[256])
{

	return g_RakClient->GetInterface()->GetSendFrequencyTable(outputFrequencyTable);
}

float HookedRakClientInterface::GetCompressionRatio(void) const
{
	return g_RakClient->GetInterface()->GetCompressionRatio();
}

float HookedRakClientInterface::GetDecompressionRatio(void) const
{

	return g_RakClient->GetInterface()->GetDecompressionRatio();
}

void HookedRakClientInterface::AttachPlugin(void *messageHandler)
{

	g_RakClient->GetInterface()->AttachPlugin(messageHandler);
}

void HookedRakClientInterface::DetachPlugin(void *messageHandler)
{
	g_RakClient->GetInterface()->DetachPlugin(messageHandler);
}

BitStream * HookedRakClientInterface::GetStaticServerData(void)
{
	return g_RakClient->GetInterface()->GetStaticServerData();
}

void HookedRakClientInterface::SetStaticServerData(const char *data, const int length)
{
	g_RakClient->GetInterface()->SetStaticServerData(data, length);
}

BitStream * HookedRakClientInterface::GetStaticClientData(const PlayerID playerId)
{
	return g_RakClient->GetInterface()->GetStaticClientData(playerId);
}

void HookedRakClientInterface::SetStaticClientData(const PlayerID playerId, const char *data, const int length)
{
	g_RakClient->GetInterface()->SetStaticClientData(playerId, data, length);
}

void HookedRakClientInterface::SendStaticClientDataToServer(void)
{
	g_RakClient->GetInterface()->SendStaticClientDataToServer();
}

PlayerID HookedRakClientInterface::GetServerID(void) const
{

	return g_RakClient->GetInterface()->GetServerID();
}

PlayerID HookedRakClientInterface::GetPlayerID(void) const
{

	return g_RakClient->GetInterface()->GetPlayerID();
}

PlayerID HookedRakClientInterface::GetInternalID(void) const
{

	return g_RakClient->GetInterface()->GetInternalID();
}

const char* HookedRakClientInterface::PlayerIDToDottedIP(const PlayerID playerId) const
{

	return g_RakClient->GetInterface()->PlayerIDToDottedIP(playerId);
}

void HookedRakClientInterface::PushBackPacket(Packet *packet, bool pushAtHead)
{

	g_RakClient->GetInterface()->PushBackPacket(packet, pushAtHead);
}

void HookedRakClientInterface::SetRouterInterface(void *routerInterface)
{

	g_RakClient->GetInterface()->SetRouterInterface(routerInterface);
}
void HookedRakClientInterface::RemoveRouterInterface(void *routerInterface)
{

	g_RakClient->GetInterface()->RemoveRouterInterface(routerInterface);
}

void HookedRakClientInterface::SetTimeoutTime(RakNetTime timeMS)
{

	g_RakClient->GetInterface()->SetTimeoutTime(timeMS);
}

bool HookedRakClientInterface::SetMTUSize(int size)
{

	return g_RakClient->GetInterface()->SetMTUSize(size);
}

int HookedRakClientInterface::GetMTUSize(void) const
{

	return g_RakClient->GetInterface()->GetMTUSize();
}

void HookedRakClientInterface::AllowConnectionResponseIPMigration(bool allow)
{

	g_RakClient->GetInterface()->AllowConnectionResponseIPMigration(allow);
}

void HookedRakClientInterface::AdvertiseSystem(const char *host, unsigned short remotePort, const char *data, int dataLength)
{

	g_RakClient->GetInterface()->AdvertiseSystem(host, remotePort, data, dataLength);
}

RakNetStatisticsStruct* const HookedRakClientInterface::GetStatistics(void)
{
	pMultiplayerSA->SendChatMessage("test5");
	return g_RakClient->GetInterface()->GetStatistics();
}

void HookedRakClientInterface::ApplyNetworkSimulator(double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance)
{

	g_RakClient->GetInterface()->ApplyNetworkSimulator(maxSendBPS, minExtraPing, extraPingVariance);
}

bool HookedRakClientInterface::IsNetworkSimulatorActive(void)
{

	return g_RakClient->GetInterface()->IsNetworkSimulatorActive();
}

PlayerIndex HookedRakClientInterface::GetPlayerIndex(void)
{

	return g_RakClient->GetInterface()->GetPlayerIndex();
}

bool HookedRakClientInterface::RPC_(int* uniqueID, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp, NetworkID networkID)
{
	return g_RakClient->GetInterface()->RPC_(uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp, networkID);
}

BYTE GetPacketID(Packet *p)
{
	if (p == 0) 
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP) {
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char)p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else {
		return (unsigned char)p->data[0];
	}
}