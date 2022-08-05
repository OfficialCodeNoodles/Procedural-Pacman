#pragma once

//Dependencies
#include "entity.hpp"
#include "pacman.hpp"
#include "path.hpp"

namespace engine {
	extern const float ghostBaseSpeed;

	class Ghost final : public Entity {
	public:
		enum class MovementMode { Chase, Scatter, Frightened, Return } 
			movementMode;
		int id; 

		Ghost();

		void init() override; 
		void update() override; 
		void kill(); 

		static void generateGhosts();
		static void updateGhosts();
		static void frightenGhosts(); 
	private:
		int modeTicks; 
		int spawnTicks; 

		gs::Vec2i getSpawnPosition();
	};

	extern vector<Ghost> ghosts; 
	extern gs::Vec2i ghostSpawnPosition; 
	extern float ghostSpeed; 

	void prepareGame(); 
}