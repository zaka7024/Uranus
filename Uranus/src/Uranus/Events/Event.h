#pragma once

#include "Uranus/Core/Core.h"

#include <sstream>
#include <string>

namespace Uranus {

	// Blocking Event System

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonRelesed, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};


#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override{ return category; }

	class URANUS_API Event {
	public:

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const {
			return GetName();
		}

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

		bool isHandled = false;
	};
	
	inline std::ostream& operator<< (std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

	class EventDispatcher {
		template<typename T>
		using EventFun = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& e)
			:_Event(e) {}

		template<typename T>
		bool Dispatch(EventFun<T> fun) {
			if (_Event.GetEventType() == T::GetStaticType()) {
				_Event.isHandled =  fun(*(T*)&_Event);
				return true;
			}
			return false;
		}

	private:
		Event& _Event;
	};
}