/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRButtonEventHandler.cpp

	See WRButtonEventHandler.h for a class
	description.
*/

#include "stdafx.h"
#include "Game.h"
#include "WRButtonEventHandler.h"

void WRButtonEventHandler::handleButtonEvents(Game *game, 
											  wchar_t *command)
{
	// SOMEONE PRESSED THE Exit BUTTON, SO CHANGE THE
	// Game State TO END THE APPLICATION
	if (wcscmp(command, L"Exit") == 0)
	{
		int msgboxID = MessageBox(
        NULL,
        (LPCWSTR)L"Are you sure you want to Exit the Game?",
        (LPCWSTR)L"Counter Colorist",
        MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2
		);

		if (msgboxID == 6)
		{
			game->changeGameState(EXIT_GAME_STATE);
		}
	}
	else if (wcscmp(command, L"Start") == 0)
	{
		game->changeGameState(GAME_PLAYING_GAME_STATE);
	}
	else if (wcscmp(command, L"New Game") == 0)
	{
		game->changeGameState(NEW_GAME_STATE);
	}
	else if (wcscmp(command, L"Quit") == 0)
	{
		game->changeGameState(SPLASH_SCREEN_GAME_STATE);
	}
	else if (wcscmp(command, L"Controls") == 0)
	{
		game->changeGameState(CONTROLS_GAME_STATE);
	}
	else if (wcscmp(command, L"About") == 0)
	{
		game->changeGameState(ABOUT_GAME_STATE);
	}
	else if (wcscmp(command, L"Help") == 0)
	{
		game->changeGameState(HELP_GAME_STATE);
	}
}