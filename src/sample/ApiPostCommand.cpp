#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
#include "ApiPostCommand.h"

StkObject* ApiPostCommand::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	StkObject* ResObj = new StkObject(L"");
	if (ReqObj == NULL) {
		ResObj->AppendChildElement(new StkObject(L"Msg0", L"No request has been presented."));
		*ResultCode = 400;
		return ResObj;
	}
	StkObject* CommandObj = ReqObj->GetFirstChildElement();
	if (CommandObj == NULL) {
		ResObj->AppendChildElement(new StkObject(L"Msg0", L"No \"Command\" element has been presented."));
		*ResultCode = 400;
		return ResObj;
	}
	int Id = -1;
	wchar_t Name[DA_MAXLEN_OF_CMDNAME] = L"";
	int Type = 0;
	wchar_t Script[DA_MAXLEN_OF_CMDSCRIPT] = L"";
	StkObject* CurObj = CommandObj->GetFirstChildElement();
	while (CurObj) {
		if (StkPlWcsCmp(CurObj->GetName(), L"Id") == 0) {
			Id = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Name") == 0) {
			StkPlWcsCpy(Name, DA_MAXLEN_OF_CMDNAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Type") == 0) {
			Type = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Script") == 0) {
			StkPlWcsCpy(Script, DA_MAXLEN_OF_CMDSCRIPT, CurObj->GetStringValue());
		}
		CurObj = CurObj->GetNext();
	}
	if (Id == -1) {
		Id = DataAccess::GetInstance()->GetMaxCommandId();
		Id++;
		DataAccess::GetInstance()->SetMaxCommandId(Id);
	}
	DataAccess::GetInstance()->SetCommand(Id, Name, Type, (char*)Script);

	StkObject* TmpObj = new StkObject(L"");
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;
	return TmpObj;
}
