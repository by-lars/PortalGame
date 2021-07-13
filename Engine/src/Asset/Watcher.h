#pragma once
#include <string>
#include <thread>
#include <Windows.h>
#include <vector>
#include <atomic>
#include <mutex>

namespace Engine {
	namespace Asset {
		class Watcher {
		public:
			Watcher();
			Watcher(const std::string& directory);
			~Watcher();

			bool HasChanges();
			void GetChanges(std::vector<std::string> &out_changes);

			bool Start();
			bool Start(const std::string& directory);
			void Stop();
		private:
			void Watch();

			HANDLE m_Handle;
			std::string m_Directory;
			std::thread m_WatchThread;
			std::atomic<bool> m_Running;
			std::mutex m_ChangedFilesMutex;
			std::vector<std::string> m_ChangedFiles;
		};
	}
}
