#pragma once

#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"

// Audit log
#define MSG_COMDELETE            4000
#define MSG_COMADD               4001
#define MSG_COMMODIFY            4002
#define MSG_CMDOPERATIONCHGD     4004
#define MSG_CMDSTATUSACQCHGD     4005
#define MSG_CMDOPSTARTED         4006
#define MSG_CMDOPENDED           4007
#define MSG_SERVICESTARTED       4010
#define MSG_SERVICESTOPPED       4011
#define MSG_SERVICETERMINATED    4012
#define MSG_AGTDELETE            4014
#define MSG_NEWAGTNOTIFIED       4015
#define MSG_SVRINFOUPDATED       4016
#define MSG_FILEUPLOADED         4017
#define MSG_FILEDELETED          4018

// Command
#define MSG_COMMANDNOTEXIST      4020
#define MSG_NOCOMMANDNAME        4023
#define MSG_COMNAMELENGTHERR     4024
#define MSG_COMSCRIPTLENERR      4025
#define MSG_FILENAMELENERR       4026
#define MSG_FILENAMEFBDNCHAR     4027
#define MSG_DUPCMDNAME           4028

// Server Info
#define MSG_NOPOLLINGINTVL       4041
#define MSG_NOSAINTVL            4042
#define MSG_INVALIDPOINTVL       4043
#define MSG_INVALIDSVINTVL       4044

// Agent Info
#define MSG_AGENTINFO_NOT_FOUND  4060

// File
#define MSG_FILE_ACCESS_ERROR    4080
#define MSG_FILE_EXCEED_SIZE     4081
#define MSG_FILE_INVALID_ORDER   4082

// Result
#define MSG_RESULT_NOTEXIST      4100

// Common error message
#define MSG_NO_ELEM_IN_REQUEST   4201
#define MSG_STRING_LEN_ERROR     4202
#define MSG_NOREQUEST            4203
#define MSG_NO_EXEC_RIGHT        4204
#define MSG_FORBIDDEN_CHAR       4205
#define MSG_COMMON_AUTH_ERROR    4206

#define SERVICE_VERSION "1.0.0"
#define SERVICE_VERSION_L L"1.0.0"

// Global variables
class Global {
public:
	static wchar_t Global_WorkDirPath[FILENAME_MAX];
};

void GetFullPathFromFileName(wchar_t[FILENAME_MAX], const wchar_t[FILENAME_MAX]);
void CommonError_NoElemInRequest(StkObject*, const wchar_t*);
void CommonError_StringLenError(StkObject*, const wchar_t*, int);
void CommonError_ForbiddenChar(StkObject*, const wchar_t*);
void EventLogging(wchar_t*, wchar_t*, int);
