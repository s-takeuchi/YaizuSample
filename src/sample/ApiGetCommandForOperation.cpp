#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiGetCommandForOperation.h"

bool ApiGetCommandForOperation::StopFlag;

StkObject* ApiGetCommandForOperation::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t TargetAgtName[DA_MAXLEN_OF_AGTNAME];
	StkStringParser::ParseInto1Param(UrlPath, L"/api/opcommand/$/", L'$', TargetAgtName, DA_MAXLEN_OF_AGTNAME);
	StkObject* TmpObj = new StkObject(L"");

	if (!DataAccess::GetInstance()->CheckExistenceOfTargetAgent(TargetAgtName)) {
		AddCodeAndMsg(TmpObj, MSG_AGENTINFO_NOT_FOUND, MessageProc::GetMsgEng(MSG_AGENTINFO_NOT_FOUND), MessageProc::GetMsgJpn(MSG_AGENTINFO_NOT_FOUND));
		*ResultCode = 400;
		return TmpObj;
	}

	StkPlSleepMs(5000);
	while (true) {
		StkPlSleepMs(1000);
		if (StopFlag) {
			break;
		}

		int PInterval = 0;
		int SaInterval = 0;
		DataAccess::GetInstance()->GetServerInfo(&PInterval, &SaInterval);
		if (PInterval <= 0) {
			PInterval = 30;
		}

		int Year = 0;
		int Mon = 0;
		int Day = 0;
		int Hour = 0;
		int Min = 0;
		int Sec = 0;
		StkPlGetTime(&Year, &Mon, &Day, &Hour, &Min, &Sec, false);

		int OpCmd = DataAccess::GetInstance()->GetAgentInfoForOpCmd(TargetAgtName);
		int OpStatus = DataAccess::GetInstance()->GetAgentInfoForOpStatus(TargetAgtName);
		if (OpCmd != -1 && OpStatus == -983) {
			int Id[DA_MAXNUM_OF_CMDRECORDS];
			wchar_t Name[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDNAME];
			int Type[DA_MAXNUM_OF_CMDRECORDS];
			char Script[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDSCRIPT];
			wchar_t ServerFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_SERVERFILENAME];
			wchar_t AgentFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_AGENTFILENAME];

			int ResCmdCount = DataAccess::GetInstance()->GetCommand(Id, Name, Type, Script, ServerFileName, AgentFileName);

			for (int Loop = 0; Loop < ResCmdCount; Loop++) {
				if (OpCmd == Id[Loop]) {
					wchar_t WScript[DA_MAXLEN_OF_CMDSCRIPT / 2] = L"";
					wchar_t* Ptr = WScript;
					for (wchar_t* LoopPtr = (wchar_t*)Script[Loop]; *LoopPtr != L'\0'; LoopPtr++) {
						if (*LoopPtr == L'\n') {
							if (Type[Loop] == 1) {
								*Ptr = L'\r';
								*(Ptr + 1) = L'\n';
								Ptr += 2;
							} else {
								*Ptr = *LoopPtr;
								Ptr++;
							}
						} else {
							*Ptr = *LoopPtr;
							Ptr++;
						}
						if ((Ptr - (wchar_t*)Script[Loop]) == DA_MAXLEN_OF_CMDSCRIPT / 2) {
							break;
						}
					}
					*Ptr = L'\0';

					AddCodeAndMsg(TmpObj, 0, L"", L"");
					StkObject* DatObj = new StkObject(L"Data");
					DatObj->AppendChildElement(new StkObject(L"Status", L"Execution"));
					StkObject* CommandObj = new StkObject(L"Command");
					CommandObj->AppendChildElement(new StkObject(L"Id", Id[Loop]));
					CommandObj->AppendChildElement(new StkObject(L"Name", Name[Loop]));
					CommandObj->AppendChildElement(new StkObject(L"Type", Type[Loop]));
					CommandObj->AppendChildElement(new StkObject(L"Script", WScript));
					CommandObj->AppendChildElement(new StkObject(L"ServerFileName", ServerFileName[Loop]));
					wchar_t TargetFullPath[FILENAME_MAX];
					GetFullPathFromFileName(TargetFullPath, ServerFileName[Loop]);
					size_t FileSize = StkPlGetFileSize(TargetFullPath);
					CommandObj->AppendChildElement(new StkObject(L"ServerFileSize", (int)FileSize));
					CommandObj->AppendChildElement(new StkObject(L"AgentFileName", AgentFileName[Loop]));
					DatObj->AppendChildElement(CommandObj);
					TmpObj->AppendChildElement(DatObj);
					*ResultCode = 200;
					break;
				}
			}
			break;
		}

		if ((PInterval == 30 && Sec % 30 < 5) ||
			(PInterval == 60 && Sec < 5) ||
			(PInterval == 300 && Min % 5 == 0 && Sec < 5) ||
			(PInterval == 900 && Min % 15 == 0 && Sec < 5)) {
			AddCodeAndMsg(TmpObj, 0, L"", L"");
			StkObject* DatObj = new StkObject(L"Data");
			DatObj->AppendChildElement(new StkObject(L"Status", L"Timeout"));
			TmpObj->AppendChildElement(DatObj);
			*ResultCode = 200;
			break;
		}
	}
	return TmpObj;
}
