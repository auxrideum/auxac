#include "main.h"

CHandlePacket* pHandlePacket = NULL;

void CHandlePacket::Process(int iRpcId, BitStream bsData) {
	switch (iRpcId) {
		case RPC_OnSendServerConfig: {
			// Otteniamo le config iniziali
			bool bMenuEnabled;
			bool bSprintEnabled;
			bool bCrouchBugEnabled;

			bsData.Read(bMenuEnabled);
			bsData.Read(bSprintEnabled);
			bsData.Read(bCrouchBugEnabled);

			pCore->SetMenuStatus(bMenuEnabled);
			pCore->SetCrouchBugStatus(bCrouchBugEnabled);
			pCore->SetSprintStatus(bSprintEnabled);
		
			char szBuf[512];

			sprintf_s(szBuf, sizeof szBuf, "Main menu\t%s\nPed Sprinting\t%s\nCrouch Bug\t%s\n", bMenuEnabled ? "{00FF00}enabled" : "{FF0000}disabled", bSprintEnabled ? "{00FF00}enabled" : "{FF0000}disabled", bCrouchBugEnabled ? "{00FF00}enabled" : "{FF0000}disabled");

			pMultiplayerSA->ShowDialog(0, 228, 4, "Server Config", szBuf, "Ok", "");

			pCore->SetConnectionStatus(true);
			break;
		}
		case RPC_OnRequestScreenShot: {
			
			break;
		}
	}
}