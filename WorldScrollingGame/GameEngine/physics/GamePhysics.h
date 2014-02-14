/*	
	Author: ____Team Little Ninja___________

	GamePhysics.h

	This class will be used to manage all game collisions
	and physics. This includes collision detection, and resolution.
	Each frame we will update the collided objects velocities and
	positions accordingly.

	Students will implement this as part of their projects.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Game.h"
#include "PhysicalProperties.h"
#include "CollidableObject.h"
#include "Collision.h"
#include "BoundingVolume.h"
#include "AnimatedSprite.h"

const int Wplayer = 0;
const int Bplayer2 = 1;
const int Bhazard = 2;
const int Whazard = 3;
const int Ctile = 4;
const int AI_TypeA =5;
const int Key_Event =6;
const int Floor = 7;
const int Ceiling =8;
const int WallL =9;
const int WallR=10;
const int SpotLight = 11;
const int FireBall=12;

class GamePhysics
{
public:
	// WE'LL DEFINE THIS CLASS LATER
	//pre-constructed dummy collisons. Fill in with tiles later
	vector<Collision*> *TileCollision;
	//pointer of how many collisions in TileCollision are used
	int PointerA;
	//real collisions that need to be solved
	vector<Collision*> *RealCollision;
	//pre-constructed dynamic collisions. data already filled in.
	vector<Collision*> *Dynamic;
	
	vector<AnimatedSprite*> *AISprite;

    void addAISprite(AnimatedSprite* bot);
	void addCollision(Collision* co);
	GamePhysics();
	~GamePhysics();
	void update(Game *game);

	
private:
	void findCollision(Game *game);
	void initCollision();
};