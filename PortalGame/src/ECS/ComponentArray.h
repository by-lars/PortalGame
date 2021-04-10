#pragma once

#include <array>
#include <unordered_map>

#include "ECS/ECSDefs.h"

namespace PGame {
	namespace ECS {
		class IComponentArray {
		public:
			virtual ~IComponentArray() = default;
		};

		template <typename T>
		class ComponentArray : public IComponentArray {
		public:
			ComponentArray() {
				m_Size = 0;
			}

			void Insert(Entity index, const T& data) {
				//TODO: Check bitmask of entity before to avoid duplicated entries
				m_Dense[m_Size] = data;
				m_EntityToDense[index] = m_Size;
				m_DenseToEntity[m_Size] = index;
				m_Size++;
			}

			void Erase(Entity entity) {
				size_t lastIndex = m_Size - 1;
				size_t removedIndex = m_EntityToDense[entity];
				Entity movedEntity = m_DenseToEntity[lastIndex];

				//Swap last element in dense array with deleted element
				m_Dense[removedIndex] = m_Dense[lastIndex];

				//Update mapping
				m_EntityToDense[movedEntity] = removedIndex;
				m_DenseToEntity[removedIndex] = movedEntity;

				//Cleanup 
				m_EntityToDense.erase(entity);
				m_DenseToEntity.erase(lastIndex);

				m_Size--;
			}

			T& Get(Entity entity) {
				return m_Dense[m_EntityToDense[entity]];
			}

			void Set(Entity entity, const T& data) {
				m_Dense[m_EntityToDense[entity]] = data;
			}

			size_t Size() { return m_Size; }

		private:
			std::array<T, ECS::MAX_ENTITIES> m_Dense = { 0 };
			std::unordered_map<Entity, size_t> m_EntityToDense;
			std::unordered_map<size_t, Entity> m_DenseToEntity;

			size_t m_Size;
		};
	}
}
