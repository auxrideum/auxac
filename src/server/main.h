#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>

#ifdef _WIN32
#include <Psapi.h>
#endif

using namespace std;

#ifndef _WIN32
#define BYTE uint8_t
#define MAX_PATH 256
#define SOCKET int
#endif

#define PLUGIN_VERSION "0.1b"
#define PLUGIN_SUPPORTED_CLIENT "0.0.2"
#define WEBSITE_AUXAC "auxac.ml"

#include "lib/subhook/subhook.h"
#include "lib/sampgdk/sampgdk.h"
#include "lib/samp-plugin-sdk/plugincommon.h"
#include "lib/samp-plugin-sdk/amx/amx.h"
#include "lib/RakNet/raknet.h"

#include "../Shared/EnumRPCs.h"

#include "natives.h"
#include "callback.h"
#include "CHandlePacket.h"
#include "CVerifiedPlayer.h"
#include "CHookServerFunctions.h"


extern CCRakServer* pRakServer;