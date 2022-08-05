#pragma once

//Dependencies
#include "../resources.hpp"

namespace engine {
	enum class TileState { Empty, Solid, Dot, SuperDot, GhostExit };

	struct Tile {
		// The original state of the Tile when initialized. 
		TileState naturalState;
		// The current state of the Tile. Having this seperate variable allows 
		// the grid to be mutable while also containing the original state.
		TileState state;
		// Used for Solid Tiles to descibe what texture it should render. 
		int face;
	};
}