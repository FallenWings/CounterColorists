/*
   see Collision.h.
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

#include "XACT.h"

//check Collision here
boolean Collision::checkCollision(){
	if(obj1->isCurrentlyCollidable()==false || obj2->isCurrentlyCollidable()==false){
		return false;
	}
	if(checked==true){
		return false;}
	
	if (obj1->getBoundingVolume()->getX()
		<=obj2->getBoundingVolume()->getX()){
		left = obj1;
		right = obj2;
	}
	else{
		left = obj2;
		right = obj1;
	}

	if(obj1->getBoundingVolume()->getY()
		<=obj2->getBoundingVolume()->getY()){
		top = obj1;
		bottom = obj2;
	}
	else{
		top = obj2;
		bottom = obj1;
	}

	float x1 = obj1->getBoundingVolume()->getX();
	float y1 = obj1->getBoundingVolume()->getY();
	float vX1 = obj1 ->getPhysicalProperties()->getVelocityX();
	float vY1 = obj1 ->getPhysicalProperties()->getVelocityY();
	float Width1 = obj1 ->getBoundingVolume()->getWidth();
	float Height1 = obj1 ->getBoundingVolume()->getHeight();
	
	float x2 = obj2->getBoundingVolume()->getX();
	float y2 = obj2->getBoundingVolume()->getY();
	float vX2 = obj2 ->getPhysicalProperties()->getVelocityX();
	float vY2 = obj2 ->getPhysicalProperties()->getVelocityY();
	float Width2 = obj2 ->getBoundingVolume()->getWidth();
	float Height2 = obj2 ->getBoundingVolume()->getHeight();

	XstartTime=calculateStartTimeOfCollisionForAnyAxis(	x1, x2, vX1, vX2, Width1, Width2);
	XendTime= calculateEndTimeOfCollisionForAnyAxis(x1, x2, vX1, vX2, Width1, Width2);
	YstartTime=calculateStartTimeOfCollisionForAnyAxis(	y1, y2, vY1, vY2, Height1, Height2);
	YendTime=calculateEndTimeOfCollisionForAnyAxis(	y1, y2, vY1, vY2, Height1, Height2);

	// NO COLLISION EVER
	if ((XendTime < YstartTime)
			||(YendTime < XstartTime))
			isColliding=false;
			
		// NO COLLISION THIS FRAME
	else if ((XstartTime > 1.0) || (YstartTime > 1.0))
		    isColliding = false;			
		// COLLISION
	else{
			// RETURN THE LATER OF THE TWO TIMES
		if (YstartTime < XstartTime){
			isColliding=true;
			Xcolliding=true;
			CoTime= XstartTime;
		}
		else{
            isColliding=true;
			Xcolliding=false;
			CoTime= YstartTime;
		}
	}

	//sort two object by top bottom left right	
	return isColliding;

}

//help methods to calculate start collision time
float Collision::calculateStartTimeOfCollisionForAnyAxis(float pos1, float pos2, float v1, float v2, float length1, float length2){
	float timeStart;
	if (pos1 == pos2)
			timeStart = 0.0;
	else if (pos1 < pos2)
			timeStart = calculateTimeOfFirstAxisCollision(pos1, pos2, v1, v2, length1, length2);
	else
			timeStart = calculateTimeOfFirstAxisCollision(pos2, pos1, v2, v1, length2, length1);
	return timeStart;

}

float Collision::calculateTimeOfFirstAxisCollision(float pos1, float pos2, float v1, float v2, float length1, float length2){
	float timeStart;
	float posDiff = pos2 - (pos1 + length1);

		// ARE THEY COLLIDING AT THE START?
	if (posDiff <= 0)
		timeStart = 0.0;
	else
	{
		float vDiff = v1 - v2;
			// MAYBE THEY'LL NEVER COLLIDE
		if (vDiff <= 0)
			timeStart = 2.0;
		else
			timeStart = posDiff/vDiff;
		}				
	return timeStart;

}

//help methods to calculate end collision time
float Collision::calculateEndTimeOfCollisionForAnyAxis(float pos1, float pos2, float v1, float v2, float length1, float length2){
	float timeEnd;
	if (pos1 < pos2)
		timeEnd = calculateTimeOfLastAxisCollision(pos1, pos2, v1, v2, length1, length2);
	else
		timeEnd = calculateTimeOfLastAxisCollision(pos2, pos1, v2, v1, length2, length1);
	return timeEnd;

}

float Collision::calculateTimeOfLastAxisCollision(float pos1, float pos2, float v1, float v2, float length1, float length2){
	float timeEnd;
	float vDiff = v1 - v2;

		// ARE THEY NOT MOVING RELATIVE TO ONE ANOTHER IN THIS AXIS?
	if (vDiff == 0)
		timeEnd = 2.0;
		// ARE THEY MOVING TOWARD EACH OTHER?
	else if (vDiff > 0.0)
	{
		float posDiff = (pos2 + length2) - pos1;
		timeEnd = posDiff/vDiff;
	}
		// THEY MUST BE MOVING AWAY FROM EACH OTHER
	else
	{
		float posDiff = (pos1 + length1) - pos2;
		if (posDiff < 0.0)
			timeEnd = 2.0;
		else
			timeEnd = posDiff/-vDiff;
	}
	return timeEnd;
}



//solve the current collision
void Collision::solveCollision(Game *game){
	// tile vs any 	sprite
	if(obj1->getCollisionType()==Ctile){
		tileVSsprite(game);
	}
	//floor vs any sprite
	else if(obj1->getCollisionType()==Floor){
       floorVSsprite(game);	
	}
	else if(obj1->getCollisionType()==Ceiling){
       ceilingVSsprite(game);
	} 
	else if(obj1->getCollisionType()==WallL){
		walllVSSprite(game);
	}
	else if(obj1->getCollisionType()==WallR){
		wallrVSSprite(game);
	}
	//player vs hazard
	else if(obj1->getCollisionType()==Bhazard || obj1->getCollisionType()==Whazard){
		hazardVSplayer(game);
	}
	// player 1 vs player2
	else if(obj1->getCollisionType()==Wplayer && obj2->getCollisionType() == Bplayer2){
		player1VSplayer2();
	}
	//player vs sprite
	else if(obj1->getCollisionType()==AI_TypeA && (obj2->getCollisionType()==Wplayer || obj2->getCollisionType()==Bplayer2)){
		playerVSai(game);
	}
	//player vs key 
	else if(obj1->getCollisionType()==Key_Event){
		keyVSplayer(game);
	}
	//player vs spotlight
	else if(obj1->getCollisionType()==SpotLight){
        spotlightVSplayer(game);
	}
	//player vs fireball and final boss
	else if(obj1->getCollisionType()==FireBall){
        fireballVSplayer(game);
	}

}
//fire ball & final boss vs player;
void Collision::fireballVSplayer(Game *game){
	checked=true;
    playerVSai(game); //same as ai vs player
}
//spotlight vs player
void Collision::spotlightVSplayer(Game *game){
	checked = true;
	AnimatedSprite *spotLight = (AnimatedSprite*) obj1;
	AnimatedSprite *player = (AnimatedSprite*) obj2;
	//only dectect if same color
	if(spotLight->color!=player->playerNumber && spotLight->isAlive){
		game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().ALARM);
		
		AnimatedSprite *AI = game->getAI()->eventList->at(spotLight->eventIndex);
		AI->setAlpha(255);
		AI->isEnabled=true;			
		
		spotLight->setCurrentlyCollidable(false);
		spotLight->getBoundingVolume()->setX(OUT_MAP_X);
		spotLight->getBoundingVolume()->setX(OUT_MAP_Y);
        spotLight->getPhysicalProperties()->setVelocityX(0);
        spotLight->getPhysicalProperties()->setVelocityY(0);
		spotLight->getPhysicalProperties()->setX(spotLight->getBoundingVolume()->getX()-spotLight->getBoundingVolume()->getOffsetX());
		spotLight->getPhysicalProperties()->setY(spotLight->getBoundingVolume()->getY()-spotLight->getBoundingVolume()->getOffsetY());	
		spotLight->isAlive=false;
	}

}
//key vs player
void Collision::keyVSplayer(Game *game){
	checked = true;
	AnimatedSprite *key = (AnimatedSprite*) obj1;
	//remove key on the map
	key->setCurrentlyCollidable(false);
	key->getBoundingVolume()->setX(OUT_MAP_X);
	key->getBoundingVolume()->setX(OUT_MAP_Y);
	key->getPhysicalProperties()->setX(key->getBoundingVolume()->getX()-key->getBoundingVolume()->getOffsetX());
	key->getPhysicalProperties()->setY(key->getBoundingVolume()->getY()-key->getBoundingVolume()->getOffsetY());

	game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().GOT_ITEM);

	//remove the door on the map
	AnimatedSprite *door = game->getAI()->eventList->at(key->eventIndex);
	door->setCurrentlyCollidable(false);
	door->setCurrentState(1);

}
//hazard vs player
void Collision::hazardVSplayer(Game *game){
	tileVSsprite(game);   //first it works like a tile.
	AnimatedSprite* player = (AnimatedSprite*) obj2;
	if( (player->playerNumber==1 && obj1->CollisionType==Whazard) || (player->playerNumber==2 && obj1->CollisionType==Bhazard) ||player->invincible>0 || player->isAlive==false){
		//same color. we do nothing
	}
	else{
		 AnimatedSprite::setPlayerHealth(AnimatedSprite::getPlayerHealth()-1);
		if(AnimatedSprite::getPlayerHealth()>0){
             
			  player->invincible=100;
			  player->setAlpha(150);
			  player->isHurt=true;

			  game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_HURT);

			  player->HurtLock=30;
			  player->getPhysicalProperties()->setVelocityX(0);
			  if(player->isFacingRight&&player->playerNumber==1){
				  player->setCurrentState(WHITE_HURTING_FACING_RIGHT);
			  }
			  else if(!player->isFacingRight&&player->playerNumber==1){
				  player->setCurrentState(WHITE_HURTING_FACING_LEFT);
			  }
			  if(player->isFacingRight&&player->playerNumber==2){
				  player->setCurrentState(BLACK_HURTING_FACING_RIGHT);
			  }
			  else if(!player->isFacingRight&&player->playerNumber==2){
				  player->setCurrentState(BLACK_HURTING_FACING_RIGHT);
			  }
		   }
		
	else{
		
		player->getPhysicalProperties()->setVelocityX(0);
		 if(player->isFacingRight&&player->playerNumber==1){
				  player->setCurrentState(WHITE_DYING_RIGHT);
			  }
		else if(!player->isFacingRight&&player->playerNumber==1){
				  player->setCurrentState(WHITE_DYING_LEFT);
			  }
		if(player->isFacingRight&&player->playerNumber==2){
				  player->setCurrentState(BLACK_DYING_RIGHT);
			  }
		else if(!player->isFacingRight&&player->playerNumber==2){
				  player->setCurrentState(BLACK_DYING_LEFT);
			  }
		 player->isAlive=false;

		 game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_DIE);
		 game->gameOver=150;
	    }
       
	}
}
//player vs collision_typeA
void Collision::playerVSai(Game *game){
	checked= true;
	AnimatedSprite* player = (AnimatedSprite*) obj2;
	AnimatedSprite* ai = (AnimatedSprite*) obj1;
    //player is attacking
	if(player->isAttacking==true && (player->playerNumber==ai->color)&&(ai->invincible<=0)&&(ai->isAlive==true)&&(ai->isEnabled)){
		ai->aiHealth--;
		player->getPhysicalProperties()->setVelocityY(-15);
		if(player->isFacingRight)
			player->getPhysicalProperties()->setVelocityX(-15);
		else
			player->getPhysicalProperties()->setVelocityX(15);
		player->isJumping = true;
		player->isAttacking = false;

		if(ai->aiHealth>0){
			ai->invincible=50;
            ai->setAlpha(150);
			game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().AI_HURT);

		}
		else{
			if(player->isFacingRight)
			{
				ai->setCurrentState(DIE_LEFT);
			}
			else
			{
				ai->setCurrentState(DIE_RIGHT);
			}
			ai->setCurrentlyCollidable(false);
			ai->getPhysicalProperties()->setVelocityY(-35);
			if(player->isFacingRight)
		 	   ai->getPhysicalProperties()->setVelocityX(15);
		    else
			   ai->getPhysicalProperties()->setVelocityX(-15);
			ai->isAlive=false;
			ai->deadLock=100;
			game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().AI_DIE);
		}
		
	}
	
	else if(player->invincible>0 || player->isAlive==false || ai->isAlive==false || !ai->isEnabled
		   ||ai->invincible>0 ){
		// do nothing
	}
	else{
		//player hurt
		AnimatedSprite::setPlayerHealth(AnimatedSprite::getPlayerHealth()-1);
		if(AnimatedSprite::getPlayerHealth()>0){
              
			  player->invincible=100;
			  player->setAlpha(150);
			  player->isHurt=true;
			  player->HurtLock=30;
			  player->getPhysicalProperties()->setVelocityX(0);
			  if(player->isFacingRight&&player->playerNumber==1){
				  player->setCurrentState(WHITE_HURTING_FACING_RIGHT);
			  }
			  else if(!player->isFacingRight&&player->playerNumber==1){
				  player->setCurrentState(WHITE_HURTING_FACING_LEFT);
			  }
			  if(player->isFacingRight&&player->playerNumber==2){
				  player->setCurrentState(BLACK_HURTING_FACING_RIGHT);
			  }
			  else if(!player->isFacingRight&&player->playerNumber==2){
				  player->setCurrentState(BLACK_HURTING_FACING_RIGHT);
			  }

			  game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_HURT);
		   }
		
	else{
		
		player->getPhysicalProperties()->setVelocityX(0);
		 if(player->isFacingRight&&player->playerNumber==1){
				  player->setCurrentState(WHITE_DYING_RIGHT);
			  }
		else if(!player->isFacingRight&&player->playerNumber==1){
				  player->setCurrentState(WHITE_DYING_LEFT);
			  }
		if(player->isFacingRight&&player->playerNumber==2){
				  player->setCurrentState(BLACK_DYING_RIGHT);
			  }
		else if(!player->isFacingRight&&player->playerNumber==2){
				  player->setCurrentState(BLACK_DYING_LEFT);
			  }
		player->isAlive=false;

		game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_DIE);
		game->gameOver=150;
	   }
	   
	}
	
}
//solving floor vs sprite collision
void Collision::floorVSsprite(Game *game){
	AnimatedSprite* player = (AnimatedSprite*) obj2;		
	obj1->getBoundingVolume()->incX(obj1->getPhysicalProperties()->getVelocityX()*CoTime);
	obj1->getBoundingVolume()->incY(obj1->getPhysicalProperties()->getVelocityY()*CoTime);	

	obj2->getBoundingVolume()->incX(obj2->getPhysicalProperties()->getVelocityX()*CoTime);
	obj2->getBoundingVolume()->incY(obj2->getPhysicalProperties()->getVelocityY()*CoTime);
	if(player->playerNumber==1 || player->playerNumber==2)
		{
				if(player->isGrabbed==true){
					player->isGrabbed=false;					
					if(player->playerNumber==1){
						game->getWorld()->getSpriteManager()->getPlayer2()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer2()->isStanding=true;
					}
					else{
						game->getWorld()->getSpriteManager()->getPlayer1()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer1()->isStanding=true;
					}
				}
				if(player->isAttacking){
					player->isAttacking=false;
					player->isJumping=true;
				}
		}
				
	obj2->getBoundingVolume()->setY
		    (obj1->getBoundingVolume()->getY()- obj2->getBoundingVolume()->getHeight()-1);
	obj2->getPhysicalProperties()->setY
			(obj2->getBoundingVolume()->getY()-obj2->getBoundingVolume()->getOffsetY());
    obj2->getPhysicalProperties()->setVelocityY(0);
				
	obj1->getPhysicalProperties()->setVelocityY(0);				
	player->isStanding = true;
	if(player->isJumping==true){
		player->isJumping = false;
		obj2->getPhysicalProperties()->setVelocityX(0);
	}


}
//solving celing vs sprite
void Collision::ceilingVSsprite(Game *game){
	AnimatedSprite* player = (AnimatedSprite*) obj2;		
	obj1->getBoundingVolume()->incX(obj1->getPhysicalProperties()->getVelocityX()*CoTime);
	obj1->getBoundingVolume()->incY(obj1->getPhysicalProperties()->getVelocityY()*CoTime);	

	obj2->getBoundingVolume()->incX(obj2->getPhysicalProperties()->getVelocityX()*CoTime);
	obj2->getBoundingVolume()->incY(obj2->getPhysicalProperties()->getVelocityY()*CoTime);
	if(player->playerNumber==1 || player->playerNumber==2)
		{
				if(player->isGrabbed==true){
					player->isGrabbed=false;					
					if(player->playerNumber==1){
						game->getWorld()->getSpriteManager()->getPlayer2()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer2()->isStanding=true;
					}
					else{
						game->getWorld()->getSpriteManager()->getPlayer1()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer1()->isStanding=true;
					}
				}
				if(player->isAttacking){
					player->isAttacking=false;
					player->isJumping=true;
				}
		}
	obj2->getBoundingVolume()->setY
					(obj1->getBoundingVolume()->getY()+ obj1->getBoundingVolume()->getHeight()+1);
	obj2->getPhysicalProperties()->setY
					(obj2->getBoundingVolume()->getY()-obj2->getBoundingVolume()->getOffsetY());
	obj2->getPhysicalProperties()->setVelocityY(0);
	obj1->getPhysicalProperties()->setVelocityY(0);

}
//solving wall left vs sprite
void Collision::walllVSSprite(Game *game){
	AnimatedSprite* player = (AnimatedSprite*) obj2;		
	obj1->getBoundingVolume()->incX(obj1->getPhysicalProperties()->getVelocityX()*CoTime);
	obj1->getBoundingVolume()->incY(obj1->getPhysicalProperties()->getVelocityY()*CoTime);	

	obj2->getBoundingVolume()->incX(obj2->getPhysicalProperties()->getVelocityX()*CoTime);
	obj2->getBoundingVolume()->incY(obj2->getPhysicalProperties()->getVelocityY()*CoTime);
	if(player->playerNumber==1 || player->playerNumber==2)
		{
				if(player->isGrabbed==true){
					player->isGrabbed=false;					
					if(player->playerNumber==1){
						game->getWorld()->getSpriteManager()->getPlayer2()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer2()->isStanding=true;
					}
					else{
						game->getWorld()->getSpriteManager()->getPlayer1()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer1()->isStanding=true;
					}
				}
				if(player->isAttacking){
					player->isAttacking=false;
					player->isJumping=true;
				}
		}
	obj2->getBoundingVolume()->setX
					(obj1->getBoundingVolume()->getX() - obj2->getBoundingVolume()->getWidth()-1);
	obj2->getPhysicalProperties()->setX
					(obj2->getBoundingVolume()->getX()-obj2->getBoundingVolume()->getOffsetX());				

	obj2->getPhysicalProperties()->setVelocityX(0);
	obj1->getPhysicalProperties()->setVelocityX(0);

}
//solving wall right vs sprite
void Collision::wallrVSSprite(Game *game){
	AnimatedSprite* player = (AnimatedSprite*) obj2;		
	obj1->getBoundingVolume()->incX(obj1->getPhysicalProperties()->getVelocityX()*CoTime);
	obj1->getBoundingVolume()->incY(obj1->getPhysicalProperties()->getVelocityY()*CoTime);	

	obj2->getBoundingVolume()->incX(obj2->getPhysicalProperties()->getVelocityX()*CoTime);
	obj2->getBoundingVolume()->incY(obj2->getPhysicalProperties()->getVelocityY()*CoTime);
	if(player->playerNumber==1 || player->playerNumber==2)
		{
				if(player->isGrabbed==true){
					player->isGrabbed=false;					
					if(player->playerNumber==1){
						game->getWorld()->getSpriteManager()->getPlayer2()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer2()->isStanding=true;
					}
					else{
						game->getWorld()->getSpriteManager()->getPlayer1()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer1()->isStanding=true;
					}
				}
				if(player->isAttacking){
					player->isAttacking=false;
					player->isJumping=true;
				}
		}
	obj2->getBoundingVolume()->setX
					(obj1->getBoundingVolume()->getX()+obj1->getBoundingVolume()->getWidth()+1);
	obj2->getPhysicalProperties()->setX
					(obj2->getBoundingVolume()->getX()-obj2->getBoundingVolume()->getOffsetX());
				
	obj2->getPhysicalProperties()->setVelocityX(0);
	obj1->getPhysicalProperties()->setVelocityX(0);
		        

}
//solving tile vs sprite collision
void Collision::tileVSsprite(Game *game){
		AnimatedSprite* player = (AnimatedSprite*) obj2;		
	    obj1->getBoundingVolume()->incX(obj1->getPhysicalProperties()->getVelocityX()*CoTime);
		obj1->getBoundingVolume()->incY(obj1->getPhysicalProperties()->getVelocityY()*CoTime);	

		obj2->getBoundingVolume()->incX(obj2->getPhysicalProperties()->getVelocityX()*CoTime);
		obj2->getBoundingVolume()->incY(obj2->getPhysicalProperties()->getVelocityY()*CoTime);

		if(Xcolliding){
			if(obj2==left){
				obj2->getBoundingVolume()->setX
					(obj1->getBoundingVolume()->getX() - obj2->getBoundingVolume()->getWidth()-1);
				obj2->getPhysicalProperties()->setX
					(obj2->getBoundingVolume()->getX()-obj2->getBoundingVolume()->getOffsetX());				

				obj2->getPhysicalProperties()->setVelocityX(0);
				obj1->getPhysicalProperties()->setVelocityX(0);
			}
			else{
				obj2->getBoundingVolume()->setX
					(obj1->getBoundingVolume()->getX()+obj1->getBoundingVolume()->getWidth()+1);
				obj2->getPhysicalProperties()->setX
					(obj2->getBoundingVolume()->getX()-obj2->getBoundingVolume()->getOffsetX());
				
				
				obj2->getPhysicalProperties()->setVelocityX(0);
				obj1->getPhysicalProperties()->setVelocityX(0);
		        }
		}
		else{
			if(obj2 == top){
				obj2->getBoundingVolume()->setY
					(obj1->getBoundingVolume()->getY()- obj2->getBoundingVolume()->getHeight()-1);
				obj2->getPhysicalProperties()->setY
					(obj2->getBoundingVolume()->getY()-obj2->getBoundingVolume()->getOffsetY());
				obj2->getPhysicalProperties()->setVelocityY(0);
				
				obj1->getPhysicalProperties()->setVelocityY(0);				
				player->isStanding = true;
				if(player->isJumping==true){
				   player->isJumping = false;
				   obj2->getPhysicalProperties()->setVelocityX(0);
				}
				
			}
			else{
				obj2->getBoundingVolume()->setY
					(obj1->getBoundingVolume()->getY()+ obj1->getBoundingVolume()->getHeight()+1);
				obj2->getPhysicalProperties()->setY
					(obj2->getBoundingVolume()->getY()-obj2->getBoundingVolume()->getOffsetY());
				obj2->getPhysicalProperties()->setVelocityY(0);
				obj1->getPhysicalProperties()->setVelocityY(0);

			}
		}		
		if(player->playerNumber==1 || player->playerNumber==2)
		{
				if(player->isGrabbed==true){
					player->isGrabbed=false;					
					if(player->playerNumber==1){
						game->getWorld()->getSpriteManager()->getPlayer2()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer2()->isStanding=true;
					}
					else{
						game->getWorld()->getSpriteManager()->getPlayer1()->isGrabbing=false;
						game->getWorld()->getSpriteManager()->getPlayer1()->isStanding=true;
					}
				}
				if(player->isAttacking){
					player->isAttacking=false;
					player->isJumping=true;
				}
		}

}

//player1 vs player2 for grabbing
void Collision::player1VSplayer2(){
	//this is one time collision
	checked = true;
	AnimatedSprite* player1 = (AnimatedSprite*) obj1;
	AnimatedSprite* player2 = (AnimatedSprite*) obj2;

	if(player1->getCurrentState()==WHITE_GRABBED_STANDING_LEFT
		||player1->getCurrentState()==WHITE_GRABBED_STANDING_RIGHT
		||player1->getCurrentState()==WHITE_GRABBED_RUNNING_LEFT
		||player1->getCurrentState()==WHITE_GRABBED_RUNNING_RIGHT){
			//set position of player being grabbed
			player2->getBoundingVolume()->setX(player1->getBoundingVolume()->getX());
			player2->getBoundingVolume()->setY(player1->getBoundingVolume()->getY()
				- player2->getBoundingVolume()->getHeight());
			//correct the image
			player2->getPhysicalProperties()->setX
				(player2->getBoundingVolume()->getX() - player2->getBoundingVolume()->getOffsetX());
			player2->getPhysicalProperties()->setY
				(player2->getBoundingVolume()->getY() - player2->getBoundingVolume()->getOffsetY());
			//correct the velocity
			player2->getPhysicalProperties()->setVelocityY(0);
			player1->isGrabbing=true;
			player2->isGrabbed=true;
		    //correct states;
			if(player1->isFacingRight){
				player2->setCurrentState(BLACK_GRABBED_FACING_RIGHT);
			    player2->isFacingRight=true;}
			else{
				player2->setCurrentState(BLACK_GRABBED_FACING_LEFT);
			    player2->isFacingRight=false;}


	}

	else if(player2->getCurrentState()==BLACK_GRABBED_STANDING_LEFT
		||player2->getCurrentState()==BLACK_GRABBED_STANDING_RIGHT
		||player2->getCurrentState()==BLACK_GRABBED_RUNNING_LEFT
		||player2->getCurrentState()==BLACK_GRABBED_RUNNING_RIGHT){
			//set position of player being grabbed
			player1->getBoundingVolume()->setX(player2->getBoundingVolume()->getX());
			player1->getBoundingVolume()->setY(player2->getBoundingVolume()->getY()
				- player1->getBoundingVolume()->getHeight());
			//correct the image
			player1->getPhysicalProperties()->setX
				(player1->getBoundingVolume()->getX() - player1->getBoundingVolume()->getOffsetX());
			player1->getPhysicalProperties()->setY
				(player1->getBoundingVolume()->getY() - player1->getBoundingVolume()->getOffsetY());
			//correct the velocity
			player1->getPhysicalProperties()->setVelocityY(0);
		    //correct states;
			player2->isGrabbing=true;
			player1->isGrabbed=true;

			if(player2->isFacingRight){
				player1->setCurrentState(WHITE_GRABBED_FACING_RIGHT);
				player1->isFacingRight=true;
			   }
			else{
				player1->setCurrentState(WHITE_GRABBED_FACING_LEFT);
			    player1->isFacingRight=false;}

	}


}



	