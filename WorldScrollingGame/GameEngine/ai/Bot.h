
#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "resource.h"
#include "stdafx.h"
#include "AnimatedSprite.h"
#include "Game.h"

#define DIE_RIGHT        0
#define DIE_LEFT         1
#define RUN_RIGHT        2
#define RUN_LEFT         3
#define ATTACKL1	     4
#define ATTACKL2	     5
#define ATTACKR1	     6
#define ATTACKR2	     7
#define RUN_RIGHT2       8
#define RUN_LEFT2        9


#define typeA 1          //pattern patrol;
#define typeB 2
#define typeC 3
#define typeD 4
#define typeE 5

class Bot
{
private:
	AnimatedSprite *animatedSprite;	
	vector<int> *pattern;
	float density;
	//help method to perform different AI;
	//type A: pattern: patrol
	void updateA();
	void updateB();
	void updateC();
	void updateD(Game *game);
	void updateE(Game *game);
	int type;
	
	

public:
	// INLINED METHODS
	boolean used;
	boolean enabled;
	int frameLeft;
	int index;
	int currentState;
	AnimatedSprite* getAnimatedSprite() { return animatedSprite; }
	void setAnimatedSprite(AnimatedSprite *as){animatedSprite=as;}
    
	float getDensity() { return density; }
	void setDensity(float initDensity) { density = initDensity; }
	vector<int>* getPattern (){return pattern;}
	void setType(int t){type = t;}
	int returnType(){ return type;}

	Bot();
	~Bot();
	void update(Game *game);
};