#ifndef PG_ECS_ECS_H
#define PG_ECS_ECS_H

#include <stdint.h>
#include <bitset>
#include <vector>

#include "ECS/ECSDefs.h"

namespace PGame {
	namespace ECS {
		//extern int s_ComponentCounter;
		//template <class T>
		//int GetComponentId() {
		//	static int id = s_ComponentCounter++;
		//	return id;
		//}

		//struct EntityDesc {
		//	Entity id;
		//	ComponentMask mask;
		//};

		//class Scene {
		//public:
		//	Scene();

		//	Entity AddEntity();

		//	template <typename T>
		//	void Assign(Entity id) {
		//		int componentId = GetComponentId<T>();
		//		pgDebug("Assigned Component#" << componentId << " to Entity#" << id);
		//		m_Entities[id].mask.set(componentId);
		//	}

		//private:
		//	std::vector<EntityDesc> m_Entities;
		//};
	}
}

#endif