#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "sample.h"
#include "ApiGetCommand.h"

StkObject* ApiGetCommand::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* TmpObj = new StkObject(L"");

	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	if (!CheckCredentials(Token, UserName)) {
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}

	int Id[DA_MAXNUM_OF_CMDRECORDS];
	wchar_t Name[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDNAME];
	int Type[DA_MAXNUM_OF_CMDRECORDS];
	char Script[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDSCRIPT];
	wchar_t ServerFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_SERVERFILENAME];
	wchar_t AgentFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_AGENTFILENAME];

	int ResCount = DataAccess::GetInstance()->GetCommand(Id, Name, Type, Script, ServerFileName, AgentFileName);
	StkObject* TmpObjD = new StkObject(L"Data");
	for (int Loop = 0; Loop < ResCount; Loop++) {
		StkObject* CmdObj = new StkObject(L"Command");
		CmdObj->AppendChildElement(new StkObject(L"Id", Id[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"Name", Name[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"Type", Type[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"Script", (wchar_t*)Script[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"ServerFileName", ServerFileName[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"AgentFileName", AgentFileName[Loop]));
		TmpObjD->AppendChildElement(CmdObj);
	}
	AddCodeAndMsg(TmpObj, 0, L"", L"");
	TmpObj->AppendChildElement(TmpObjD);
	*ResultCode = 200;
	return TmpObj;
}
