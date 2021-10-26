struct stVerifiedPlayer {
	bool bIsPlayerUsingClient;
	string sPlayerHWID;
	string sVersion;
	vector<string> modules;
	ofstream sendImage;
};

class CVerifiedPlayer {
public:
	stVerifiedPlayer m_Player[MAX_PLAYERS];

	CVerifiedPlayer();
	~CVerifiedPlayer();
	
	bool IsPlayerUsingClient(int iPlayerId) {
		return m_Player[iPlayerId].bIsPlayerUsingClient;
	};

	string GetPlayerHWID(int iPlayerId) {
		return m_Player[iPlayerId].sPlayerHWID;
	};

	string GetClientVersion(int iPlayerId) {
		return m_Player[iPlayerId].sVersion;
	}
};

extern CVerifiedPlayer* pVerifiedPlayer;