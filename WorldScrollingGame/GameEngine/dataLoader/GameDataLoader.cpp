#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "BoundingVolume.h"
#include "CollidableObject.h"
#include "Game.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameWorld.h"
#include "PhysicalProperties.h"
#include "TiledLayer.h"
#include "SparseLayer.h"
#include "ParallaxLayer.h"
#include "SpriteManager.h"
#include "Viewport.h"
#include "CXBOXController.h"
#include "Collision.h"
#include "GamePhysics.h"
#include "GameAI.h"
#include "Bot.h"
#include <sstream>
#include "XACT.h"

/*
	loadLevelExample - This method loads the current
	level with data. It illustrates all the data
	we would need to load ourselves. You should
	load your data by reading it from a file.
*/
void GameDataLoader::loadLevelExample(Game *game, int level)
{
	// INIT EVERYTHING

	spriteImageID = new vector<int>();
	aiSpriteTypes = new vector<AnimatedSpriteType*>();
	//parallaxLayerTiles = new vector<pTile>();
	//decoLayerTiles = new vector<pTile>();
	//tiledLayerTiles = new vector<Tile>();
	ast = new AnimatedSpriteType();
	player1 = new AnimatedSprite();
	player1Props = new PhysicalProperties();
	player1->setPhysicalProperties(player1Props);
	player1BV = new BoundingVolume();
	player1->setBoundingVolume(player1BV);
	initPlayer1Controller =  new CXBOXController(1);
	player1->setPlayerController(initPlayer1Controller);

	player2 = new AnimatedSprite();
	player2Props = new PhysicalProperties();
	player2->setPhysicalProperties(player2Props);
	player2BV = new BoundingVolume();
	player2->setBoundingVolume(player2BV);
	initPlayer2Controller =  new CXBOXController(2);
	player2->setPlayerController(initPlayer2Controller);

	AnimatedSprite::setPlayerHealth(5);
	
	
	
	// FIRST SETUP THE GAME WORLD DIMENSIONS
	GameWorld *world = game->getWorld();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();
	Viewport *viewport = world->getViewport();
	//vector<int> *gameImages = new vector<int>();
	// Load Game Setup Data

	int matrixCols;
	int matrixRows;
	int tileSizeX;
	int tileSizeY;
	int sprite1ImageX;
	int sprite1ImageY;
	int sprite1InitalX;
	int sprite1InitalY;
	int sprite1BVX;
	int sprite1BVY;
	int sprite1OffsetX;
	int sprite1OffsetY;
	int sprite1AccelerationX;
	int sprite1AccelerationY;
	int sprite1InitalStateID;
	int sprite1Color;
	int sprite2ImageX;
	int sprite2ImageY;
	int sprite2InitalX;
	int sprite2InitalY;
	int sprite2BVX;
	int sprite2BVY;
	int sprite2OffsetX;
	int sprite2OffsetY;
	int sprite2AccelerationX;
	int sprite2AccelerationY;
	int sprite2InitalStateID;
	int sprite2Color;
	int parallaxCols;
	int parallaxRows;
	int parallaxTileSizeX;
	int parallaxTileSizeY;
	int decoCols;
	int decoRows;
	int decoTileSizeX;
	int decoTileSizeY;
	
	string line;
	string gameFileLocation = "Design\\Level ";
	std::stringstream out;
	out << level;
	gameFileLocation.append(out.str());
	gameFileLocation.append("\\GameLayoutSetup.txt");
	out.clear();

	ifstream gameSetupFile (gameFileLocation.c_str());
	if (gameSetupFile.is_open())
	{
		while (! gameSetupFile.eof() )
		{
			getline (gameSetupFile,line);
			if("**********" != line)
			{
				int divLocation = line.find_first_of("-");
				string valueName = (line.substr(0, divLocation));
				int value = atoi((line.substr(divLocation+1).c_str()));
				if(valueName == "WorldSizeX")
					world->setWorldWidth(value);
				else if(valueName == "WorldSizeY")
					world->setWorldHeight(value);
				else if(valueName == "ViewportSizeX")
					viewport->setViewportWidth(value);
				else if(valueName == "ViewportSizeY")
					viewport->setViewportHeight(value);
				else if(valueName == "ViewportOffsetX")
					viewport->setViewportOffsetX(value);
				else if(valueName == "ViewportOffsetY")
					viewport->setViewportOffsetY(value);
				else if(valueName == "ViewportCurrentX")
					viewport->setViewportX(value);
				else if(valueName == "ViewportCurrentY")
					viewport->setViewportY(value);
				else if(valueName == "MatrixColumns")
					matrixCols = value;
				else if(valueName == "MatrixRows")
					matrixRows = value;
				else if(valueName == "TileSizeX")
					tileSizeX = value;
				else if(valueName == "TileSizeY")
					tileSizeY = value;
				else if(valueName == "ParallaxColumns")
					parallaxCols = value;
				else if(valueName == "ParallaxRows")
					parallaxRows = value;
				else if(valueName == "ParallaxTileSizeX")
					parallaxTileSizeX = value;
				else if(valueName == "ParallaxTileSizeY")
					parallaxTileSizeY = value;
				else if(valueName == "DecoColumns")
					decoCols = value;
				else if(valueName == "DecoRows")
					decoRows = value;
				else if(valueName == "DecoTileSizeX")
					decoTileSizeX = value;
				else if(valueName == "DecoTileSizeY")
					decoTileSizeY = value;
				else if(valueName == "Sprite1ImageX")
					sprite1ImageX = value;
				else if(valueName == "Sprite1ImageY")
					sprite1ImageY = value;
				else if(valueName == "Sprite1InitalX")
					sprite1InitalX = value;
				else if(valueName == "Sprite1InitalY")
					sprite1InitalY = value;
				else if(valueName == "Sprite1BVX")
					sprite1BVX = value;
				else if(valueName == "Sprite1BVY")
					sprite1BVY = value;
				else if(valueName == "Sprite1OffsetX")
					sprite1OffsetX = value;
				else if(valueName == "Sprite1OffsetY")
					sprite1OffsetY = value;
				else if(valueName == "Sprite1InitalStateID")
					sprite1InitalStateID = value;
				else if(valueName == "Sprite1Color")
					sprite1Color = value;
				else if(valueName == "Sprite2ImageX")
					sprite2ImageX = value;
				else if(valueName == "Sprite2ImageY")
					sprite2ImageY = value;
				else if(valueName == "Sprite2InitalX")
					sprite2InitalX = value;
				else if(valueName == "Sprite2InitalY")
					sprite2InitalY = value;
				else if(valueName == "Sprite2BVX")
					sprite2BVX = value;
				else if(valueName == "Sprite2BVY")
					sprite2BVY = value;
				else if(valueName == "Sprite2OffsetX")
					sprite2OffsetX = value;
				else if(valueName == "Sprite2OffsetY")
					sprite2OffsetY = value;
				else if(valueName == "Sprite2InitalStateID")
					sprite2InitalStateID = value;
				else if(valueName == "Sprite2Color")
					sprite2Color = value;
				else if(valueName == "Sprite1AccelerationX")
					sprite1AccelerationX = value;
				else if(valueName == "Sprite1AccelerationY")
					sprite1AccelerationY = value;
				else if(valueName == "Sprite2AccelerationX")
					sprite2AccelerationX = value;
				else if(valueName == "Sprite2AccelerationY")
					sprite2AccelerationY = value;
				
			}
		}
		gameSetupFile.close();
	}

	// Load Images
	
	wchar_t *fileName;
	bool startCopying = false;
	ifstream imageMappingFile ("Design\\ImageMapping.txt");

	if (imageMappingFile.is_open())
	{
		while (! imageMappingFile.eof() )
		{
			getline (imageMappingFile,line);
			if("**********" == line)
			{
				if(startCopying == true)
					startCopying = false;
				else
					startCopying = true;
			}
			else
			{
				if(startCopying)
				{	
					int divLocation = line.find_first_of("-");
					string location = (line.substr(divLocation+1));
					int index = atoi((line.substr(0, divLocation).c_str()));
					int size = MultiByteToWideChar(CP_ACP, 0, location.c_str(), -1, 0, 0);
					wchar_t* buf = (wchar_t*)_malloca(size * sizeof(wchar_t));
					MultiByteToWideChar(CP_ACP, 0, location.c_str(), -1, buf, size);
					fileName = constructEmptyWCHAR_TArray(buf);
					spriteImageID->push_back(worldTextureManager->loadTexture(fileName));
				}
			}
		}
		imageMappingFile.close();
	}	

	// NOW LOAD A PARALLAX LAYER
	ParallaxLayer *parallaxLayer = new ParallaxLayer(parallaxCols, parallaxRows, parallaxTileSizeX, parallaxTileSizeY, 0, true, viewport, world->getWorldWidth(), world->getWorldHeight());

	string parallaxMapFileLocation = "Design\\Level ";
	std::stringstream parallaxOut;
	parallaxOut << level;
	parallaxMapFileLocation.append(parallaxOut.str());
	parallaxMapFileLocation.append("\\ParallaxLayerMap.csv");
	ifstream parallaxMapFile(parallaxMapFileLocation.c_str());
	if(parallaxMapFile)
	{
		int type;
		string inputLine;
		int tileIDToUse;
		for(int i = 0; i<parallaxRows; i++)
		{
			for(int k = 0; k<parallaxCols;k++)
			{
				//check the tile type
				getline(parallaxMapFile,inputLine,',');
				istringstream a(inputLine);
				a>>type;
				//check the collidable
				getline(parallaxMapFile,inputLine,',');
							
				
				tileIDToUse = spriteImageID->at(type);
				pTile *tileToAdd = new pTile();
				//parallaxLayerTiles->push_back(*tileToAdd);
				tileToAdd->collidable = false;
				tileToAdd->textureID = tileIDToUse;
				parallaxLayer->addTile(tileToAdd);
				
			}
		}
		parallaxMapFile.close();
	}
	else
	{
		cout<<"<Parallax Map.cvs fails";
	}
    world->addLayer(parallaxLayer);

	// NOW LOAD A DECO MAP
	decoLayer = new ParallaxLayer(decoCols, decoRows, decoTileSizeX, decoTileSizeY, 0, true, viewport, world->getWorldWidth(), world->getWorldHeight());

	string decorationMapFileLocation = "Design\\Level ";
	std::stringstream decorationOut;
	decorationOut << level;
	decorationMapFileLocation.append(decorationOut.str());
	decorationMapFileLocation.append("\\DecorationLayerMap.csv");
	ifstream decorationMapFile(decorationMapFileLocation.c_str());
	if(decorationMapFile)
	{
		int type;
		string inputLine;
		int tileIDToUse;
		for(int i = 0; i<decoRows; i++)
		{
			for(int k = 0; k<decoCols;k++)
			{
				//check the tile type
				getline(decorationMapFile,inputLine,',');
				istringstream a(inputLine);
				a>>type;
				//check the collidable
				getline(decorationMapFile,inputLine,',');
							
				
				tileIDToUse = spriteImageID->at(type);
				pTile *tileToAdd = new pTile();
				//decoLayerTiles->push_back(*tileToAdd);
				tileToAdd->collidable = false;
				tileToAdd->textureID = tileIDToUse;
				decoLayer->addTile(tileToAdd);
				
			}
		}
		decorationMapFile.close();
	}
	else
	{
		cout<<"<Deco Map.cvs fails";
	}
    world->addLayer(decoLayer);

	// NOW LOAD A TILED BACKGROUND
	tiledLayer = new TiledLayer(matrixCols, matrixRows, tileSizeX, tileSizeY, 0, true, viewport, world->getWorldWidth(), world->getWorldHeight());
	
	string gameMapFileLocation = "Design\\Level ";
	std::stringstream mapOut;
	mapOut << level;
	gameMapFileLocation.append(mapOut.str());
	gameMapFileLocation.append("\\Map.csv");
	ifstream mapFile(gameMapFileLocation.c_str());
	if(mapFile)
	{
		int type;
		string inputLine;
		bool isCollidable;
		bool bhazard,whazard;
		int tileIDToUse;
		int side;
		for(int i = 0; i<matrixRows; i++)
		{
			for(int k = 0; k<matrixCols;k++)
			{
				//check the tile type
				getline(mapFile,inputLine,',');
				istringstream a(inputLine);
				a>>type;
				//check the collidable
				getline(mapFile,inputLine,',');
				if(inputLine=="#")
				{
					isCollidable=true;
					whazard = false;
					bhazard = false;
					side = AllSide;
				}
				else if(inputLine=="[")
				{
					isCollidable=true;
					whazard = false;
					bhazard = false;
					side = LeftSide;
				}
				else if(inputLine=="]")
				{
					isCollidable=true;
					whazard = false;
					bhazard = false;
					side = RightSide;
				}
				else if(inputLine=="^")
				{
					isCollidable=true;
					whazard = false;
					bhazard = false;
					side = TopSide;
				}
				else if(inputLine=="v")
				{
					isCollidable=true;
					whazard = false;
					bhazard = false;
					side = BottomSide;
				}
				else if(inputLine == "w")
				{
					whazard = true;
					bhazard = false;
					isCollidable=true;
					side = AllSide;
				}
				else if(inputLine == "b")
				{
					whazard = false;
					bhazard = true;
					isCollidable=true;
					side = AllSide;
				}

				else
				{
					isCollidable=false;
					whazard = false;
					bhazard = false;
					side = AllSide;
				}							
				// todo
				tileIDToUse = spriteImageID->at(type);
				Tile *tileToAdd = new Tile();
				//tiledLayerTiles->push_back(*tileToAdd);
				tileToAdd->collidable = isCollidable;
				tileToAdd->textureID = tileIDToUse;
				tileToAdd->whazard = whazard;
				tileToAdd->bhazard = bhazard;
				tiledLayer->addTile(tileToAdd);
				tileToAdd->side=side;
				
			}
		}
		mapFile.close();
	}
	else
	{
		cout<<"<Map.cvs fails";
	}
   
    world->addLayer(tiledLayer);



	// AND NOW LET'S MAKE A MAIN CHARACTER SPRITE
	//AnimatedSpriteType *ast = new AnimatedSpriteType();
	
	bool isNewState = false;
	bool isNewName = false;
	bool isNewIndex = false;
	int index = -1;
	line = "";
	ifstream animationFile ("Design\\AnimationMapping.txt");
	if (animationFile.is_open())
	{
		while (! animationFile.eof() )
		{
			getline (animationFile,line);
			if("##########" == line)
			{
				if(isNewState)
					isNewState = false;
				else
					isNewState = true;
			}
			else if ("----------" == line)
			{
				if(isNewName)
					isNewName = false;
				else
					isNewName = true;
			}
			else if ("**********" == line)
			{
				if(isNewIndex)
					isNewIndex = false;
				else
					isNewIndex = true;
			}
			else
			{
				if(isNewIndex && isNewState)
				{
					int spriteIndex = atoi((line.c_str()));
					ast->addAnimationFrame(index, spriteImageID->at(spriteIndex));
				}
				else if(isNewName && isNewState)
				{
					int divLocation = line.find_first_of("-");
					string spriteState = (line.substr(divLocation+1));
					index = atoi((line.substr(0, divLocation).c_str()));
					int size = MultiByteToWideChar(CP_ACP, 0, spriteState.c_str(), -1, 0, 0);
					wchar_t* buf = (wchar_t*)_malloca(size * sizeof(wchar_t));
					MultiByteToWideChar(CP_ACP, 0, spriteState.c_str(), -1, buf, size);
					wchar_t *animStateName = constructEmptyWCHAR_TArray(buf);
					ast->addAnimationState(animStateName);
				}
			}
		}
		animationFile.close();
	}

	// CHANGE THE IMAGE ONCE EVERY 6 FRAMES
	ast->setAnimationSpeed(6);

	// SIZE OF SPRITE IMAGES
	ast->setTextureSize(sprite1ImageX, sprite1ImageY);


	SpriteManager *spriteManager = world->getSpriteManager();
	spriteManager->addSpriteType(ast);

	

	player1->setCollisionType(0);
	player1->setSpriteType(ast);
	player1->getPhysicalProperties()->setX(sprite1InitalX);
	player1->getPhysicalProperties()->setY(sprite1InitalY);
	player1->getPhysicalProperties()->setVelocityX(0);
	player1->getPhysicalProperties()->setVelocityY(0);
	player1->getPhysicalProperties()->setAccelerationX(sprite1AccelerationX);
	player1->getPhysicalProperties()->setAccelerationY(sprite1AccelerationY);
	player1->isFacingRight = true;
	player1->getBoundingVolume()->setWidth(sprite1BVX);
	player1->getBoundingVolume()->setHeight(sprite1BVY);
	player1->getBoundingVolume()->setOffsetX(sprite1OffsetX);
	player1->getBoundingVolume()->setOffsetY(sprite1OffsetY);
	player1->getBoundingVolume()->setX(sprite1InitalX+sprite1OffsetX);
	player1->getBoundingVolume()->setY(sprite1InitalY+sprite1OffsetY);

	

	// WE WILL SET LOTS OF OTHER PROPERTIES ONCE
	// WE START DOING COLLISIONS AND PHYSICS

	player1->setAlpha(255);
	player1->setCurrentState(sprite1InitalStateID);
	player1->playerNumber = sprite1Color;
	player1->setCurrentlyCollidable(true);

	spriteManager->setPlayer1(player1);

	player2->setSpriteType(ast);
	player2->setCollisionType(1);
	player2->getPhysicalProperties()->setX(sprite2InitalX);
	player2->getPhysicalProperties()->setY(sprite2InitalY);
	player2->getPhysicalProperties()->setVelocityX(0);
	player2->getPhysicalProperties()->setVelocityY(0);
	player2->getPhysicalProperties()->setAccelerationX(sprite2AccelerationX);
	player2->getPhysicalProperties()->setAccelerationY(sprite2AccelerationY);
	player2->isFacingRight = true;
	player2->getBoundingVolume()->setWidth(sprite2BVX);
	player2->getBoundingVolume()->setHeight(sprite2BVY);
	player2->getBoundingVolume()->setX(sprite2InitalX);
	player2->getBoundingVolume()->setY(sprite2InitalY);
	player2->getBoundingVolume()->setOffsetX(sprite2OffsetX);
	player2->getBoundingVolume()->setOffsetY(sprite2OffsetY);
	player2->getBoundingVolume()->setX(sprite2InitalX+sprite2OffsetX);
	player2->getBoundingVolume()->setY(sprite2InitalY+sprite2OffsetY);

	
	// WE WILL SET LOTS OF OTHER PROPERTIES ONCE
	// WE START DOING COLLISIONS AND PHYSICS

	player2->setAlpha(255);
	player2->setCurrentState(sprite2InitalStateID);
	player2->playerNumber = sprite2Color;
	player2->setCurrentlyCollidable(true);
	spriteManager->setPlayer2(player2);

	//setup collisions between player1 and player2
	Collision *Grab = new Collision();
	Grab->setObj1(player1);
	Grab->setObj2(player2);
	game->getPhysics()->addCollision(Grab);
    
	
	
	// Load Animated Sprite Types
	spriteManager = world->getSpriteManager();
	ifstream overlayTypesFile ("Design\\OverlayTypes.txt");
	if (overlayTypesFile.is_open())
	{
		bool newType = false;
		bool copyInfo = false;
		bool copyStates = false;
		bool getNumStates = false;
		string typeName = "";
		int imageID;
		int speed;
		int textureSizeX;
		int textureSizeY;
		int numStates;
		int index;
		int spriteTypeID;
		while (! overlayTypesFile.eof() )
		{
			getline (overlayTypesFile,line);
			if("++++++++++" == line)
			{
				if (getNumStates)
					getNumStates = false;
				else
					getNumStates = true;
			}
			else if("==========" == line)
			{
				if (newType)
					newType = false;
				else
					newType = true;
			}
			else if("**********" == line)
			{
				if (copyStates)
					copyStates = false;
				else
					copyStates = true;
			}
			else if("##########" == line)
			{
				if (copyInfo)
				{
					copyInfo = false;
					AnimatedSpriteType *newSpriteType = new AnimatedSpriteType();
					newSpriteType->setAnimationSpeed(speed);
					newSpriteType->setTextureSize(textureSizeX, textureSizeY);
					newSpriteType->setSpriteTypeID(spriteTypeID);
					aiSpriteTypes->push_back(newSpriteType);
					spriteManager->addSpriteType(newSpriteType);
				}
				else 
					copyInfo = true;
			}
			else
			{
				if(getNumStates)
				{
					int divLocation = line.find_first_of("-");
					string valueName = (line.substr(0, divLocation));
					if(valueName == "TypeName")
						numStates = atoi((line.substr(divLocation+1).c_str()));
				}
				if(newType && copyInfo)
				{
					int divLocation = line.find_first_of("-");
					string valueName = (line.substr(0, divLocation));
					if(valueName == "TypeName")
						typeName = (line.substr(divLocation+1).c_str());
					else if(valueName == "ImageID")
						imageID = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "SpriteTypeID")
						spriteTypeID = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "Speed")
						speed = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "TextureSizeX")
						textureSizeX = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "TextureSizeY")
						textureSizeY = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "Index")
						index = atoi((line.substr(divLocation+1).c_str()));
				}
				else if(newType && copyStates)
				{
					int divLocation = line.find_first_of("-");
					int stateIndex = atoi((line.substr(0,divLocation).c_str()));
					int divLoc2 = line.find_first_of(":");
					int imageID = atoi((line.substr(divLocation+1, divLoc2).c_str()));
					string stateName = (line.substr(divLoc2+1).c_str());

					int size = MultiByteToWideChar(CP_ACP, 0, stateName.c_str(), -1, 0, 0);
					wchar_t* buf = (wchar_t*)_malloca(size * sizeof(wchar_t));
					MultiByteToWideChar(CP_ACP, 0, stateName.c_str(), -1, buf, size);
					wchar_t *spriteStateName = constructEmptyWCHAR_TArray(buf);
					
					if(aiSpriteTypes->at(index)->getNumberOfSequences() <= stateIndex)
						aiSpriteTypes->at(index)->addAnimationState(spriteStateName);
					aiSpriteTypes->at(index)->addAnimationFrame(stateIndex, imageID);
				}
			}
		}
		overlayTypesFile.close();
	}


		// Load Animated Sprites
	spriteManager = world->getSpriteManager();
	string animatedSpriteLocation = "Design\\Level ";
	std::stringstream animatedSpriteOut;
	animatedSpriteOut << level;
	animatedSpriteLocation.append(animatedSpriteOut.str());
	animatedSpriteLocation.append("\\OverlayPositions.txt");
	ifstream animatedSpritesFile(animatedSpriteLocation.c_str());
	if (animatedSpritesFile.is_open())
	{
		bool newType = false;
		bool copyInfo = false;
		bool copyStates = false;
		int spriteAIPosX;
		int spriteAIPosY;
		int spriteAccelerationX;
		int spriteAccelerationY;
		int spriteVelocityX;
		int spriteVelocityY;
		int spriteAlpha;
		int spriteCurrentState;
		int spriteCollisionType;
		int spriteIsAlive;
		int spriteIsFacingRight;
		int spriteTypeIndex;
		int spriteAIBoundingHeight;
		int spriteAIBoundingWidth;
		int spriteAIOffsetX;
		int spriteAIOffsetY;
		int spriteAIType;
		int spriteAIColor;
		int spriteAIHealth;
		int spriteEventIndex;
		int spriteAIIsBoss;
		int enabled=1;

		Bot* currentBot;
		
		while (! animatedSpritesFile.eof() )
		{
			getline (animatedSpritesFile,line);
			if("==========" == line)
			{
				if (newType)
					newType = false;
				else
					newType = true;
			}
			else if("**********" == line)
			{
				if (copyStates)
					copyStates = false;
				else
					copyStates = true;
			}
			else if("##########" == line)
			{
				if (copyInfo)
				{
					copyInfo = false;
					AnimatedSprite *newAI = new AnimatedSprite();
				
					newAI->isFacingRight = spriteIsFacingRight;
					AnimatedSpriteType* temp = aiSpriteTypes->at(spriteTypeIndex - 1);
					newAI->setSpriteType(temp);
					newAI->setAlpha(spriteAlpha);
					newAI->setCurrentState(spriteCurrentState);
					newAI->setCurrentlyCollidable(true);
					newAI->color=spriteAIColor;
					newAI->aiHealth=spriteAIHealth;
					newAI->setCollisionType(spriteCollisionType);
					newAI->isAlive = spriteIsAlive;
					PhysicalProperties *AIProps1 = new PhysicalProperties();
					newAI->setPhysicalProperties(AIProps1);
					AIProps1->setAccelerationX(spriteAccelerationX);
					AIProps1->setAccelerationY(spriteAccelerationY);
					AIProps1->setVelocityX(spriteVelocityX);
					AIProps1->setVelocityY(spriteVelocityY);
					BoundingVolume *AIBV1 = new BoundingVolume();      //4th new
					newAI->setBoundingVolume(AIBV1);
					AIBV1->setHeight(spriteAIBoundingHeight);
					AIBV1->setWidth(spriteAIBoundingWidth);
					AIBV1->setOffsetX(spriteAIOffsetX);
					AIBV1->setOffsetY(spriteAIOffsetY);
					AIBV1->setType(spriteAIType);
					AIBV1->setX(spriteAIPosX);
					AIBV1->setY(spriteAIPosY);
					AIProps1->setX(AIBV1->getX() - newAI->getBoundingVolume()->getOffsetX());
					AIProps1->setY(AIBV1->getY() - newAI->getBoundingVolume()->getOffsetY());
					if(enabled==1){
						newAI->isEnabled=true;
					}
					else{
						newAI->isEnabled=false;}
					
					Collision *player1VSai1 = new Collision();//player1 vs ai1                            5th new
					player1VSai1->setObj1(newAI);							//obj1 is always the ai
					player1VSai1->setObj2(player1);						//obj2 is always the player
					game->getPhysics()->addCollision(player1VSai1);		// register the collision.
     
					Collision *player2VSai1 = new Collision();			//player2 vs ai1                 6th new
					player2VSai1->setObj1(newAI);							//obj1 is always the ai
					player2VSai1->setObj2(player2);						//obj2 is always the player
					game->getPhysics()->addCollision(player2VSai1);		// register the collision.
					
    
					
					if(spriteTypeIndex == 1 || spriteTypeIndex == 4 || spriteTypeIndex == 5) // BOT
					{
						game->getPhysics()->addAISprite(newAI);//tile collision
						Bot *Bot1 = game->getAI()->getFreeBot();//request an AI
						currentBot = Bot1;
						Bot1->setAnimatedSprite(newAI);
						Bot1->used = true;
						Bot1->setType(spriteAIType);
						
						vector<int> *pattern =  Bot1->getPattern();// AI pattern setup.
						pattern->clear();//clear the old pattern. Not necessary if you want to add pattern.

						if(spriteAIIsBoss == 1)
							game->getAI()->boss = Bot1;
						
					}
					else if(spriteTypeIndex == 7 || spriteTypeIndex == 8 || spriteTypeIndex == 10){
						game->getPhysics()->addAISprite(newAI);//tile collision
						Bot *Bot1 = game->getAI()->getFreeBot();//request an AI
						currentBot = Bot1;
						Bot1->setAnimatedSprite(newAI);
						Bot1->used = true;
						Bot1->setType(spriteAIType);						
						vector<int> *pattern =  Bot1->getPattern();// AI pattern setup.
						pattern->clear();//clear the old pattern. Not necessary if you want to add pattern.
						newAI->eventIndex = game->getAI()->addEvent(spriteManager->getSprite(spriteEventIndex - 1));
						if(spriteAIIsBoss == 1)
							game->getAI()->boss = Bot1;

					}
					else if(spriteTypeIndex == 2) //KEY
					{
						newAI->eventIndex = game->getAI()->addEvent(spriteManager->getSprite(spriteEventIndex - 1));
					}
					else if(spriteTypeIndex == 9){  //fireball
						game->getPhysics()->addAISprite(newAI);//tile collision
						Bot *Bot1 = game->getAI()->getFreeBot();//request an AI
						currentBot = Bot1;
						Bot1->setAnimatedSprite(newAI);
						Bot1->used = true;
						Bot1->setType(spriteAIType);

					}

					
					spriteManager->addSprite(newAI);

				}
				else 
					copyInfo = true;
			}
			else
			{
				if(newType && copyInfo)
				{
					int divLocation = line.find_first_of("-");
					string valueName = (line.substr(0, divLocation));
					if(valueName == "SpriteTypeIndex")
						spriteTypeIndex = atoi(line.substr(divLocation+1).c_str());
					else if(valueName == "AIPositionX")
						spriteAIPosX = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AIPositionY")
						spriteAIPosY = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AccelerationX")
						spriteAccelerationX = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AccelerationY")
						spriteAccelerationY = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "VelocityX")
						spriteVelocityX = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "VelocityY")
						spriteVelocityY = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "Alpha")
						spriteAlpha = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "CurrentState")
						spriteCurrentState = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "CollisionType")
						spriteCollisionType = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "IsAlive")
						spriteIsAlive = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "IsFacingRight")
						spriteIsFacingRight = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AIBoundingHeight")
						spriteAIBoundingHeight = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AIBoundingWidth")
						spriteAIBoundingWidth = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AIOffsetX")
						spriteAIOffsetX = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AIOffsetY")
						spriteAIOffsetY = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AIType")
						spriteAIType = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AIColor")
						spriteAIColor = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "AIHealth")
						spriteAIHealth = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "EventIndex")
						spriteEventIndex = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "IsBoss")
						spriteAIIsBoss = atoi((line.substr(divLocation+1).c_str()));
					else if(valueName == "Enabled")
						enabled = atoi((line.substr(divLocation+1).c_str()));
					
				}
				else if(newType && copyStates)
				{
					int pattern = atoi((line.c_str()));
					currentBot->getPattern()->push_back(pattern);
				}
			}
		}
		animatedSpritesFile.close();
	}
    
	delete spriteImageID;

}


/*
	loadWorld - This method should load the data
	for the GameWorld's current level. The GameWorld
	stores a vector of all level file names. Such
	a file would describe how a level would be
	constructed.
*/
void GameDataLoader::loadWorld(Game *game, int level)	
{
	// NOTE:	I AM DEMONSTRATING HOW TO LOAD A LEVEL
	//			PROGRAMICALLY. YOU SHOULD DO THIS
	//			USING CSV FILES.
	loadLevelExample(game, level);
}

/*
	loadGUI - One could use this method to build
	the GUI based on the contents of a GUI file.
	That way we could change the GUI and add to
	it without having to rebuild the project.
*/
void GameDataLoader::loadGUI(Game *game)
{
	
}


GameDataLoader::GameDataLoader()
{

}

GameDataLoader::~GameDataLoader()
{
	//spriteImageID->clear();
	//parallaxLayerTiles->clear();
	//decoLayerTiles->clear();
	//tiledLayerTiles->clear();
	
}