#include "main.h"
#include "CModulesList.h"

CModulesList* pModulesList = NULL;

vector<string> allowedModules
{
	"7362899b580ad0599b0d35793fb9fea5", // sensfix.asi
	"479f790d850aa208a73787411c1df97e", // NormalMapFix.asi
	"795df56453ab16ed321a3937efa33d82", // OutFitFix.asi
	"f71c26dfcdcf70615ba63eec031e5921", // SAMPGraphicRestore.asi
	"9d337b486160ab57eb1f3d980b4d98cb", // ShellFix.asi
	"2e8712c56f19f92f7f74f2d0c76f9c5f", // StreamMemFix.asi
	"e01e24c493eeb29896f3c4c6b"
};

struct stModulesList {
	int iCount;
	vector<string> modules;
};

CModulesList::CModulesList() {
	
}

CModulesList::~CModulesList() {

}

void CModulesList::Update() {
	HMODULE hMods[1024];
	DWORD cbNeeded;

	unsigned int i;
	int iModules = 0;

	// Get all modules handle in the current process.
	if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded))
	{
		// Loop through all modules
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			// Se è il nostro modulo continuamo a cercare
			if (hMods[i] == g_hDllModule)
				continue;

			// Create a variable to hold the path name on each module
			char szModName[MAX_PATH];

			// Get the module file name for the variable we just created.
			if (GetModuleFileName( hMods[i], szModName, sizeof(szModName) / sizeof(char)))
			{
				iModules ++;
				
				std::string file(szModName);
				
				string fileName = file.substr(file.find_last_of("/\\") + 1);
				string extension = fileName.substr(fileName.find_last_of(".") + 1);

				// L'estensione del modulo sospetto non deve essere dll/exe/DLL/DRV
				if (extension != "dll" && extension != "exe" && extension != "DLL" && extension != "DRV") {

					// Prendiamo l'MD5 del file per vedere se possiamo eseguirlo.
					MD5 md5;
					char* szMd5 = md5.digestFile(file.c_str());

					string md5File(szMd5);

					// Se non c'è il modulo nella nostra lista
					if (find(allowedModules.begin(), allowedModules.end(), md5File) == allowedModules.end()) {
						#if DEVELOPER_VERSION == 1
						Log("Not loading %s -> %s", fileName.c_str(), md5File.c_str());
						#endif
						
						if (!FreeLibrary(hMods[i])) {
							Log("Couldn't unload module: %s", fileName.c_str());
						}
					}
				}

				if (find(modulesList.begin(), modulesList.end(), fileName) == modulesList.end()) {
					modulesList.push_back(fileName);
					if (pCore->GetConnectionStatus()) {

						for (int i = 0; i < modulesList.size(); i++) {
							BitStream bsSend;

							bsSend.Write((BYTE)iPacketRPC);
							bsSend.Write(RPC_OnModulesUpdate);

							g_RakClient->WriteStringToBitStream(modulesList.at(i).c_str(), &bsSend);
							g_RakClient->Send(&bsSend, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
						}
						
					//	pMultiplayerSA->SendChatMessage("Sent");
					}
				}
			}

		}

		/*
		if (m_bFirstCheck) {
			m_iFirstCount = iModules;
			m_bFirstCheck = false;
		}
		else {
			if ((iModules - m_iFirstCount) == 1 || (iModules - m_iFirstCount) == 2) {
				Log("m_iModules %d m_iFirstCount %d", iModules, m_iFirstCount);
				ExitProcess(0);
			}
			else {
				m_iFirstCount = iModules;
			}
		}
		*/
	}
}