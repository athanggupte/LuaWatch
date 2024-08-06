#pragma once
#include "lwdef.h"

enum lw_LogLevel
{
	lw_KDebug = 0,
	lw_KWarn,
	lw_KError,
	lw_KFatalError,
};

DEFINE_TYPE(lw_LogData);

typedef int (*lw_LogCallback) (const lw_LogData * logdata);
