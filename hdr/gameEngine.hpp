#pragma once

//Dependencies
#include "graphics/render.hpp"
#include "logic/generation.hpp"

namespace gameEngine {
	extern gs::util::Clock timer; 

	void create(); 
	void close(); 

	void updateEvents(); 
	void updateGraphics(); 
	// Updates both events and graphics and adds a clock. 
	void updateGameLoop(); 

	bool isOpen(); 
}