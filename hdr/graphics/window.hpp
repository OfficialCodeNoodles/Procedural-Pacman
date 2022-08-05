#pragma once

//Dependencies
#include "../resources.hpp"

namespace engine {
	namespace render {
		namespace window {
			extern const gs::Vec2i windowSize;

			extern sf::RenderWindow* winmain;
			extern bool isFullscreen; 

			extern int framerate; 
			extern int virtualFramerate; 

			extern sf::Image logo; 

			void create(bool fullscreen); 
			void initStates(); 
			void update(); 
			void close(); 

			void begin(gs::Color fillColor = gs::Color());
			void end(); 
		}
	}
}