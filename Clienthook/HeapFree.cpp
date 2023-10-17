#include "ClientHook.h"

using namespace std; 

int HEAPFREE_RETORIG = 0x00BE1060;
int HEAPFREE_RETEXIT = 0x00BE10B8;

int ENABLE_RET = 0x0087C711;
int DISABLE_RET = 0x004088B6;

int PLAYER_RETEXIT = 0x0049AFC5;
int PLAYER_RETORIG = 0x0049AF2D;

extern int ENABLE;

void __declspec(naked) HeapFreeCheck()
{
	__asm mov esi,dword ptr ss:[ebp+0x8]
	__asm test esi,esi
	__asm je EXIT
	
	__asm cmp ENABLE, 0x0
	__asm je RET_ORG

	__asm cmp esi,0x20000000
	__asm jl EXIT2
	__asm nop

	// IsBadReadPtr
	__asm push 0x20
	__asm push esi
	__asm call dword ptr ds:[0xCA7258]
	__asm cmp eax,0x1
	__asm je EXIT2

RET_ORG:
	__asm mov esi,dword ptr ss:[ebp+0x8]
	__asm jmp HEAPFREE_RETORIG

EXIT2:
	__asm nop

EXIT:
	__asm mov esi,0x0
	__asm jmp HEAPFREE_RETEXIT
}

void __declspec(naked) EnableHeapFree()
{
	__asm mov ENABLE,0x1
	__asm mov eax,dword ptr ds:[0xDD6BF0]
	__asm jmp ENABLE_RET
}

void __declspec(naked) DisableHeapFreeGate()
{
	__asm mov ENABLE,0x0
	__asm sub esp,0xA60
	__asm jmp DISABLE_RET
}

void __declspec(naked) PlayerExit()
{
	__asm mov edx,dword ptr ds:[0xDE4B08]
	__asm test edx,edx
	__asm je EXIT
	__asm jmp PLAYER_RETORIG

EXIT:
	__asm jmp PLAYER_RETEXIT

}
