#include "Asset/Watcher.h"

#include <Windows.h>
#include <array>
#include <filesystem>

#include "Core/Base.h"

namespace Engine {
	namespace Asset {
		Watcher::Watcher() : m_Handle(nullptr) {
			ENGINE_DEBUG("Created Directory Watcher");
		}

		Watcher::Watcher(const std::string& directory)
			: m_Handle(nullptr) {
			ENGINE_DEBUG("Created Directory Watcher - Dir=" << directory);
			m_Directory = std::filesystem::absolute(directory).string();
		}

		Watcher::~Watcher() {
			if (m_WatchThread.joinable()) {
				Stop();
			}
		}

		bool Watcher::Start(const std::string& directory) {
			m_Directory = std::filesystem::absolute(directory).string();
			return Start();
		}

		bool Watcher::Start() {
			if (m_Directory.empty()) {
				ENGINE_ERROR("Can't watch empty path!");
				return ENGINE_FAILURE;
			}

			m_Handle = CreateFile(
				m_Directory.c_str(),
				FILE_LIST_DIRECTORY,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS,
				NULL);

			if (m_Handle == INVALID_HANDLE_VALUE) {
				ENGINE_ERROR("Couldn't open handle to watch directory -> " << m_Directory);
				return false;
			}

			m_Running = true;
			m_WatchThread = std::thread(&Watcher::Watch, this);

			ENGINE_DEBUG("Starting to watch " << m_Directory);

			return true;
		}

		void Watcher::Stop() {
			m_Running = false;
			CancelIoEx(m_Handle, NULL);
			m_WatchThread.join();
			CloseHandle(m_Handle);
		}

		bool Watcher::HasChanges() {
			return m_ChangedFiles.size() > 0;
		}

		void Watcher::GetChanges(std::vector<std::string> &out_changes) {
			std::lock_guard<std::mutex> guard(m_ChangedFilesMutex);
			out_changes = std::move(m_ChangedFiles);
			m_ChangedFiles.clear();
		}

		void Watcher::Watch() {
			std::array<BYTE, 4069> buffer = { 0 };
			DWORD bytesReturned;

			while (m_Running.load()) {
				BOOL succ = ReadDirectoryChangesW(m_Handle, &buffer[0], (DWORD)buffer.size(), true,
					FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesReturned, NULL, NULL);

				if (succ == false) { 
					if (m_Running) { ENGINE_ERROR("IO Error while trying to watch directory" << m_Directory); } 
					continue; 
				}

				FILE_NOTIFY_INFORMATION* info = (FILE_NOTIFY_INFORMATION*)&buffer[0];
				do {
					std::wstring tmpString(info->FileName, info->FileNameLength / sizeof(wchar_t));
					std::string fileName(tmpString.begin(), tmpString.end());
					std::replace(fileName.begin(), fileName.end(), '\\', '/');

					m_ChangedFilesMutex.lock();
					if (std::find(m_ChangedFiles.begin(), m_ChangedFiles.end(), fileName) == m_ChangedFiles.end()) {
						m_ChangedFiles.push_back(fileName);
						ENGINE_DEBUG("File Changed: " << fileName);
					}
					m_ChangedFilesMutex.unlock();


					info = (FILE_NOTIFY_INFORMATION*)((BYTE*)info + info->NextEntryOffset);
				} while (info->NextEntryOffset != 0);
			}

			ENGINE_DEBUG("Stopped watching " << m_Directory);
		}
	}
}