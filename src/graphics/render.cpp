#include "../../hdr/graphics/render.hpp"

namespace engine {
	namespace render {
		const float spriteScaleDivisor = 13.0f / 7.0f; 
		const int tileSize = 7;
		const int spriteSize = 13; 

		float scale = 7.2f;
		sf::Texture tTileSheet;
		sf::Texture tSpriteSheet;
		sf::Texture tLife;
		sf::Texture tSoundTextures[2];
		sf::Sprite sTile;
		sf::Sprite sSprite;
		sf::Sprite sLife;
		sf::Sprite sSoundTextures[2];
		gs::Vec2f gridOffset;
		sf::Font font0;
		sf::Font font1;
		gs::Text text;
		gs::Button muteButton;
		gs::Button exitButton;
		gs::Textbox seedTextBox;
		sf::RectangleShape preGameBar;
		bool showInfo = true; 
		std::string seedEntered;
		int textEnteredTicks = 0; 
		gs::Transition transition;
		std::string introText = "CodeNoodles";
		std::string currentIntroText;

		void loadAssets() {
			tTileSheet.loadFromFile(mAssetDirectory + "textures/tileSheet.png");
			tSpriteSheet.loadFromFile(mAssetDirectory + "textures/spriteSheet.png");
			tLife.loadFromFile(mAssetDirectory + "textures/life.png");
			tSoundTextures[0].loadFromFile(mAssetDirectory + "textures/unmuted.png");
			tSoundTextures[1].loadFromFile(mAssetDirectory + "textures/muted.png");
			font0.loadFromFile(mAssetDirectory + "font0.ttf"); 
			font1.loadFromFile(mAssetDirectory + "font1.ttf"); 

			handleAssets(); 
		}
		void handleAssets() {
			// sTile
			sTile.setTexture(tTileSheet); 
			sTile.setScale(scale, scale); 

			// sSprite
			sSprite.setTexture(tSpriteSheet); 

			// sLife
			sLife.setTexture(tLife);
			sLife.setScale(4.0f, 4.0f); 

			// text
			text.setFont(font0); 

			// muteButton
			muteButton.setSize(100.0f, 100.0f);
			muteButton.setPosition(1780.0f, 50.0f); 
			muteButton.clearColors();
			muteButton.setSelectedFillColor(gs::Color(200, 200, 200));
			muteButton.setScaleModifiers(1.0f); 

			// exitButton
			exitButton.setSize(150.0f, 70.0f); 
			exitButton.setPosition(1730.0f, 950.0f);
			exitButton.setFont(font0); 
			exitButton.setTextScale(1.5f, 1.5f);
			exitButton.setString("Exit"); 
			exitButton.setShapeColors(gs::Color::Transparent); 
			exitButton.setScaleModifiers(1.0f); 

			// seedTextBox 
			seedTextBox.setSize(200.0f, 60.0f); 
			seedTextBox.setPosition(1570.0f, 850.0f);
			seedTextBox.setShapeColors(gs::Color::Transparent); 
			seedTextBox.setScaleModifiers(1.0f);
			seedTextBox.setFont(font0); 
			seedTextBox.setMessageString("Type Seed");
			seedTextBox.setMaxInputLength(5);
			seedTextBox.validInputs = gs::Textbox::ValidInputs::Numeric; 

			// preGameBar
			preGameBar.setSize(gs::Vec2f(1000.0f, 200.0f));
			preGameBar.setPosition(
				960.0f - (preGameBar.getSize().x / 2.0f),
				540.0f - (preGameBar.getSize().y / 2.0f)
			); 
			preGameBar.setFillColor(gs::Color(0, 0, 0, 127));
		}

		void setTileTexture(int textureId) {
			static sf::IntRect bounds(0, 0, tileSize, tileSize);

			// The 2D position in the tTileSheet that the given textureId is
			// located at. 
			gs::Vec2i texturePosition = gs::Vec2i(
				textureId % 15, textureId / 15 // Sheet is 16 tiles wide
			);

			bounds.left = (texturePosition.x * bounds.width) 
				+ texturePosition.x;
			bounds.top = (texturePosition.y * bounds.height) 
				+ texturePosition.y; 

			sTile.setTextureRect(bounds); 
		}
		void setSpriteTexture(int textureId) {
			static sf::IntRect bounds(0, 0, spriteSize, spriteSize);

			// The 2D position in the tSpriteSheet that the given textureId is
			// located at. 
			gs::Vec2i texturePosition = gs::Vec2i(
				textureId % 9, textureId / 9 // Sheet is 9 tiles wide
			);

			bounds.left = (texturePosition.x * bounds.width)
				+ texturePosition.x;
			bounds.top = (texturePosition.y * bounds.height)
				+ texturePosition.y;

			sSprite.setTextureRect(bounds); 
		}

		void updateIntro() {
			if (ticks > 60 && (ticks + 1) % 5 == 0 &&
				ticks < 60 + (introText.size() * 5)
			) {
				currentIntroText += introText[
					static_cast<int>(std::floor((ticks + 1) / 5)) - 13
				];
			}
		}
		void updateUI() {
			// Changes grid to new state. 
			static auto changeGrid = [](int setSeed = -1) {
				gameState = GameState::PreGame;
				gameStateTicks = -5;
				superDotTicks = 0; 
				score = 0;
				prvsScore = 0; 
				lifes = 0;

				// Score current highScore. 
				highScores[defaultPacman ? 0 : 1] = highScore;

				// IDK why I choose -2 for this but oh well. 
				if (setSeed == -2) {
					useDefaultGrid();
					defaultPacman = true; 
				}
				else {
					generateNewGrid(setSeed);
					defaultPacman = false; 
				}

				initGrid();
				generateGridFaces();
				audio::muteSounds();
				
				// Swap highscores if needed. 
				highScore = highScores[defaultPacman ? 0 : 1]; 
			};

			muteButton.update();
			exitButton.update(); 
			// Only updates seed box if the menu is actually open to prevent
			// ghost interactions. 
			if (showInfo) seedTextBox.update(); 

			if (muteButton.isSelected && muteButton.isClickedOn) {
				audio::isMuted = !audio::isMuted;

				if (audio::isMuted) audio::muteSounds();
			}
			else if (exitButton.isSelected && exitButton.isClickedOn)
				window::winmain->close(); 
			else if (seedTextBox.getActive()) 
				textEnteredTicks = 0;

			seedEntered = seedTextBox.getStoredString();

			// This generates a grid based on the seed 1 tick after the seed
			// has been entered. 
			if (textEnteredTicks == 1) {
				if (seedEntered != "") {
					seed = std::stoi(seedEntered);
					changeGrid(seed);
				}
			}

			textEnteredTicks++; 

			// I know the ternary operator is redundent but it makes this more
			// readable. 
			muteButton.setTexture(tSoundTextures[audio::isMuted ? 1 : 0]); 

			// Keyboard inputs. 
			if (input::isKeyClicked(input::Mute)) {
				audio::isMuted = !audio::isMuted; 
				
				if (audio::isMuted)
					audio::muteSounds(); 
			}
			else if (input::isKeyClicked(input::F1)) 
				changeGrid(-2);
			else if (input::isKeyClicked(input::F2)) 
				changeGrid(); 
			else if (input::isKeyClicked(input::F3)) 
				window::virtualFramerate = window::virtualFramerate == 30 ? 60
					: 30; 
			else if (input::isKeyClicked(input::HideInfoMenu)) 
				showInfo = !showInfo;
		}

		void renderText(
			const std::string& string, gs::Vec2f position, float scale, 
			gs::Color fillColor, float outlineThickness, gs::Color outlineColor
		) {
			text.setString(string); 
			text.setPosition(position); 
			text.setScale(scale, scale); 
			text.setFillColor(fillColor); 
			text.setOutlineThickness(outlineThickness); 
			text.setOutlineColor(outlineColor); 

			gs::draw(window::winmain, text); 
		}
		void renderTile(const Tile& tile, gs::Vec2i position) {
			TileState tileState = tile.state; 
			int textureId; 

			switch (tileState) {
			case TileState::Empty:
				// No need to render an empty Tile, duh.
				return; 
			case TileState::Solid:
				textureId = 0 + tile.face;
				// If a solid tile is fully encased it appears empty. 
				if (textureId == 15)
					return; 

				break; 
			case TileState::Dot:
				textureId = 15;
				break; 
			case TileState::SuperDot:
				// Allows superdots to blink on and off. 
				if (ticks % 30 > 14)
					return; 

				textureId = 16;
				break; 
			case TileState::GhostExit:
				textureId = 17; 
				break; 
			}

			setTileTexture(textureId); 

			sTile.setPosition(
				gridOffset.x + (position.x * tileSize * scale),
				gridOffset.y + (position.y * tileSize * scale)
			);

			window::winmain->draw(sTile); 
		}
		void renderGrid() {
			for (int xpos = 0; xpos < gridSize.x; ++xpos) {
				for (int ypos = 0; ypos < gridSize.y; ++ypos) {
					Tile& currentTile = grid[xpos][ypos];

					renderTile(currentTile, gs::Vec2i(xpos, ypos)); 
				}
			}
		}
		void renderPacman(const Pacman& pacman) {
			const float spriteScale = scale / spriteScaleDivisor; 

			int textureId = 0; 

			sSprite.setPosition(
				gridOffset.x + (pacman.position.x * tileSize * scale), 
				gridOffset.y + (pacman.position.y * tileSize * scale)
			);
			// Resets transformations
			sSprite.setScale(spriteScale, spriteScale);
			sSprite.setRotation(0.0f);

			if (gameState == GameState::Death) {
				// Dying animation. 
				textureId = 50 + ((60 * 3) - gameStateTicks) / 15;

				// Stops animation once textureId gets to high. 
				if (textureId >= 55)
					return; 
			}
			else if (pacman.position.x != spawnPosition.x ||
				pacman.position.y != spawnPosition.y) {
				// Changes textureId to animate mouth opening and closing. 
				textureId = 2 - (static_cast<int>(
					pacman.position.x * 2 + pacman.position.y * 2) % 2);

				// Alters the oriantation of sSprite to match the direction of
				// pacman. Note: Right is not included as it is the default
				// direction and thus doesn't need any transformations. 
				switch (pacman.facingDirection) {
				case Entity::Direction::Left:
					sSprite.setScale(-spriteScale, spriteScale);
					sSprite.move(spriteSize * spriteScale, 0.0f);
					break; 
				case Entity::Direction::Up:
					sSprite.setRotation(-90.0f); 
					sSprite.move(0.0f, spriteSize * spriteScale);
					break; 
				case Entity::Direction::Down:
					sSprite.setRotation(90.0f); 
					sSprite.move(spriteSize * spriteScale, 0.0f);
					break; 
				}
			}

			setSpriteTexture(textureId);

			window::winmain->draw(sSprite);
		}
		void renderGhost(const Ghost& ghost) {
			const float spriteScale = scale / spriteScaleDivisor;

			int textureId; 
			
			switch (ghost.movementMode) {
			case Ghost::MovementMode::Frightened:
				textureId = 43 + ((ticks / 12) % 2);
				break; 
			case Ghost::MovementMode::Return:
				// Renders just the eyes of the ghosts. 
				textureId = 45 + static_cast<int>(ghost.facingDirection);
				break; 
			default:
				// Sets color frame. 
				textureId = 3 + ((ticks / 6) % 2) + (10 * ghost.id);
				// Allows for eyes to look in different direction. 
				textureId += static_cast<int>(ghost.facingDirection) * 2;
			}

			sSprite.setPosition(
				gridOffset.x + (ghost.position.x * tileSize * scale),
				gridOffset.y + (ghost.position.y * tileSize * scale)
			);
			// Resets transformations
			sSprite.setScale(spriteScale, spriteScale);
			sSprite.setRotation(0.0f);

			setSpriteTexture(textureId); 

			window::winmain->draw(sSprite); 
		}
		void renderGhosts() {
			for (const Ghost& ghost : ghosts)
				renderGhost(ghost); 
		}
		void renderIntro() {
			text.setFont(font1);
			renderText(currentIntroText, gs::Vec2f(690.0f, 480.0f), 1.5f);
			text.setFont(font0); 
		}
		void renderUI() {
			// Left hand information. 
			renderText(
				"HI-SCORE", gs::Vec2f(50.0f, 50.0f), 1.6f, gs::Color::Red
			);
			renderText(
				std::to_string(highScore), gs::Vec2f(50.0f, 110.0f), 1.5f
			);

			renderText(
				"SCORE", gs::Vec2f(50.0f, 200.0f), 1.6f, gs::Color::Red
			);
			renderText(std::to_string(score), gs::Vec2f(50.0f, 260.0f), 1.5f);

			// Renders the number of lifes left. 
			for (int lifeIndex = 0; lifeIndex < lifes; ++lifeIndex) {
				sLife.setPosition(50.0f + (80.0f * lifeIndex), 950.0f);
				window::winmain->draw(sLife); 
			}

			// Rendering buttons. 
			gs::draw(window::winmain, muteButton); 
			gs::draw(window::winmain, exitButton); 

			// Right hand information. 
			if (showInfo) {
				renderText("Controls", gs::Vec2f(1480.0f, 200.0f), 1.25f);
				renderText(
					"WASD -> Movement\n\n"
					"M -> Mute / Unmute\n\n"
					"F1 -> Use Normal Grid\n\n"
					"F2 -> Generate Grid\n\n"
					"F3 -> Change Fps: " + std::to_string(
						window::virtualFramerate) + "\n\n"
					"F11 -> Go Fullscreen\n\n"
					"H -> Hide This Menu",
					gs::Vec2f(1500.0f, 280.0f), 1.0f
				);
				renderText("Seed", gs::Vec2f(1480.0f, 700.0f), 1.25f);
				renderText(
					"Current Seed: " + std::to_string(seed),
					gs::Vec2f(1500.0f, 780.0f), 1.0f
				);
				gs::draw(window::winmain, seedTextBox);
			}
		}
	}
}