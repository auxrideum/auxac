# auxac
Discontinued anti-cheat built for SA-MP

# Structure
```bash
├───bin
│   ├───client
│   └───server
│       └───toolset
├───obj
│   ├───client
│   │   
│   └───server
└───src
    ├───client
    │   └───lib
    │       ├───curl
    │       │   ├───bin
    │       │   ├───include
    │       │   │   └───curl
    │       │   └───lib
    │       ├───detours
    │       │   ├───includes
    │       │   └───lib
    │       ├───imgui
    │       └───RakNet
    ├───server
    │   └───lib
    │       ├───RakNet
    │       ├───samp-plugin-sdk
    │       │   └───amx
    │       ├───sampgdk
    │       └───subhook
    └───Shared
        ├───md5
        └───memory
```

# Features
- Integrity check while sending any packet
- Persistent scan of running processes
- Persistent scan of currently loaded modules
- Call hooks intercepting LoadLibrary, CreateThread and SetWindowText
- D3D9 table compare check for modified values
- Disabled CLEO scripts
- Patch for starting multiple GTA:SA processes
- Hack to disable s0beit at startup
- Ped speed handled server-side
- Main menu handled server-side
- Prevent the little delay when screenshotting
- Picture of client's screen on admin request
- Removed SA-MP delay when quitting/connecting
- Patch for instant loading GTA:SA, mine loads in 1-2 secs.

