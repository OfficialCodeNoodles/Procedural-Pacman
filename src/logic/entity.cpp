#include "../../hdr/logic/entity.hpp"

namespace engine {
	Entity::Entity() : currentDirection(Direction::Nuetral),
		nextDirection(Direction::Nuetral), facingDirection(Direction::Nuetral),
		speed(0.075f), alive(true), internalTicks(0) {
	}
	
	gs::Vec2i Entity::getTilePosition() const {
		return gs::Vec2i(std::round(position.x), std::round(position.y));
	}

	bool Entity::isFlush() const {
		const gs::Vec2i tilePosition = getTilePosition(); 
		// Is flush if on or very close to center of a Tile. 
		return std::abs(position.x - tilePosition.x) <= speed && 
			std::abs(position.y - tilePosition.y) <= speed;
	}

	bool Entity::intersect(const Entity& entity0, const Entity& entity1) {
		float distance = gs::util::distance(
			entity0.position, entity1.position
		);
		return distance < 0.75f; // 3/4 of a tile. 
	}

	void Entity::updatePosition() {
		const gs::Vec2i tilePosition = getTilePosition();

		// Move entity based on currentDirection. 
		if (currentDirection == Direction::Left)
			position.x -= speed;
		else if (currentDirection == Direction::Right)
			position.x += speed;
		else if (currentDirection == Direction::Up)
			position.y -= speed;
		else if (currentDirection == Direction::Down)
			position.y += speed; 

		if (isFlush()) {
			Direction prvsDirection = currentDirection; 
			bool safeToTurn = false; 

			// Checks weither it is safe to turn in all 4 directions by 
			// checking the nearby tile states. 
			if (getGridTile(gs::Vec2i(tilePosition.x - 1, 
				tilePosition.y)).naturalState == TileState::Solid
			) {
				if (currentDirection == Direction::Left)
					currentDirection = Direction::Nuetral;
			}
			else if (nextDirection == Direction::Left)
				safeToTurn = true; 

			if (getGridTile(gs::Vec2i(tilePosition.x + 1, 
				tilePosition.y)).naturalState == TileState::Solid
			) {
				if (currentDirection == Direction::Right)
					currentDirection = Direction::Nuetral;
			}
			else if (nextDirection == Direction::Right)
				safeToTurn = true; 

			if (getGridTile(gs::Vec2i(tilePosition.x,
				tilePosition.y - 1)).naturalState ==
				TileState::Solid
			) {
				if (currentDirection == Direction::Up) 
					currentDirection = Direction::Nuetral;
			}
			else if (nextDirection == Direction::Up)
				safeToTurn = true; 

			if (getGridTile(gs::Vec2i(tilePosition.x, 
				tilePosition.y + 1)).naturalState ==
				TileState::Solid
			) {
				if (currentDirection == Direction::Down)
					currentDirection = Direction::Nuetral;
			}
			else if (nextDirection == Direction::Down)
				safeToTurn = true; 

			// If it is determined to be safe then turn. 
			if (safeToTurn) currentDirection = nextDirection; 

			// Makes entity pefectly flush with tile to prevent drifting. 
			if (currentDirection != prvsDirection) {
				position.x = std::round(position.x); 
				position.y = std::round(position.y); 
			}
		}

		// Used to change direction of sprites. 
		if (currentDirection != Direction::Nuetral) 
			facingDirection = currentDirection; 
	}
}