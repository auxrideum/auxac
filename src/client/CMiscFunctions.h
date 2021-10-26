class CMiscFunctions
{
public:
	std::string GetHardwareID();

	static void SetWorkingDirectory(char* g_szWorkingDirectory);
	BOOL TakeScreenShot(wstring path);

	int UploadImage(const char* szFileName, char* szHost);
	int GetEncoderClsid(const WCHAR* wcFormat, CLSID* pClsid);
};
std::string random_string(int length);

extern CMiscFunctions* pMiscFunctions;
