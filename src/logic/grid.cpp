#include "../../hdr/logic/grid.hpp"

namespace engine {
	const gs::Vec2i defaultGridSize = gs::Vec2i(19, 21); 
	const int defaultGrid[21][19] = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, 
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1 }, 
		{ 1, 3, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 3, 1 }, 
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 }, 
		{ 1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1 }, 
		{ 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1 }, 
		{ 1, 1, 1, 1, 2, 1, 1, 1, 0, 1, 0, 1, 1, 1, 2, 1, 1, 1, 1 }, 
		{ 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0 }, 
		{ 0, 0, 0, 1, 2, 1, 0, 1, 1, 4, 1, 1, 0, 1, 2, 1, 0, 0, 0 }, 
		{ 0, 0, 0, 1, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0, 2, 1, 0, 0, 0 }, 
		{ 0, 0, 0, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 1, 0, 0, 0 }, 
		{ 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0 }, 
		{ 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1 }, 
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1 }, 
		{ 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1 }, 
		{ 1, 3, 2, 1, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 1, 2, 3, 1 }, 
		{ 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1 }, 
		{ 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1 }, 
		{ 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1 }, 
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 }, 
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }  
	}; 
	const Tile outOfBoundsTile = { TileState::Solid, TileState::Solid, -1 };

	gs::Vec2i gridSize = gs::Vec2i(19, 21); 
	Tile grid[mGridWidth][mGridHeight];
	int numOfDots = 0; 
	
	void initGrid() {
		numOfDots = 0; 

		for (int xpos = 0; xpos < gridSize.x; ++xpos) {
			for (int ypos = 0; ypos < gridSize.y; ++ypos) {
				Tile& currentTile = grid[xpos][ypos]; 

				// Sets the current state of the tiles to match the one 
				// generated. This resets the dots. 
				currentTile.state = currentTile.naturalState; 

				// Counts number of dots. 
				if (currentTile.state == TileState::Dot ||
					currentTile.state == TileState::SuperDot
				)
					numOfDots++; 
			}
		}
	}
	Tile getGridTile(gs::Vec2i position) {
		// Checks if position is in bounds of the grid. 
		if (position.x >= 0 && position.y >= 0 && 
			position.x < gridSize.x && position.y < gridSize.y
		)
			return grid[position.x][position.y];

		// Returned if position is not located in the grid. 
		return outOfBoundsTile;
	}
	void setGridTileState(gs::Vec2i position, TileState tileState) {
		// Checks if position is in bounds of the grid. 
		if (position.x >= 0 && position.y >= 0 && 
			position.x < gridSize.x && position.y < gridSize.y
		)
			grid[position.x][position.y].state = tileState; 
	}
	void generateGridFaces() {
		for (int xpos = 0; xpos < gridSize.x; ++xpos) {
			for (int ypos = 0; ypos < gridSize.y; ++ypos) {
				Tile& currentTile = grid[xpos][ypos];
				TileState currentState = currentTile.state; 

				if (currentState == TileState::Solid) {
					currentTile.face = 0;
					
					if (ypos > 0) {
						if (getGridTile(gs::Vec2i(xpos, ypos - 1)).state == 
							TileState::Solid
						)
							currentTile.face |= 1; 
					}
					if (xpos < gridSize.x - 1) {
						if (getGridTile(gs::Vec2i(xpos + 1, ypos)).state == 
							TileState::Solid
						)
							currentTile.face |= 2;
					}
					if (ypos < gridSize.y - 1) {
						if (getGridTile(gs::Vec2i(xpos, ypos + 1)).state == 
							TileState::Solid
						)
							currentTile.face |= 4;
					}
					if (xpos > 0) {
						if (getGridTile(gs::Vec2i(xpos - 1, ypos)).state == 
							TileState::Solid
						)
							currentTile.face |= 8;
					}
				}
			}
		}
	}
}