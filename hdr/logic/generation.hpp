#pragma once

//Dependencies
#include "grid.hpp"
#include "pacman.hpp"
#include "ghost.hpp"
#include "../graphics/render.hpp"

namespace engine {
	extern int seed; 
	extern gs::Vec2i ghostSpawningAreaSize;
	extern int ghostSpawn[5][7];
	extern int dotsGenerated;

	class Branch {
	public:
		enum class Direction { Vertical, Horizontal }
			direction;
		gs::Vec2i base;
		bool forward; 
		int stepCount; 
		bool subBranch; 

		Branch(); 

		// Generates a starting point for the branch to move outwards from. 
		void generateBase();  
		// Moves along the branch to generate pathways. 
		void traceBranch(); 
	private:
		gs::Vec2i aquireNextStep(gs::Vec2i currentStep) const; 
		bool onEdge(gs::Vec2i position) const; 
	};

	extern queue<Branch> branchQueue; 
	extern vector<gs::Vec2i> branchBases; 
	extern vector<gs::Vec2i> superDots; 

	// Changes grid to be the default pacman grid. 
	void useDefaultGrid(); 
	gs::Vec2i getGridCenter(); 
	// True if tile position is valid to be changed in generation context. 
	bool isValidTile(gs::Vec2i position);
	// Sets a tile if valid and returns it's validity. 
	bool setValidTile(gs::Vec2i position, TileState tileState); 
	// Scales screen to new grid size. 
	void resetGridSize(); 
	// Generates basic template for all procedurally generated grids. 
	void generateBaseGrid(); 
	void generateNewGrid(int setSeed = -1);
}