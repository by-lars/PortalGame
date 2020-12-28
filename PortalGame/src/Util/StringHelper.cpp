#include "Util/StringHelper.h"

namespace PGame {
	std::string SHelper::GetBetween(const std::string& input, const std::string& start, const std::string& end) {
		size_t first = input.find(start);

		if (first == std::string::npos) {
			return "";
		}

		first += start.length();
		size_t last = input.find(end, first);

		return input.substr(first, last - first);
	}
}