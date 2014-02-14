/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRKeyEventHandler.cpp

	See WRKeyEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "Cursor.h"
#include "WRKeyEventHandler.h"
#include "Game.h"
#include "GameInput.h"
#include "GameGraphics.h"
#include "GameGUI.h"
#include "GameTimer.h"
#include "GameWorld.h"
#include "StringTable.h"
#include "WindowsGameTimer.h"
#include "SpriteManager.h"
#include "AnimatedSprite.h"

#include "XACT.h"

const int A_KEY = 0X41;
const int B_KEY = 0X42;
const int C_KEY = 0X43;
const int D_KEY = 0X44;


const int PLAYER_1_GAMEPAD_DPAD_UP = 0;
const int PLAYER_1_GAMEPAD_DPAD_DOWN = 1;
const int PLAYER_1_GAMEPAD_DPAD_LEFT = 2;
const int PLAYER_1_GAMEPAD_DPAD_RIGHT = 3;
const int PLAYER_1_GAMEPAD_START = 4;
const int PLAYER_1_GAMEPAD_BACK = 5;
const int PLAYER_1_GAMEPAD_LEFT_THUMB = 6;
const int PLAYER_1_GAMEPAD_RIGHT_THUMB  = 7;
const int PLAYER_1_GAMEPAD_LEFT_SHOULDER = 8;
const int PLAYER_1_GAMEPAD_RIGHT_SHOULDER = 9;
const int PLAYER_1_GAMEPAD_A = 10;
const int PLAYER_1_GAMEPAD_B = 11;
const int PLAYER_1_GAMEPAD_X = 12;
const int PLAYER_1_GAMEPAD_Y = 13;
const int PLAYER_2_GAMEPAD_DPAD_UP = 14;
const int PLAYER_2_GAMEPAD_DPAD_DOWN = 15;
const int PLAYER_2_GAMEPAD_DPAD_LEFT = 16;
const int PLAYER_2_GAMEPAD_DPAD_RIGHT = 17;
const int PLAYER_2_GAMEPAD_START = 18;
const int PLAYER_2_GAMEPAD_BACK = 19;
const int PLAYER_2_GAMEPAD_LEFT_THUMB = 20;
const int PLAYER_2_GAMEPAD_RIGHT_THUMB  = 21;
const int PLAYER_2_GAMEPAD_LEFT_SHOULDER = 22;
const int PLAYER_2_GAMEPAD_RIGHT_SHOULDER = 23;
const int PLAYER_2_GAMEPAD_A = 24;
const int PLAYER_2_GAMEPAD_B = 25;
const int PLAYER_2_GAMEPAD_X = 26;
const int PLAYER_2_GAMEPAD_Y = 27;


void WRKeyEventHandler::handleKeyEvents(Game *game)
{
	wchar_t *title;
	wchar_t *counterText;
	GameInput *input = game->getInput();
	GameGraphics *graphics = game->getGraphics();
	AnimatedSprite *player1 = game->getWorld()->getSpriteManager()->getPlayer1();
	AnimatedSprite *player2 = game->getWorld()->getSpriteManager()->getPlayer2();

	unsigned int oneKey = (unsigned int)'1';
	unsigned int twoKey = (unsigned int)'2';
	unsigned int threeKey = (unsigned int)'3';
	unsigned int plusKey = (unsigned int)'0';

	if (game->getGameState() == GAME_PLAYING_GAME_STATE)
	{
		unsigned int aKey = (unsigned int)'A';
		unsigned int bKey = (unsigned int)'B';
		unsigned int dKey = (unsigned int)'D';
		unsigned int sKey = (unsigned int)'S';
		unsigned int wKey = (unsigned int)'W';
		unsigned int tKey = (unsigned int)'T';
		unsigned int xKey = (unsigned int)'X';
		unsigned int yKey = (unsigned int)'Y';
		unsigned int jKey = (unsigned int)'J';
		unsigned int lKey = (unsigned int)'L';
		unsigned int leftKey = 37;
		unsigned int rightKey = 39;
		unsigned int upKey = 38;
		unsigned int downKey = 40;
		unsigned int braketLeft = 219;
		unsigned int braketRight = 221;
		
		

		int incX = 0;
		int incY = 0;
		bool moveViewport = false;
        
		if(!player1->isAttacking){
			player1->isStanding = true;
			player1->isRunning = false;
		}
		else{
			player1->isStanding = false;
			player1->isRunning = false;
		}

		if(!player2->isAttacking){
			player2->isStanding = true;		
			player2->isRunning = false;
		}
		else{
			player2->isStanding = false;
			player2->isRunning = false;
		}
		
		if(player1->getCurrentState() != WHITE_DYING_LEFT && player1->getCurrentState() != WHITE_DYING_RIGHT &&
			player1->getCurrentState() != WHITE_HURTING_FACING_LEFT && player1->getCurrentState() != WHITE_HURTING_FACING_RIGHT &&
			player1->getCurrentState() !=  WHITE_ATTACKING)
		{
			player1->getPlayerController()->Vibrate(0,0);
		}
		
		if(player2->getCurrentState() != BLACK_DYING_LEFT && player2->getCurrentState() != BLACK_DYING_RIGHT &&
			player2->getCurrentState() != BLACK_HURTING_FACING_LEFT && player2->getCurrentState() != BLACK_HURTING_FACING_RIGHT &&
			player2->getCurrentState() !=  BLACK_ATTACKING)
		{
			player2->getPlayerController()->Vibrate(0,0);
		}
		

		if ((input->isKeyDown(aKey) || input->isGamepadDown(PLAYER_1_GAMEPAD_DPAD_LEFT)) && player1->isAlive&&!player1->isHurt  && !player1->isAttacking &&!player1->isGrabbed)
		{
			player1->isStanding = false;
			player1->isRunning=true;
			player1->isFacingRight=false;
			player1->isAttacking=false;
			player1->getPhysicalProperties()->setVelocityX(-5);
		}

		if ((input->isKeyDown(dKey) || input->isGamepadDown(PLAYER_1_GAMEPAD_DPAD_RIGHT)) && player1->isAlive&& !player1->isHurt && !player1->isAttacking&&!player1->isGrabbed)
		{
			player1->isStanding = false;
			player1->isRunning=true;
			player1->isFacingRight=true;
			player1->isAttacking=false;
			player1->getPhysicalProperties()->setVelocityX(5);
		}

		if ((input->isKeyDownForFirstTime(tKey) || input->isGamepadDownForFirstTime(PLAYER_1_GAMEPAD_A)) && player1->isAlive && !player1->isJumping&& !player1->isHurt && !player1->isAttacking&&!player1->isGrabbed)
		{
			player1->isStanding = false;
			player1->isRunning=false;			
			player1->isJumping = true;
			player1->isAttacking=false;
			player1->isGrabbing=false;
			player1->getPhysicalProperties()->setVelocityY(-31);

			game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_JUMP);

			player2->isGrabbed=false;
			
		}
		if ((input->isKeyDownForFirstTime(yKey) || input->isGamepadDownForFirstTime(PLAYER_1_GAMEPAD_X)) && player1->isAlive && !player1->isHurt&&!player1->isGrabbed&& !player1->isJumping &&!player1->isThrowing)
		{	
			//attack
			if(player1->isGrabbing&&player2->isGrabbed){
				player1->isGrabbing=false;
				player2->isGrabbed=false;

				player2->isAttacking=true;
				player2->isStanding=false;
				player2->isRunning=false;

				if(player1->isFacingRight){
					if(player1->isRunning){
						player2->getPhysicalProperties()->setVelocityX(27);
						player2->getPhysicalProperties()->setVelocityY(-5);
					}
					else{
						player2->getPhysicalProperties()->setVelocityX(5);
						player2->getPhysicalProperties()->setVelocityY(-37);
					}
				}
				else{
					if(player1->isRunning){
						player2->getPhysicalProperties()->setVelocityX(-27);
						player2->getPhysicalProperties()->setVelocityY(-5);
					}
					else{
						player2->getPhysicalProperties()->setVelocityX(-5);
						player2->getPhysicalProperties()->setVelocityY(-37);
					}
				}
				player1->isThrowing=true;
				player1->throwLock=5;
				game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_GRAB);
				
			}		
			//grab
			else{
				player1->isGrabbing=true;
				player1->GrabLock=5;

				game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_GRAB);
			}		
							
		}
		//player2

		if ((input->isKeyDown(leftKey) || input->isGamepadDown(PLAYER_2_GAMEPAD_DPAD_LEFT)) && player2->isAlive && !player2->isHurt && !player2->isAttacking &&!player2->isGrabbed)
		{
			player2->isStanding = false;
			player2->isRunning=true;
			player2->isFacingRight=false;
			player2->isAttacking=false;
			player2->getPhysicalProperties()->setVelocityX(-5);			
			
		}

		if ((input->isKeyDown(rightKey) || input->isGamepadDown(PLAYER_2_GAMEPAD_DPAD_RIGHT)) && player2->isAlive && !player2->isHurt && !player2->isAttacking&&!player2->isGrabbed)
		{			
			player2->isStanding = false;
			player2->isRunning=true;
			player2->isFacingRight=true;
			player2->isAttacking=false;
			player2->getPhysicalProperties()->setVelocityX(5);
		}

		if ((input->isKeyDownForFirstTime(braketLeft)|| input->isGamepadDownForFirstTime(PLAYER_2_GAMEPAD_A)) && player2->isAlive && !player2->isJumping &&!player2->isHurt && !player2->isAttacking&&!player2->isGrabbed)
		{
			player2->isStanding = false;
			player2->isRunning=false;			
			player2->isJumping = true;
			player2->isGrabbing=false;
			player2->isAttacking=false;
			player2->getPhysicalProperties()->setVelocityY(-31);

			game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_JUMP);

			player1->isGrabbed=false;
			
			
		}

		if ((input->isKeyDownForFirstTime(braketRight) || input->isGamepadDownForFirstTime(PLAYER_2_GAMEPAD_X)) && player2->isAlive && !player2->isJumping &&!player2->isHurt&& !player2->isJumping&&!player2->isGrabbed&&!player1->isThrowing)
		{		
			//attack
			
			if(player2->isGrabbing&&player1->isGrabbed){
				player2->isGrabbing=false;
				player1->isGrabbed=false;

				player1->isAttacking=true;
				player1->isStanding=false;
				player1->isRunning=false;

				if(player2->isFacingRight){
					if(player2->isRunning){
						player1->getPhysicalProperties()->setVelocityX(27);
						player1->getPhysicalProperties()->setVelocityY(-5);
					}
					else{
						player1->getPhysicalProperties()->setVelocityX(5);
						player1->getPhysicalProperties()->setVelocityY(-37);
					}
				}
				else{
					if(player2->isRunning){
						player1->getPhysicalProperties()->setVelocityX(-27);
						player1->getPhysicalProperties()->setVelocityY(-5);
					}
					else{
						player1->getPhysicalProperties()->setVelocityX(-5);
						player1->getPhysicalProperties()->setVelocityY(-37);
					}
				}
				player2->isThrowing=true;
				player2->throwLock=5;
				game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_GRAB);
					
				
			}		
			//grab
			else{
				player2->isGrabbing=true;
				player2->GrabLock=5;

				game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().PLAYER_GRAB);
			}			
			
			
		}
		//player1 correction
		//jump correction
		if(player1->getPhysicalProperties()->getVelocityY()!=0&& !player1->isAttacking){
			player1->isStanding = false;
			player1->isRunning=false;			
			player1->isJumping = true;
			player1->isGrabbing=false;
			player1->isAttacking=false;
		}	
			
		//is running
		if(player1->isRunning==true){
			//grab running
			if(player1->GrabLock>0 || player1->isGrabbing){
				if(player1->isFacingRight){
					player1->setCurrentState(WHITE_GRABBED_RUNNING_RIGHT);
				}
				else{
					player1->setCurrentState(WHITE_GRABBED_RUNNING_LEFT);
				}
			}
			//regular running state
			else{
				if(player1->isFacingRight){
					player1->setCurrentState(WHITE_RUNNING_FACING_RIGHT);
				}
				else{
					player1->setCurrentState(WHITE_RUNNING_FACING_LEFT);
				}
			}
			
		}
        //is standing
		if(player1->isStanding == true)
		{
			//grab standing states
			if(!player1->isAttacking)
			    player1->getPhysicalProperties()->setVelocityX(0);
			if(player1->GrabLock>0 || player1->isGrabbing){
				if(player1->isFacingRight){
					player1->setCurrentState(WHITE_GRABBED_STANDING_RIGHT);
				}
				else{
					player1->setCurrentState(WHITE_GRABBED_STANDING_LEFT);
				}
			}
			//regular standing state
			else{
				if(player1->isFacingRight){
					player1->setCurrentState(WHITE_STANDING_FACING_RIGHT);
				}
				else{
					player1->setCurrentState(WHITE_STANDING_FACING_LEFT);
				}
			}
		}
		
		
       //is jumping
		if(player1->isJumping == true)
		{
			if(player1->isFacingRight){
				player1->setCurrentState(WHITE_JUMPING_FACING_RIGHT);
				}
				else{
				player1->setCurrentState(WHITE_JUMPING_FACING_LEFT);
				}
		}
		//is throwinging
		if(player1->isThrowing){
			if(player1->isFacingRight){
				player1->setCurrentState(WHITE_THROWING_STANDING_RIGHT);
				}
				else{
				player1->setCurrentState(WHITE_THROWING_STANDING_LEFT);
				}

		}
			
		//is attacking
		if(player1->isAttacking){
			
			player1->setCurrentState(WHITE_ATTACKING);
			player1->getPlayerController()->Vibrate(65535,65535);

		}
		//is hurt
		if(player1->isHurt){
			if(player1->isFacingRight){
				player1->setCurrentState(WHITE_HURTING_FACING_RIGHT);
			}
			else{
				player1->setCurrentState(WHITE_HURTING_FACING_LEFT);

			}
			player1->getPlayerController()->Vibrate(65535,65535);
		}
		//is dead
		if(!player1->isAlive){
			if(player1->isFacingRight){
				player1->setCurrentState(WHITE_DYING_RIGHT);
			}
			else{
				player1->setCurrentState(WHITE_DYING_LEFT);

			}
			player1->getPlayerController()->Vibrate(65535,65535);
		}
		//correcting some bools
		if(player1->GrabLock>0){
			player1->GrabLock--;
			if(player1->GrabLock==0)
			    player1->isGrabbing=false;}

		if(player1->throwLock>0){
			player1->throwLock--;
			if(player1->throwLock==0)
				player1->isThrowing=false;}

		
        //player2 correction
		//jump correction
		if(player2->getPhysicalProperties()->getVelocityY()!=0&& !player2->isAttacking){
			player2->isStanding = false;
			player2->isRunning=false;			
			player2->isJumping = true;
			player2->isGrabbing=false;
			player2->isAttacking=false;
		}
		
		//is running
		if(player2->isRunning==true){
			//grab running
			if(player2->GrabLock>0 || player2->isGrabbing){
				if(player2->isFacingRight){
					player2->setCurrentState(BLACK_GRABBED_RUNNING_RIGHT);
				}
				else{
					player2->setCurrentState(BLACK_GRABBED_RUNNING_LEFT);
				}
			}
			//regular running state
			else{
				if(player2->isFacingRight){
					player2->setCurrentState(BLACK_RUNNING_FACING_RIGHT);
				}
				else{
					player2->setCurrentState(BLACK_RUNNING_FACING_LEFT);
				}
			}	
			
		}
        //is standing
		if(player2->isStanding == true)
		{
			//grab standing states
			if(!player2->isAttacking)
			    player2->getPhysicalProperties()->setVelocityX(0);
			if(player2->GrabLock>0 || player2->isGrabbing){
				if(player2->isFacingRight){
					player2->setCurrentState(BLACK_GRABBED_STANDING_RIGHT);
				}
				else{
					player2->setCurrentState(BLACK_GRABBED_STANDING_LEFT);
				}
			}
			//regular standing state
			else{
				if(player2->isFacingRight){
					player2->setCurrentState(BLACK_STANDING_FACING_RIGHT);
				}
				else{
					player2->setCurrentState(BLACK_STANDING_FACING_LEFT);
				}
			}
		}
		
		
      //is jumping
		if(player2->isJumping == true)
		{
			if(player2->isFacingRight){
				player2->setCurrentState(BLACK_JUMPING_FACING_RIGHT);
				}
				else{
				player2->setCurrentState(BLACK_JUMPING_FACING_LEFT);
				}
		}
		//is throwinging
		if(player2->isThrowing){
			if(player2->isFacingRight){
				player2->setCurrentState(BLACK_THROWING_STANDING_RIGHT);
				}
				else{
				player2->setCurrentState(BLACK_THROWING_STANDING_LEFT);
				}

		}
		//is attacking
		if(player2->isAttacking){
			
			player2->setCurrentState(BLACK_ATTACKING);
			player2->getPlayerController()->Vibrate(65535,65535);

		}
		//is hurt
		if(player2->isHurt){
			if(player2->isFacingRight){
				player2->setCurrentState(BLACK_HURTING_FACING_RIGHT);
			}
			else{
				player2->setCurrentState(BLACK_HURTING_FACING_LEFT);

			}
			player2->getPlayerController()->Vibrate(65535,65535);
		}
		//is dead
		if(!player2->isAlive){
			if(player2->isFacingRight){
				player2->setCurrentState(BLACK_DYING_RIGHT);
			}
			else{
				player2->setCurrentState(BLACK_DYING_LEFT);

			}
			player2->getPlayerController()->Vibrate(65535,65535);
		}
		//correcting some bools
		if(player2->GrabLock>0){
			player2->GrabLock--;
			if(player2->GrabLock==0)
			    player2->isGrabbing=false;}
		if(player2->throwLock>0){
			player2->throwLock--;
			if(player2->throwLock==0)
				player2->isThrowing=false;}

		
		//states correction!!
		
		
				
		//states correction!!	

		if (input->isKeyDownForFirstTime(VK_ESCAPE) || input->isGamepadDownForFirstTime(PLAYER_1_GAMEPAD_START) || input->isGamepadDownForFirstTime(PLAYER_2_GAMEPAD_START))
		{
			game->changeGameState(PAUSE_GAME_STATE);

			game->getAudio()->PlayAudioCue(game->getAudio()->getAudioState().GAME_PAUSE);
		}
	}
	else if(game->getGameState() == PAUSE_GAME_STATE)
	{
		if (input->isKeyDownForFirstTime(VK_ESCAPE) || input->isGamepadDownForFirstTime(PLAYER_1_GAMEPAD_START) || input->isGamepadDownForFirstTime(PLAYER_2_GAMEPAD_START))
		{
			game->changeGameState(GAME_PLAYING_GAME_STATE);
		}
	}
	else if(game->getGameState() == CONTROLS_GAME_STATE || game->getGameState() == HELP_GAME_STATE || game->getGameState() == ABOUT_GAME_STATE)
	{
		if (input->isKeyDownForFirstTime(VK_ESCAPE)|| input->isGamepadDownForFirstTime(PLAYER_1_GAMEPAD_START) || input->isGamepadDownForFirstTime(PLAYER_2_GAMEPAD_START))
		{
			if(game->getIsPaused())
			{
				game->changeGameState(PAUSE_GAME_STATE);
			}
			else
			{
				game->changeGameState(SPLASH_SCREEN_GAME_STATE);
			}
		}		
	
	}
	else if(game->getGameState() == GAME_OVER_STATE || game->getGameState() == GAME_WIN_STATE){
		if (input->isKeyDownForFirstTime(VK_ESCAPE) || input->isGamepadDownForFirstTime(PLAYER_1_GAMEPAD_START) || input->isGamepadDownForFirstTime(PLAYER_2_GAMEPAD_START)){
			game->changeGameState(SPLASH_SCREEN_GAME_STATE);
		}
	}
	

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		StringTable *guiStringTable = graphics->getGUITextureManager()->getStringTable();
		int greenCursorID = guiStringTable->getIndexOfStringFromStringTable(L"textures/gui/cursor/cursor.png");
		int redCursorID = guiStringTable->getIndexOfStringFromStringTable(L"textures/gui/cursor/cursor2.png");
		int currentCursorID = cursor->getActiveCursorID();
		if (currentCursorID == greenCursorID)
			cursor->setActiveCursorID(redCursorID);
		else
			cursor->setActiveCursorID(greenCursorID);
	}
	if(game->getGameState()==GAME_PLAYING_GAME_STATE||game->getGameState()==PAUSE_GAME_STATE){
		if ((input->isKeyDownForFirstTime(oneKey))
		&& input->isKeyDown(VK_CONTROL))
		{
			game->getWorld()->setCurrentLevel(0);
			game->changeGameState(NEXT_LEVEL_STATE);
		}

		if ((input->isKeyDownForFirstTime(twoKey))
			&& input->isKeyDown(VK_CONTROL))
		{
			game->getWorld()->setCurrentLevel(1);
			game->changeGameState(NEXT_LEVEL_STATE);
		}

		if ((input->isKeyDownForFirstTime(threeKey))
			&& input->isKeyDown(VK_CONTROL))
		{
			game->getWorld()->setCurrentLevel(2);
			game->changeGameState(NEXT_LEVEL_STATE);
		}
		
		if (((input->isKeyDownForFirstTime(plusKey))
			&& input->isKeyDown(VK_CONTROL)) 
			|| input->isGamepadDownForFirstTime(PLAYER_1_GAMEPAD_BACK)
			|| input->isGamepadDownForFirstTime(PLAYER_2_GAMEPAD_BACK))
		{
			AnimatedSprite::setPlayerHealth(5);
		}
	}
		

	if (input->isKeyDown(VK_HOME))
	{
		WindowsGameTimer *timer = (WindowsGameTimer*)game->getTimer();
		int fps = timer->getTargetFPS();
		if (fps < 100)
			timer->setTargetFPS(fps + 1);
	}
	else if (input->isKeyDown(VK_END))
	{
		WindowsGameTimer *timer = (WindowsGameTimer*)game->getTimer();
		int fps = timer->getTargetFPS();
		if (fps > 1)
			timer->setTargetFPS(fps - 1);
	}
}