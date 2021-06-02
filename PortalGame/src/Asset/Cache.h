#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "OpenGL/Shader.h"

#include "Core/Base.h"
#include "Asset/Loader.h"
#include "Asset/Watcher.h"


namespace PGame {
	namespace Asset {
		class Cache {
		public:
			Cache() {
			#ifdef _DEBUG
				pgDebug("Created asset Cache - Watching for changes!");
				m_Watcher.Start("assets");
			#else
				pgDebug("Created asset Cache");
			#endif
			}

			void OnFileChanged(std::string filename, Asset::FileAction action) {
				filename = "assets/" + filename;
				if (m_Cache[filename] != nullptr) {
					pgInfo("Recompiling?");
					m_Loader.Load(filename, std::static_pointer_cast<GL::Shader>(m_Cache[filename]));
				}
				else {
					pgInfo("Shits null");
				}
			}

			template<typename T>
			std::shared_ptr<T> Get(const std::string& path) {
				if (m_Cache[path] == nullptr) {
					pgInfo("Caching Asset " << path);
					CacheAsset<T>(path);
				}

				return std::static_pointer_cast<T>( m_Cache[path] );
			}

			template<typename T>
			bool CacheAsset(const std::string& path) {
				if (m_Cache[path] == nullptr) {
					m_Cache[path] = std::make_shared<T>();
				}

				return m_Loader.Load(path, std::static_pointer_cast<T>( m_Cache[path] ));
			}

		private:
			#ifdef _DEBUG
			Asset::Watcher m_Watcher;
			#endif

			Asset::Loader m_Loader;
			std::unordered_map<std::string, std::shared_ptr<void>> m_Cache;
		};
	}
}