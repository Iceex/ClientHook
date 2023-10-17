#include <windows.h>
#include <string>

void Hook_Main();

// HeapFree Patch
void HeapFreeCheck();
void EnableHeapFree();
void DisableHeapFreeGate();
void PlayerExit();

// URL Conn Fix
void LoginBindURL();
void SetGameMallURL();
void MegrURL();
void BindDNS();
void SetURL();

// GameGuard
void GameGuardinitState();
