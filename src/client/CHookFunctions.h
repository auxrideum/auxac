#undef LoadLibrary

typedef HMODULE(WINAPI* LoadLibrary_t)(const wchar_t* lpFileName);
typedef HMODULE(WINAPI* SetWindowText_t)(HWND hWnd, LPCSTR lpString);
typedef HANDLE(WINAPI* CreateThread_t)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId );

class CHookFunctions
{
public:
	CHookFunctions();
	~CHookFunctions();

	void Install();
private:
	static LoadLibrary_t m_pLoadLibrary;
	static HMODULE WINAPI LoadLibrary(const wchar_t* lpFileName);

	static SetWindowText_t m_pSetWindowText;
	static HMODULE WINAPI SetWindowTextEx(HWND hWnd, LPCSTR lpString);

	static CreateThread_t m_pCreateThread;
	static HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
};

extern CHookFunctions* pHookFunctions;