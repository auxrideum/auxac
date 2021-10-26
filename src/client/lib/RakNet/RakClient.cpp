#include "Main.h"

RakClient *g_RakClient = NULL;

RakClient::RakClient( void *pRakClientInterface )
{
	pRakClient = (RakClientInterface *)pRakClientInterface;
}

bool RakClient::RPC( int rpcId, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp )
{
	if ( !pRakClient )
		return false;

	return pRakClient->RPC( &rpcId, bitStream, priority, reliability, orderingChannel, shiftTimestamp );
}

bool RakClient::Send(BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	if (!pRakClient)
		return false;

	return pRakClient->Send( bitStream, priority, reliability, orderingChannel );
}

void RakClient::WriteStringToBitStream(const char* szString, BitStream* bOutPut)
{
	bOutPut->Write((unsigned short)strlen(szString));
	bOutPut->Write(szString, strlen(szString));
}

bool RakClient::IsPlayerConnected() {
	if (!pMultiplayerSA->IsInitializedMultiplayerSA())
		return false;

	if (!pRakClient)
		return false;

	return pRakClient->IsConnected();
}