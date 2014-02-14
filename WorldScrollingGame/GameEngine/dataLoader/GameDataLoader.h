/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameDataLoader.h

	This class should read data from CSV (Comma Separated Values)
	files and uses them to load game data for a game
	application. Game data for rendering the game GUI is
	loaded when the game starts, data for displaying each
	game level would be loaded as needed. Note that we are not
	using this in the first benchmark.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "TiledLayer.h"
#include "SparseLayer.h"
#include "ParallaxLayer.h"
#include "AnimatedSpriteType.h"
#include "AnimatedSprite.h"
#include "PhysicalProperties.h"
#include "BoundingVolume.h"
#include "CXBOXController.h"

#include <vector>

// FORWARD DECLARATIONS OF CLASSES USED BY GAME
class Game;

class GameDataLoader
{
public:
	// SIMPLE INLINED CONSTRUCTOR/DESTRUCTOR
	GameDataLoader();			
	~GameDataLoader();			

	// METHODS TO BE DEFINED INSIDE GameDataLoader.cpp
	void loadGUI(Game *game);
	void loadWorld(Game *game, int level);
	void loadLevelExample(Game *game, int level);

private: 
	vector<int> *spriteImageID;
	vector<pTile> *parallaxLayerTiles;
	vector<pTile> *decoLayerTiles;
	vector<Tile> *tiledLayerTiles;
	vector<AnimatedSpriteType*> *aiSpriteTypes;
	ParallaxLayer *decoLayer;
	TiledLayer *tiledLayer;
	AnimatedSpriteType *ast;
	AnimatedSprite *player1;
	AnimatedSprite *player2;
	PhysicalProperties *player1Props;
	PhysicalProperties *player2Props;
	BoundingVolume *player1BV;
	BoundingVolume *player2BV;
	CXBOXController* initPlayer1Controller;
	CXBOXController* initPlayer2Controller;
};