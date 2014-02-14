/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.cpp

	See AnimatedSprite.h for a class description.
*/

#include "stdafx.h"
#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "PhysicalProperties.h"

/*
	AnimatedSprite - Default constructor, just sets everything to 0.
*/
int AnimatedSprite::playerHealth=0;
AnimatedSprite::AnimatedSprite()  
{
	spriteType = 0;
	currentState = 0;
	currentFrame = 0;
	frameIndex = 0;
	animationCounter = 0; 
	playerNumber=0;
	color =0;
	invincible=0;
	HurtLock=0;
	GrabLock=0;
	throwLock=0;
	deadLock=0;
	
	
    isEnabled=true;
	isAlive = true;
	isJumping = false;
	isStanding = false;
	isGrabbing = false;
	isRunning = false;	
	isAttacking=false;
	isGrabbed=false;
	isHurt=false;
	isThrowing=false;
}

/*
	Destructor - Nothing to clean up. We don't want to clean up the
	sprite type because it is a shared variable.
*/
AnimatedSprite::~AnimatedSprite() 
{
	delete bv;
	delete pp;
	
	if(playerController!=NULL){
		delete playerController;}

}

/*
	changeFrame - This method allows for the changing of an image in an 
	animation sequence for a given animation state.
*/
void AnimatedSprite::changeFrame()
{	
	currentFrame = spriteType->getAnimationFrameID(currentState, frameIndex);
	frameIndex++;
	if(!isAlive){
		if(frameIndex>=spriteType->getSequenceSize(currentState)){
			frameIndex=spriteType->getSequenceSize(currentState)-1;
		}
	}
	else
	  frameIndex = frameIndex % spriteType->getSequenceSize(currentState);	
	
}

void AnimatedSprite::setCurrentState(int newState) 
{
	if(!isAlive)
	{
		//do nothing if is dead
	}
	else if (newState != currentState)
	{
		lastState = currentState;
		currentState = newState;
	    frameIndex = 0;
	}
	

	

	// UPDATE WHERE YOU ARE FACING
	
}

/*
	updateSprite - To be called every frame of animation. This
    method advances the animation counter appropriately per
	the animation speed. It also updates the sprite location
	per the current velocity.
*/
void AnimatedSprite::updateSprite()
{
	animationCounter++;
	animationCounter = animationCounter%spriteType->getAnimationSpeed();

	// WE ONLY CHANGE THE FRAME OF ANIMATION ONCE EVERY animationSpeed FRAMES
	if (animationCounter == 0)
	{
		changeFrame();
	}
	//invinciblity
	if(invincible>0){
		invincible--;
		if(invincible==0){
		this->setAlpha(255);
	   }
	}
	
	//hurt freeze for some frames
	if(HurtLock>0){
		HurtLock--;
		if(HurtLock==0){
		   isHurt=false;
	       if(isFacingRight&&playerNumber==1){
				  setCurrentState(WHITE_STANDING_FACING_RIGHT);
			  }
			  else if(!isFacingRight&&playerNumber==1){
				  setCurrentState(WHITE_STANDING_FACING_LEFT);
			  }
			  if(isFacingRight&&playerNumber==2){
				 setCurrentState(BLACK_STANDING_FACING_RIGHT);
			  }
			  else if(!isFacingRight&&playerNumber==2){
				  setCurrentState(BLACK_STANDING_FACING_LEFT);
			  }
		
	   }
	}

}

