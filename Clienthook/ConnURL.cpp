#include "ClientHook.h"
#include <string>

using namespace std; 


// Login URL
// Find Strings login.playwith (Find adbillsoft ... )
int LOGINURL_RET = 0x00454DEB;

// Report Funs 2021 KR 105.588
//int CONN_FUN = 0x00402450;
//int PLAYWITH_FUN = 0x00427420;

// Bind DNS
//unsigned char BIND_DNS[32] = {0};
unsigned char BIND_DNS[32] = "127.0.0.1";
int BIND_DNS_ADDRS = (DWORD)BIND_DNS;

// Save Host URL
int OPTION = 0;
int HOSTURL;
char LOGINURL[100] = {0};
int LOGINURL_ADDRS = (DWORD)LOGINURL;

int PUSERID;
char USERID_BUFFER[100] = {0};
int USERID_ADDRS = (DWORD)USERID_BUFFER;

int MegrEnd;

// GameMall
int GAMEMALLFUN = 0x009452E0;
int GAMEMALL_RET = 0x005860A9;

void __declspec(naked) LoginBindURL()
{
	// Save Host URL
	__asm mov HOSTURL,ebx
	__asm mov OPTION,0x0
	MegrURL();

	//SetURL();
	BindDNS();

	__asm mov ecx,dword ptr ds:[0xDE05E8]
	__asm jmp LOGINURL_RET
}

/**** Game Mall ****/
void __declspec(naked) SetGameMallURL()
{
	// pID
	__asm lea eax,dword ptr ss:[esp+0x28]
	__asm mov PUSERID,eax
	__asm mov OPTION,0x1

	MegrURL();

	__asm mov ecx,dword ptr ds:[esi+0xF4]
	__asm mov eax,LOGINURL_ADDRS
	__asm push eax
	__asm call GAMEMALLFUN
	__asm jmp GAMEMALL_RET
}

void MegrURL()
{
	//int addrs;
	char bytep;
	int SRC = 0;
	int DST = 0;

	if (OPTION == 0)
	{
		char webfile[] = "/gamemall.php?";
		SRC = HOSTURL;
		DST = LOGINURL_ADDRS;
		for (int i = 0; i < 100; i++ )
		{
			bytep = *(reinterpret_cast<unsigned char*>(SRC));
			if (bytep == 0) break;
			*(reinterpret_cast<unsigned char*>(DST)) = bytep;
			SRC += 1;
			DST += 1;
		}

		/**** Strings merge ****/
		int n = 100 - strlen(LOGINURL);
		if (n > 0)
		{
			strncat_s(LOGINURL, webfile, n);
			LOGINURL[100] = 0;
		}
		MegrEnd = 0;
	}
	else
	{
		if (MegrEnd == 0)
		{
			SRC = PUSERID;
			DST = USERID_ADDRS;
			for (int i = 0; i < 100; i++ )
			{
				bytep = *(reinterpret_cast<unsigned char*>(SRC));
				if (bytep == 0) break;
				*(reinterpret_cast<unsigned char*>(DST)) = bytep;
				SRC += 1;
				DST += 1;
			}
		
			/**** Strings merge ****/
			int n = 100 - strlen(LOGINURL);
			if (n > 0)
			{
				strncat_s(LOGINURL, USERID_BUFFER, n);
				LOGINURL[100] = 0;
			}
			MegrEnd = 1;
		}
	}
}

void BindDNS()
{
	__asm mov eax, BIND_DNS_ADDRS
	__asm mov dword ptr ds : [0xDE05D8] , eax
}

void SetURL()
{
	// http://127.0.0.1
	__asm mov eax,BIND_DNS_ADDRS
	__asm mov dword ptr ds:[eax],0x70747468
	__asm mov dword ptr ds:[eax+0x4],0x312F2F3A
	__asm mov dword ptr ds:[eax+0x8],0x302E3732
	__asm mov dword ptr ds:[eax+0xC],0x312E302E
	__asm mov dword ptr ds:[eax+0x10],0x0
	__asm mov dword ptr ds:[eax+0x14],0x0
	__asm mov dword ptr ds:[eax+0x18],0x0

	/***
	// http://114.33.76.52
	__asm mov eax,BIND_DNS_ADDRS
	__asm mov dword ptr ds:[eax],0x70747468
	__asm mov dword ptr ds:[eax+0x4],0x312F2F3A
	__asm mov dword ptr ds:[eax+0x8],0x332E3431
	__asm mov dword ptr ds:[eax+0xC],0x36372E33
	__asm mov dword ptr ds:[eax+0x10],0x0032352E
	__asm mov dword ptr ds:[eax+0x14],0x00000000
	__asm mov dword ptr ds:[eax+0x18],0x0
	***/
}