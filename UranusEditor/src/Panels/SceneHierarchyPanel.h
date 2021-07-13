#pragma once

#include "Uranus/Core/Core.h"
#include "Uranus/Scene/Entity.h"

namespace Uranus {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();

	private:
		void DrawComponents(Entity entity);
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> _Context;
		Entity _SelectionContext;
	};
}