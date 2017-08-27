#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <stdio.h>
#include <Psapi.h>
#include "..\..\..\YaizuComLib\src\commonfunc\StkObject.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkProperties.h"
#include "..\..\..\YaizuComLib\src\stkthread\stkthread.h"
#include "..\..\..\YaizuComLib\src\stksocket\stksocket.h"
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebApp.h"
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"
#include "sample_elem1.h"
#include "sample_elem2.h"
#include "sample_elem3.h"

void Sample(TCHAR* IpAddr, int Port)
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 3, IpAddr, Port);

	Sample_Elem1* Test1Hndl = new Sample_Elem1();
	int Add1 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, _T("/api/aaa/bbb/"), (StkWebAppExec*)Test1Hndl);
	Sample_Elem2* Test2Hndl = new Sample_Elem2();
	int Add2 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, _T("/api/aaa/xxx/"), (StkWebAppExec*)Test2Hndl);
	Sample_Elem3* Test3Hndl = new Sample_Elem3();
	int Add3 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, _T("/api/bbb/$/"), (StkWebAppExec*)Test3Hndl);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, _T("/api/aaa/bbb/"));
	int Del2 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, _T("/api/aaa/xxx/"));
	int Del3 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, _T("/api/bbb/$/"));

	delete Soc;
}

int main(int Argc, char* Argv[])
{
	TCHAR Buf[256];
	GetModuleFileName(NULL, Buf, 255);
	LPTSTR Addr = StrStr(Buf, _T("\\stkwebapp.exe"));
	lstrcpy(Addr, _T(""));
	SetCurrentDirectory(Buf);
	printf("Current Directory = %S\r\n", Buf);

	char IpAddrTmp[256];
	TCHAR IpAddr[256];
	int Port;
	StkProperties *Prop = new StkProperties();
	
	if (Prop->GetProperties(_T("stkwebapp.conf")) == 0) {
		if (Prop->GetPropertyStr("servicehost", IpAddrTmp) != 0) {
			printf("servicehost property is not found.\r\n");
			return -1;
		}
		printf("servicehost property = %s\r\n", IpAddrTmp);
		wsprintf(IpAddr, _T("%S"), IpAddrTmp);
		if (Prop->GetPropertyInt("serviceport", &Port) != 0) {
			printf("serviceport property is not found.\r\n");
			return -1;
		}
		printf("serviceport property = %d\r\n", Port);
	} else {
		printf("stkwebapp.conf is not found.\r\n");
		return -1;
	}

	Sample(IpAddr, Port);

	return 0;
}
