#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "MessageCode.h"
#include "ApiPostUser.h"

StkObject* ApiPostUser::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, int LocaleType, wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");

	if (!IsAdminUser(Token)) {
		ResObj->AppendChildElement(CommonError_NoExecRight());
		*ResultCode = 403;
		return ResObj;
	}
	if (ReqObj == NULL) {
		ResObj->AppendChildElement(CommonError_NoRequest());
		*ResultCode = 400;
		return ResObj;
	}
	StkObject* UserObj = ReqObj->GetFirstChildElement();
	if (UserObj == NULL) {
		ResObj->AppendChildElement(CommonError_NoElemInRequest(L"User"));
		*ResultCode = 400;
		return ResObj;
	}
	int Id = -1;
	wchar_t Name[] = L"";
	wchar_t Password[DA_MAXLEN_OF_PASSWORD] = L"";
	int Role = 0;
	wchar_t Url[DA_MAXLEN_OF_TARGETURL] = L"";
	StkObject* CurObj = UserObj->GetFirstChildElement();
	while (CurObj) {
		if (StkPlWcsCmp(CurObj->GetName(), L"Id") == 0) {
			Id = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Name") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_USERNAME) {
				ResObj->AppendChildElement(CommonError_StringLenError(L"user name", DA_MAXLEN_OF_USERNAME));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(Name, DA_MAXLEN_OF_USERNAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Password") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= DA_MAXLEN_OF_PASSWORD) {
				ResObj->AppendChildElement(CommonError_StringLenError(L"password", DA_MAXLEN_OF_PASSWORD));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(Name, DA_MAXLEN_OF_PASSWORD, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Role") == 0) {
			Role = CurObj->GetIntValue();
		}
		CurObj = CurObj->GetNext();
	}
	return NULL;
}
