#pragma once

//Dependencies
#include "../resources.hpp"

namespace engine {
	namespace input {
		enum Key {
			MoveLeft, 
			MoveRight, 
			MoveUp, 
			MoveDown, 
			Space, 
			F1, 
			F2,
			F3,
			Mute,
			HideInfoMenu
		};
		extern sf::Keyboard::Key keyMap[mNumOfKeys]; 
		extern bool keyLocks[mNumOfKeys];
		extern bool keySwitches[mNumOfKeys]; 
		extern int keyTicks[mNumOfKeys]; 

		bool isKeyPressed(Key key);
		bool isKeyClicked(Key key);
		int getKeyTicks(Key key);

		void initKeys();
		void updateKeys(); 
	}
}