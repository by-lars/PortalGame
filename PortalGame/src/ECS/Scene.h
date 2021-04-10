#pragma once

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
