#pragma once

#include "Event.h"

namespace Uranus {

	class URANUS_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(float width, float height)
			: _Width(width), _Height(height) {}

		inline float GetWidth() const { return _Width; }
		inline float GetHeight() const { return _Height; }

		std::string ToString() const override{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << _Width << ", " << _Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		float _Width, _Height;
	};

	class URANUS_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}