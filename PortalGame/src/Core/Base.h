#pragma once

#include <iostream>
#include <string.h>
#include <memory>
#include <cassert>
#include <thread>

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

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define pgInfo(msg)	  std::cout << PG_FG_BLU << "[INFO ] [" << std::this_thread::get_id() << "] "  << msg << PG_CL_RST << std::endl
#define pgWarn(msg)	  std::cout << PG_FG_YLO << "[WARN ] [" << std::this_thread::get_id() << "] "  << msg << PG_CL_RST << std::endl
#define pgError(msg)  std::cout << PG_FG_RED << "[ERROR] [" << std::this_thread::get_id() << "] " << __FILENAME__ << "(" << __LINE__ << "): " << msg << PG_CL_RST << std::endl

#define pgAssert(test, msg) if(!(test)) {pgError(STR_HELPER(test) << " -> " << msg); return PG_FAILURE;}
#define pgTest(test, msg) if(!(test)) {pgError(STR_HELPER(test) << " -> " << msg); return PG_FAILURE;}

#ifdef _DEBUG 
#define pgDebug(msg) std::cout << PG_FG_MGT << "[DEBUG] [" << std::this_thread::get_id() << "] " << __FILENAME__ << "(" << __LINE__ << "): " << msg << PG_CL_RST << std::endl
#else
#define pgDebug(msg)
#endif //PG_DEBUG_LOG