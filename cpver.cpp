// cpver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

int wmain(int argc, wchar_t* argv[])
{
	if (argc != 2) {
		printf("Usage: project.exe\n\n          copy file with version project-verXX.exe");
		return 1;
	}

	wchar_t filename_without_exe[MAX_PATH];
	wchar_t filename[MAX_PATH];
	wchar_t run_command_w[MAX_PATH];

	wcscpy_s(filename, MAX_PATH, argv[1]);
	wcscpy_s(filename_without_exe, MAX_PATH, argv[1]);


	DWORD dwDummy;
	DWORD dwFVISize = GetFileVersionInfoSize(filename, &dwDummy);

	LPBYTE lpVersionInfo = new BYTE[dwFVISize];

	GetFileVersionInfo(filename, 0, dwFVISize, lpVersionInfo);

	UINT uLen;
	VS_FIXEDFILEINFO* lpFfi = NULL;

	VerQueryValue(lpVersionInfo, L"\\", (LPVOID*)&lpFfi, &uLen);

	if (lpFfi == NULL) {
	
		wprintf(L"===== File %s not found =====\n", filename);
		return 1;
	}

	int len = wcslen(filename_without_exe);

	for (int i = len; i >= 0; i--) {
		if (filename_without_exe[i] == '.') {
			filename_without_exe[i] = 0;
			break;
		}
	}

	wchar_t* result = wcsstr(filename, L"Win32");
	if (result != nullptr)
		swprintf_s(run_command_w, L"copy %s.exe %s32-ver%d%d.exe", filename_without_exe, filename_without_exe, HIWORD(lpFfi->dwFileVersionMS), LOWORD(lpFfi->dwFileVersionMS));
	else
		swprintf_s(run_command_w, L"copy %s.exe %s-ver%d%d.exe", filename_without_exe, filename_without_exe, HIWORD(lpFfi->dwFileVersionMS), LOWORD(lpFfi->dwFileVersionMS));

	int aret=_wsystem(run_command_w);



	delete[] lpVersionInfo;

	if(aret!=0)
		printf("===== Copy Error =====\n");
	else
		printf("===== Done =====\n");

	return 0;
}

