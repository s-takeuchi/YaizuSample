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
			StkPlSwPrintf(TmpCmd, FILENAME_MAX + 32, L"cmd /c \"%ls\\svcadd.bat\"", TmpPath2);
			char* TmpCmds = StkPlCreateUtf8FromWideChar(TmpCmd);
			StkPlPrintf("%s\r\n", TmpCmds);
			StkPlSleepMs(1000);
			system(TmpCmds);
			delete TmpCmds;
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
