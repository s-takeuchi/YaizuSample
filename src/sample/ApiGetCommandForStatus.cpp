#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "ApiGetCommandForStatus.h"

bool ApiGetCommandForStatus::StopFlag;

StkObject* ApiGetCommandForStatus::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t TargetAgtName[DA_MAXLEN_OF_AGTNAME];
	StkStringParser::ParseInto1Param(UrlPath, L"/api/statuscommand/$/", L'$', TargetAgtName, DA_MAXLEN_OF_AGTNAME);
	StkObject* TmpObj = new StkObject(L"");
	while (true) {
		StkPlSleepMs(1000);
		if (StopFlag) {
			break;
		}

		int PInterval = 0;
		int SaInterval = 0;
		wchar_t BucketPath[DA_MAXLEN_OF_BUCKETPATH] = L"";
		DataAccess::GetInstance()->GetServerInfo(&PInterval, &SaInterval, BucketPath);
		if (PInterval <= 0) {
			PInterval = 30;
		}
		if (SaInterval <= 0) {
			SaInterval = 300;
		}

		int Year = 0;
		int Mon = 0;
		int Day = 0;
		int Hour = 0;
		int Min = 0;
		int Sec = 0;
		StkPlGetTime(&Year, &Mon, &Day, &Hour, &Min, &Sec, false);

		if ((SaInterval == 300  && Min % 5 == 0  && Sec == 0) ||
			(SaInterval == 900  && Min % 15 == 0 && Sec == 0) ||
			(SaInterval == 1800 && Min % 30 == 0 && Sec == 0) ||
			(SaInterval == 3600 && Min == 0      && Sec == 0)) {

			TmpObj->AppendChildElement(new StkObject(L"Msg0", L"Execution"));
			*ResultCode = 200;

			int Id[DA_MAXNUM_OF_CMDRECORDS];
			wchar_t Name[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDNAME];
			int Type[DA_MAXNUM_OF_CMDRECORDS];
			char Script[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDSCRIPT];
			wchar_t ServerFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_SERVERFILENAME];
			wchar_t AgentFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_AGENTFILENAME];
			wchar_t WScript[DA_MAXLEN_OF_CMDSCRIPT / 2] = L"";
			wchar_t* Ptr = WScript;
			int ResCmdCount = DataAccess::GetInstance()->GetCommand(Id, Name, Type, Script, ServerFileName, AgentFileName);

			wchar_t AgtName[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_AGTNAME];
			int Status[DA_MAXNUM_OF_AGTRECORDS];
			int StatusCmd[DA_MAXNUM_OF_AGTRECORDS];
			wchar_t TimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
			wchar_t TimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
			wchar_t UdTimeUtc[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
			wchar_t UdTimeLocal[DA_MAXNUM_OF_AGTRECORDS][DA_MAXLEN_OF_TIME];
			int ReAgtCount = DataAccess::GetInstance()->GetAgentInfo(AgtName, Status, StatusCmd, TimeUtc, TimeLocal, UdTimeUtc, UdTimeLocal);

			// Check the agent specified in URI is managed by server
			int TargetAgtIndex = -1;
			for (int Loop = 0; Loop < ReAgtCount; Loop++) {
				if (StkPlWcsCmp(AgtName[Loop], TargetAgtName) == 0) {
					TargetAgtIndex = Loop;
				}
			}

			if (ResCmdCount >= 1 && ReAgtCount >= 1 && TargetAgtIndex != -1) {
				int FoundIndex = -1;
				for (int Loop = 0; Loop < ResCmdCount; Loop++) {
					if (Id[Loop] == StatusCmd[TargetAgtIndex]) {
						FoundIndex = Loop;
					}
				}
				if (FoundIndex != -1) {
					for (wchar_t* Loop = (wchar_t*)Script[FoundIndex]; *Loop != L'\0'; Loop++) {
						if (*Loop == L'\n') {
							if (Type[FoundIndex] == 1) {
								*Ptr = L'\r';
								*(Ptr + 1) = L'\n';
								Ptr += 2;
							} else {
								*Ptr = *Loop;
								Ptr++;
							}
						} else {
							*Ptr = *Loop;
							Ptr++;
						}
						if ((Ptr - (wchar_t*)Script[FoundIndex]) == DA_MAXLEN_OF_CMDSCRIPT / 2) {
							break;
						}
					}
					*Ptr = L'\0';

					StkObject* CommandObj = new StkObject(L"Command");
					CommandObj->AppendChildElement(new StkObject(L"Id", Id[FoundIndex]));
					CommandObj->AppendChildElement(new StkObject(L"Name", Name[FoundIndex]));
					CommandObj->AppendChildElement(new StkObject(L"Type", Type[FoundIndex]));
					CommandObj->AppendChildElement(new StkObject(L"Script", WScript));
					CommandObj->AppendChildElement(new StkObject(L"ServerFileName", ServerFileName[FoundIndex]));
					wchar_t TargetFullPath[DA_MAXLEN_OF_SERVERFILENAME];
					DataAccess::GetInstance()->GetFullPathFromFileName(TargetFullPath, ServerFileName[FoundIndex]);
					size_t FileSize = StkPlGetFileSize(TargetFullPath);
					CommandObj->AppendChildElement(new StkObject(L"ServerFileSize", (int)FileSize));
					TmpObj->AppendChildElement(CommandObj);
				}
			}
			break;
		}

		if ((PInterval == 30 && Sec % 30 == 0) ||
			(PInterval == 60 && Sec == 0) ||
			(PInterval == 300 && Min % 5 == 0 && Sec == 0) ||
			(PInterval == 900 && Min % 15 == 0 && Sec == 0)) {
			TmpObj->AppendChildElement(new StkObject(L"Msg0", L"Timeout"));
			*ResultCode = 200;
			break;
		}
	}
	return TmpObj;
}
