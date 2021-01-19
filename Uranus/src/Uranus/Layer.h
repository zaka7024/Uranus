#pragma once

#include "Uranus/Core.h"
#include "Events/Event.h"

namespace Uranus {

	class URANUS_API Layer {

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetLayerName() const{ return _debugeName; }

	private:
		std::string _debugeName;
	};

}