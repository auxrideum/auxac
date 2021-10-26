#define SAMP_VERSION "0.3.7-R4"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT)) 

#define CNetGame_Addr 0x26EA0C
#define CChatWindowAddr 0x26E9F8
#define CChatWindow_Print 0x68030

enum eVersionMultiplayerSA {
	SAMP037_R4 = 0x9E880000,
	SAMP037 = 0x6A03F8
};

struct stSAMP
{
	uint8_t _pad0[20];
	void* pUnk0;
	struct stServerInfo* pServerInfo;
	uint8_t _pad1[16];
	void* pRakClientInterface;
	char szIP[256 + 1];
	char szHostname[256 + 1];
	uint8_t _pad2;
	bool m_bUpdateCameraTarget;
	bool m_bNoNameTagStatus;
	uint32_t ulPort;
	BOOL m_bLanMode;
	uint32_t ulMapIcons[100];
	int iGameState;
	uint32_t ulConnectTick;
	struct stServerPresets* pSettings;
	uint8_t _pad3[5];
	struct stSAMPPools* pPools;
};

struct stServerInfo {
	uint32_t uiIP;
	uint16_t usPort;
};

struct stServerPresets
{
	uint8_t byteCJWalk;
	int m_iDeathDropMoney;
	float	fWorldBoundaries[4];
	bool m_bAllowWeapons;
	float	fGravity;
	uint8_t byteDisableInteriorEnterExits;
	uint32_t ulVehicleFriendlyFire;
	bool m_byteHoldTime;
	bool m_bInstagib;
	bool m_bZoneNames;
	bool m_byteFriendlyFire;
	int		iClassesAvailable;
	float	fNameTagsDistance;
	bool m_bManualVehicleEngineAndLight;
	uint8_t byteWorldTime_Hour;
	uint8_t byteWorldTime_Minute;
	uint8_t byteWeather;
	uint8_t byteNoNametagsBehindWalls;
	int iPlayerMarkersMode;
	float	fGlobalChatRadiusLimit;
	uint8_t byteShowNameTags;
	bool m_bLimitGlobalChatRadius;
};

struct stSAMPPools
{
	void* pActor;
	void* pObject;
	void* pGangzone;
	void* pText3D;
	void* pTextdraw;
	void* pPlayerLabels;
	struct stPlayerPool* pPlayer;
	void* pVehicle;
	void* pPickup;
};

struct stPlayerPool
{
	uint32_t ulMaxPlayerID;
	uint16_t sLocalPlayerID;
	void* pVTBL_txtHandler;
	std::string strLocalPlayerName;
	void* pLocalPlayer;
	int iLocalPlayerPing;
	int iLocalPlayerScore;
	void* pRemotePlayer[1000];
	int iIsListed[1000];
	DWORD dwPlayerIP[1000]; // always 0
};

struct stSAMPKeys
{
	uint8_t keys_primaryFire : 1;
	uint8_t keys_horn__crouch : 1;
	uint8_t keys_secondaryFire__shoot : 1;
	uint8_t keys_accel__zoomOut : 1;
	uint8_t keys_enterExitCar : 1;
	uint8_t keys_decel__jump : 1;
	uint8_t keys_circleRight : 1;
	uint8_t keys_aim : 1;
	uint8_t keys_circleLeft : 1;
	uint8_t keys_landingGear__lookback : 1;
	uint8_t keys_unknown__walkSlow : 1;
	uint8_t keys_specialCtrlUp : 1;
	uint8_t keys_specialCtrlDown : 1;
	uint8_t keys_specialCtrlLeft : 1;
	uint8_t keys_specialCtrlRight : 1;
	uint8_t keys__unused : 1;
};

struct stOnFootData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fPosition[3];
	float		fQuaternion[4];
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSpecialAction;
	float		fMoveSpeed[3];
	float		fSurfingOffsets[3];
	uint16_t	sSurfingVehicleID;
	short		sCurrentAnimationID;
	short		sAnimFlags;
};

struct stAimData
{
	BYTE	byteCamMode;
	float	vecAimf1[3];
	float	vecAimPos[3];
	float	fAimZ;
	BYTE	byteCamExtZoom : 6;	// 0-63 normalized
	BYTE	byteWeaponState : 2;	// see eWeaponState
	BYTE	bUnk;
};

typedef void(*ScreenShotFn)(void);

extern struct stSAMP* g_SAMP;
extern DWORD dwSAMPAddr;

class CMultiplayerSA {
public:
	CMultiplayerSA();
	~CMultiplayerSA();

	void InstallHooks();
	void InstallPatches();
	void CheckVersion();

	bool IsInitializedMultiplayerSA() {
		return m_bIsInitializedMultiplayerSA;
	};

	static void OnTakeScreen(void);

	void SendChatMessage(const char* text, ...);
	void SetCursorStatus(bool bEnabled);
	void ShowDialog(int send, int dialogID, int typedialog, char* caption, char* text, char* button1, char* button2);

private:
	static ScreenShotFn pfnScreenShot;

	bool m_bIsInitializedMultiplayerSA = false;
};

extern CMultiplayerSA* pMultiplayerSA;