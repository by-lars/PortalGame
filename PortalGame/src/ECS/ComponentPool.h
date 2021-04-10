#pragma once

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

			template<typename T>
			void Assign(Entity  entityID) {
				int id = GetComponentId<T>();

				if (m_Pool[id] == nullptr) {
					pgError("Can't add unregistered component#" << id << " - " << typeid(T).name() << " to entity#" << entityID);
					return;
				}

				GetComponentArray<T>(id)->Insert(entityID, T());
				pgDebug("Added '" << typeid(T).name() << "' to entity#" << id);
			}

			template<typename T>
			void Remove(Entity entityID) {
				int id = GetComponentId<T>();

				if (m_Pool[id] == nullptr) {
					pgError("Can't remove unregistered component#" << id << " - " << typeid(T).name() << " to entity#" << entityID);
					return;
				}

				GetComponentArray<T>(id)->Erase(entityID);
				pgDebug("Removed '" << typeid(T).name() << "' to entity#" << id);
			}

			template<typename T>
			T& GetComponent(Entity entityID) {
				int id = GetComponentId<T>();

				if (m_Pool[id] == nullptr) {
					pgError("Can't get data for unregistered component#" << id << " - " << typeid(T).name());
					return;
				}

				return GetComponentArray<T>(id)->Get(entityID);
			}

		private:
			template<typename T>
			std::shared_ptr<ComponentArray<T>> GetComponentArray(int id) {
				return std::static_pointer_cast<ComponentArray<T>>(m_Pool[id]);
			}

		private:
			std::unordered_map<size_t, std::shared_ptr<IComponentArray>> m_Pool;
		};
	}
}
