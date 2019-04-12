#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "..\..\..\YaizuComLib\src\stkdata\stkdata.h"
#include "..\..\..\YaizuComLib\src\stkdata\stkdataapi.h"

DataAccess* DataAccess::ThisInstance;

DataAccess::DataAccess()
{
}

DataAccess::~DataAccess()
{
}

// Get this instance
DataAccess* DataAccess::GetInstance()
{
	static int Init = 1;
	if (Init == 1) {
		ThisInstance = new DataAccess();
		Init = 0;
	}
	return ThisInstance;
}
