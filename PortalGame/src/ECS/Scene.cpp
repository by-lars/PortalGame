#include "ECS/Scene.h"
#include "Core/Base.h"


namespace PGame {
	namespace ECS {
		Scene::Scene() {
			Registry = std::make_unique<ComponentPool>();
		}
	}
}