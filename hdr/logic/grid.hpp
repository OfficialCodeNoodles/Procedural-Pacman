#pragma once

//Dependencies
#include "tile.hpp"

namespace engine {
	extern const gs::Vec2i defaultGridSize;
	// The grid used in normal pacman. 
	extern const int defaultGrid[21][19];
	// Returned if tile requested is oob.
	extern const Tile outOfBoundsTile; 

	extern gs::Vec2i gridSize; 
	// A 2D array storing all of the Tiles in the map. 
	extern Tile grid[mGridWidth][mGridHeight]; 
	extern int numOfDots; 

	// Resets the grids dots. 
	void initGrid(); 
	// Gets a Tile from the grid and adds a check if out of bounds. 
	Tile getGridTile(gs::Vec2i position); 
	// Sets a Tile's currentState. 
	void setGridTileState(gs::Vec2i position, TileState tileState); 
	// Updates the Solid Tiles face member to connect with nearby Tiles.
	void generateGridFaces(); 
}