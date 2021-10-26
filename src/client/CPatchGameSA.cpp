#include "main.h"
#include "QuickLoad.h"

CPatchGameSA* pPatchGameSA = NULL;

void __stdcall MenuProcessHook();


void CPatchGameSA::Apply() {
	// Carichiamo il gioco velocemente
	LoadGame();

	// Non lasciamo che CLEO 4 carichi gli scripts
	mem::copy((void*)0x53BDD7, "\xE8\x74\xCF\xF2\xFF", 5);
	mem::copy((void*)0x5BA340, "\xE8\x0B\xEA\xEA\xFF", 5);

	// Patch per avviare più di un processo di GTA:SA
	mem::set((PVOID)0x74872D, 0x90, 9);
	mem::set((PVOID)0x406946, 0x00, 4);

	// Munizioni infinite
	mem::copy((void*)0x7428A6, "\x90\x90", 2);

	// Ossigeno infinito
	mem::copy((void*)0x60A8E2, "\x90\x90", 2);

	// HP infiniti
	mem::copy((void*)0x4B322C, "\xEB", 1);

	// Non facciamo funzionare s0beit
	hook::jump(0x54307B, GravityHook, 6);

	// Gestiamo la velocità del Ped
	hook::jump(0x541C98, KeyPress, 8);

	// Gestiamo il menu principale
	hook::call(0x53BF44, MenuProcessHook, 5);

	// Diminuiamo lo sleep interno del gioco
	*(BYTE*)0xBAB318 = 0;  
	*(BYTE*)0x53E94C = 0;

}

DWORD KeyPressCall = 0x53EF80;
DWORD KeyPressJmpBack = 0x541C9D;
HOOK CPatchGameSA::KeyPress()
{
	__asm
	{
		pushad
	}
	
	if (*(int*)0xB6F5F0 == 0 || *(BYTE*)((*(int*)0xB6F5F0) + 0x46C) == 0)
	{
		__asm
		{
			popad

			call[KeyPressCall]
			lea ecx, [ebx + 78h]
			jmp[KeyPressJmpBack]
		}
	}

	// Stiamo correndo?
	if (*(BYTE*)0xB73478 != 0)
	{
		if (*(int*)0xBA18FC == 0 && !pCore->GetSprintStatus())
		{
			// Set the player's sprint speed to whatever the limit is
			*(float*)0xB7CDB8 = 0.0f;
		}
	}

	// Check se il Ped sta correndo
	if (*(BYTE*)0xB7347C > 0)
	{
		// La crouch key deve essere 5, se non lo è significa che è ancora in corso lo sparo.
		if (*(int*)0xB5F898 != 5 && !pCore->GetCrouchBugStatus())
		{
			// Reimpostiamo il crouch
			*(BYTE*)0xB7347C = 0;
		}

		// Preveniamo il floorbug
		if (*(int*)0xB5EB28 == 77000016)
		{
			*(BYTE*)0xB7347C = 0;
		}
	}

	__asm
	{
		popad

		call[KeyPressCall]
		lea ecx, [ebx + 78h]
		jmp[KeyPressJmpBack]
	}
}

void __stdcall MenuProcessHook() {
	if (pCore->GetMenuStatus()) {
		func::thiscall(0x57B440, (void*)0xBA6748);
	}
}

static DWORD GravityHookJmpBack1 = 0x543081;
static DWORD GravityHookJmpBack2 = 0x543093;
HOOK CPatchGameSA::GravityHook()
{
	__asm
	{
		pushad
	}

	// Controlliamo se c'è l'hook di sobeit

	if (*(BYTE*)0x543081 != 0xD9)
	{
		// Facciamolo crashare

		ExitProcess(0);
	}

	__asm
	{
		popad

		test byte ptr[esi + 1Ch], 01h
		je DONE_EQUAL
		jmp[GravityHookJmpBack1]

		DONE_EQUAL:
		jmp[GravityHookJmpBack2]
	}
}