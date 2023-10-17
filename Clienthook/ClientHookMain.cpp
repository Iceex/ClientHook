#include "ClientHook.h"

using namespace std; 

int ENABLE = 0;
int HEAPFREE = 1;
#pragma unmanaged
// =====================================================================
INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
	switch(Reason)
	{
		case DLL_PROCESS_ATTACH:
			Hook_Main();
			break;
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}

/**** KR 105.643 ****/
void Hook_Main()
{
	DWORD ProtectPTR = PAGE_EXECUTE_READWRITE;
	DWORD Target_Address;
	DWORD Proc_Address;
	DWORD Address;
	DWORD Distance;
	int SIZE;

	if (HEAPFREE == 1)
	{
		// HeapFree Patch
		SIZE = 5;
		Target_Address = 0x00BE1059;
		Proc_Address = (DWORD)HeapFreeCheck;
		Distance = ((DWORD)Proc_Address - (DWORD)Target_Address - SIZE);
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xE9;
		Address = Target_Address + 1;
		*(reinterpret_cast<int*>(Address)) = Distance;

		// Enable HeapFree (IsBadReadPtr Call 3)
		SIZE = 5;
		Target_Address = 0x0087C70C;
		Proc_Address = (DWORD)EnableHeapFree;
		Distance = ((DWORD)Proc_Address - (DWORD)Target_Address - SIZE);
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xE9;
		Address = Target_Address + 1;
		*(reinterpret_cast<int*>(Address)) = Distance;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

		// Disable HeapFree (0xE002 RCM_GATE_ALLCHARACTER_1 Recv)
		SIZE = 5;
		Target_Address = 0x004088B0;
		Proc_Address = (DWORD)DisableHeapFreeGate;
		Distance = ((DWORD)Proc_Address - (DWORD)Target_Address - SIZE);
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xE9;
		Address = Target_Address + 1;
		*(reinterpret_cast<int*>(Address)) = Distance;
		Address = Target_Address + 5;
		*(reinterpret_cast<unsigned char*>(Address)) = 0x90;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

		// EndGame Exit Player
		SIZE = 5;
		Target_Address = 0x0049AF27;
		Proc_Address = (DWORD)PlayerExit;
		Distance = ((DWORD)Proc_Address - (DWORD)Target_Address - SIZE);
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xE9;
		Address = Target_Address + 1 ;
		*(reinterpret_cast<int*>(Address)) = Distance;
		Address = Target_Address + 5;
		*(reinterpret_cast<unsigned char*>(Address)) = 0x90;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

		// GameGuard Remove
		SIZE = 2;
		Target_Address = 0x004552BD;
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xEB;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);
		SIZE = 1;
		Target_Address = 0x004562F0;
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xC3;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);	
	}
	else
	{
		// Find "GameGuard InitFaield!!"
		// GameGuard init State 0x775 Patch
		SIZE = 5;
		Target_Address = 0x00455E85;
		Proc_Address = (DWORD)GameGuardinitState;
		Distance = ((DWORD)Proc_Address - (DWORD)Target_Address - SIZE);
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xE9;
		Address = Target_Address + 1;
		*(reinterpret_cast<int*>(Address)) = Distance;
		Address = Target_Address + 5;
		*(reinterpret_cast<unsigned char*>(Address)) = 0x90;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

		SIZE = 5;
		Target_Address = 0x00455E94;
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xE9;
		Address = Target_Address + 1;
		*(reinterpret_cast<int*>(Address)) = 0x000000E2;
		Address = Target_Address + 5;
		*(reinterpret_cast<unsigned char*>(Address)) = 0x90;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

		// Find "nProtect detect hack"
		// nProtect detect hack Patch asm: retn 0x8
		SIZE = 3;
		Target_Address = 0x00456C10;
		*(reinterpret_cast<unsigned int*>(Target_Address)) = 0x900008C2;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);
		//SIZE = 2;
		//Target_Address = 0x0044FC2E;
		//*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xEB;
		//VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

		// BP CreateProcessA to Track
		// Disable GameMon
		SIZE = 2;
		Target_Address = 0x00BFC28C;
		*(reinterpret_cast<unsigned short*>(Target_Address)) = 0x64EB;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

		// BP CreateProcessA & Close Game to Track
		// ggerror.des Patch
		SIZE = 2;
		Target_Address = 0x00BFCF65;
		*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xEB;
		VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);
	}

	// Nation Limit
	Target_Address = 0x00454F06;
	*(reinterpret_cast<unsigned int*>(Target_Address)) = 0x90909090;
	Address = Target_Address + 4;
	*(reinterpret_cast<unsigned short*>(Address)) = 0x9090;

	/***
	// Nation US
	SIZE = 5;
	Target_Address = 0x004549A4;
	Address = Target_Address + 1;
	*(reinterpret_cast<int*>(Address)) = 0x00CAAD08;
	VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

	// Nation TW
	SIZE = 5;
	Target_Address = 0x004549A4;
	Address = Target_Address + 1;
	*(reinterpret_cast<int*>(Address)) = 0x00CAACFC;
	VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);
	***/

	// Create Character Name Encoding Patch
	// Find "click.wav" about "\bitmaps\interface4\UI\CreateCharacter1.ui"
	// Into case 5 Call, Into Call 2, into call 2
	// Code:
	//0050E908   |> \56                        push esi                        ; Name
	//0050E909   |.  8BCF                      mov ecx,edi
	//0050E90B   |.  E8 40EFFFFF               call 0050D850                   ; Encoding Check
	//0050E910   |.  84C0                      test al,al                      ; 0 = Encoding OK
	//0050E912   |.  74 10                     je 0050E924                     ; Patch to JMP
	//0050E914   |.  8D4C24 08                 lea ecx,dword ptr ss:[esp+0x8]
	//0050E918   |.  E8 730D6A00               call 00BAF690
	//0050E91D   |.  B8 04000000               mov eax,0x4
	SIZE = 2;
	Target_Address = 0x0050F1D2;
	*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xEB;
	VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

	// Find http://login.rohan.co.kr/closebeta/bug_report/rohan_report.asp
	SIZE = 5;
	Target_Address = 0x00BBA60B;
	Address = Target_Address + 1;
	*(reinterpret_cast<int*>(Address)) = 0x00DAAFA0;
	VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

	// Login URL
	SIZE = 5;
	Target_Address = 0x00454DE5;
	Proc_Address = (DWORD)LoginBindURL;
	Distance = ((DWORD)Proc_Address - (DWORD)Target_Address - SIZE);
	*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xE9;
	Address = Target_Address + 1;
	*(reinterpret_cast<int*>(Address)) = Distance;
	Address = Target_Address + 5;
	*(reinterpret_cast<unsigned char*>(Address)) = 0x90;
	VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);

	// Set GameMall URL
	SIZE = 5;
	Target_Address = 0x00586099;
	Proc_Address = (DWORD)SetGameMallURL;
	Distance = ((DWORD)Proc_Address - (DWORD)Target_Address - SIZE);
	*(reinterpret_cast<unsigned char*>(Target_Address)) = 0xE9;
	Address = Target_Address + 1;
	*(reinterpret_cast<int*>(Address)) = Distance;
	VirtualProtect((LPVOID)Target_Address, SIZE, PAGE_EXECUTE_READWRITE, &ProtectPTR);


}
