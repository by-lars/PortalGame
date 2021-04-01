#ifndef PG_ECS_SCENE_H
#define PG_ECS_SCENE_H

#include <memory>

#include "ECS/ECSDefs.h"
#include "ECS/ComponentPool.h"

namespace PGame {
	namespace ECS {
		class Scene {
		public:
			Scene();

			Entity CreateEntity();


			std::unique_ptr<ComponentPool> Registry;
		private:
		};
	}
}

#endif