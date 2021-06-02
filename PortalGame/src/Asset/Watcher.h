#pragma once
#include <string>
#include <thread>
#include <Windows.h>
#include <functional>
#include <queue>

namespace PGame {
	namespace Asset {
		enum class FileAction {
			CREATED = FILE_ACTION_ADDED,
			DELETED = FILE_ACTION_REMOVED,
			MODIFIED = FILE_ACTION_MODIFIED
		};

		class Watcher {
		public:
			Watcher();
			Watcher(const std::string& directory);
			~Watcher();

			bool Start();
			bool Start(const std::string& directory);
			void Stop();
		private:
			void Watch();

			HANDLE m_Handle;
			std::string m_Directory;
			std::thread m_WatchThread;
			std::atomic<bool> m_Running;

			std::queue<std::string> m_ChangedFiles;
		};
	}
}
