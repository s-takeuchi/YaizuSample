#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "dataaccess.h"
#include "ApiGetUser.h"

StkObject* ApiGetUser::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType, wchar_t* Token)
{
	int UserId = 0;
	wchar_t UserName[DA_MAXLEN_OF_USERNAME] = L"";
	wchar_t UserPassword[DA_MAXLEN_OF_PASSWORD] = L"";
	int Role = 0;
	wchar_t TargetUrl[DA_MAXLEN_OF_TARGETURL] = L"";
	if (!CheckCredentials(Token, UserName)) {
		*ResultCode = 403;
		return NULL;
	}
	DataAccess::GetInstance()->GetTargetUserByName(UserName, &UserId, UserPassword, &Role);
	if (StkPlWcsStr(UrlPath, L"?target=all") != NULL) {
		if (Role != 0) {
			*ResultCode = 403;
			return NULL;
		}
		int AryUserId[DA_MAXNUM_OF_USERRECORDS];
		wchar_t AryUserName[DA_MAXNUM_OF_USERRECORDS][DA_MAXLEN_OF_USERNAME];
		wchar_t AryUserPassword[DA_MAXNUM_OF_USERRECORDS][DA_MAXLEN_OF_PASSWORD];
		int AryRole[DA_MAXNUM_OF_USERRECORDS];
		int Cnt = DataAccess::GetInstance()->GetTargetUsers(AryUserId, AryUserName, AryUserPassword, AryRole);
		StkObject* TmpObj = new StkObject(L"");
		StkObject* TmpObjD = new StkObject(L"Data");
		for (int Loop = 0; Loop < Cnt; Loop++) {
			StkObject* TmpObjC = new StkObject(L"User");
			TmpObjC->AppendChildElement(new StkObject(L"Id", AryUserId[Loop]));
			TmpObjC->AppendChildElement(new StkObject(L"Name", AryUserName[Loop]));
			TmpObjC->AppendChildElement(new StkObject(L"Role", AryRole[Loop]));
			TmpObjD->AppendChildElement(TmpObjC);
		}
		TmpObj->AppendChildElement(TmpObjD);
		TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
		*ResultCode = 200;
		return TmpObj;
	} else {
		StkObject* TmpObj = new StkObject(L"");
		StkObject* TmpObjD = new StkObject(L"Data");
		StkObject* TmpObjC = new StkObject(L"User");
		TmpObjC->AppendChildElement(new StkObject(L"Id", UserId));
		TmpObjC->AppendChildElement(new StkObject(L"Name", UserName));
		TmpObjC->AppendChildElement(new StkObject(L"Role", Role));
		TmpObjD->AppendChildElement(TmpObjC);
		TmpObj->AppendChildElement(TmpObjD);
		TmpObj->AppendChildElement(new StkObject(L"Msg0", L""));
		*ResultCode = 200;
		return TmpObj;
	}
	return NULL;
}
