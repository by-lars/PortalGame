#pragma once

#include <bitset>

namespace PGame {
	namespace ECS {
		const int MAX_ENTITIES = 5000;
		const int MAX_COMPONENTS = 32;

		using ComponentMask = std::bitset<MAX_COMPONENTS>;	

		using Entity = uint64_t;

		extern int s_ComponentCounter;
		template <class T>
		int GetComponentId() {
			static int id = s_ComponentCounter++;
			return id;
		}
	}
}
