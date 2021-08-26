#include "Asset/Cache.h"
#include <GLFW/glfw3.h>

namespace Engine {
	namespace Asset {

		Cache& GetCache() {
			static Cache instance = Cache(true);
			return instance;
		}

		Cache::Cache(bool watchForChanges) : m_WatchForChanges(watchForChanges) {
			pgDebug("Created asset Cache");

			if (m_WatchForChanges) {
				m_Watcher.Start("assets");
			}
		}

		Cache::~Cache() {
			m_Cache.clear();
		}

		void Cache::Update() {
			static double lastCallTime;
			double thisCallTime = glfwGetTime();
			if (thisCallTime - lastCallTime < 0.1f) { return; }

			if (m_WatchForChanges) {
				HotReloadChangedAssets();
			}

			CleanupCache();

			lastCallTime = thisCallTime;
		}

		void Cache::HotReloadChangedAssets() {
			if (m_Watcher.HasChanges() == false) { return; }

			std::vector<std::string> files;
			m_Watcher.GetChanges(files);

			for (std::string file : files) {
				if (m_Cache["assets/" + file] == nullptr) { continue; }

				if (file._Starts_with("shader")) {
					CacheAsset<GL::Shader>("assets/" + file);
				}
				else {
					pgWarn("Can't reload unknown asset: " << file);
				}
			}
		}

		void Cache::CleanupCache() {
			for (auto x = m_Cache.begin(); x != m_Cache.end();) {
				if (x->second.use_count() == 1) {
					pgWarn(x->first << " is unused and got deleted.");
					x = m_Cache.erase(x);
				}
				else {
					x++;
				}
			}
		}
	}
}