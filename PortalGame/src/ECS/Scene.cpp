#include "ECS/Scene.h"
#include "Core/Base.h"


namespace PGame {
	namespace ECS {
		Scene::Scene() {
			for (Entity i = 0; i < MAX_ENTITIES; i++) {
				m_FreeEntityIds.push(i);
			}
		}

		void Scene::Update() {
			if (m_EntitiesToDelete.size() > 0) {
				for (auto& entity : m_EntitiesToDelete) {
					m_FreeEntityIds.push(entity);
					m_EntityComponentMasks.erase(entity);
				}

				for (auto& componentArray : m_ComponentPool) {
					componentArray.second->OnEntitiesDeleted(m_EntitiesToDelete);
				}

				for (auto& system : m_Systems) {
					system->RemoveEntities(m_EntitiesToDelete);
				}

				m_EntitiesToDelete.clear();
			}

			for (auto& system : m_Systems) {
				system->Update();
			}
		}

		Entity Scene::CreateEntity() {
			Entity id = m_FreeEntityIds.front();
			m_FreeEntityIds.pop();
			m_EntityComponentMasks[id] = ComponentMask();
			return id;
		}

		void Scene::DeleteEntity(const Entity& entity) {
			m_EntitiesToDelete.push_back(entity);
		}
	}
}