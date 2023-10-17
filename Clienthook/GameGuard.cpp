#include "ClientHook.h"

using namespace std; 

int GGSTATE_RET = 0x00455E8B;
//int CHKGGMON_RET = 0x004555AA;

void __declspec(naked) GameGuardinitState()
{
	__asm mov dword ptr ss:[ebp+0x8],0x755
	__asm mov ebx,dword ptr ss:[ebp+0x8]
	__asm jmp GGSTATE_RET
}
