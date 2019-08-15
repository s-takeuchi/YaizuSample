#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiGetCommand.h"

StkObject* ApiGetCommand::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType, wchar_t* Token)
{
	if (!CheckCredentials(Token)) {
		*ResultCode = 401;
		return NULL;
	}

	int Id[DA_MAXNUM_OF_CMDRECORDS];
	wchar_t Name[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDNAME];
	int Type[DA_MAXNUM_OF_CMDRECORDS];
	char Script[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_CMDSCRIPT];
	wchar_t ServerFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_SERVERFILENAME];
	wchar_t AgentFileName[DA_MAXNUM_OF_CMDRECORDS][DA_MAXLEN_OF_AGENTFILENAME];

	int ResCount = DataAccess::GetInstance()->GetCommand(Id, Name, Type, Script, ServerFileName, AgentFileName);
	StkObject* TmpObj = new StkObject(L"");
	for (int Loop = 0; Loop < ResCount; Loop++) {
		StkObject* CmdObj = new StkObject(L"Command");
		CmdObj->AppendChildElement(new StkObject(L"Id", Id[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"Name", Name[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"Type", Type[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"Script", (wchar_t*)Script[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"ServerFileName", ServerFileName[Loop]));
		CmdObj->AppendChildElement(new StkObject(L"AgentFileName", AgentFileName[Loop]));
		TmpObj->AppendChildElement(CmdObj);
	}
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
