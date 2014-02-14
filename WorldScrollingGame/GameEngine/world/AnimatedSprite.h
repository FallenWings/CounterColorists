/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.h

	This class represents a sprite that can can
	be used to animate a game character or object.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "AnimatedSpriteType.h"
#include "BoundingVolume.h"
#include "CollidableObject.h"
#include "PhysicalProperties.h"
#include "Viewport.h"
#include "CXBOXController.h"
#include <vector>

// WE COULD KEEP TRACK OF ANIMATION STATES
// USING CONSTANTS IF WE LIKE
const int WHITE_STANDING_FACING_LEFT = 0;
const int WHITE_STANDING_FACING_RIGHT = 1;
const int WHITE_RUNNING_FACING_LEFT = 2;
const int WHITE_RUNNING_FACING_RIGHT = 3;
const int WHITE_JUMPING_FACING_LEFT = 4;
const int WHITE_JUMPING_FACING_RIGHT = 5;
const int WHITE_HURTING_FACING_LEFT = 6;
const int WHITE_HURTING_FACING_RIGHT = 7;
const int WHITE_GRABBED_FACING_LEFT = 8;
const int WHITE_GRABBED_FACING_RIGHT = 9;

const int WHITE_GRABBED_STANDING_LEFT = 10;
const int WHITE_GRABBED_STANDING_RIGHT = 11;
const int WHITE_GRABBED_RUNNING_LEFT = 12;
const int WHITE_GRABBED_RUNNING_RIGHT = 13;

const int WHITE_DYING_LEFT = 14;
const int WHITE_DYING_RIGHT = 15;
const int WHITE_ATTACKING = 16;
const int WHITE_THROWING_STANDING_LEFT = 17;
const int WHITE_THROWING_STANDING_RIGHT = 18;
const int BLACK_STANDING_FACING_LEFT = 19;
const int BLACK_STANDING_FACING_RIGHT = 20;
const int BLACK_RUNNING_FACING_LEFT = 21;
const int BLACK_RUNNING_FACING_RIGHT = 22;
const int BLACK_JUMPING_FACING_LEFT = 23;
const int BLACK_JUMPING_FACING_RIGHT = 24;
const int BLACK_HURTING_FACING_LEFT = 25;
const int BLACK_HURTING_FACING_RIGHT = 26;
const int BLACK_GRABBED_FACING_LEFT = 27;
const int BLACK_GRABBED_FACING_RIGHT = 28;

const int BLACK_GRABBED_STANDING_LEFT = 29;
const int BLACK_GRABBED_STANDING_RIGHT = 30;
const int BLACK_GRABBED_RUNNING_LEFT = 31;
const int BLACK_GRABBED_RUNNING_RIGHT = 32;

const int BLACK_DYING_LEFT = 33;
const int BLACK_DYING_RIGHT = 34;
const int BLACK_ATTACKING = 35;
const int BLACK_THROWING_STANDING_LEFT = 36;
const int BLACK_THROWING_STANDING_RIGHT = 37;



class AnimatedSprite : public CollidableObject
{
private:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;	

	// TRANSPARENCY/OPACITY
	int alpha;

	// ANIMATION SEQUENCE CURRENTLY IN USE
	int currentState;

	// LAST STATE
	int lastState;

	// FRAME OF ANIMATION CURRENTLY BEING USED FOR currentState
	int currentFrame;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	int animationCounter;

	// INSTANCE OF CONTROLLER OBJECT
	CXBOXController* playerController;

public:
	// SPRITE VARIABLES
	bool isAlive;	
	bool isJumping;
	bool isStanding;
	bool isGrabbing;
	bool isGrabbed;
	bool isFacingRight;
	bool isAttacking;
	bool isRunning;
	bool isHurt;
	bool isThrowing;
	bool isEnabled;
	int color;
	int playerNumber;
	int aiHealth;
	int invincible;
	int HurtLock;
	int GrabLock;
	int throwLock;
	int deadLock;
	
	static int playerHealth;
	int eventIndex;
	

	// INLINED ACCESSOR METHODS
	int					getAlpha()			{ return alpha;			}
	int					getCurrentFrame()	{ return currentFrame;	}
	int					getCurrentState()	{ return currentState;	}
	int					getFrameIndex()		{ return frameIndex;	}
	AnimatedSpriteType*	getSpriteType()		{ return spriteType; }
	CXBOXController*	getPlayerController() { return playerController; }

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;				}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;	}
	void setPlayerController(CXBOXController *initPlayerController)
	{	playerController = initPlayerController;	}

	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame();
	void setCurrentState(int newState);
	void updateSprite();
	static void setPlayerHealth(int t){AnimatedSprite::playerHealth =t;}
	static int getPlayerHealth(){return AnimatedSprite::playerHealth;}
	
};
