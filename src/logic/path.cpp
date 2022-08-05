#include "../../hdr/logic/path.hpp"

namespace engine {
	size_t HashContainer::operator()(const gs::Vec2i& vec) const {
		// Used this post as a reference to make hash for 2 component vector. 
		// https://stackoverflow.com/questions/45395071/hash-for-a-stdpair-for-use-in-an-unordered-map
		return size_t(vec.x) << 32 | vec.y;
	}

	queue<gs::Vec2i> frontier;
	vector<gs::Vec2i> searched;
	unordered_map<gs::Vec2i, gs::Vec2i, HashContainer> pathMap;
	vector<gs::Vec2i> paths[mNumOfGhosts];
	gs::Vec2i targetDestinations[mNumOfGhosts];

	void floodGrid(gs::Vec2i start, gs::Vec2i target, int ghostIndex) {
		// Clears the frontier queue. 
		static auto clearFrontier = [=]() {
			while (!frontier.empty())
				frontier.pop(); 
		};

		gs::Vec2i nextPositions[4]; 
		int neighborCount = 0; 

		targetDestinations[ghostIndex] = target;

		clearFrontier(); 
		searched.clear(); 
		pathMap.clear(); 

		// Begins search with the starting position. 
		frontier.push(start); 
		searched.push_back(start); 

		pathMap[start] = start;

		// Checks if any areas still need to be searched. 
		while (!frontier.empty()) {
			gs::Vec2i current = frontier.front(); 
			frontier.pop(); 

			neighborCount = 0; 

			for (int neighborIndex = 0; neighborIndex < 4; ++neighborIndex) {
				gs::Vec2i testingPosition = current;

				// Fancy code to generate the 4 cardinal directions around the
				// current tile position. 
				if ((neighborIndex & 1) == 1)
					testingPosition.x += neighborIndex == 1 ? 1 : -1;
				else
					testingPosition.y += neighborIndex == 0 ? -1 : 1; 		

				// If tile position isn't solid then it's safe for the path.
				if (getGridTile(testingPosition).naturalState != 
					TileState::Solid
				) {
					nextPositions[neighborCount] = testingPosition;
					++neighborCount;
				}
			}

			for (int nextIndex = 0; nextIndex < neighborCount; ++nextIndex) {
				gs::Vec2i next = nextPositions[nextIndex]; 

				// If not already in searched vector then it's a valid tile. 
				if (std::find(searched.begin(), searched.end(), next) ==
					searched.end()
				) {
					frontier.push(next); 
					searched.push_back(next); 

					// Creates a link between the next tile and the current. 
					pathMap[next] = current; 

					// If path is found exit early. 
					if (current == targetDestinations[ghostIndex])
						return; 
				}
			}
		}
	}
	void generatePath(gs::Vec2i start, int ghostIndex) {
		gs::Vec2i current = targetDestinations[ghostIndex];

		paths[ghostIndex].clear(); 

		// Works backwards starting from the target and finding the path it 
		// took to get to the start. If the current position is equal to a 
		// default gs::Vec2i() then there is no valid path and thus can't
		// continue. 
		while (current != start && current != gs::Vec2i()) {
			paths[ghostIndex].push_back(current);
			current = pathMap[current];
		}

		paths[ghostIndex].push_back(current);
	}
	gs::Vec2i aquireNextStep(gs::Vec2i start, int ghostIndex) {
		vector<gs::Vec2i>& path = paths[ghostIndex]; 

		if (path.size() > 1) {
			// Iterator in path that includes the given position. 
			vector<gs::Vec2i>::iterator iter = std::find(
				path.begin(), path.end(), start
			);

			// If a valid match is found move one step forward.
			if (iter < path.end() && iter > path.begin())
				return *(iter-1);
		}

		return start; 
	}
}