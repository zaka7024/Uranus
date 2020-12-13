#pragma once

#include "Event.h"

namespace Uranus {

	class URANUS_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return _keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);
	
	protected:
		int _keyCode;
		KeyEvent(const int keyCode) 
			: _keyCode(keyCode) {}
	};

	class URANUS_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(const int keyCode, int repateCount)
			: KeyEvent(keyCode), _repeatCount(repateCount) {};

		inline int GetRepeatCount() const { return _repeatCount; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent" << _keyCode << " (" << _repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	
	private:
		int _repeatCount;
	};

	class URANUS_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(const int keyCode)
			: KeyEvent(keyCode) {};

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent" << _keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
