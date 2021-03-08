#ifndef PG_ECS_COMPONENT_POOL_H
#define PG_ECS_COMPONENT_POOL_H

#include <vector>
#include <memory>

#include "ECS/ComponentArray.h"
#include "Core/Base.h"

namespace PGame {
	namespace ECS {
		class ComponentPool {
		public:
			template<typename T>
			void RegisterComponent() {
				int id = GetComponentId<T>();

				if (m_Pool[id] != nullptr) {
					pgWarn("Tried to register component#" << id << " twice!");
					return;
				}

				m_Pool[id] = std::make_shared<ComponentArray<T>>();
				pgDebug("Registered '" << typeid(T).name() << "' as component#" << id);
			}

		private:
			std::unordered_map<size_t, std::shared_ptr<IComponentArray>> m_Pool;
		};
	}
}

#endif