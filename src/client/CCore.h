class CCore {
public:

	CCore::CCore();
	CCore::~CCore();

	void HandleLibraryLoad(string moduleName);
	void HandleMenu();

	void SetMenuStatus(bool bStatus) { m_bIsMenuEnabled = bStatus; };
	void SetSprintStatus(bool bStatus) { m_bIsSprintEnabled = bStatus; };
	void SetCrouchBugStatus(bool bStatus) { m_bIsCrouchBugEnabled = bStatus; };
	void SetConnectionStatus(bool bStatus) { m_bIsConnected = bStatus; };

	bool GetMenuStatus() { return m_bIsMenuEnabled; };
	bool GetSprintStatus() { return m_bIsSprintEnabled; };
	bool GetCrouchBugStatus() { return m_bIsCrouchBugEnabled; };
	bool GetConnectionStatus() { return m_bIsConnected; };

	bool m_bIsGraphicUserInterfaceShown = false;
private:
	bool m_bIsMenuEnabled = true;
	bool m_bIsSprintEnabled = true;
	bool m_bIsCrouchBugEnabled = true;
	bool m_bIsConnected = false;


};

extern CCore* pCore;