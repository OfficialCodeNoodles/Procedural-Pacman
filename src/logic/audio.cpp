#include "../../hdr/logic/audio.hpp"

namespace engine {
	namespace audio {
		sf::SoundBuffer soundBuffers[mNumOfSounds];
		sf::Sound sounds[mNumOfSounds];
		bool isMuted = false;

		void loadAssets() {
			for (int soundIndex = 0; soundIndex < mNumOfSounds; ++soundIndex) {
				if (soundBuffers[soundIndex].loadFromFile(
					mAssetDirectory + "sounds/" + std::to_string(soundIndex) +
					".wav"
				))
					sounds[soundIndex].setBuffer(soundBuffers[soundIndex]); 
			}
		}

		void muteSounds() {
			for (sf::Sound& sound : sounds) 
				sound.stop(); 
		}
		void playSound(int soundId) {
			if (!isMuted) sounds[soundId].play(); 
		}
	}
}