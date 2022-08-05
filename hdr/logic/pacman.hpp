#pragma once

//Dependencies
#include "input.hpp"
#include "entity.hpp"
#include "ghost.hpp"
#include "audio.hpp"

namespace engine {
	class Pacman final : public Entity {
	public:
		Pacman(); 

		void init() override; 
		void update() override; 
	private:
	};

	extern Pacman pacman; 
	extern gs::Vec2i spawnPosition; 
	extern int superDotTicks; 
	extern int ghostsEaten; 
}