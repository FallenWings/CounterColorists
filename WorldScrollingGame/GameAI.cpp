/*
   see GameAI.h
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
#include "Bot.h"

GameAI::GameAI(){
	botList = new vector<Bot*>();
	eventList = new vector<AnimatedSprite*>();
	for(int i=0;i<maxBot;i++){
		Bot *bot = new Bot();
		botList->push_back(bot);
	}
	aliveNum=0;
}

GameAI::~GameAI(){
	botList->clear();
	eventList->clear();
	delete eventList;
	delete botList;
}
//reset all bots and free them

void GameAI::freeAllBot(){
	for(int i = 0;i<botList->size();i++){
		if(botList->at(i)->used==true){
			botList->at(i)->used=false;	
			botList->at(i)->getAnimatedSprite()->getPhysicalProperties()->setVelocityX(0);
			botList->at(i)->getAnimatedSprite()->getPhysicalProperties()->setVelocityY(0);
			botList->at(i)->getAnimatedSprite()->getPhysicalProperties()->setAccelerationX(0);
			botList->at(i)->getAnimatedSprite()->getPhysicalProperties()->setAccelerationY(0);
			botList->at(i)->getAnimatedSprite()->getBoundingVolume()->setX(OUT_MAP_X);
			botList->at(i)->getAnimatedSprite()->getBoundingVolume()->setY(OUT_MAP_Y);
			botList->at(i)->getAnimatedSprite()->getPhysicalProperties()->setX(OUT_MAP_X);
			botList->at(i)->getAnimatedSprite()->getPhysicalProperties()->setY(OUT_MAP_Y);
			botList->at(i)->getAnimatedSprite()->setCurrentState(DIE_RIGHT);
			
		}
		
		botList->at(i)->getPattern()->clear();
		aliveNum=0;
		botList->at(i)->currentState=0;
		botList->at(i)->index=0;
		botList->at(i)->frameLeft=0;

	}
	boss=NULL;
}

int GameAI::addEvent(AnimatedSprite *a){
	eventList->push_back(a);
	return eventList->size()-1;
}

void GameAI::update(Game *game){
	aliveNum=0;	
	for(int i = 0;i<botList->size();i++){			
		if(botList->at(i)->used==true){
			botList->at(i)->update(game);
			if(botList->at(i)->getAnimatedSprite()->isAlive==true){
				aliveNum++;
		       }
		}		
	}
	if(boss!=NULL){
		if(!boss->getAnimatedSprite()->isAlive&&game->nextLevel<0){
			game->nextLevel= 150;
		}	
	}
}

Bot* GameAI::getFreeBot(){
	Bot *freeBot;
	for(int i=0;i<maxBot;i++){
		if(botList->at(i)->used==false){
			freeBot = botList->at(i);
			botList->at(i)->used=true;
			return freeBot;
		}
	}
	return freeBot;
}