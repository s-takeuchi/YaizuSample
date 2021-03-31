#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "sample.h"
#include "ApiGetFileList.h"

StkObject* ApiGetFileList::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t TmpWorkDir[FILENAME_MAX] = L"";
	StkPlWcsCpy(TmpWorkDir, FILENAME_MAX, Global::Global_WorkDirPath);
	StkPlWcsCat(TmpWorkDir, FILENAME_MAX, L"\\*");
	FileNameChain* FileNameChainObj = StkPlCreateFileNameList(TmpWorkDir);
	FileNameChain* TopFileNameChainObj = FileNameChainObj;
	if (FileNameChainObj == NULL) {
		StkObject* TmpObj = new StkObject(L"");
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		*ResultCode = 200;
		return TmpObj;
	} else {
		StkObject* TmpObj = new StkObject(L"");
		StkObject* TmpObjD = new StkObject(L"Data");
		while (FileNameChainObj) {
			StkObject* FileNameObj = new StkObject(L"FileInfo");
			FileNameObj->AppendChildElement(new StkObject(L"Name", FileNameChainObj->FileName));
			TmpObjD->AppendChildElement(FileNameObj);
			FileNameChainObj = FileNameChainObj->Next;
		}
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		TmpObj->AppendChildElement(TmpObjD);
		*ResultCode = 200;
		StkPlDeleteFileNameChain(TopFileNameChainObj);
		return TmpObj;
	}
}
