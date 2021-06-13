#pragma once

#include "ECS/Scene.h"
#include "ECS/Systems/SystemBase.h"

namespace PGame {
	namespace ECS {
		class DummySystem : public ISystem {
		public:
			DummySystem(Scene& scene);
			void Update() override;
		private:
			Scene& m_Scene;
		};
	}
}