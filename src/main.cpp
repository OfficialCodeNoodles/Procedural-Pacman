#include "../hdr/gameEngine.hpp"

int main() {
	gameEngine::create(); 

	while (gameEngine::isOpen()) 
		gameEngine::updateGameLoop(); 

	gameEngine::close(); 

	return 0; 
}