/*
GameRenderThread enables render thread functionality in our game to enhance the performance.
*/

#pragma once

#include "stdafx.h"
#include "resource.h"
#include "stdafx.h"
#include <list>
#include <vector>

class Game;

class GameRenderThread{
public:
	//killing the thread
	virtual void kill()=0;
	//Tied to the gameloop, update once per frame
	virtual void runFromMainThread(Game *game)=0;
	//Start the thread.
	virtual void start(Game *game)=0;

};

