#pragma once

/*	
	Author: ____Team Little Ninja___________

	GameAI.h

	This class will be used to manage all game bots, and the
	Artificial Intelligence associated with them. Each frame
	we will update the AI state, which should update all bots
	accordingly.

	Students will implement this as part of their projects.
*/

// PREPROCESSOR INCLUDE STATEMENTS
#include "resource.h"
#include "stdafx.h"
#include "Game.h"
#include "Bot.h"

const int OUT_MAP_X  = 99999; //outside of the map x position
const int OUT_MAP_Y  = 99999;// outside of the map y position

const int maxBot = 50;

class GameAI
{
public:
	// WE'LL DEFINE THESE METHODS LATER
	GameAI();
	~GameAI();
	vector<Bot*> *botList;
	Bot *boss;
	vector<AnimatedSprite*> *eventList;
	int addEvent(AnimatedSprite* a);
	int aliveNum; 
	void update(Game *game);	
	void freeAllBot();
	vector<Bot*> getBotList(){return *botList;}	
	Bot* getFreeBot();
};