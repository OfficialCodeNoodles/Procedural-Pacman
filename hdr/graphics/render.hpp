#pragma once

//Dependencies
#include "window.hpp"
#include "../logic/grid.hpp"
#include "../logic/pacman.hpp"
#include "../logic/ghost.hpp"
#include "../logic/generation.hpp"

namespace engine {
	namespace render {
		extern const float spriteScaleDivisor; 
		extern const int tileSize;  
		extern const int spriteSize; 

		extern float scale; 
		extern sf::Texture tTileSheet; 
		extern sf::Texture tSpriteSheet; 
		extern sf::Texture tLife;
		extern sf::Texture tSoundTextures[2]; 
		extern sf::Sprite sTile; 
		extern sf::Sprite sSprite; 
		extern sf::Sprite sLife; 
		extern sf::Sprite sSoundTextures[2]; 
		extern gs::Vec2f gridOffset; 
		extern sf::Font font0;
		extern sf::Font font1; 
		extern gs::Text text; 
		extern gs::Button muteButton;
		extern gs::Button exitButton; 
		extern gs::Textbox seedTextBox; 
		extern sf::RectangleShape preGameBar; 
		extern bool showInfo; 
		extern std::string seedEntered; 
		extern int textEnteredTicks; 
		extern gs::Transition transition; 
		extern std::string introText;
		extern std::string currentIntroText; 

		void loadAssets();
		void handleAssets(); 

		// Sets the sTile sprite to only render a specific part of the 
		// tTileSheet texture. 
		void setTileTexture(int textureId); 
		// Sets the sSprite sprite to only render a specific part of the 
		// tSpriteSheet texture.
		void setSpriteTexture(int textureId); 

		void updateIntro(); 
		void updateUI(); 

		void renderText(
			const std::string& string, gs::Vec2f position, float scale,
			gs::Color fillColor = gs::Color::White, 
			float outlineThickness = 0.0f, gs::Color outlineColor = gs::Color()
		);
		void renderTile(const Tile& tile, gs::Vec2i position); 
		// Renders the entire grid by calling renderTile() many times. 
		void renderGrid(); 
		void renderPacman(const Pacman& pacman); 
		void renderGhost(const Ghost& ghost);
		// Renders all ghosts by calling renderGhost() mant times. 
		void renderGhosts(); 
		void renderIntro(); 
		void renderUI();
	}
}