#pragma once

#include "Uranus/Core/Core.h"
#include "Uranus/Events/Event.h"
#include "Uranus/Core/Timestep.h"

namespace Uranus {

	class URANUS_API Layer {

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {};

		inline const std::string& GetLayerName() const{ return _DebugeName; }

	private:
		std::string _DebugeName;
	};

}