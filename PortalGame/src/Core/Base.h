#ifndef _PG_UTIL_DEFINES_H_
#define _PG_UTIL_DEFINES_H_

#include <iostream>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define PG_VERSION_MAJOR 0
#define PG_VERSION_MINOR 1
#define PG_VERSION_PATCH 0
#define PG_VERSION STR(PG_VERSION_MAJOR.PG_VERSION_MINOR.PG_VERSION_PATCH)

#define PG_SUCCESS 1
#define PG_FAILURE 0

#define PG_CL_RST "\x1B[0m"
#define PG_FG_RED "\x1B[91m"
#define PG_FG_YLO "\x1B[93m"
#define PG_FG_BLU "\x1B[94m"
#define PG_FG_MGT "\x1B[96m"

#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define pgInfo(msg)	  std::cout << PG_FG_BLU << "[INFO] " << msg << PG_CL_RST << std::endl
#define pgWarn(msg)	  std::cout << PG_FG_YLO << "[WARN] " << msg << PG_CL_RST << std::endl
#define pgError(msg)  std::cout << PG_FG_RED << "[ERROR] " << msg << PG_CL_RST << std::endl

#ifdef _DEBUG 
#define pgDebug(msg) std::cout << PG_FG_MGT << "[DEBUG] " << __FILENAME__ << "(" << __LINE__ << "): " << msg << PG_CL_RST << std::endl
#else
#define pgDebug(msg)
#endif //PG_DEBUG_LOG

#endif