#pragma once
#include <string>

namespace PGame {
	class DirectoryWatcher {
	public:
		DirectoryWatcher();
		DirectoryWatcher(const std::string& directory);
		~DirectoryWatcher();

		void Start();
		void Stop();
	private:
		void Watch();
		std::string m_Directory;
	};
}
