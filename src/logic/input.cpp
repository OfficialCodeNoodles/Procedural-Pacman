#include "../../hdr/logic/input.hpp"

namespace engine {
	namespace input {
		sf::Keyboard::Key keyMap[mNumOfKeys] = {
			sf::Keyboard::A,
			sf::Keyboard::D,
			sf::Keyboard::W,
			sf::Keyboard::S,
			sf::Keyboard::Space, 
			sf::Keyboard::F1,
			sf::Keyboard::F2,
			sf::Keyboard::F3,
			sf::Keyboard::M,
			sf::Keyboard::H
		};
		bool keyLocks[mNumOfKeys];
		bool keySwitches[mNumOfKeys];
		int keyTicks[mNumOfKeys];

		bool isKeyPressed(Key key) {
			return sf::Keyboard::isKeyPressed(keyMap[key]);
		}
		bool isKeyClicked(Key key) {
			return keySwitches[key]; 
		}
		int getKeyTicks(Key key) {
			return keyTicks[key]; 
		}

		void initKeys() {
			for (int keyIndex = 0; keyIndex < mNumOfKeys; ++keyIndex) {
				keyLocks[keyIndex] = false;
				keySwitches[keyIndex] = false;
				keyTicks[keyIndex] = 0;
			}
		}
		void updateKeys() {
			for (int keyIndex = 0; keyIndex < mNumOfKeys; ++keyIndex) {
				sf::Keyboard::Key currentKey = keyMap[keyIndex];
				bool keyPressed = sf::Keyboard::isKeyPressed(currentKey);

				if (!keyLocks[keyIndex] && keyPressed)
					keySwitches[keyIndex] = true;
				else
					keySwitches[keyIndex] = false;

				keyLocks[keyIndex] = keyPressed;

				if (!keyLocks[keyIndex])
					keyTicks[keyIndex] = 0;
				else
					++keyTicks[keyIndex];
			}
		}
	}
}