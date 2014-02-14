/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameWorld.cpp

	See GameWorld.h for a class description.
*/

#include "stdafx.h"
#include "Game.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameWorld.h"
#include "RenderList.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Viewport.h"
#include "GamePhysics.h"
#include "GameAI.h"
#include <vector>

#include "XACT.h"

/*
	GameWorld - Default Constructor, it constructs the layers
	vector, allowing new layers to be added.
*/
GameWorld::GameWorld()	
{
	layers = new vector<WorldLayer*>();
	worldWidth = 0;
	worldHeight = 0;
	currentLevel = 0;
	spriteManager = new SpriteManager();
	viewport = new Viewport();
	levelFileNames = new vector<wchar_t*>();
}

/*
	~GameWorld - This destructor will remove the memory allocated
	for the layer vector.
*/
GameWorld::~GameWorld()	
{
	delete layers;
	delete spriteManager;
	delete viewport;
	delete levelFileNames;
}

/*
	addLayer - This method is how layers are added to the World.
	These layers might be TiledLayers, SparseLayers, or 
	IsometricLayers, all of which are child classes of WorldLayer.
*/
void GameWorld::addLayer(WorldLayer *layerToAdd)
{
	layers->push_back(layerToAdd);
}

/*
	addWorldRenderItemsToRenderList - This method sends the render
	list and viewport to each of the layers such that they
	may fill it with RenderItems to draw.
*/
void GameWorld::addWorldRenderItemsToRenderList(Game *game)
{

	if (game->getGameState() == GAME_PLAYING_GAME_STATE
		|| game->getGameState() == PAUSE_GAME_STATE
		|| game->getGameState() == CONTROLS_GAME_STATE
		|| game->getGameState() == ABOUT_GAME_STATE
		|| game->getGameState() == HELP_GAME_STATE)
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		for (int i = 0; i < layers->size(); i++)
		{
			layers->at(i)->addRenderItemsToRenderList(	renderList,
													viewport);
		}
		spriteManager->addSpriteItemsToRenderList(renderList, viewport);
	}
}

/*
	addLevelFileName - This method adds a level file name to the vector
	of all the level file names. Storing these file names allows us to
	easily load a desired level at any time.
*/
void GameWorld::addLevelFileName(wchar_t *levelFileName)
{
	levelFileNames->push_back(levelFileName);
}

/*
	clear - This method removes all data from the World. It should
	be called first when a level is unloaded or changed. If it
	is not called, an application runs the risk of having lots
	of extra data sitting around that may slow the progam down.
	Or, if the world thinks a level is still active, it might add
	items to the render list using image ids that have already been
	cleared from the GameGraphics' texture manager for the world.
	That would likely result in an exception.
*/
void GameWorld::unloadCurrentLevel(Game *game)
{
	game->getPhysics()->Dynamic->clear();
	game->getPhysics()->RealCollision->clear();
	game->getPhysics()->AISprite->clear();
	game->getAI()->eventList->clear();
	game->getAI()->freeAllBot();	
	spriteManager->clearSprites();	
	layers->clear();
	game->getAudio()->getAudioState().pSoundBank->Stop(1, NULL );
	game->getAudio()->getAudioState().pSoundBank->Stop(2, NULL );
	game->getAudio()->getAudioState().pSoundBank->Stop(3, NULL );
	currentLevel = 0;
	worldWidth = 0;
	worldHeight = 0;
	game->gameOver=0;
	game->nextLevel=-1;
	
}

/*
	getLevelFileName - This method gets the name of the file used to
	load the current level. Each level has a file where the layout
	of the level and what artwork to use is specified.
*/
wchar_t* GameWorld::getLevelFileName(int levelNumber)
{
	return levelFileNames->at(levelNumber-1);
}

/*
	loadCurrentLevel - This method changes the current level. This method should
	be called before loading all the data from a level file.
*/
void GameWorld::loadCurrentLevel(Game *game, int initLevel)
{
	game->getAudio()->getAudioState().pSoundBank->Stop(0, NULL );
	if ((initLevel > 0) && (initLevel <= levelFileNames->size()))
	{
		unloadCurrentLevel(game);
		currentLevel = initLevel;
		GameDataLoader *dataLoader = game->getDataLoader();
		dataLoader->loadWorld(game, initLevel);

		//audio test
		game->getAudio()->PlayAudioCue(initLevel);
	}
}

/*
	update - This method should be called once per frame. It updates
	all of the dynamic level data, like sprite animation states and
	particle locations.
*/
void GameWorld::update(Game *game)
{
	spriteManager->updateAllSprites();
	boolean moveViewport = false;
	AnimatedSprite *player1 = getSpriteManager()->getPlayer1();
	AnimatedSprite *player2 = getSpriteManager()->getPlayer2();
	player1->getPlayerController()->Vibrate(0,0);
	player2->getPlayerController()->Vibrate(0,0);
	int incX = 0;
	int incY = 0;
	float XRbound = viewport->getViewportX()+(viewport->getViewportWidth()*0.55);
	float XLbound = viewport->getViewportX()+(viewport->getViewportWidth()*0.45);

	if((player1->getBoundingVolume()->getX()) < XLbound && (player2->getBoundingVolume()->getX()) < XLbound)
	{				
		moveViewport = true;
		incX = -5;						
	}
	else if((player1->getBoundingVolume()->getX()) > XRbound && (player2->getBoundingVolume()->getX()) > XRbound)
	{				
		moveViewport = true;
		incX = 5;				
	}

	if(viewport->getViewportX()+viewport->getViewportWidth()>game->getAI()->boss->getAnimatedSprite()->getBoundingVolume()->getX()){
		if(game->getAI()->boss->getAnimatedSprite()->isAlive && !game->getAI()->boss->getAnimatedSprite()->isEnabled){
		    game->getAI()->boss->getAnimatedSprite()->isEnabled=true;
	    }
	}

	

	if((player1->getBoundingVolume()->getX() + player1->getBoundingVolume()->getWidth()) > (viewport->getViewportX() + viewport->getViewportWidth()))
	{
		player1->getBoundingVolume()->setX(viewport->getViewportX() + viewport->getViewportWidth() - player1->getBoundingVolume()->getWidth());
		player1->getPhysicalProperties()->setX(player1->getBoundingVolume()->getX() - player1->getBoundingVolume()->getOffsetX());
		player1->getPlayerController()->Vibrate(65535,65535);
	}
	if((player2->getBoundingVolume()->getX() + player2->getBoundingVolume()->getWidth()) > (viewport->getViewportX() + viewport->getViewportWidth()))
	{
		player2->getBoundingVolume()->setX(viewport->getViewportX() + viewport->getViewportWidth() - player2->getBoundingVolume()->getWidth());
		player2->getPhysicalProperties()->setX(player2->getBoundingVolume()->getX() - player2->getBoundingVolume()->getOffsetX());
		player2->getPlayerController()->Vibrate(65535,65535);
	}
	if((player1->getBoundingVolume()->getX()) < viewport->getViewportX())
	{
		player1->getBoundingVolume()->setX(viewport->getViewportX());
		player1->getPhysicalProperties()->setX(player1->getBoundingVolume()->getX() - player1->getBoundingVolume()->getOffsetX());
		player1->getPlayerController()->Vibrate(65535,65535);
	}
	if((player2->getBoundingVolume()->getX()) < viewport->getViewportX())
	{
		player2->getBoundingVolume()->setX(viewport->getViewportX());
		player2->getPhysicalProperties()->setX(player2->getBoundingVolume()->getX() - player2->getBoundingVolume()->getOffsetX());
		player2->getPlayerController()->Vibrate(65535,65535);
	}
	if(player1->getBoundingVolume()->getX() <= 0)
	{
		player1->getBoundingVolume()->setX(0);
		player1->getPhysicalProperties()->setX(player1->getBoundingVolume()->getX() - player1->getBoundingVolume()->getOffsetX());
		player1->getPlayerController()->Vibrate(65535,65535);
	}
	if(player2->getBoundingVolume()->getX() <= 0)
	{
		player2->getBoundingVolume()->setX(0);
		player2->getPhysicalProperties()->setX(player2->getBoundingVolume()->getX() - player2->getBoundingVolume()->getOffsetX());
		player2->getPlayerController()->Vibrate(65535,65535);
	}

	if(player1->getBoundingVolume()->getX() > game->getWorld()->getWorldWidth()
		&& player2->getBoundingVolume()->getX() > game->getWorld()->getWorldWidth())
	{
		game->changeGameState(NEXT_LEVEL_STATE);
	}

	if (moveViewport)
	{		
		viewport->moveViewport( incX,
								incY,
								getWorldWidth(),
								getWorldHeight());
	}
}