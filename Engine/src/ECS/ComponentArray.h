#pragma once

#include <array>
#include <unordered_map>

#include "Core/Base.h"
#include "ECS/ECSDefs.h"
#include "ECS/Components/Basic.h"

namespace Engine {
	namespace ECS {
		class IComponentArray {
		public:
			virtual ~IComponentArray() = default;
			virtual void OnEntitiesDeleted(const std::vector<Entity>& entities) = 0;
		};

		template <typename T>
		class ComponentArray : public IComponentArray{
		public:
			ComponentArray() {
				m_Size = 0;
			}

			void OnEntitiesDeleted(const std::vector<Entity>& entities) override {
				for (const Entity& entity : entities) {
					if (m_EntityToDense.find(entity) != m_EntityToDense.end()) {
						ENGINE_DEBUG("Deleting Entity " << entity << " from component array " << typeid(T).name());
						Erase(entity);
					}
				}
			}

			T& Insert(Entity entity, const T& data) {
				ENGINE_ASSERT(m_EntityToDense.find(entity) == m_EntityToDense.end(), "Component '" << typeid(T).name() << "' is already on Entity#" << entity);
				m_Dense[m_Size] = data;
				m_EntityToDense[entity] = m_Size;
				m_DenseToEntity[m_Size] = entity;
				m_Size++;
				return m_Dense[m_EntityToDense[entity]];
			}

			void Erase(Entity entityToErase) {
				ENGINE_ASSERT(m_EntityToDense.find(entityToErase) != m_EntityToDense.end(), "Can't access non existing Entity#" << entityToErase);

				size_t lastIndex = m_Size - 1;
				size_t removedIndex = m_EntityToDense[entityToErase];
				Entity movedEntity = m_DenseToEntity[lastIndex];

				//Swap last element in dense array with deleted element
				m_Dense[removedIndex] = m_Dense[lastIndex];

				//Update mapping
				m_EntityToDense[movedEntity] = removedIndex;
				m_DenseToEntity[removedIndex] = movedEntity;

				//Cleanup 
				m_EntityToDense.erase(entityToErase);
				m_DenseToEntity.erase(lastIndex);

				m_Size--;
			}

			T& Get(Entity entity) {
				ENGINE_ASSERT(m_EntityToDense.find(entity) != m_EntityToDense.end(), "Can't access non existing Entity#" << entity);
				return m_Dense[m_EntityToDense[entity]];
			}

			void Set(Entity entity, const T& data) {
				ENGINE_ASSERT(m_EntityToDense.find(entity) != m_EntityToDense.end(), "Can't access non existing Entity#" << entity);
				m_Dense[m_EntityToDense[entity]] = data;
			}

			size_t Size() { return m_Size; }

		private:
			std::array<T, ECS::MAX_ENTITIES> m_Dense = {  };
			std::unordered_map<Entity, size_t> m_EntityToDense;
			std::unordered_map<size_t, Entity> m_DenseToEntity;
			size_t m_Size;
		};
	}
}
