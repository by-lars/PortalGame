#ifndef PG_UTIL_STRINGHELPER_H
#define PG_UTIL_STRINGHELPER_H

#include <string>

namespace PGame {
	namespace SHelper {
		std::string GetBetween(const std::string& input, const std::string& start, const std::string& end);
	}
}


#endif