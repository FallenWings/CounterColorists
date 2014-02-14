/*	
	Author: ___Chun Li____________

	Collision.h

	This is the pair of objects that collide.

	Students will implement this as part of their projects.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Game.h"



class Collision{
public:
	Collision()	{}
	~Collision()	{}
    float CoTime;
	//one time collisions will be set to true if the collision is solved once already
	bool checked;

	void setObj1(CollidableObject *obj){obj1=obj;}
	void setObj2(CollidableObject *obj){obj2=obj;}
	CollidableObject* getObj1(){return obj1;}
	CollidableObject* getObj2(){return obj2;}   

	boolean checkCollision();	
	void solveCollision(Game *game);
private:
	CollidableObject *obj1;
	CollidableObject *obj2;
	float XstartTime;
	float XendTime;
	float YstartTime;
	float YendTime;
	float totalVelocityX;
	float totalVelocityY;
	float distanceX;
	float distanceY;	
	
	//testing if two object are actually colliding in both x,y direction
	
	boolean isColliding;
    //check which direction it actually collides
	boolean Xcolliding;
    //check the position accordingly.
	CollidableObject *top;
	CollidableObject *left;
	CollidableObject *bottom;
	CollidableObject *right;
    //help methods to calculat the time of collisions;
	float calculateStartTimeOfCollisionForAnyAxis(float pos1, float pos2, float v1, float v2, float length1, float length2);
	float calculateTimeOfFirstAxisCollision(float pos1, float pos2, float v1, float v2, float length1, float length2);

	float calculateEndTimeOfCollisionForAnyAxis(float pos1, float pos2, float v1, float v2, float length1, float length2);
	float calculateTimeOfLastAxisCollision(float pos1, float pos2, float v1, float v2, float length1, float length2);
	//help method to slove different collisions types
	void tileVSsprite(Game *game);

	void floorVSsprite(Game *game);
	void ceilingVSsprite(Game *game);
	void wallrVSSprite(Game *game);
	void walllVSSprite(Game *game);

	void player1VSplayer2();
	void playerVSai(Game *game);
	void hazardVSplayer(Game *game);
	void keyVSplayer(Game *game);
	void spotlightVSplayer(Game *game);
	void fireballVSplayer(Game *game);
	
};
