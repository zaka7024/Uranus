#pragma once

#include "Event.h"
#include "Uranus/Core/MouseCodes.h"

namespace Uranus {

	class MouseMovedEvent : public Event {
	private:
		float _MouseX, _MouseY;
	
	public:
		MouseMovedEvent(float mouseX, float mouseY)
			: _MouseX(mouseX), _MouseY(mouseY) {}

		inline float GetX() const { return _MouseX; }
		inline float GetY() const { return _MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << _MouseX << ", " << _MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse)
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: _OffsetX(offsetX), _OffsetY(offsetY) {}

		inline float GetOffsetX() const { return _OffsetX; }
		inline float GetOffsetY() const { return _OffsetY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << _OffsetX << ", " << _OffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse)

	private:
		float _OffsetX, _OffsetY;
	};


	class MouseButtonEvent : public Event {
	public:

		inline MouseButtons GetMouseButton() const { return _MouseButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton)
	
	protected:
		MouseButtonEvent(MouseButtons button)
			: _MouseButton(button) {}

		MouseButtons _MouseButton;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(MouseButtons button)
			: MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressed: " << (int)_MouseButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonRelesedEvent : public MouseButtonEvent {
	public:
		MouseButtonRelesedEvent(MouseButtons button)
			: MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonRelesedEvent: " << (int)_MouseButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonRelesed)
	};
}
