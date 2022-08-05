#include "../../hdr/logic/generation.hpp"

namespace engine {
	int seed = 0;
	gs::Vec2i ghostSpawningAreaSize = gs::Vec2i(3, 2); 
	int ghostSpawn[5][7] = {
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 1, 1, 4, 1, 1, 0 },
		{ 0, 1, 0, 0, 0, 1, 0 },
		{ 0, 1, 1, 1, 1, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 }
	};
	int dotsGenerated = 0; 

	Branch::Branch() : stepCount(80), subBranch(false) {
	}

	void Branch::generateBase() {
		const gs::Vec2i gridCenter = getGridCenter();
BASE:
		direction = static_cast<Direction>(rand() % 2);
		forward = rand() % 2; 

		// Generates base position based on direction. 
		if (direction == Direction::Vertical) {
			base.x = (rand() % ((ghostSpawningAreaSize.x * 2) + 1)) -
				ghostSpawningAreaSize.x;
			base.y = (ghostSpawningAreaSize.y + 1) * (forward ? 1 : -1);
		}
		else {
			base.y = (rand() % ((ghostSpawningAreaSize.y * 2) + 1)) -
				ghostSpawningAreaSize.y;
			base.x = (ghostSpawningAreaSize.x + 1) * (forward ? 1 : -1);
		}

		// The base is offset from the center. 
		base += gridCenter; 

		for (gs::Vec2i branchBase : branchBases) {
			// Distance to any one of the already generated branches. 
			int distance = std::abs(base.x - branchBase.x) +
				std::abs(base.y - branchBase.y);

			// If too close then regenerate the base. 
			if (distance < 2)
				goto BASE; 
		}

		branchBases.push_back(base); 
	}
	void Branch::traceBranch() {
		gs::Vec2i current = base;
		int tilesOverlaped = 0; 

		for (int step = 0; step < stepCount; ++step) {
			if (!setValidTile(current, TileState::Dot))
				return; 
			
			dotsGenerated++; 

			current = aquireNextStep(current); 
			TileState currentTileState = getGridTile(current).naturalState; 

			// Continues sub branch if still on a solid tile. 
			if (subBranch && currentTileState == TileState::Solid && 
				step == stepCount - 1
			)
				++stepCount; 

			// Prevents paths from overlapping already generated areas to much.
			if (currentTileState == TileState::Dot) tilesOverlaped++; 
			if (tilesOverlaped > 2)
				return; 

			// Changes direction if on the edge of grid or randomly. 
			if (onEdge(current) || rand() % 40 == 0) {
				direction = direction == Direction::Vertical ? 
					Direction::Horizontal : Direction::Vertical;
				forward = rand() % 2;
				
				// Changes direction if on an invalid tile. 
				if (!isValidTile(aquireNextStep(current)))
					forward = !forward; 
			}

			// Generates a sub branch ocasionally. 
			if (!subBranch && rand() % 40 == 0) {
				Branch subBranch; 

				subBranch.direction = direction == Direction::Vertical ?
					Direction::Horizontal : Direction::Vertical;
				subBranch.base = current; 
				subBranch.forward = rand() % 2; 
				subBranch.stepCount = 1 + (rand() % 15);
				subBranch.subBranch = true;

				// If branch leads to an invalid tile then change it's 
				// direction.
				if (!isValidTile(subBranch.aquireNextStep(subBranch.base))) {
					subBranch.forward = !subBranch.forward;
					branchQueue.push(subBranch);
				}
				else {
					branchQueue.push(subBranch);

					// Allows branch to travel in both directions. 
					if (rand() % 2 == 0) {
						subBranch.forward = !subBranch.forward;
						branchQueue.push(subBranch);
					}
				}
			}
		}
	}

	gs::Vec2i Branch::aquireNextStep(gs::Vec2i currentStep) const {
		// Moves in the direction of the branch from the currentStep. 
		if (direction == Direction::Vertical)
			currentStep.y += forward ? 1 : -1;
		else
			currentStep.x += forward ? 1 : -1; 

		return currentStep;
	}
	bool Branch::onEdge(gs::Vec2i position) const {
		// Sorry this is so ugly :(.  
		return (position.x == 1 && direction == Direction::Horizontal) || 
			(position.x == gridSize.x - 2 && direction == Direction::Horizontal) ||
			(position.y == 1 && direction == Direction::Vertical) || 
			(position.y == gridSize.y - 2 && direction == Direction::Vertical);
	}

	queue<Branch> branchQueue;
	vector<gs::Vec2i> branchBases; 
	vector<gs::Vec2i> superDots;

	void useDefaultGrid() {
		gridSize = defaultGridSize; 
		render::scale = 7.2f; 

		resetGridSize(); 

		spawnPosition = gs::Vec2i(9, 15); 
		ghostSpawnPosition = gs::Vec2i(9, 9); 

		for (int xpos = 0; xpos < gridSize.x; ++xpos) {
			for (int ypos = 0; ypos < gridSize.y; ++ypos) {
				Tile& currentTile = grid[xpos][ypos];

				currentTile.naturalState = static_cast<TileState>(
					defaultGrid[ypos][xpos]
				);
			}
		}
	}
	gs::Vec2i getGridCenter() {
		// Is naturally truncated. Ex. Center of 19 is 9. 
		return gs::Vec2i(gridSize.x / 2, gridSize.y / 2); 
	}
	bool isValidTile(gs::Vec2i position) {
		const gs::Vec2i gridCenter = getGridCenter(); 
		// Makes sure tile isn't on the edge. 
		bool valid = position.x > 0 && position.x < gridSize.x - 1 &&
			position.y > 0 && position.y < gridSize.y - 1; 

		// Checks if tile is in the ghost spawning area. 
		valid = valid && !(
			(position.x >= gridCenter.x - ghostSpawningAreaSize.x) && 
			(position.x <= gridCenter.x + ghostSpawningAreaSize.x) && 
			(position.y >= gridCenter.y - ghostSpawningAreaSize.y) && 
			(position.y <= gridCenter.y + ghostSpawningAreaSize.y)
		); 		

		return valid; 
	}
	bool setValidTile(gs::Vec2i position, TileState tileState) {
		if (isValidTile(position)) {
			grid[position.x][position.y].naturalState = tileState; 
			return true;
		}

		return false; 
	}
	void resetGridSize() {
		render::gridOffset = gs::Vec2f(
			(render::window::windowSize.x - (gridSize.x * render::tileSize *
				render::scale)) / 2.0f,
			(render::window::windowSize.y - (gridSize.y * render::tileSize *
				render::scale)) / 2.0f
		);
		render::sTile.setScale(render::scale, render::scale);

		// Pacman's spawn will be 5 tiles below the center. 
		spawnPosition = getGridCenter() + gs::Vec2i(0, 5);
		ghostSpawnPosition = getGridCenter(); 
	}
	void generateBaseGrid() {
		gridSize = gs::Vec2i(27, 31);
		render::scale = 4.8f;

		const gs::Vec2i gridCenter = getGridCenter();

		resetGridSize();

		for (int xpos = 0; xpos < gridSize.x; ++xpos) {
			for (int ypos = 0; ypos < gridSize.y; ++ypos) {
				Tile& currentTile = grid[xpos][ypos];
				// Offset of current position from the top left corner of the 
				// ghost spawn box. 
				gs::Vec2i ghostSpawnOffset = gs::Vec2i(
					xpos - gridCenter.x + ghostSpawningAreaSize.x, 
					ypos - gridCenter.y + ghostSpawningAreaSize.y
				);

				// Defaults to a solid tile. 
				currentTile.naturalState = TileState::Solid;

				// If the current position is in bounds of the ghost spawn 
				// generate the ghost box. 
				if (ghostSpawnOffset.x >= 0 && ghostSpawnOffset.x <
					(ghostSpawningAreaSize.x * 2) + 1 &&
					ghostSpawnOffset.y >= 0 && ghostSpawnOffset.y <
					(ghostSpawningAreaSize.y * 2) + 1
				) 
					currentTile.naturalState = static_cast<TileState>(
						ghostSpawn[ghostSpawnOffset.y][ghostSpawnOffset.x]
					);
			}
		}
	}
	void generateNewGrid(int setSeed) {
		generateBaseGrid();

		dotsGenerated = 0; 
		branchBases.clear(); 
		superDots.clear(); 

		// Generates seed if none is provided. 
		if (setSeed == -1) seed = rand();
		srand(seed);

		// Creates 6 base branches. 
		for (int branchIndex = 0; branchIndex < 6; ++branchIndex) {
			Branch branch; 

			branch.generateBase(); 
			branch.traceBranch();  
		}

		// Traces all of the generated sub-branches. 
		while (!branchQueue.empty()) {
			Branch currentBranch = branchQueue.front(); 
			branchQueue.pop(); 

			currentBranch.traceBranch(); 
		}

		// Fills in large sections of dots. 
		for (int xpos = 0; xpos < gridSize.x; ++xpos) {
			for (int ypos = 0; ypos < gridSize.y; ++ypos) {
				Tile& currentTile = grid[xpos][ypos];
				int emptyNeighbors = 0; 
				
				// Skip invalid tiles. 
				if (!isValidTile(gs::Vec2i(xpos, ypos)))
					continue; 
				
				// Only works if tile isn't solid. 
				if (currentTile.naturalState != TileState::Solid) {
					for (int localXpos = xpos - 1; localXpos < xpos + 2;      
						++localXpos) {
						for (int localYpos = ypos - 1; localYpos < ypos + 2;
							++localYpos) {
							if (getGridTile(gs::Vec2i(localXpos, localYpos)).naturalState !=
								TileState::Solid
							)
								emptyNeighbors++;
						}
					}
					
					// If tile has more than 7 non-solid neighbors then make it 
					// a solid tile.
					if (emptyNeighbors > 7)
						currentTile.naturalState = TileState::Solid; 
				}
			}
		}

		int superDotCount = dotsGenerated / 40; 

		while (superDotCount > 0) {
			gs::Vec2i superDotPosition; 

			do {
SUPERDOT:
				superDotPosition.x = rand() % gridSize.x; 
				superDotPosition.y = rand() % gridSize.y; 
			} 
			while (!isValidTile(superDotPosition) ||
				getGridTile(superDotPosition).naturalState != TileState::Dot);

			for (gs::Vec2i superDot : superDots) {
				float distance = gs::util::distance(
					superDotPosition, superDot
				);

				// Regenerates super dot if too close. 
				if (distance < 5.0f)
					goto SUPERDOT; 
			}

			setValidTile(superDotPosition, TileState::SuperDot); 

			superDots.push_back(superDotPosition);
			superDotCount--;
		}

		gs::Vec2i current = spawnPosition;

		// Generates path from pacman to the ghost spawn area to ensure pacman
		// isn't trapped. 
		while (isValidTile(current)) {
			setValidTile(current, TileState::Empty);
			current.y--;
		}
	}
}