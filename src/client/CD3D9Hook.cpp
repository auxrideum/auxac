#include "main.h"

#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_impl_dx9.h"
#include "lib/imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CD3D9Hook* pD3D9Hook = NULL;

vector<DWORD> vValuesDirectD3D9{
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0xdbb8106a,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x81ed2be9,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x73b8646a,
	0x30b8086a,
	0x29b8386a,
	0x30b8086a,
	0xbb8146a,
	0x2eb8146a,
	0xdeb8406a,
	0x8b55ff8b,
	0x30b8086a,
	0x82b8106a,
	0x76b8146a,
	0x30b8086a,
	0xafb8086a,
	0x30b8086a,
	0x8468,
	0x30b8086a,
	0x8068,
	0x30b8086a,
	0xc0b8106a,
	0x32b8106a,
	0xe2b8086a,
	0x55b80c6a,
	0xe2b8186a,
	0x92b8106a,
	0xfb8186a,
	0x30b8086a,
	0x30b8086a,
	0x30b8086a,
	0x8b55ff8b,
	0x9bb8146a,
	0x91b8086a,
	0x78b8106a,
	0x97b8086a,
	0x19468,
	0xe4b8106a,
	0x30b8086a,
	0x6b8146a,
	0x30b8086a,
	0x53b8146a,
	0x30b8086a,
	0x4b8106a,
	0x8b55ff8b,
	0xbeb8146a,
	0x8b55ff8b,
	0x8b55ff8b,
	0x8b55ff8b,
	0x72b8286a,
	0x4fb8246a,
	0x5ab8646a,
	0xefb8146a,
	0x50b8106a,
	0x30b8086a,
	0x8b55ff8b,
	0x30b8086a,
	0x9db8286a,
	0x8b55ff8b,
	0x30b8086a,
	0x1eb81c6a,
	0x3cb8146a,
	0xf1b81c6a,
	0x5fb8146a,
	0x90b81c6a,
	0x19b8146a,
	0x8b55ff8b,
	0x30b8086a,
	0xf9b8086a,
	0x30b8086a,
	0x8b55ff8b,
	0x30b8086a,
	0x5db8386a,
	0xc6b8086a,
	0x30b8086a,
	0x83b8206a,
	0x30b8086a,
	0xc1b8206a,
	0x30b8086a,
	0x45b8206a,
	0x30b8086a,
	0x95b81c6a,
	0xb8b81c6a,
	0x2cb8106a,
	0xc7b8146a,
};

ID3DXFont* pFont = NULL;
ID3DXLine* pLine = NULL;

bool bPresentInitialized = false;


CD3D9Hook::CD3D9Hook() {

}

CD3D9Hook::~CD3D9Hook() {

}

LRESULT CALLBACK hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(pD3D9Hook->GetWindow(), hwnd, uMsg, wParam, lParam);
}

HRESULT WINAPI Present(LPDIRECT3DDEVICE9 dev, CONST RECT* scrRect, CONST RECT* destRect, HWND wnd, CONST RGNDATA* reg)
{
	D3DXMATRIX devMx;
	D3DXMATRIX devView;

	dev->GetTransform(D3DTS_VIEW, &devView);
	dev->GetTransform(D3DTS_PROJECTION, &devMx);

	if (!bPresentInitialized) {

		if (FAILED(D3DXCreateFontA(dev, 13, 0, FW_HEAVY, 1, 0, 1, 0, ANTIALIASED_QUALITY, 0, "Tahoma", &pFont)))
			return E_FAIL;

		if (FAILED(D3DXCreateLine(dev, &pLine)))
			return E_FAIL;

		pD3D9Hook->SetWindow((WNDPROC)SetWindowLongPtr(GetActiveWindow(), GWL_WNDPROC, (LONG)(LONG_PTR)hWndProc));

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplWin32_Init(GetActiveWindow());
		ImGui_ImplDX9_Init(dev);

		Log("Present initialized.");

		bPresentInitialized = true;
	}
	
	pCore->HandleMenu();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	
	dev->SetTransform(D3DTS_VIEW, &devView);
	dev->SetTransform(D3DTS_PROJECTION, &devMx);

	return pD3D9Hook->GetPresent()(dev, scrRect, destRect, wnd, reg);
}

HRESULT WINAPI Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pp) {
	if (pFont) pFont->OnLostDevice();
	if (pLine) pLine->OnLostDevice();

	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hRet = pD3D9Hook->GetReset()(pDevice, pp);

	if (SUCCEEDED(hRet))
	{
		if (pFont) pFont->OnResetDevice();
		if (pLine) pLine->OnResetDevice();
	}

	return hRet;
}

DWORD CD3D9Hook::CompareTable(DWORD* dwInitTable) {
	for (int i = 0; i < vValuesDirectD3D9.size(); i++) {
		if (*(DWORD*)dwInitTable[i] != vValuesDirectD3D9.at(i)) {
			Log("0x%x doesn't match 0x%x", *(DWORD*)dwInitTable[i], vValuesDirectD3D9.at(i));
		}
	}
}

void CD3D9Hook::Install() {
	while (*(DWORD*)0xC8D4C0 != 9) {
		Sleep(100);
	}

	DWORD* dwInitTable = (DWORD*)(*(void**)0xC97C28);
	dwInitTable = (DWORD*)dwInitTable[0];

	// CompareTable(dwInitTable);

	m_pPresent = (Present_t)DetourFunction((PBYTE)dwInitTable[17], (PBYTE)Present);
	m_pReset = (Reset_t)DetourFunction((PBYTE)dwInitTable[16], (PBYTE)Reset);

	if (m_pPresent != NULL && dwInitTable[17] != NULL) {
		Log("D3D9Hook has been installed on: 0x%x", dwInitTable[17]);
	}
}

bool CD3D9Hook::LoadTextureFromFile(IDirect3DDevice9* pDevice, const char *szFileName, stImage *image)
{
	// Load texture from disk

	PDIRECT3DTEXTURE9 texture;
	HRESULT hRes = D3DXCreateTextureFromFileA(pDevice, szFileName, &texture);

	if (hRes != S_OK)
		return false;

	D3DSURFACE_DESC d3d_desc;

	texture->GetLevelDesc(0, &d3d_desc);

	image->m_pdTexture = texture;
	image->m_iWidth = (int)d3d_desc.Width;
	image->m_iHeight = (int)d3d_desc.Height;

	return true;
}