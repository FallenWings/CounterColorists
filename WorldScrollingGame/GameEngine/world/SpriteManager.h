/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.h

	This class manages all of the sprites in a given game level. Note
	that the player sprite is also managed by this class.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "GameGraphics.h"
#include <vector>

class SpriteManager
{
private:
	vector<AnimatedSpriteType*> *spriteTypes;
	vector<AnimatedSprite*> *sprites;
	AnimatedSprite *player1;
	AnimatedSprite *player2;

public:
	// INLINED ACCESSOR METHODS
	int				getNumberOfSprites()	{ return sprites->size();	}
	AnimatedSprite* getPlayer1()			{ return player1;			}
	AnimatedSprite* getPlayer2()			{ return player2;			}


	// INLINE ACCESSOR METHODS
	void setPlayer1(AnimatedSprite *initPlayer)
	{	player1 = initPlayer;				}
	void setPlayer2(AnimatedSprite *initPlayer)
	{	player2 = initPlayer;				}
	void setSprite(AnimatedSprite *initSprite, int spriteID)
	{	sprites->at(spriteID) = initSprite;	}

	// METHODS DEFINED IN SpriteManager.cpp
	SpriteManager();
	~SpriteManager();
	void				addSpriteItemsToRenderList(RenderList *renderList, Viewport *viewport);
	void				addSprite(AnimatedSprite *spriteToAdd);
	void				addSpriteType(AnimatedSpriteType *spriteTypeToAdd);
	void				addSpriteToRenderList(AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	void				clearSprites();
	AnimatedSprite*		getSprite(int spriteID);
	AnimatedSpriteType* getSpriteType(int typeIndex);
	void				updateAllSprites();
};