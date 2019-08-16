#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "dataaccess.h"
#include "ApiGetUser.h"

StkObject* ApiGetUser::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType, wchar_t* Token)
{
	wchar_t UserName[DA_MAXLEN_OF_USERNAME];
	wchar_t UserPassword[DA_MAXLEN_OF_PASSWORD];
	int Role = 0;
	wchar_t TargetUrl[DA_MAXLEN_OF_TARGETURL];
	if (!CheckCredentials(Token, UserName)) {
		*ResultCode = 403;
		return NULL;
	}
	DataAccess::GetInstance()->GetTargetUser(UserName, UserPassword, &Role, TargetUrl);

	StkObject* TmpObj = new StkObject(L"");
	StkObject* TmpObjC = new StkObject(L"User");
	TmpObjC->AppendChildElement(new StkObject(L"Name", UserName));
	TmpObjC->AppendChildElement(new StkObject(L"Role", Role));
	TmpObjC->AppendChildElement(new StkObject(L"Url", TargetUrl));
	TmpObj->AppendChildElement(TmpObjC);
	TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
	*ResultCode = 200;

	return TmpObj;
}
