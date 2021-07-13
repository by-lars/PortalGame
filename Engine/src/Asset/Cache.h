#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>

#include <GLFW/glfw3.h>

#include "OpenGL/Shader.h"

#include "Core/Base.h"
#include "Asset/Loader.h"
#include "Asset/Watcher.h"

namespace Engine {
	namespace Asset {
		class Cache {
		public:
			Cache(bool watchForChanges);
			~Cache();
			
			void Update();

			template<typename T>
			std::shared_ptr<T> Get(const std::string& path) {
				if (m_Cache[path] == nullptr) {
					CacheAsset<T>(path);
				}

				return std::static_pointer_cast<T>( m_Cache[path] );
			}

			template<typename T>
			bool CacheAsset(const std::string& path) {
				pgInfo("Caching Asset " << path);
				if (m_Cache[path] == nullptr) {
					m_Cache[path] = std::make_shared<T>();
				}

				return Asset::Load(path, std::static_pointer_cast<T>( m_Cache[path] ));
			}

		private:
			void HotReloadChangedAssets();
			void CleanupCache();

			bool m_WatchForChanges;
			Asset::Watcher m_Watcher;
			std::unordered_map<std::string, std::shared_ptr<void>> m_Cache;
		};
	}
}