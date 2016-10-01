#include "error.h"
#include <Windows.h>
#include <stdio.h>

void ExitShowError()
{

	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0)
		return;

	char msg_buff[256];
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL, 
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		msg_buff, 
		256, 
		NULL
		);

	printf("%s\n", msg_buff);
	ExitProcess(EXIT_FAILURE);
}