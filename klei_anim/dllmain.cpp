/***************************
* Encoding: GB2312
***************************/
// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "pch.h"
#ifdef _WIN32

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//win_spec::hmoudle = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


#endif // _WIN32
