#include "../../hdr/logic/ghost.hpp"

namespace engine {
	const float ghostBaseSpeed = 0.065f; 

	Ghost::Ghost() : Entity(), movementMode(MovementMode::Chase), id(0), 
		modeTicks(0), spawnTicks(0) {
	}

	void Ghost::init() {
		currentDirection = Direction::Nuetral; 
		nextDirection = Direction::Nuetral; 
		position = gs::Vec2f(getSpawnPosition().x, getSpawnPosition().y);
		speed = ghostSpeed; 
		alive = true; 
		movementMode = MovementMode::Chase; 
		modeTicks = rand() % 1000;
		spawnTicks = id * 60 * 2; 
	}
	void Ghost::update() {
		bool reachedTarget = gs::util::distance(
			getTilePosition(), targetDestinations[id]
		) == 0;

		// Only regenerates path every second to increase performance and to prevent
		// ghosts from perfectly following pacman. 
		if (internalTicks % 120 == 0 || reachedTarget || currentDirection ==
			Direction::Nuetral
		) {
			gs::Vec2i newTarget;
			
			// Targets pacman when in Chase mode. 
			if (movementMode == MovementMode::Chase)
				newTarget = pacman.getTilePosition();
			else if (movementMode == MovementMode::Return)
				newTarget = getSpawnPosition(); 
			// Otherwise targets a random tile. 
			else {
				if (internalTicks % 240 != 0 && !reachedTarget &&
					currentDirection != Direction::Nuetral
				)
					goto SEEK; 

				do {
					newTarget.x = rand() % gridSize.x;
					newTarget.y = rand() % gridSize.y; 
				} 
				// Ensures tile is actually valid. 
				while (getGridTile(newTarget).naturalState == TileState::Solid);
			}

			engine::floodGrid(getTilePosition(), newTarget, id);
			engine::generatePath(getTilePosition(), id);
		}

		// Resets ghost when ghost reaches spawning area. 
		if (!alive && reachedTarget) {
			init();
			spawnTicks = 120;
		}
SEEK:
		gs::Vec2i step = aquireNextStep(getTilePosition(), id);
		// Difference between step and ghost position. Shows net movement for 
		// next turn. 
		gs::Vec2i delta = step - getTilePosition();
		
		if (spawnTicks <= 0) {
			// Sets next turn based on what direction the next tile on ghost's path
			// shows. 
			if (delta.x < 0) nextDirection = Direction::Left;
			else if (delta.x > 0) nextDirection = Direction::Right;
			else if (delta.y < 0) nextDirection = Direction::Up;
			else if (delta.y > 0) nextDirection = Direction::Down;
		}

		updatePosition();
		
		++internalTicks; 
		--modeTicks;
		--spawnTicks; 

		// Changes movement after timer runs out. 
		if (modeTicks <= 0 && alive) {
			if (movementMode == MovementMode::Chase) {
				movementMode = MovementMode::Scatter;
				internalTicks = 0;
				// Ghosts spend less time scattering as the level increases.
				modeTicks = std::max(60 * 8 - level * 60, 60);
			}
			else if (movementMode == MovementMode::Scatter) {
CHASE: 
				movementMode = MovementMode::Chase;
				internalTicks = 0; 
				modeTicks = 60 * 6;
			}
			else if (movementMode == MovementMode::Frightened) {
				// Same as chase movement but slower. 
				speed = ghostSpeed; 
				goto CHASE;
			}
		}
	}
	void Ghost::kill() {
		// Ghost return home 150% their normal speed. 
		speed = ghostSpeed * 1.5f;
		alive = false;
		movementMode = MovementMode::Return; 

		// Generates path back to spawn area. 
		engine::floodGrid(getTilePosition(), getSpawnPosition(), id);
		engine::generatePath(getTilePosition(), id);
	}

	void Ghost::generateGhosts() {
		ghosts.clear(); 

		for (int ghostIndex = 0; ghostIndex < mNumOfGhosts; ++ghostIndex) {
			Ghost newGhost; 

			newGhost.id = ghostIndex; 
			newGhost.init(); 

			ghosts.push_back(newGhost); 
		}
	}
	void Ghost::updateGhosts() {
		for (Ghost& ghost : ghosts) 
			ghost.update(); 
	}
	void Ghost::frightenGhosts() {
		for (Ghost& ghost : ghosts) {
			if (ghost.alive) {
				ghost.movementMode = MovementMode::Frightened;
				ghost.speed = ghostSpeed / 2.0f;
				ghost.modeTicks = superDotTicks;
			}
		}
	}

	gs::Vec2i Ghost::getSpawnPosition() {
		gs::Vec2i position = ghostSpawnPosition;

		// Orients the ghosts to be in different spots in spawning area. 
		if (id > 0)
			position.x += -2 + id;
		else
			position.y--; // Red ghost is up first. 

		return position; 
	}

	vector<Ghost> ghosts;
	gs::Vec2i ghostSpawnPosition;
	float ghostSpeed = ghostBaseSpeed;

	void prepareGame() {
		level = 0;
		score = 0; 
		prvsScore = 0; 
		lifes = 2; 

		ghostSpeed = ghostBaseSpeed; 

		engine::pacman.init();
		engine::Ghost::generateGhosts();
	}
}