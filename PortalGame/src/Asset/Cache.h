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

namespace PGame {
	namespace Asset {
		class Cache {
		public:
			Cache() {
				pgDebug("Created asset Cache - Watching for changes!");
				m_Watcher.Start("assets");
			}

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

			void HotReloadChangedAssets() {
				static double lastCallTime;
				double thisCallTime = glfwGetTime();

				if (thisCallTime - lastCallTime < 2) { return; }

				if (m_Watcher.HasChanges() == false) { return; }

				std::vector<std::string> files;
				m_Watcher.GetChanges(files);

				for (std::string file : files) {
					if (file._Starts_with("shader")) {
						CacheAsset<GL::Shader>("assets/" + file);
					} else {
						pgWarn("Can't reload unknown asset: " << file);
					}

				}

				lastCallTime = thisCallTime;
			}

			void CleanupCache() {
				for (auto x = m_Cache.begin(); x != m_Cache.end();) {
					if (x->second.use_count() == 1) {
						pgWarn(x->first << " is unused and got deleted.");
						x = m_Cache.erase(x);
					} else {
						x++;
					}
				}
			}

		private:
			Asset::Watcher m_Watcher;
			std::unordered_map<std::string, std::shared_ptr<void>> m_Cache;
		};
	}
}