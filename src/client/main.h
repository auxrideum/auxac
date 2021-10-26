#define DEVELOPER_VERSION 1
#define VERSION_BUILD "0.0.2"

#define WIN32_LEAN_AND_MEAN


#include <Windows.h>
#include <psapi.h>
#include <assert.h>
#include <string>
#include <list>
#include <time.h>
#include <vector>
#include <wchar.h>
#include <fstream>
#include <d3d9.h>
#include <d3dx9.h>
#include <filesystem>
#include <gdiplus.h>

#pragma comment (lib, "detours.lib")
#pragma comment (lib, "gdiplus.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

extern HMODULE g_hDllModule;
extern BOOL bIsSampInitialized;

void Log(const char* fmt, ...);

#include "detours.h"

#include "../Shared/memory/memory.h"
#include "../Shared/EnumRPCs.h"
#include "../Shared/md5/md5.h"

#include "BitStream.h"
#include "RakClient.h"
#include "HookedRakClient.h"

#include "CCore.h"
#include "CD3D9Hook.h"
#include "CHandlePacket.h"
#include "CMiscFunctions.h"
#include "CMultiplayerSA.h"
#include "CPatchGameSA.h"
#include "QuickLoad.h"