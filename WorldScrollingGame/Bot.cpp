/*
  See bot.h
*/
#include "stdafx.h"
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
#include "SpriteManager.h"
#include "Viewport.h"
#include "BoundingVolume.h"
#include "GamePhysics.h"
#include "Collision.h"
#include "GameAI.h"
#include "Bot.h"
#include "XACT.h"

Bot::Bot(){
	index=0;
	currentState=0;
	frameLeft=0;
	used = false;
	pattern = new vector<int>();
	enabled = true;
}
Bot::~Bot(){
	pattern->clear();
	delete pattern;
	delete animatedSprite;
}

void Bot::update(Game *game){
	if(animatedSprite->isEnabled){
		switch(type){
			case typeA:
				updateA();
				break;
			case typeB:
				updateB();
				break;
			case typeC:
				updateC();
				break;
			case typeD:
				updateD(game);
				break;
			case typeE:
				updateE(game);
				break;
			}
	}	

}
//type A AI:pattern, patrol
void Bot::updateA(){
	if(animatedSprite->isAlive==true){
		if(index>=pattern->size()){
			index=0;
		}
		if(frameLeft==0){
			currentState = pattern->at(index);
			index++;
			frameLeft = pattern->at(index);
			index++;
			if(currentState == RUN_RIGHT){
				animatedSprite->isFacingRight = true;
				animatedSprite->setCurrentState(RUN_RIGHT);

			}
			else if (currentState == RUN_LEFT){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(RUN_LEFT);
		}		
		
		if(frameLeft<=0){
			frameLeft =0;}		
		}
		if(currentState==2){
			animatedSprite->getPhysicalProperties()->setVelocityX(5);
		}
		else if(currentState==3){
			animatedSprite->getPhysicalProperties()->setVelocityX(-5);
		}
		frameLeft--;

	}
	else{
		
		if(animatedSprite->deadLock>0){
			animatedSprite->deadLock--;
			if(animatedSprite->deadLock==0){
				used=false;
				animatedSprite->getPhysicalProperties()->setVelocityX(0);
				animatedSprite->getPhysicalProperties()->setVelocityY(0);
				animatedSprite->getPhysicalProperties()->setAccelerationX(0);
				animatedSprite->getPhysicalProperties()->setAccelerationY(0);
				animatedSprite->getBoundingVolume()->setX(OUT_MAP_X);
				animatedSprite->getBoundingVolume()->setY(OUT_MAP_Y);
				animatedSprite->getPhysicalProperties()->setX(OUT_MAP_X);
				animatedSprite->getPhysicalProperties()->setY(OUT_MAP_Y);
				pattern->clear();
				animatedSprite->setCurrentState(0);
			}
		}
		
	}
}
//type B ai
void Bot::updateB(){
	if(animatedSprite->isAlive==true){
		if(index>=pattern->size()){
			index=0;
		}
		if(frameLeft==0){
			currentState = pattern->at(index);
			index++;
			frameLeft = pattern->at(index);
			index++;
			if(currentState == RUN_RIGHT){
				animatedSprite->isFacingRight = true;
				animatedSprite->setCurrentState(RUN_RIGHT);
                animatedSprite->color=1;
			}
			else if (currentState == RUN_LEFT){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(RUN_LEFT);
				animatedSprite->color=1;
		     }
			else if (currentState == RUN_LEFT2){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(RUN_LEFT2);
				animatedSprite->color=2;
		     }
			else if (currentState == RUN_RIGHT2){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(RUN_RIGHT2);
				animatedSprite->color=2;
		     }
			else if (currentState == ATTACKL1){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(ATTACKL1);
				animatedSprite->color=1;
		     }
			else if (currentState == ATTACKR1){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(ATTACKR1);
				animatedSprite->color=1;
		     }
			else if (currentState == ATTACKL2){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(ATTACKL2);
				animatedSprite->color=2;
		     }
			else if (currentState == ATTACKR2){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(ATTACKR2);
				animatedSprite->color=2;
		     }
			
			
		
			if(frameLeft<=0){
				frameLeft =0;}		
			}
			if(currentState==2||currentState==8){
				animatedSprite->getPhysicalProperties()->setVelocityX(5);
			}
			else if(currentState==3||currentState==9){
				animatedSprite->getPhysicalProperties()->setVelocityX(-5);
			}
			else if(currentState==4||currentState==5){
				animatedSprite->getPhysicalProperties()->setVelocityX(-15);
			}
			else if(currentState==6||currentState==7){
				animatedSprite->getPhysicalProperties()->setVelocityX(15);
			}
			frameLeft--;

	 }
	else{
		
		if(animatedSprite->deadLock>0){
			animatedSprite->deadLock--;
			if(animatedSprite->deadLock==0){
				used=false;
				animatedSprite->getPhysicalProperties()->setVelocityX(0);
				animatedSprite->getPhysicalProperties()->setVelocityY(0);
				animatedSprite->getPhysicalProperties()->setAccelerationX(0);
				animatedSprite->getPhysicalProperties()->setAccelerationY(0);
				animatedSprite->getBoundingVolume()->setX(OUT_MAP_X);
				animatedSprite->getBoundingVolume()->setY(OUT_MAP_Y);
				animatedSprite->getPhysicalProperties()->setX(OUT_MAP_X);
				animatedSprite->getPhysicalProperties()->setY(OUT_MAP_Y);
				pattern->clear();
				animatedSprite->setCurrentState(0);
			}
		}
		
	}

}
// type c is spotlight
void Bot::updateC(){
	if(animatedSprite->isAlive==true){
		if(index>=pattern->size()){
			index=0;
		}
		if(frameLeft==0){
			currentState = pattern->at(index);
			index++;
			frameLeft = pattern->at(index);
			index++;
			if(currentState == RUN_RIGHT){
				animatedSprite->isFacingRight = true;
				
			}
			else if (currentState == RUN_LEFT){
				animatedSprite->isFacingRight = false;
				
		    }
			if(currentState == ATTACKL1){
				animatedSprite->isFacingRight = true;				

			}
			else if (currentState == ATTACKL2){
				animatedSprite->isFacingRight = false;				
		    }		
		
			if(frameLeft<=0){
				frameLeft =0;}		
			}
			if(currentState==2){
				animatedSprite->getPhysicalProperties()->setVelocityX(8);
				animatedSprite->getPhysicalProperties()->setVelocityY(0);
			}
			else if(currentState==3){
				animatedSprite->getPhysicalProperties()->setVelocityX(-8);
				animatedSprite->getPhysicalProperties()->setVelocityY(0);
			}
			else if(currentState==4){
				animatedSprite->getPhysicalProperties()->setVelocityX(0);
				animatedSprite->getPhysicalProperties()->setVelocityY(8);
			}
			else if(currentState==5){
				animatedSprite->getPhysicalProperties()->setVelocityX(0);
				animatedSprite->getPhysicalProperties()->setVelocityY(-8);
			}
			frameLeft--;

	}
	
}
//type D is final boss

void Bot::updateD(Game *game){
	AnimatedSprite *fireball=game->getAI()->eventList->at(animatedSprite->eventIndex);
	if(animatedSprite->isAlive==true){
		if(index>=pattern->size()){
			index=0;
		}
		if(frameLeft==0){
			currentState = pattern->at(index);
			index++;
			frameLeft = pattern->at(index);
			index++;
			if(currentState == RUN_RIGHT){
				animatedSprite->isFacingRight = true;
				animatedSprite->setCurrentState(RUN_RIGHT);
				animatedSprite->color=2;
			}
			else if (currentState == RUN_LEFT){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(RUN_LEFT);
				animatedSprite->color=2;
			}
			else if (currentState == ATTACKL1){
				animatedSprite->isFacingRight = true;
				animatedSprite->setCurrentState(ATTACKL1);
				animatedSprite->color=1;
			}
			else if (currentState == ATTACKL2){
				animatedSprite->isFacingRight = false;
				animatedSprite->setCurrentState(ATTACKL2);
				animatedSprite->color=1;
			}
				
		
		if(frameLeft<=0){
			frameLeft =0;}		
		}
		if(currentState==2||currentState==4){
			animatedSprite->getPhysicalProperties()->setVelocityX(7);
		}
		else if(currentState==3||currentState==5){
			animatedSprite->getPhysicalProperties()->setVelocityX(-7);
		}
		frameLeft--;		
		if(fireball->isEnabled==false)// spawn fire ball
		{
			
			fireball->isEnabled=true;
			fireball->setAlpha(220);
			fireball->getBoundingVolume()->setX(animatedSprite->getBoundingVolume()->getX()+80);
			fireball->getBoundingVolume()->setY(animatedSprite->getBoundingVolume()->getY()+animatedSprite->getBoundingVolume()->getHeight());
            fireball->getPhysicalProperties()->setX(fireball->getBoundingVolume()->getX()-fireball->getBoundingVolume()->getOffsetX());
		    fireball->getPhysicalProperties()->setY(fireball->getBoundingVolume()->getY()-fireball->getBoundingVolume()->getOffsetY());	

		}

	}
	else{
		
		if(animatedSprite->deadLock>0){
			fireball->isAlive=false;  //reset the fire ball
			fireball->isEnabled=false;
			fireball->setAlpha(0);
			fireball->getPhysicalProperties()->setVelocityX(0);
			fireball->getPhysicalProperties()->setVelocityY(0);
			fireball->getPhysicalProperties()->setAccelerationX(0);
			fireball->getPhysicalProperties()->setAccelerationY(0);
			fireball->getBoundingVolume()->setX(OUT_MAP_X);
			fireball->getBoundingVolume()->setY(OUT_MAP_Y);
			fireball->getPhysicalProperties()->setX(OUT_MAP_X);
			fireball->getPhysicalProperties()->setY(OUT_MAP_Y);

			animatedSprite->deadLock--;
			if(animatedSprite->deadLock==0){
				used=false;
				animatedSprite->getPhysicalProperties()->setVelocityX(0);
				animatedSprite->getPhysicalProperties()->setVelocityY(0);
				animatedSprite->getPhysicalProperties()->setAccelerationX(0);
				animatedSprite->getPhysicalProperties()->setAccelerationY(0);
				animatedSprite->getBoundingVolume()->setX(OUT_MAP_X);
				animatedSprite->getBoundingVolume()->setY(OUT_MAP_Y);
				animatedSprite->getPhysicalProperties()->setX(OUT_MAP_X);
				animatedSprite->getPhysicalProperties()->setY(OUT_MAP_Y);
				pattern->clear();
				animatedSprite->setCurrentState(0);
			}
		}
		
	}
}
//type D is final boss
void Bot::updateE(Game *game){
	AnimatedSprite *player1 = game->getWorld()->getSpriteManager()->getPlayer1();
	AnimatedSprite *player2 = game->getWorld()->getSpriteManager()->getPlayer2();
	if(animatedSprite->isEnabled){
		bool target1;
		int dp1 = player1->getBoundingVolume()->getX()-animatedSprite->getBoundingVolume()->getX();
		if(dp1<0)
			dp1=0-dp1;
        int dp2 = player2->getBoundingVolume()->getX()-animatedSprite->getBoundingVolume()->getX();
		if(dp2<0)
			dp2=0-dp2;
		if (dp1<=dp2)
			target1=true;
		else
			target1=false;

        animatedSprite->getPhysicalProperties()->setVelocityY(5);
		if(target1){           
			dp1 = player1->getBoundingVolume()->getX()-animatedSprite->getBoundingVolume()->getX();
			if(dp1>0)
				animatedSprite->getPhysicalProperties()->setVelocityX(4);			
			else
				animatedSprite->getPhysicalProperties()->setVelocityX(-4);
		}
		else{
			dp2 = player2->getBoundingVolume()->getX()-animatedSprite->getBoundingVolume()->getX();
			if(dp2>0)
				animatedSprite->getPhysicalProperties()->setVelocityX(4);			
			else
				animatedSprite->getPhysicalProperties()->setVelocityX(-4);
		}

		if(animatedSprite->getBoundingVolume()->getX()<game->getWorld()->getViewport()->getViewportX()
		   ||animatedSprite->getBoundingVolume()->getX()>game->getWorld()->getViewport()->getViewportX()+game->getWorld()->getViewport()->getViewportWidth()
	       ||animatedSprite->getBoundingVolume()->getY()<game->getWorld()->getViewport()->getViewportY()
		   ||animatedSprite->getBoundingVolume()->getY()>game->getWorld()->getViewport()->getViewportY()+game->getWorld()->getViewport()->getViewportHeight()){
           animatedSprite->isEnabled=false;
		   animatedSprite->setAlpha(0);
		}    
	}

}