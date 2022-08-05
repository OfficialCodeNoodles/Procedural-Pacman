#pragma once

//Dependencies
#include "grid.hpp"

namespace engine {
	// Used to allow hashing for gs::Vec2i.
	struct HashContainer {
		size_t operator()(const gs::Vec2i& vec) const;
	};

	// A list of tiles to be checked. 
	extern queue<gs::Vec2i> frontier; 
	// Tiles that have already been searched. 
	extern vector<gs::Vec2i> searched; 
	// Collection of points that link all open tiles together to create a path.
	extern unordered_map<gs::Vec2i, gs::Vec2i, HashContainer> pathMap; 
	// A path for each one of the ghosts. 
	extern vector<gs::Vec2i> paths[mNumOfGhosts]; 
	// The target position for the path of each ghost. 
	extern gs::Vec2i targetDestinations[mNumOfGhosts];

	// Generates a path system by filling grid from a starting point. 
	void floodGrid(gs::Vec2i start, gs::Vec2i target, int ghostIndex);
	// Generates path from pathMap based on a starting point. 
	void generatePath(gs::Vec2i start, int ghostIndex);
	// Gets next position on path to target. 
	gs::Vec2i aquireNextStep(gs::Vec2i start, int ghostIndex);
}