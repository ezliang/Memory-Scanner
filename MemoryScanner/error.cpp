#include "error.h"
#include <Windows.h>
#include <stdio.h>

void ExitProcess()
{

	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0)
		return;

	wchar_t msg_buff[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL, 
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		msg_buff, 
		256, 
		NULL
		);

	wprintf(L"%s\n", msg_buff);
	ExitProcess(EXIT_FAILURE);
}