#pragma once

namespace Uranus {

	class Timestep {
	public:
		Timestep(float time)
			:_Time(time) {}
		inline float GetSeconds() const { return _Time; }
		inline float GetMilliSeconds() const { return _Time * 1000.0f; }

		operator float() { return _Time; }

	private:
		float _Time;
	};
}