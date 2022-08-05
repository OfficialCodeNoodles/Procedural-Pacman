#include "../../hdr/logic/pacman.hpp"

namespace engine {
	Pacman::Pacman() : Entity() {
	}

	void Pacman::init() {
		currentDirection = Direction::Nuetral;
		nextDirection = Direction::Nuetral;
		position = gs::Vec2f(spawnPosition.x, spawnPosition.y);
	}
	void Pacman::update() {
		const gs::Vec2i tilePosition = getTilePosition();

		// Sets pacman's next turn based on what key is pressed. 
		if (input::isKeyPressed(input::MoveLeft))
			nextDirection = Direction::Left;
		else if (input::isKeyPressed(input::MoveRight))
			nextDirection = Direction::Right; 
		else if (input::isKeyPressed(input::MoveUp))
			nextDirection = Direction::Up;
		else if (input::isKeyPressed(input::MoveDown))
			nextDirection = Direction::Down;

		updatePosition(); 

		if (isFlush()) {
			// Checks if pacman is on a dot, if so then the dot is eaten. 
			if (getGridTile(tilePosition).state == TileState::Dot) {
				setGridTileState(tilePosition, TileState::Empty);

				numOfDots--; 
				score += 20;
				audio::playSound(0); 
			}
			else if (getGridTile(tilePosition).state == TileState::SuperDot) {
				setGridTileState(tilePosition, TileState::Empty);

				//Resets ghostsEaten if not still in super dot mode. 
				if (superDotTicks <= 0) ghostsEaten = 0; 

				numOfDots--; 
				score += 100;
				superDotTicks = 60 * 8; 
				Ghost::frightenGhosts(); 
				audio::muteSounds(); 
				audio::playSound(0);
			}
		}

		if (superDotTicks > 0 && superDotTicks % 120 == 0) 
			audio::playSound(3);

		--superDotTicks; 

		for (Ghost& ghost : ghosts) {
			// Checks for collision between pacman and any one ghost. 
			if (Entity::intersect(*this, ghost)) {
				// Doesn't apply to dead ghosts going back to spawn. 
				if (ghost.alive) {
					if (ghost.movementMode == Ghost::MovementMode::Frightened) {
						ghost.kill();
						ghostsEaten++; 

						// Makes each ghost eaten in a row worth double from
						// 200 -> 400 -> 800 -> 1600 points. 
						score += std::pow(2, ghostsEaten) * 100; 
						audio::playSound(2);
					}
					else {
						gameState = GameState::Death; 
						gameStateTicks = 60 * 3;
						superDotTicks = 0; 

						lifes--; 
						engine::audio::muteSounds();
						audio::playSound(1); 
					}
				}
			}
		}
	}

	Pacman pacman = Pacman(); 
	gs::Vec2i spawnPosition;
	int superDotTicks = 0;
	int ghostsEaten = 0; 
}