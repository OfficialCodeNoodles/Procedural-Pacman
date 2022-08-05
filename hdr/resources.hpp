#pragma once

//Dependencies
#include <queue>
#include <unordered_map>
#include <memory>

#include <Glass/glass.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

//Macros

#define mAssetDirectory std::string("assets/")
#define mGridWidth 27
#define mGridHeight 31
#define mNumOfGhosts 4
#define mNumOfSounds 11
#define mNumOfKeys 10

#define mGetStyle(fullscreen) fullscreen ? sf::Style::Fullscreen \
	: sf::Style::Default

//Globals 

enum class GameState { Intro, PreGame, Playing, GridClear, Death, GameOver };
extern GameState gameState;
extern int gameStateTicks; 
extern int ticks; 

extern bool defaultPacman; 
extern int level;
extern int highScores[2];
extern int highScore; 
extern int score; 
extern int prvsScore; 
extern int lifes; 

//Others

using std::vector; 
using std::queue; 
using std::unordered_map;