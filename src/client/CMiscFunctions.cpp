#include "main.h"

bool bSendImage = false;

CMiscFunctions* pMiscFunctions = NULL;

std::string random_string(int length)
{
	srand(time(NULL));

	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);

	return str;
}


std::string CMiscFunctions::GetHardwareID() {
	string hwidMd5 = random_string(64);

	return hwidMd5;
}

void CMiscFunctions::SetWorkingDirectory(char* g_szWorkingDirectory) {
	if (GetModuleFileName(NULL, g_szWorkingDirectory, MAX_PATH) != 0)
	{
		if (strrchr(g_szWorkingDirectory, '\\') != NULL)
			*strrchr(g_szWorkingDirectory, '\\') = 0;
		else
			strcpy(g_szWorkingDirectory, ".");
	}
	else
	{
		strcpy(g_szWorkingDirectory, ".");
	}
}

int CMiscFunctions::GetEncoderClsid(const WCHAR* wcFormat, CLSID* pClsid) {
	using namespace Gdiplus;

	UINT  uiNum = 0;
	UINT  uiSize = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&uiNum, &uiSize);
	if (uiSize == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(uiSize));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(uiNum, uiSize, pImageCodecInfo);

	for (UINT j = 0; j < uiNum; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, wcFormat) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);

			return j;
		}
	}

	free(pImageCodecInfo);
	return 0;
}

BOOL CMiscFunctions::TakeScreenShot(wstring path) {
	using namespace Gdiplus;

	IStream* istream;
	HRESULT hRes = CreateStreamOnHGlobal(NULL, true, &istream);
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{
		HDC scrdc, memdc;
		HBITMAP membit;
		CLSID clsid;

		scrdc = ::GetDC(0);
		if (scrdc == NULL)
			return false;

		int iHeight = GetSystemMetrics(SM_CYSCREEN);
		int iWidth = GetSystemMetrics(SM_CXSCREEN);

		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, iWidth, iHeight);

		if (memdc && membit) {
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);

			BitBlt(memdc, 0, 0, iWidth, iHeight, scrdc, 0, 0, SRCCOPY);

			Gdiplus::Bitmap bitmap(membit, NULL);

			if (GetEncoderClsid(L"image/jpeg", &clsid) == -1)
				return false;

			bitmap.Save(path.c_str(), &clsid, NULL);
		}
		else {
			return false;
		}
	
		delete& clsid;
		DeleteObject(memdc);
		DeleteObject(membit);
		::ReleaseDC(0, scrdc);
		istream->Release();
	}
	GdiplusShutdown(gdiplusToken);

	return true;
}

int CMiscFunctions::UploadImage(const char* szFileName, char *szHost) {
	/*
	CURL* curl;
	CURLcode res;

	struct curl_httppost* formpost = NULL;
	struct curl_httppost* lastptr = NULL;

	curl_global_init(CURL_GLOBAL_ALL);

	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "fileToUpload",
		CURLFORM_FILE, szFileName,
		CURLFORM_END);

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, szHost);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		res = curl_easy_perform(curl);
	}
	*/
	return 1;
}