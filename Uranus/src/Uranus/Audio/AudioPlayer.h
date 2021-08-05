#pragma once
#include "Uranus/Scene/Components.h"

#include<filesystem>
#include <irrKlang.h>

namespace Uranus {

	class AudioPlayer {
		
	public:
		static void Init();
		static void Play(const AudioSourceComponent& as);
	};
}