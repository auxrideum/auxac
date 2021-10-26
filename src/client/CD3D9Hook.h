typedef HRESULT(WINAPI* Present_t)(LPDIRECT3DDEVICE9 dev, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
typedef HRESULT(WINAPI* Reset_t)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pp);

struct stImage {
	int m_iWidth = 0;
	int m_iHeight = 0;
	PDIRECT3DTEXTURE9 m_pdTexture = NULL;
};

class CD3D9Hook {
public:
	CD3D9Hook();
	~CD3D9Hook();

	void Install();
	bool LoadTextureFromFile(IDirect3DDevice9* pDevice, const char* szFileName, stImage* image);

	DWORD CompareTable(DWORD* dwInitTable);

	Present_t GetPresent() { return m_pPresent; };
	Reset_t GetReset() { return m_pReset; };
	WNDPROC GetWindow() { return origWndProc; };

	void SetWindow(WNDPROC tmpWndProc) { origWndProc = tmpWndProc; };

private:
	Present_t m_pPresent;
	Reset_t m_pReset;
	WNDPROC origWndProc;

};

extern CD3D9Hook* pD3D9Hook;