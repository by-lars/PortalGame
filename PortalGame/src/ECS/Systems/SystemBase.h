#pragma once

#include <set>
#include <vector>

#include "ECS/ECSDefs.h"
#include "Core/Base.h"

namespace PGame {
	namespace ECS {
		class ISystem {
		public:
			ISystem() = default;
			virtual ~ISystem() = default;

			virtual void Update() = 0;

			inline void AddEntity(Entity entity) { m_Entities.insert(entity); }
			inline void RemoveEntity(Entity entity) { m_Entities.erase(entity); }
			inline void RemoveEntities(const std::vector<Entity>& entities) {
				for (auto& entity : entities) {
					m_Entities.erase(entity);
				}
			}

			inline const ComponentMask& GetSignature() { return m_Signature; }

		protected:
			ComponentMask m_Signature;
			std::set<Entity> m_Entities;
		};
	}
}