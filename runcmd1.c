#include <strsafe.h>
#include <Shellapi.h>

// Глобальные переменные:
WCHAR lpCommandLine[] = _T("C:\\inv_prg.exe");
HINSTANCE hInst;                                // текущий экземпляр

void ErrorExit(LPWSTR lpszFunction);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	SHELLEXECUTEINFOW ExecInfo;
	memset(&ExecInfo, 0, sizeof(SHELLEXECUTEINFOW));
	ExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	ExecInfo.hwnd = GetForegroundWindow();
	ExecInfo.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
	ExecInfo.lpVerb = NULL; //PChar('runas');   // админ - права!
	ExecInfo.lpFile = lpCmdLine;
	ExecInfo.nShow = SW_HIDE;
	ExecInfo.lpParameters = _T("");

	BOOL b = ShellExecuteExW(&ExecInfo);
	if (!b)
	{
		ErrorExit(lpCommandLine);
	}
	BOOL a = b;

	return 0;
}

void ErrorExit(LPWSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCWSTR)lpMsgBuf) + lstrlen((LPCWSTR)lpszFunction) + 40) * sizeof(WCHAR));
	StringCchPrintf((LPWSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(WCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCWSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}
