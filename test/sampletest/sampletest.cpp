#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

void TestGetUser(StkWebAppSend* StkWebAppSendObj)
{
	{
		StkPlPrintf("GetUser(admin) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		if (ResObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkObject* ResUserObj = ResObj->GetFirstChildElement()->GetFirstChildElement();
		if (ResUserObj == NULL || StkPlWcsCmp(ResUserObj->GetName(), L"User") != 0) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
		delete ResObj;
	}

	{
		StkPlPrintf("GetUser(admin + Method=OPTIONS) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_OPTIONS, "/api/user/", NULL, &ResultCode);
		if (ResObj != NULL || ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
		delete ResObj;
	}

	{
		StkPlPrintf("GetUser(dummy) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer dummy dummy");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		if (ResultCode != 401) {
			StkPlPrintf("[NG] Res=%d\n", ResultCode);
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
		delete ResObj;
	}
}

bool TestForGetApi(StkWebAppSend* StkWebAppSendObj, const char* Url, const char* Auth, StkObject* CompObj)
{
	int ResultCode = 0;
	StkWebAppSendObj->SetAutholization(Auth);
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, Url, NULL, &ResultCode);
	if (ResultCode != 200) {
		if (ResObj != NULL) {
			delete ResObj;
		}
		return false;
	}

	if (ResObj->Contains(CompObj) == NULL) {
		delete ResObj;
		return false;
	}
	return true;
}

void TestNewAgentInfoNotificationNormal(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification (testagent) ... ");
	int ResultCode = 0;

	{
		StkObject* NewObj = new StkObject(L"");
		NewObj->AppendChildElement(new StkObject(L"Name", L"testagent"));
		NewObj->AppendChildElement(new StkObject(L"Status", -980));
		NewObj->AppendChildElement(new StkObject(L"AcqTime", L"00000000FFFFFFFF"));

		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", NewObj, &ResultCode);
		if (ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ResObj;
		delete NewObj;
	}
	int ErrCode = 0;
	StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{ \"Data\" : {\"AgentInfo\" : {\"Name\":\"testagent\", \"Status\":-980, \"AcqTime\" : \"00000000ffffffff\"}}}", &ErrCode);
	bool Result = TestForGetApi(StkWebAppSendObj, "/api/agent/", "Bearer admin manager", ReqObj);
	if (Result != true) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	delete ReqObj;
	StkPlPrintf("[OK]\n");
}

void TestNewAgentInfoNotificationNull(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification (NULL) ... ");
	int ResultCode = 0;
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", NULL, &ResultCode);
	if (ResultCode != 400 || ResObj == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkObject* ChkObj = new StkObject(L"Code", 4203);
	StkObject* FndObj = ResObj->Contains(ChkObj);
	if (FndObj == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	delete ChkObj;
	StkPlPrintf("[OK]\n");
	delete ResObj;
}

void TestNewAgentInfoNotificationNoAgentInfo(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification (no agent info) ... ");
	int ResultCode = 0;
	int ErrCode = 0;
	StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"XXX\" : {\"Name\":\"testagent\", \"Status\":-980}}", &ErrCode);
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", ReqObj, &ResultCode);
	if (ResultCode != 400 || ResObj == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkObject* ChkObj = new StkObject(L"Code", 4201);
	StkObject* FndObj = ResObj->Contains(ChkObj);
	if (FndObj == NULL) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	delete ChkObj;
	StkPlPrintf("[OK]\n");
	delete ReqObj;
	delete ResObj;
}

void TestNewAgentInfoNotificationForbiddenChar(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("New agent information notification (invalid host name) ... ");
	const wchar_t* HostName[] = { L"abc_xyz", L"あいうえお", L"hello!" };
	wchar_t ReqStr[256] = L"";

	for (int Loop = 0; Loop < 3; Loop++) {
		StkPlSwPrintf(ReqStr, 256, L"{\"Name\":\"%ls\", \"Status\":-980}", HostName[Loop]);
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqStr, &ErrCode);
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/agent/", ReqObj, &ResultCode);
		if (ResultCode != 400 || ResObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkObject* ChkObj = new StkObject(L"Code", 4205);
		StkObject* FndObj = ResObj->Contains(ChkObj);
		if (FndObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ChkObj;
		delete ReqObj;
		delete ResObj;
	}
	StkPlPrintf("[OK]\n");
}

void TestPostOperationStop(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("PostOperationStop ... ");
	int ResultCode = 0;
	StkObject* ReqObj = new StkObject(L"");
	ReqObj->AppendChildElement(new StkObject(L"Operation", L"Stop"));
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/service/", ReqObj, &ResultCode);
	delete ResObj;
	StkPlPrintf("[OK]\n");
}

void TestGetCommand(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("Get Status Command ... ");
	int ResultCode1 = 0;
	StkObject* ResObj1 = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/statuscommand/aaa/", NULL, &ResultCode1);
	if (ResultCode1 != 400) {
		if (ResObj1 != NULL) {
			delete ResObj1;
		}
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\n");

	StkPlPrintf("Get Operation Command ... ");
	int ResultCode2 = 0;
	StkObject* ResObj2 = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/opcommand/aaa/", NULL, &ResultCode2);
	if (ResultCode2 != 400) {
		if (ResObj2 != NULL) {
			delete ResObj2;
		}
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\n");

	StkPlPrintf("Get Status/Operation Command (Result checking) ... ");
	if (!ResObj1->Equals(ResObj2)) {
		delete ResObj1;
		delete ResObj2;
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	int ErrCode = 0;
	StkObject *CodeObj = StkObject::CreateObjectFromJson(L"{\"Code\" : 4060}", &ErrCode);
	if (!ResObj1->Contains(CodeObj) || !ResObj2->Contains(CodeObj)) {
		StkPlPrintf("[NG]\n");
		StkPlExit(1);
	}
	delete CodeObj;

	delete ResObj1;
	delete ResObj2;
	StkPlPrintf("[OK]\n");
}

void TestNewCommand(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("Add new Command ... ");
	{
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"Name\" : \"hello\", \"Type\" : 0, \"Script\" : \"echo hello, world!!\", \"ServerFileName\" : \"\", \"AgentFileName\" : \"\", \"Timeout\" : 30}", &ErrCode);
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/command/", ReqObj, &ResultCode);
		if (ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		if (ResObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ReqObj;
		delete ResObj;
	}
	{
		int ResultCode = 0;
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/command/", NULL, &ResultCode);
		if (ResultCode != 200) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		int ErrCode = 0;
		StkObject* CompObj = StkObject::CreateObjectFromJson(L"\"Command\" : {\"Name\" : \"hello\"}", &ErrCode);
		if (!ResObj->Contains(CompObj)) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete CompObj;
		delete ResObj;
	}
	StkPlPrintf("[OK]\n");
}

void TestPostFile(StkWebAppSend* StkWebAppSendObj)
{
	{
		StkPlPrintf("Post file (normal scenarios) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"FileName\" : \"testpostfile.dat\", \"FileOffset\" : 0, \"FileData\" : \"20202020\", \"FileSize\" : 4}", &ErrCode);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/file/", ReqObj, &ResultCode);

		if (ResultCode != 200) {
			wchar_t TmpBuf[1024] = L"";
			ResObj->ToJson(TmpBuf, 1024);
			StkPlWPrintf(L"[NG] Res=%d;\n%ls\n", ResultCode, TmpBuf);
			StkPlExit(1);
		}
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\n");
	}
	{
		StkPlPrintf("Post file (abnormal scenarios (no FileName)) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"FileOffset\" : 0, \"FileData\" : \"20202020\", \"FileSize\" : 4}", &ErrCode);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/file/", ReqObj, &ResultCode);

		if (ResultCode != 400) {
			StkPlPrintf("[NG] Res=%d;\n", ResultCode);
			StkPlExit(1);
		}
		StkObject* ChkObj = new StkObject(L"Code", 4201);
		StkObject* FndObj = ResObj->Contains(ChkObj);
		if (FndObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ChkObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\n");
	}
	{
		StkPlPrintf("Post file (abnormal scenarios (non-continuous data reception)) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"FileName\" : \"testpostfile.dat\", \"FileOffset\" : 100000, \"FileData\" : \"20202020\", \"FileSize\" : 4}", &ErrCode);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/file/", ReqObj, &ResultCode);

		if (ResultCode != 400) {
			StkPlPrintf("[NG] Res=%d;\n", ResultCode);
			StkPlExit(1);
		}
		StkObject* ChkObj = new StkObject(L"Code", 4082);
		StkObject* FndObj = ResObj->Contains(ChkObj);
		if (FndObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ChkObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\n");
	}
	{
		StkPlPrintf("Post file (abnormal scenarios (command execution result:Element insufficient)) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"FileName\" : \"testpostfile.dat\", \"FileOffset\" : 100000, \"FileData\" : \"20202020\", \"FileSize\" : 4, \"Type\" : 1}", &ErrCode);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/file/", ReqObj, &ResultCode);

		if (ResultCode != 400) {
			StkPlPrintf("[NG] Res=%d;\n", ResultCode);
			StkPlExit(1);
		}
		StkObject* ChkObj = new StkObject(L"Code", 4201);
		StkObject* FndObj = ResObj->Contains(ChkObj);
		if (FndObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		delete ChkObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\n");
	}
	{
		StkPlPrintf("Post file (normal scenarios (command execution result)) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"FileName\" : \"testpostfile.dat\", \"FileOffset\" : 100000, \"FileData\" : \"20202020\", \"FileSize\" : 4, \"Type\" : 1, \"AgentName\" : \"aaa\", \"CommandName\" : \"ccc\"}", &ErrCode);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/file/", ReqObj, &ResultCode);

		if (ResultCode != 200) {
			StkPlPrintf("[NG] Res=%d;\n", ResultCode);
			StkPlExit(1);
		}
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\n");
	}
	{
		StkPlPrintf("Get commandresult (normal scenarios) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/commandresult/", NULL, &ResultCode);

		if (ResultCode != 200) {
			StkPlPrintf("[NG] Res=%d;\n", ResultCode);
			StkPlExit(1);
		}
		bool SuccessFlag = false;
		StkObject* TopObj = ResObj;
		while (ResObj) {
			if (StkPlWcsCmp(ResObj->GetName(), L"") == 0 || StkPlWcsCmp(ResObj->GetName(), L"Data") == 0) {
				ResObj = ResObj->GetFirstChildElement();
				continue;
			}
			if (StkPlWcsCmp(ResObj->GetName(), L"Result") == 0) {
				ResObj = ResObj->GetFirstChildElement();
				while (ResObj) {
					if (StkPlWcsCmp(ResObj->GetName(), L"AgentName") == 0) {
						if (StkPlWcsCmp(ResObj->GetStringValue(), L"aaa") == 0) {
							SuccessFlag = true;
							break;
						}
					}
					ResObj = ResObj->GetNext();
				}
				break;
			}
			ResObj = ResObj->GetNext();
		}
		delete TopObj;
		if (!SuccessFlag) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
	}
	{
		StkPlPrintf("Get commandresult (abnormal scenarios:not exist) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/commandresult/2/", NULL, &ResultCode);

		if (ResultCode != 400) {
			StkPlPrintf("[NG] Res=%d;\n", ResultCode);
			StkPlExit(1);
		}
		StkObject* ChkObj = new StkObject(L"Code", 4100);
		StkObject* FndObj = ResObj->Contains(ChkObj);
		if (FndObj == NULL) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}

		delete ResObj;
		delete ChkObj;

		StkPlPrintf("[OK]\n");
	}
}

void TestGetFile(StkWebAppSend* StkWebAppSendObj)
{
	int TargetFileSize = -1;
#ifdef WIN32
	wchar_t ExefileW[32] = L"sample.exe";
	char Exefile[32] = "sample.exe";
#else
	wchar_t ExefileW[32] = L"sample";
	char Exefile[32] = "sample";
#endif

	{
		StkPlPrintf("Get file list (normal scenarios) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/filelist/", NULL, &ResultCode);

		if (ResultCode != 200) {
			wchar_t TmpBuf[1024] = L"";
			ResObj->ToJson(TmpBuf, 1024);
			StkPlWPrintf(L"[NG] Res=%d;\n%ls\n", ResultCode, TmpBuf);
			StkPlExit(1);
		}
		bool FndFlag4SampleExe = false;
		bool FndFlag4SampleDat = false;

		StkObject* CurObj = ResObj->GetFirstChildElement();
		while (CurObj) {
			if (StkPlWcsCmp(CurObj->GetName(), L"Data") == 0) {
				StkObject* FileListObj = CurObj->GetFirstChildElement();
				while (FileListObj) {
					if (StkPlWcsCmp(FileListObj->GetName(), L"FileInfo") == 0) {
						StkObject* FileInfoObj = FileListObj->GetFirstChildElement();
						wchar_t FileName[256] = L"";
						int FileSize = -1;
						wchar_t FileUpdTime[64] = L"";
						while (FileInfoObj) {
							if (StkPlWcsCmp(FileInfoObj->GetName(), L"Name") == 0) {
								StkPlWcsCpy(FileName, 256, FileInfoObj->GetStringValue());
							} else if (StkPlWcsCmp(FileInfoObj->GetName(), L"Size") == 0) {
								FileSize = FileInfoObj->GetIntValue();
							} else if (StkPlWcsCmp(FileInfoObj->GetName(), L"UpdTime") == 0) {
								StkPlWcsCpy(FileUpdTime, 64, FileInfoObj->GetStringValue());
							}
							FileInfoObj = FileInfoObj->GetNext();
						}
						if (StkPlWcsCmp(FileName, ExefileW) == 0) {
							FndFlag4SampleExe = true;
							TargetFileSize = FileSize;
						}
						if (StkPlWcsCmp(FileName, L"sample.dat") == 0) {
							FndFlag4SampleDat = true;
						}
					}
					FileListObj = FileListObj->GetNext();
				}
			}
			CurObj = CurObj->GetNext();
		}
		delete ResObj;
		if (FndFlag4SampleExe == false || FndFlag4SampleDat == false) {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\n");
	}
	{
		StkPlPrintf("Get file (normal scenarios) ... ");
		int ResultCode = 0;
		int ErrCode = 0;
		int TryCount = TargetFileSize / 100000 + 1;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");

		int FirstPhMem = -1;
		int FirstViMem = -1;
		int LastPhMem = -1;
		int LastViMem = -1;
		const int LOOPLD_COUNT = 50;
		for (int LoopLd = 0; LoopLd < LOOPLD_COUNT; LoopLd++) {
			for (int Loop = 0; Loop < TryCount; Loop++) {
				char TmpUrl[128] = "";
				StkPlSPrintf(TmpUrl, 128, "/api/file/%s/%d/", Exefile, Loop * 100000);
				StkObject*ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, TmpUrl, NULL, &ResultCode);
				if (ResultCode != 200) {
					wchar_t TmpBuf[1024] = L"";
					ResObj->ToJson(TmpBuf, 1024);
					StkPlWPrintf(L"[NG] Res=%d;\n%ls\n", ResultCode, TmpBuf);
					StkPlExit(1);
				}
				delete ResObj;
			}
			StkObject*ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/server/", NULL, &ResultCode);
			if (ResultCode != 200) {
				wchar_t TmpBuf[1024] = L"";
				ResObj->ToJson(TmpBuf, 1024);
				StkPlWPrintf(L"[NG] Res=%d;\n%ls\n", ResultCode, TmpBuf);
				StkPlExit(1);
			}
			StkObject* TgtObj = new StkObject(L"ServerInfo");
			StkObject* PhMemObj = ResObj->Contains(TgtObj)->GetFirstChildElement();
			delete TgtObj;
			while (PhMemObj) {
				if (StkPlWcsCmp(PhMemObj->GetName(), L"UsedPhysicalMemory") == 0) {
					if (LoopLd == 0) {
						FirstPhMem = PhMemObj->GetIntValue();
					} else if (LoopLd == LOOPLD_COUNT - 1) {
						LastPhMem = PhMemObj->GetIntValue();
					}
				}
				if (StkPlWcsCmp(PhMemObj->GetName(), L"UsedVirtualMemory") == 0) {
					if (LoopLd == 0) {
						FirstViMem = PhMemObj->GetIntValue();
					} else if (LoopLd == LOOPLD_COUNT - 1) {
						LastViMem = PhMemObj->GetIntValue();
					}
				}
				PhMemObj = PhMemObj->GetNext();
			}
		}
		StkPlPrintf("1st(Ph/Vi)=(%d/%d) ==> last(Ph/Vi)=(%d/%d): ", FirstPhMem, FirstViMem, LastPhMem, LastViMem);
		if (FirstPhMem * 2 > LastPhMem) {
			StkPlPrintf("[OK]\n");
		} else {
			StkPlPrintf("[NG]\n");
			StkPlExit(1);
		}
	}
}

int main(int Argc, char* Argv[])
{
	StkPlSleepMs(3000);
	StkWebAppSend* StkWebAppSendObj = new StkWebAppSend(10, L"localhost", 10009);
	TestGetUser(StkWebAppSendObj);

	TestNewAgentInfoNotificationNormal(StkWebAppSendObj);
	TestNewAgentInfoNotificationNull(StkWebAppSendObj);
	TestNewAgentInfoNotificationNoAgentInfo(StkWebAppSendObj);
	TestNewAgentInfoNotificationForbiddenChar(StkWebAppSendObj);

	TestGetCommand(StkWebAppSendObj);
	TestNewCommand(StkWebAppSendObj);

	TestPostFile(StkWebAppSendObj);
	TestGetFile(StkWebAppSendObj);

	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
