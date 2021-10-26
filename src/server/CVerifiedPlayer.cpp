#include "main.h"

CVerifiedPlayer* pVerifiedPlayer;

CVerifiedPlayer::CVerifiedPlayer() {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		m_Player[i].bIsPlayerUsingClient = false;
		m_Player[i].sPlayerHWID = "";
		m_Player[i].sVersion = "";
	}
}

CVerifiedPlayer::~CVerifiedPlayer() {

}