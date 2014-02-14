/*
  see GamePhysics.h
*/
#include "stdafx.h"
#include "Button.h"
#include "Cursor.h"
#include "Game.h"
#include "GameAI.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameGUI.h"
#include "GameInput.h"
#include "GameOS.h"
#include "GamePhysics.h"
#include "GameText.h"
#include "GameTimer.h"
#include "GameWorld.h"
#include "ScreenGUI.h"
#include "TextGenerator.h"
#include "TextFileWriter.h"
#include "PhysicalProperties.h"
#include "AnimatedSprite.h"
#include "SpriteManager.h"
#include "CollidableObject.h"
#include "BoundingVolume.h"
#include "TiledLayer.h"

GamePhysics::GamePhysics(){
	TileCollision = new vector<Collision*>();
	PointerA =0;
	RealCollision = new vector<Collision*>();
	Dynamic = new vector<Collision*>();
	AISprite = new vector<AnimatedSprite*>();
	for(int i = 0; i < 100; i++){

		Collision *temp1 = new Collision();
		CollidableObject *temp2 = new CollidableObject();
		BoundingVolume *temp3 = new BoundingVolume();
		PhysicalProperties *temp4 = new PhysicalProperties();

        temp2->setCurrentlyCollidable(true);
		temp2->setPhysicalProperties(temp4);
		temp2->setBoundingVolume(temp3);
		temp1->setObj1(temp2);
		
		TileCollision->push_back(temp1);
	}

}

GamePhysics::~GamePhysics(){
	for(int i = 0; i < 100; i++){
		delete TileCollision->at(i)->getObj1()->getBoundingVolume();
		delete TileCollision->at(i)->getObj1();
		delete TileCollision->at(i);
	}
    TileCollision->clear();
	delete TileCollision;
	RealCollision->clear();
	delete RealCollision;
	Dynamic->clear();
	delete Dynamic;
	AISprite->clear();
	delete AISprite;


}
//a compare function for sorting
boolean compare(Collision* co1, Collision* co2){
	return (co1->CoTime<co2->CoTime);
}
// adding bots so we can deal with its collision
void GamePhysics::addAISprite(AnimatedSprite* AI){
	AISprite->push_back(AI);
}
// adding collisions so we can test them
void GamePhysics::addCollision(Collision *co){
	Dynamic->push_back(co);
}
//function called by the main game, update once per frame
void GamePhysics::update(Game *game){
	SpriteManager *spriteManager = game->getWorld()->getSpriteManager();
	AnimatedSprite *player1,*player2,*player3;
    PhysicalProperties *pp1,*pp2,*pp3;

	//updates acc and v for all moving obj
	//bots
	for(int i=0; i< AISprite->size();i++){
		player3 = AISprite->at(i);
		pp3= player3->getPhysicalProperties();
		pp3->setVelocityX(pp3->getVelocityX() + pp3->getAccelerationX());
	    pp3->setVelocityY(pp3->getVelocityY() - pp3->getAccelerationY());
	}
   	
	player1 = spriteManager->getPlayer1();
    pp1= player1->getPhysicalProperties();
	player2 = spriteManager->getPlayer2();
    pp2= player2->getPhysicalProperties();
	player1->isStanding = false;
	player2->isStanding = false;
	//set new velocity according to the acc
	pp1->setVelocityX(pp1->getVelocityX() + pp1->getAccelerationX());
	pp1->setVelocityY(pp1->getVelocityY() - pp1->getAccelerationY());
	pp2->setVelocityX(pp2->getVelocityX() + pp2->getAccelerationX());
	pp2->setVelocityY(pp2->getVelocityY() - pp2->getAccelerationY());

	//initCollision() must only be called once before check collisions	
    initCollision();
    //find collisions
	findCollision(game);
	while(RealCollision->size()>0){
		sort(RealCollision->begin(),RealCollision->end(),compare);
		Collision *currentCollision = RealCollision->at(0);        
		currentCollision->solveCollision(game);
        PointerA=0;
        RealCollision->clear();
		findCollision(game);
	}
	
    //move player1
	pp1->incX(pp1->getVelocityX());
    pp1->incY(pp1->getVelocityY());
	player1->getBoundingVolume()->setX(pp1->getX()+player1->getBoundingVolume()->getOffsetX());
	player1->getBoundingVolume()->setY(pp1->getY()+player1->getBoundingVolume()->getOffsetY());	
	//player1->invincible=999;

	//move player2
	pp2->incX(pp2->getVelocityX());
    pp2->incY(pp2->getVelocityY());
	player2->getBoundingVolume()->setX(pp2->getX()+player2->getBoundingVolume()->getOffsetX());
	player2->getBoundingVolume()->setY(pp2->getY()+player2->getBoundingVolume()->getOffsetY());		
	//player2->invincible=999;
	


    //move bots
	for(int i=0; i< AISprite->size();i++){
		player3 = AISprite->at(i);
		
		pp3= player3->getPhysicalProperties();
		pp3->incX(pp3->getVelocityX());
		pp3->incY(pp3->getVelocityY());
		player3->getBoundingVolume()->setX(pp3->getX()+player3->getBoundingVolume()->getOffsetX());
		player3->getBoundingVolume()->setY(pp3->getY()+player3->getBoundingVolume()->getOffsetY());		
		
	}
	
	
}
//This will construct the RealCollision array.
void GamePhysics::findCollision(Game *game){
	SpriteManager *spriteManager = game->getWorld()->getSpriteManager();
	AnimatedSprite *player1,*player2,*player3;
    PhysicalProperties *pp1,*pp2,*pp3;
	TiledLayer *layer = (TiledLayer*)game->getWorld()->getLayers()->at(2);
	int minRow;
	int maxRow;
	int minColumn;
	int maxColumn;

	int minRowBound = 0;
	int maxRowBound = layer->getRows();
	int minColumBound = 0;
	int maxColumBound = layer->getColumns();

	// find the collisions for main characters(tile collision);
	//grid test.
	player1 = spriteManager->getPlayer1();
	pp1 = player1->getPhysicalProperties();
	BoundingVolume *ppBV=player1->getBoundingVolume();
	if(pp1->getVelocityX()>0){
		minColumn = (int)(ppBV->getX()/layer->getTileWidth());
		maxColumn = ceil( (ppBV->getX()+pp1->getVelocityX()+ppBV->getWidth())/layer->getTileWidth());
		if(minColumn<minColumBound){minColumn =minColumBound;}
		if(maxColumn>=maxColumBound){maxColumn=maxColumBound;}
	}
	else if(pp1->getVelocityX()<=0){
		minColumn = (int)( (ppBV->getX()+pp1->getVelocityX()) /layer->getTileWidth()) ;
		maxColumn = ceil( (ppBV->getX()+ppBV->getWidth())/layer->getTileWidth());
		if(minColumn<minColumBound){minColumn =minColumBound;}
		if(maxColumn>=maxColumBound){maxColumn=maxColumBound;}
	}


	if(pp1->getVelocityY()>0){
		minRow = (int)(ppBV->getY()/layer->getTileHeight());
		maxRow = ceil( (ppBV->getY()+pp1->getVelocityY()+ppBV->getHeight())/layer->getTileHeight());
		if(minRow<minRowBound){minRow =minRowBound;}
		if(maxRow>=maxRowBound){maxRow=maxRowBound;}
	}
	else if(pp1->getVelocityY()<=0){
        minRow = (int)((ppBV->getY()+pp1->getVelocityY())/layer->getTileHeight());
		maxRow = ceil( (ppBV->getY()+ppBV->getHeight())/layer->getTileHeight()) ;
		if(minRow<minRowBound){minRow =minRowBound;}
		if(maxRow>=maxRowBound){maxRow=maxRowBound;}
	}
	for(int i = minRow;i<maxRow;i++){
		for(int j = minColumn;j<maxColumn;j++){
			Tile *tile = layer->getTile(i,j);
			if (tile->collidable == true){
				//setup collision object 2 :player
				TileCollision->at(PointerA)->setObj2(player1);
				
				//setup collision object 1 : tile				
				//set up boundingbox
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setX(j*(layer->getTileWidth()));
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setWidth(layer->getTileWidth());
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setY(i*(layer->getTileHeight()));
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setHeight(layer->getTileHeight());
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setOffsetX(0);
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setOffsetY(0);	
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setType(0);
				TileCollision->at(PointerA)->getObj1()->setCurrentlyCollidable(true);
				
				//set physical property
                TileCollision->at(PointerA)->getObj1()->getPhysicalProperties()->setVelocityX(0);
				TileCollision->at(PointerA)->getObj1()->getPhysicalProperties()->setVelocityY(0);
				if(TileCollision->at(PointerA)->checkCollision()){
					if(tile->whazard==false&&tile->bhazard==true){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Bhazard);}
					else if(tile->bhazard==false&&tile->whazard==true){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Whazard);}
					else if(tile->side==TopSide){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Floor);}
					else if(tile->side==BottomSide){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Ceiling);}
					else if(tile->side==LeftSide){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(WallL);}
					else if(tile->side==RightSide){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(WallR);}					
					else{
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Ctile);}
					
					RealCollision->push_back(TileCollision->at(PointerA));
					PointerA++;
				}				
				
			}
		}
	}

	//testing for player 2
	player2 = spriteManager->getPlayer2();
	pp2 = player2->getPhysicalProperties();
	ppBV=player2->getBoundingVolume();
	if(pp2->getVelocityX()>0){
		minColumn = (int)(ppBV->getX()/layer->getTileWidth());
		maxColumn = ceil( (ppBV->getX()+pp2->getVelocityX()+ppBV->getWidth())/layer->getTileWidth());
		if(minColumn<minColumBound){minColumn =minColumBound;}
		if(maxColumn>=maxColumBound){maxColumn=maxColumBound;}
	}
	else if(pp2->getVelocityX()<=0){
		minColumn = (int)( (ppBV->getX()+pp2->getVelocityX()) /layer->getTileWidth()) ;
		maxColumn = ceil( (ppBV->getX()+ppBV->getWidth())/layer->getTileWidth());
		if(minColumn<minColumBound){minColumn =minColumBound;}
		if(maxColumn>=maxColumBound){maxColumn=maxColumBound;}
	}


	if(pp2->getVelocityY()>0){
		minRow = (int)(ppBV->getY()/layer->getTileHeight());
		maxRow = ceil( (ppBV->getY()+pp2->getVelocityY()+ppBV->getHeight())/layer->getTileHeight());
		if(minRow<minRowBound){minRow =minRowBound;}
		if(maxRow>=maxRowBound){maxRow=maxRowBound;}
	}
	else if(pp2->getVelocityY()<=0){
        minRow = (int)((ppBV->getY()+pp2->getVelocityY())/layer->getTileHeight());
		maxRow = ceil( (ppBV->getY()+ppBV->getHeight())/layer->getTileHeight()) ;
		if(minRow<minRowBound){minRow =minRowBound;}
		if(maxRow>=maxRowBound){maxRow=maxRowBound;}
	}

    
	for(int i = minRow;i<maxRow;i++){
		for(int j = minColumn;j<maxColumn;j++){
			Tile *tile = layer->getTile(i,j);
			if (tile->collidable == true){
				//setup collision object 2 :player
				TileCollision->at(PointerA)->setObj2(player2);
				
				//setup collision object 1 : tile				
				//set up boundingbox
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setX(j*(layer->getTileWidth()));
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setWidth(layer->getTileWidth());
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setY(i*(layer->getTileHeight()));
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setHeight(layer->getTileHeight());
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setOffsetX(0);
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setOffsetY(0);	
				TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setType(0);
				TileCollision->at(PointerA)->getObj1()->setCurrentlyCollidable(true);
				
				//set physical property
                TileCollision->at(PointerA)->getObj1()->getPhysicalProperties()->setVelocityX(0);
				TileCollision->at(PointerA)->getObj1()->getPhysicalProperties()->setVelocityY(0);
				if(TileCollision->at(PointerA)->checkCollision()){
					if(tile->whazard==false&&tile->bhazard==true){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Bhazard);}
					else if(tile->bhazard==false&&tile->whazard==true){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Whazard);}
					else if(tile->side==TopSide){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Floor);}
					else if(tile->side==BottomSide){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Ceiling);}
					else if(tile->side==LeftSide){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(WallL);}
					else if(tile->side==RightSide){
						TileCollision->at(PointerA)->getObj1()->setCollisionType(WallR);}	
					else{
						TileCollision->at(PointerA)->getObj1()->setCollisionType(Ctile);}
					RealCollision->push_back(TileCollision->at(PointerA));
					PointerA++;
				}				
				
			}
		}
	}

    //testing tiles of other sprites
   for(int i =0; i<AISprite->size();i++){
	    
		player3 = AISprite->at(i);
		if(player3->CollisionType!=SpotLight && player3->CollisionType!=FireBall){
			pp3 = player3->getPhysicalProperties();
			ppBV=player3->getBoundingVolume();

			if(pp3->getVelocityX()>0){
				minColumn = (int)(ppBV->getX()/layer->getTileWidth());
				maxColumn = ceil( (ppBV->getX()+pp3->getVelocityX()+ppBV->getWidth())/layer->getTileWidth());
				if(minColumn<minColumBound){minColumn =minColumBound;}
				if(maxColumn>=maxColumBound){maxColumn=maxColumBound;}
			}
			else if(pp3->getVelocityX()<=0){
				minColumn = (int)( (ppBV->getX()+pp3->getVelocityX()) /layer->getTileWidth()) ;
				maxColumn = ceil( (ppBV->getX()+ppBV->getWidth())/layer->getTileWidth());
				if(minColumn<minColumBound){minColumn =minColumBound;}
				if(maxColumn>=maxColumBound){maxColumn=maxColumBound;}
			}


			if(pp3->getVelocityY()>0){
				minRow = (int)(ppBV->getY()/layer->getTileHeight());
				maxRow = ceil( (ppBV->getY()+pp3->getVelocityY()+ppBV->getHeight())/layer->getTileHeight());
				if(minRow<minRowBound){minRow =minRowBound;}
				if(maxRow>=maxRowBound){maxRow=maxRowBound;}
			}
			else if(pp3->getVelocityY()<=0){
				minRow = (int)((ppBV->getY()+pp3->getVelocityY())/layer->getTileHeight());
				maxRow = ceil( (ppBV->getY()+ppBV->getHeight())/layer->getTileHeight()) ;
				if(minRow<minRowBound){minRow =minRowBound;}
				if(maxRow>=maxRowBound){maxRow=maxRowBound;}
			}
			for(int i = minRow;i<maxRow;i++){
				for(int j = minColumn;j<maxColumn;j++){
					Tile *tile = layer->getTile(i,j);
					if (tile->collidable == true){
						//setup collision object 2 :player
						TileCollision->at(PointerA)->setObj2(player3);
						
						//setup collision object 1 : tile				
						//set up boundingbox
						TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setX(j*(layer->getTileWidth()));
						TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setWidth(layer->getTileWidth());
						TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setY(i*(layer->getTileHeight()));
						TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setHeight(layer->getTileHeight());
						TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setOffsetX(0);
						TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setOffsetY(0);	
						TileCollision->at(PointerA)->getObj1()->getBoundingVolume()->setType(0);
						TileCollision->at(PointerA)->getObj1()->setCurrentlyCollidable(true);
						
						//set physical property
						TileCollision->at(PointerA)->getObj1()->getPhysicalProperties()->setVelocityX(0);
						TileCollision->at(PointerA)->getObj1()->getPhysicalProperties()->setVelocityY(0);
						if(TileCollision->at(PointerA)->checkCollision()){
							if(tile->side==TopSide){
								TileCollision->at(PointerA)->getObj1()->setCollisionType(Floor);}
							else if(tile->side==BottomSide){
								TileCollision->at(PointerA)->getObj1()->setCollisionType(Ceiling);}
							else if(tile->side==LeftSide){
								TileCollision->at(PointerA)->getObj1()->setCollisionType(WallL);}
							else if(tile->side==RightSide){
								TileCollision->at(PointerA)->getObj1()->setCollisionType(WallR);}	
							else{
								TileCollision->at(PointerA)->getObj1()->setCollisionType(Ctile);}
							
							RealCollision->push_back(TileCollision->at(PointerA));
							PointerA++;
						}				
						
					}
				}
			}
		}
	}
	


	
	//checking dynamic object collision
	for(int i=0;i<Dynamic->size();i++){
		if(Dynamic->at(i)->checkCollision()){
			RealCollision->push_back(Dynamic->at(i));
		}
	}   
}

//init necessary stuffs to check collisions
void GamePhysics::initCollision(){
	//set all one time collision to false;
	for(int i=0;i<Dynamic->size();i++){
		Dynamic->at(i)->checked=false;
	}
}
