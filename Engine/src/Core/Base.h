#pragma once

#include <iostream>
#include <cstring>
#include <memory>
#include <cassert>
#include <thread>

#ifdef __unix__
#include <signal.h>
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 1
#define ENGINE_VERSION_PATCH 0
#define ENGINE_VERSION STR(ENGINE_VERSION_MAJOR.ENGINE_VERSION_MINOR.ENGINE_VERSION_PATCH)

#define ENGINE_SUCCESS 1
#define ENGINE_FAILURE 0

#ifdef _MSC_VER
#define ENGINE_DEBUG_BREAK __debugbreak()
#else
#define ENGINE_DEBUG_BREAK raise(SIGTRAP);
#endif

#define EN_CL_RST "\x1B[0m"
#define EN_FG_RED "\x1B[91m"
#define EN_FG_YLO "\x1B[93m"
#define EN_FG_BLU "\x1B[94m"
#define EN_FG_CYN "\x1B[96m"
#define EN_FG_MGT "\x1B[35m"
#define EN_FG_BRED "\x1B[31;1m"
#define EN_FG_BGRN "\x1B[32;1m"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define ENGINE_INFO(msg) std::cout << EN_FG_BLU << "[ I ] [" << std::this_thread::get_id() << "] "  << msg << EN_CL_RST << std::endl
#define ENGINE_WARN(msg) std::cout << EN_FG_YLO << "[ W ] [" << std::this_thread::get_id() << "] "  << msg << EN_CL_RST << std::endl
#define ENGINE_ERROR(msg) std::cout << EN_FG_RED << "[ E ] [" << std::this_thread::get_id() << "] " << __FILENAME__ << "(" << __LINE__ << "): " << msg << EN_CL_RST << std::endl

#define ENGINE_ASSERT(test, msg) if(!(test)) {ENGINE_ERROR(STR_HELPER(test) << " -> " << msg ); ENGINE_DEBUG_BREAK;}

#ifdef _DEBUG 
#define ENGINE_DEBUG(msg) std::cout << EN_FG_CYN << "[ D ] [" << std::this_thread::get_id() << "] " << __FILENAME__ << "(" << __LINE__ << "): " << msg << EN_CL_RST << std::endl
#define ENGINE_DEBUG_ALLOC(msg) std::cout << EN_FG_BGRN << "[ + ] [" << std::this_thread::get_id() << "] " << __FILENAME__ << "(" << __LINE__ << "): " << msg << EN_CL_RST << std::endl 
#define ENGINE_DEBUG_DEALLOC(msg) std::cout << EN_FG_BRED << "[ - ] [" << std::this_thread::get_id() << "] " << __FILENAME__ << "(" << __LINE__ << "): " << msg << EN_CL_RST << std::endl 
#else
#define ENGINE_DEBUG(msg)
#define ENGINE_DEBUG_ALLOC(msg)
#define ENGINE_DEBUG_DEALLOC(msg)
#endif 

