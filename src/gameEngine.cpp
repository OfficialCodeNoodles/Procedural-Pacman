#include "../hdr/gameEngine.hpp"

//Globals

GameState gameState = GameState::Intro;
int gameStateTicks = -300; 
int ticks = 0;

bool defaultPacman = false; 
int level = 0;
int highScores[2] = { 0, 0 };
int highScore = 0;
int score = 0;
int prvsScore = 0; 
int lifes = 0; 

namespace gameEngine {
	gs::util::Clock timer;

	void create() {
		HWND hwnd = GetConsoleWindow();
		// Hides console window. 
		ShowWindow(hwnd, 0);

		// Even though the seed gets reset when a grid is procedurally
		// generated the seed gets selected with rand() so this prevents
		// repitition. 
		srand(time(0));
		engine::render::window::create(true);
		engine::input::initKeys(); 
		engine::render::loadAssets(); 
		engine::audio::loadAssets(); 

		// Uses default pacman grid. 
		if (defaultPacman)
			engine::useDefaultGrid(); 
		else
			engine::generateNewGrid();

		engine::initGrid(); 
		engine::generateGridFaces();
	}
	void close() {
		engine::render::window::close(); 
	}

	void updateEvents() {
		engine::render::window::update(); 
		engine::input::updateKeys(); 

		// If the window isn't being used don't update anything. 
		if (!engine::render::window::winmain->hasFocus()) {
			// Ensures internal clock doesn't change. 
			ticks--;
			return;
		}

		switch (gameState) {
		case GameState::Intro:
			engine::render::updateIntro(); 

			if (ticks == 60)
				engine::audio::playSound(10); 
			else if (ticks == 200)
				engine::render::transition.start(); 

			if (engine::render::transition.switchedState) 
				gameState = GameState::PreGame; 

			break; 
		case GameState::PreGame:
			// Starts game. 
			if (engine::input::isKeyPressed(engine::input::Space)) {
				engine::prepareGame(); 

				engine::initGrid();
				engine::generateGridFaces();

				gameState = GameState::Playing; 
			}

			engine::render::updateUI();
			break; 
		case GameState::Playing:
			engine::pacman.update();
			engine::Ghost::updateGhosts();

			// Plays the ghost woo woo sound. Note: It becomes higher pitched
			// as the levels go on. 
			if (ticks % 48 == 0 && engine::superDotTicks <= 0)
				engine::audio::playSound(std::min(4 + level, 8)); 

			// Detects when grid is cleared. 
			if (engine::numOfDots == 0) {
				gameState = GameState::GridClear;
				gameStateTicks = 60 * 2;
				engine::superDotTicks = 0; 
				engine::audio::muteSounds();
			}

			engine::render::updateUI();
			break;
		case GameState::GridClear:
			if (gameStateTicks <= -5) {
				gameState = GameState::Playing;

				engine::initGrid();
				engine::pacman.init();
				engine::Ghost::generateGhosts();
				level++; 

				// Makes ghosts faster every level. 
				engine::ghostSpeed = engine::ghostBaseSpeed + (0.0075f * level);
				engine::ghostSpeed = std::min(engine::ghostSpeed, 0.1f); 
			}
			break; 
		case GameState::Death:
			// Resets life. 
			if (gameStateTicks <= -5) {
				gameState = GameState::Playing;

				if (lifes <= -1)
					gameState = GameState::GameOver; 
				else {
					engine::pacman.init();
					engine::Ghost::generateGhosts();
				}
			}

			engine::render::updateUI();
			break; 
		case GameState::GameOver:
			// Starts game over again. 
			if (engine::input::isKeyPressed(engine::input::Space)) {
				engine::prepareGame();

				engine::initGrid();
				engine::generateGridFaces();

				gameState = GameState::Playing;
			}

			engine::render::updateUI();
			break; 
		}

		engine::render::transition.update(); 

		// Gives pacman an extra life every 20000 points. 
		if (prvsScore / 20000 != score / 20000 &&
			lifes < 5
		) {
			lifes++;
			engine::audio::playSound(9); 
		}

		highScore = std::max(highScore, score); 
		highScores[defaultPacman ? 0 : 1] = highScore; 
		prvsScore = score; 
		// Maximum of 5 lifes. 
		lifes = std::min(lifes, 5); 

		--gameStateTicks; 
	}
	void updateGraphics() {
		engine::render::window::begin(); 
		
		if (!gs::util::inBound(gameStateTicks, 0, -5)) {
			switch (gameState) {
			case GameState::Intro:
				engine::render::renderIntro(); 
				break;
			case GameState::PreGame:
				engine::render::renderGrid();
				engine::render::renderUI();

				// Renders game starting information after a few seconds. 
				if (gameStateTicks < -300) {
					engine::render::window::winmain->draw(
						engine::render::preGameBar
					);
					engine::render::renderText(
						"Press Space to Begin", gs::Vec2f(600.0f, 500.0f), 2.0f
					);
				}

				break; 
			case GameState::GridClear:
				// Causes flickering when the grid is cleared. 
				if (ticks % 20 > 14)
					break; 

				engine::render::renderGrid();
				break; 
			case GameState::GameOver:
				engine::render::renderGrid();
				engine::render::renderGhosts();
				engine::render::renderUI();

				engine::render::window::winmain->draw(
					engine::render::preGameBar
				);
				engine::render::renderText(
					"Game Over", gs::Vec2f(760.0f, 450.0f), 2.5f, gs::Color::Red
				);
				engine::render::renderText(
					"Press Space to Begin", gs::Vec2f(690.0f, 550.0f), 1.5f
				);

				break; 
			default:
				engine::render::renderGrid();
				engine::render::renderGhosts();
				engine::render::renderPacman(engine::pacman);
				engine::render::renderUI();
				break; 
			}
		}

		engine::render::transition.apply(*engine::render::window::winmain); 

		engine::render::window::end(); 
	}
	void updateGameLoop() {
		timer.begin(); 

		updateEvents(); 

		if (engine::render::window::virtualFramerate !=
			engine::render::window::framerate) {
			if (ticks % (engine::render::window::framerate /
				engine::render::window::virtualFramerate) == 0)
				updateGraphics(); 
		}
		else
			updateGraphics(); 

		timer.end(); 
		timer.wait(engine::render::window::framerate); 
		++ticks; 
	}

	bool isOpen() {
		return engine::render::window::winmain->isOpen(); 
	}
}