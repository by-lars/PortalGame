#include "ECS/Systems/DummySystem.h"
#include "ECS/Components/Basic.h"

namespace PGame {
	namespace ECS {
		DummySystem::DummySystem(Scene& scene) : m_Scene(scene) {
			m_Signature.set(ECS::GetComponentId<ECS::Tag>());
		}

		void DummySystem::Update() {
			for (auto& entity : m_Entities) {
				auto& tag = m_Scene.GetComponent<ECS::Tag>(entity);
				pgInfo(tag.Tag);
			}
		}
	}
}