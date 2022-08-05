#pragma once

//Dependencies
#include "../resources.hpp"

namespace engine {
	namespace audio {
		extern sf::SoundBuffer soundBuffers[mNumOfSounds];
		extern sf::Sound sounds[mNumOfSounds];
		extern bool isMuted; 

		void loadAssets();

		// Even though platSound() prevents sounds from being played when muted
		// this stops already playing sounds. 
		void muteSounds(); 
		void playSound(int soundId); 
	}
}