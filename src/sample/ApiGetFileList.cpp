#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "serval.h"
#include "ApiGetFileList.h"

StkObject* ApiGetFileList::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t UserName[ApiBase::MAXLEN_OF_USERNAME];
	int UserId = -1;
	if (!CheckCredentials(Token, UserName, &UserId)) {
		StkObject* TmpObj = new StkObject(L"");
		AddCodeAndMsg(TmpObj, MSG_COMMON_AUTH_ERROR, MessageProc::GetMsgEng(MSG_COMMON_AUTH_ERROR), MessageProc::GetMsgJpn(MSG_COMMON_AUTH_ERROR));
		*ResultCode = 401;
		return TmpObj;
	}

	wchar_t TmpWorkDir[FILENAME_MAX] = L"";
	StkPlWcsCpy(TmpWorkDir, FILENAME_MAX, Global::Global_WorkDirPath);
	FileInfoList* FileInfoListObj = StkPlCreateFileInfoList(TmpWorkDir);
	FileInfoList* TopFileInfoListObj = FileInfoListObj;
	bool AllDir = true;
	while (FileInfoListObj) {
		if (FileInfoListObj->IsDir == false) {
			AllDir = false;
		}
		FileInfoListObj = FileInfoListObj->Next;
	}
	FileInfoListObj = TopFileInfoListObj;

	if (FileInfoListObj == NULL || AllDir) {
		if (FileInfoListObj != NULL) {
			StkPlDeleteFileInfoList(TopFileInfoListObj);
		}
		StkObject* TmpObj = new StkObject(L"");
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		*ResultCode = 200;
		return TmpObj;
	} else {
		StkObject* TmpObj = new StkObject(L"");
		StkObject* TmpObjD = new StkObject(L"Data");
		while (FileInfoListObj) {
			if (FileInfoListObj->IsDir == false) {
				StkObject* FileNameObj = new StkObject(L"FileInfo");
				FileNameObj->AppendChildElement(new StkObject(L"Name", FileInfoListObj->FileName));
				FileNameObj->AppendChildElement(new StkObject(L"Size", (int)FileInfoListObj->Size));
				wchar_t UpdTimeStr[DA_MAXLEN_OF_UNIXTIME * 2 + 1] = L"";
				StkPlSwPrintf(UpdTimeStr, DA_MAXLEN_OF_UNIXTIME * 2 + 1, L"%016x", FileInfoListObj->UpdateTime);
				FileNameObj->AppendChildElement(new StkObject(L"UpdTime", UpdTimeStr));
				TmpObjD->AppendChildElement(FileNameObj);
			}
			FileInfoListObj = FileInfoListObj->Next;
		}
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		TmpObj->AppendChildElement(TmpObjD);
		*ResultCode = 200;
		StkPlDeleteFileInfoList(TopFileInfoListObj);
		return TmpObj;
	}
}
