#include <windows.h>
#include <cstdlib>
#include "../../../YaizuComLib/src/stkpl/StkPl.h"


int main(int argc, char* argv[])
{
	if (argc == 2) {
		wchar_t TmpPath[FILENAME_MAX] = L"";
		wchar_t TmpPath2[FILENAME_MAX] = L"";
		wchar_t TmpCmd[FILENAME_MAX + 32] = L"";
		StkPlGetFullPathFromFileName(L"agent.exe", TmpPath);
		StkPlGetFullPathWithoutFileName(TmpPath, TmpPath2);
		StkPlPrintf("Service configuration [%s]\r\n", argv[1]);
		StkPlSleepMs(1000);

		if (StkPlStrCmp(argv[1], "svcadd") == 0) {
			{
				char WorkDirPath[512] = "";
				char* TmpCmd2s = StkPlCreateUtf8FromWideChar(TmpPath2);
				StkPlSPrintf(WorkDirPath, 512, "workdir=%s\\work\r\n", TmpCmd2s);
				StkPlPrintf(WorkDirPath);
				delete TmpCmd2s;
				StkPlSwPrintf(TmpCmd, FILENAME_MAX + 32, L"%ls\\agent.conf", TmpPath2);
				void* FileHndl = StkPlOpenFileForWrite(TmpCmd, true);
				StkPlSeekFromEnd(FileHndl, 0);
				size_t ActSize = 0;
				StkPlWrite(FileHndl, WorkDirPath, StkPlStrLen(WorkDirPath), &ActSize);
				StkPlCloseFile(FileHndl);
			}
			{
				StkPlSwPrintf(TmpCmd, FILENAME_MAX + 32, L"cmd /c mkdir \"%ls\\work\"", TmpPath2);
				char* TmpCmds = StkPlCreateUtf8FromWideChar(TmpCmd);
				StkPlPrintf("%s\r\n", TmpCmds);
				StkPlSleepMs(1000);
				system(TmpCmds);
				delete TmpCmds;
			}
			{
				wchar_t SystemDir[MAX_PATH];
				GetSystemDirectory(SystemDir, MAX_PATH);
				StkPlSwPrintf(TmpCmd, FILENAME_MAX + 32, L"\"%ls\\icacls.exe\" \"%ls\\work\" /grant Users:(OI)(CI)M /T", SystemDir, TmpPath2);
				StkPlPrintf("%ls\r\n", TmpCmd);
				StkPlSleepMs(1000);

				STARTUPINFO si_acl;
				PROCESS_INFORMATION pi_acl;
				ZeroMemory(&si_acl, sizeof(si_acl));
				si_acl.cb = sizeof(si_acl);
				CreateProcess(NULL, TmpCmd, NULL, NULL, false, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si_acl, &pi_acl);
				HANDLE AclProcHndl = pi_acl.hProcess;
				WaitForSingleObject(AclProcHndl, INFINITE);
			}
			{
				StkPlSwPrintf(TmpCmd, FILENAME_MAX + 32, L"cmd /c \"%ls\\svcadd.bat\"", TmpPath2);
				char* TmpCmds = StkPlCreateUtf8FromWideChar(TmpCmd);
				StkPlPrintf("%s\r\n", TmpCmds);
				StkPlSleepMs(1000);
				system(TmpCmds);
				delete TmpCmds;
			}
		} else if (StkPlStrCmp(argv[1], "svcdel") == 0) {
			StkPlSwPrintf(TmpCmd, FILENAME_MAX + 32, L"cmd /c \"%ls\\svcdel.bat\"", TmpPath2);
			char* TmpCmds = StkPlCreateUtf8FromWideChar(TmpCmd);
			StkPlPrintf("%s\r\n", TmpCmds);
			StkPlSleepMs(1000);
			system(TmpCmds);
			delete TmpCmds;
		}
	} else {
		StkPlPrintf("Usage: %s [svcadd | svcdel]\r\n", argv[0]);
	}

	return 0;
}
