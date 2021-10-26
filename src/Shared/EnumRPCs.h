const int iPacketRPC = 125;

enum RPCClientEnumeration
{
	RPC_OnFirstConnection = 1,
	RPC_OnTakeScreenImage,
	RPC_OnLoadLibrary,
	RPC_OnSendDetectedModules,
	RPC_OnModulesUpdate,
	RPC_OnUnknownAimData,
	RPC_OnRequestScreenShot,
	//
	RPC_OnSendServerConfig,
};