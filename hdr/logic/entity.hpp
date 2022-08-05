#pragma once

//Dependencies
#include "grid.hpp"

namespace engine {
	class Entity {
	public:
		enum class Direction { Nuetral, Left, Right, Up, Down } 
			currentDirection, nextDirection, facingDirection;
		gs::Vec2f position; 
		float speed; 
		bool alive; 

		Entity(); 

		virtual void init() = 0;
		virtual void update() = 0; 

		// Converts position to an integer grid position. 
		gs::Vec2i getTilePosition() const; 
		// True if in center of a tile. 
		bool isFlush() const; 

		// Checks for an intersection between 2 entities using distance. 
		static bool intersect(const Entity& entity0, const Entity& entity1);
	protected:
		// Used for time keeping. 
		int internalTicks;

		void updatePosition(); 
	};
}