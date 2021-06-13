#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <set>

#include "Core/Base.h"

#include "ECS/ECSDefs.h"
#include "ECS/Systems/SystemBase.h"
#include "ECS/ComponentArray.h"

namespace PGame {
	namespace ECS {
		class Scene {
		public:
			Scene();

			void Update();

			Entity CreateEntity();
			void DeleteEntity(const Entity& entity);

			template<typename Component>
			void RegisterComponent() {
				int id = ECS::GetComponentId<Component>();

				pgAssert(m_ComponentPool[id] == nullptr, "Component '" << typeid(Component).name() << "' is already registed.");

				m_ComponentPool[id] = std::make_shared<ComponentArray<Component>>();
				pgDebug("Registered '" << typeid(Component).name() << "' as component#" << id);
			}

			template<typename Component>
			Component& Assign(const Entity& entity) {
				for (auto& system : m_Systems) {
					if (system->GetSignature().test(ECS::GetComponentId<Component>())) {
						system->AddEntity(entity);
					}
				}

				pgDebug("Added '" << typeid(Component).name() << "' to entity#" << entity);
				return GetComponentArray<Component>()->Insert(entity, Component());
			}

			template<typename Component>
			void Remove(const Entity& entity) {
				GetComponentArray<Component>()->Erase(entity);

				for (auto& system : m_Systems) {
					if (system->GetSignature().test(ECS::GetComponentId<Component>())) {
						system->RemoveEntity(entity);
					}
				}

				pgDebug("Removed '" << typeid(Component).name() << "' from entity#" << entity);
			}

			template<typename Component>
			Component& GetComponent(const Entity& entity) {
				return GetComponentArray<Component>()->Get(entity);
			}

			template<typename T>
			void RegisterSystem() {
				m_Systems.push_back(std::make_shared<T>(*this));
				pgDebug("Registered System '" << typeid(T).name() << "'");
			}

		private:
			template<typename Component>
			std::shared_ptr<ComponentArray<Component>> GetComponentArray() {
				int id = ECS::GetComponentId<Component>();

				pgAssert(m_ComponentPool[id] != nullptr,
					"Can't get Component Array for unregistered component '" << typeid(Component).name());

				return std::static_pointer_cast<ComponentArray<Component>>(m_ComponentPool[id]);
			}

			std::queue<Entity> m_FreeEntityIds;
			std::vector<Entity> m_EntitiesToDelete;
			std::vector<std::shared_ptr<ISystem>> m_Systems;
			std::unordered_map<size_t, std::shared_ptr<IComponentArray>> m_ComponentPool;
		};
	}
}
