#ifndef PG_ECS_ECSDEFS_H
#define PG_ECS_ECSDEFS_H

#include <bitset>

namespace PGame {
	namespace ECS {
		using Entity = uint64_t;

		const Entity MAX_ENTITIES = 5000;
		const Entity MAX_COMPONENTS = 32;

		using ComponentMask = std::bitset<MAX_COMPONENTS>;

		extern int s_ComponentCounter;
		template <class T>
		int GetComponentId() {
			static int id = s_ComponentCounter++;
			return id;
		}
	}
}

#endif