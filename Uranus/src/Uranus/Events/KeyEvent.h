#pragma once

#include "Event.h"

namespace Uranus {

	class URANUS_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return _KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);
	
	protected:
		int _KeyCode;
		KeyEvent(const int keyCode) 
			: _KeyCode(keyCode) {}
	};

	class URANUS_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(const int keyCode, int repateCount)
			: KeyEvent(keyCode), _RepeatCount(repateCount) {};

		inline int GetRepeatCount() const { return _RepeatCount; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << _KeyCode << " (" << _RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	
	private:
		int _RepeatCount;
	};

	class URANUS_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(const int keyCode)
			: KeyEvent(keyCode) {};

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << _KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
