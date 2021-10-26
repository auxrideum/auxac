#include "main.h"
#include "CHookFunctions.h"

CHookFunctions* pHookFunctions = NULL;

LoadLibrary_t CHookFunctions::m_pLoadLibrary = NULL;
SetWindowText_t CHookFunctions::m_pSetWindowText = NULL;
CreateThread_t CHookFunctions::m_pCreateThread = NULL;

vector<string> trustedLibraries
{
	"HID.DLL", 
	"SETUPAPI.DLL", 
	"WINMM.DLL", 
	"USER32.DLL", 
	"DINPUT8.dll", 
	"nvldumd.dll", 
	"DSOUND.dll", 
	"nvldumd.dll"
};

CHookFunctions::CHookFunctions() {

}

CHookFunctions::~CHookFunctions() {

}

void CHookFunctions::Install() {
	PBYTE pbLocation = NULL;

	pbLocation = DetourFindFunction((char*)"kernel32.dll", (char*)"LoadLibraryW");
	if (pbLocation != NULL) {
		m_pLoadLibrary = (LoadLibrary_t)DetourFunction(pbLocation, (BYTE*)LoadLibrary);
	}
	else {

	}

	pbLocation = DetourFindFunction((char*)"kernel32.dll", (char*)"CreateThread");
	if (pbLocation != NULL) {
		m_pCreateThread = (CreateThread_t)DetourFunction(pbLocation, (BYTE*)CreateThread);
	}
	else {
		
	}

	pbLocation = DetourFindFunction((char*)"user32.dll", (char*)"SetWindowTextA");
	if (pbLocation != NULL) {
		m_pSetWindowText = (SetWindowText_t)DetourFunction(pbLocation, (BYTE*)SetWindowTextEx);
	}
	else {
		Log("ops.");
	}
}

HMODULE WINAPI CHookFunctions::SetWindowTextEx(HWND hWnd, LPCSTR lpString)
{
	return m_pSetWindowText(hWnd, lpString);
}

HMODULE WINAPI CHookFunctions::LoadLibrary(const wchar_t* lpFileName)
{
	char szLibName[MAX_PATH];
	wcstombs(szLibName, lpFileName, MAX_PATH);

	pCore->HandleLibraryLoad(szLibName);

	return m_pLoadLibrary(lpFileName);
}

HANDLE WINAPI CHookFunctions::CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId) {

	return m_pCreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}