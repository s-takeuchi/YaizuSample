#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "sample.h"
#include "dataaccess.h"
#include "ApiGetCommandForStatus.h"

bool ApiGetCommandForStatus::StopFlag;

StkObject* ApiGetCommandForStatus::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t TargetAgtName[DA_MAXLEN_OF_AGTNAME];
	StkStringParser::ParseInto1Param(UrlPath, L"/api/statuscommand/$/", L'$', TargetAgtName, DA_MAXLEN_OF_AGTNAME);
	StkObject* TmpObj = new StkObject(L"");

	if (!DataAccess::GetInstance()->CheckExistenceOfTargetAgent(TargetAgtName)) {
		AddCodeAndMsg(TmpObj, MSG_AGENTINFO_NOT_FOUND, MessageProc::GetMsgEng(MSG_AGENTINFO_NOT_FOUND), MessageProc::GetMsgJpn(MSG_AGENTINFO_NOT_FOUND));
		*ResultCode = 400;
		return TmpObj;
	}

	long long ReqTime = DataAccess::GetInstance()->SetAgentInfoForTime(TargetAgtName, 0);
	long long IniTime = DataAccess::GetInstance()->GetAgentInfoForIniTime(TargetAgtName);

	while (true) {
		if (StopFlag) {
			break;
		}

		int PInterval = 0;
		int SaInterval = 0;
		DataAccess::GetInstance()->GetServerInfo(&PInterval, &SaInterval);

		if (PInterval <= 0) {
			PInterval = 30;
		}
		if (SaInterval <= 0) {
			SaInterval = 300;
		}

		// Check that it is time to return a response.
		long long CurTime = StkPlGetTime();
		bool ItIsTimeToSa = false;
		bool ItIsTimeToPolling = false;
		for (int Loop = 0; Loop < 5; Loop++) {
			long long CurTimeOffsetS;
			long long IniTimeOffsetS;
			long long CurTimeOffsetP;
			long long IniTimeOffsetP;
			CurTimeOffsetS = CurTime % SaInterval;
			IniTimeOffsetS = (IniTime + Loop) % SaInterval;
			if (CurTimeOffsetS == IniTimeOffsetS) {
				ItIsTimeToSa = true;
				break;
			}
			CurTimeOffsetP = CurTime % PInterval;
			IniTimeOffsetP = (IniTime + Loop) % PInterval;
			if (CurTimeOffsetP == IniTimeOffsetP) {
				ItIsTimeToPolling = true;
				break;
			}
		}

		if (ItIsTimeToSa) {
			AddCodeAndMsg(TmpObj, 0, L"", L"");
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
			int OpStatus[DA_MAXNUM_OF_AGTRECORDS];
			int OpCmd[DA_MAXNUM_OF_AGTRECORDS];
			long long ReqTime[DA_MAXNUM_OF_AGTRECORDS];
			long long AcqTime[DA_MAXNUM_OF_AGTRECORDS];
			long long UpdTime[DA_MAXNUM_OF_AGTRECORDS];
			long long IniTime[DA_MAXNUM_OF_AGTRECORDS];
			long long OpeTime[DA_MAXNUM_OF_AGTRECORDS];
			int ReAgtCount = DataAccess::GetInstance()->GetAgentInfo(AgtName, Status, StatusCmd, OpStatus, OpCmd, ReqTime, AcqTime, UpdTime, IniTime, OpeTime);

			// Check the agent specified in URI is managed by server
			int TargetAgtIndex = -1;
			for (int Loop = 0; Loop < ReAgtCount; Loop++) {
				if (StkPlWcsCmp(AgtName[Loop], TargetAgtName) == 0) {
					TargetAgtIndex = Loop;
				}
			}

			StkObject* DatObj = new StkObject(L"Data");
			if (ReAgtCount >= 1 && TargetAgtIndex != -1) {
				DatObj->AppendChildElement(new StkObject(L"Status", L"Execution"));
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
					wchar_t TargetFullPath[FILENAME_MAX];
					GetFullPathFromFileName(TargetFullPath, ServerFileName[FoundIndex]);
					size_t FileSize = StkPlGetFileSize(TargetFullPath);
					CommandObj->AppendChildElement(new StkObject(L"ServerFileSize", (int)FileSize));
					CommandObj->AppendChildElement(new StkObject(L"AgentFileName", AgentFileName[FoundIndex]));
					DatObj->AppendChildElement(CommandObj);
				}
			}
			TmpObj->AppendChildElement(DatObj);
			StkPlSleepMs(5000);
			break;
		} else if (ItIsTimeToPolling) {
			AddCodeAndMsg(TmpObj, 0, L"", L"");
			StkObject* DatObj = new StkObject(L"Data");
			DatObj->AppendChildElement(new StkObject(L"Status", L"Timeout"));
			TmpObj->AppendChildElement(DatObj);
			*ResultCode = 200;
			StkPlSleepMs(5000);
			break;
		} else {
			StkPlSleepMs(1000);
		}
	}
	return TmpObj;
}
