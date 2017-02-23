#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <stdio.h>
#include <Psapi.h>
#include "..\..\..\YaizuComLib\src\stkthread\stkthread.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkObject.h"
#include "..\..\..\YaizuComLib\src\stksocket\stksocket.h"
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebApp.h"
#include "sample_elem1.h"
#include "sample_elem2.h"
#include "sample_elem3.h"

void Sample()
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 3, _T("127.0.0.1"), 8081);

	Sample_Elem1* Test1Hndl = new Sample_Elem1();
	int Add1 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/aaa/bbb/"), (StkWebAppExec*)Test1Hndl);
	Sample_Elem2* Test2Hndl = new Sample_Elem2();
	int Add2 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/aaa/xxx/"), (StkWebAppExec*)Test2Hndl);
	Sample_Elem3* Test3Hndl = new Sample_Elem3();
	int Add3 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/bbb/$/"), (StkWebAppExec*)Test3Hndl);

	////////// Main logic starts
	while (TRUE) {
		Sleep(100);
	}
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/aaa/bbb/"));
	int Del2 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/aaa/xxx/"));
	int Del3 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/bbb/$/"));

	delete Soc;
}

int main(int Argc, char* Argv[])
{
	Sample();

	return 0;
}
