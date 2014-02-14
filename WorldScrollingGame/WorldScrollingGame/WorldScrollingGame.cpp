/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WorldRenderingGame.cpp

	This is a test game application, a game that
	demonstrates use of	the game engine simply
	for rendering images and text as well as for reacting to
	key presses and button cicks. Students will extend
	this for their projects, gradually adding other components,
	like additional gui controls, tiling, sprites, collision 
	detection, etc.
 */

#include "stdafx.h"

// GAME OBJECT INCLUDES
#include "Cursor.h"
#include "ScreenGUI.h"
#include "Game.h"
#include "GameGraphics.h"
#include "GameGUI.h"
#include "GameInput.h"
#include "GameOS.h"
#include "GameText.h"
#include "XACT.h"

// EMPTY GAME INCLUDES
#include "WRButtonEventHandler.h"
#include "WRKeyEventHandler.h"
#include "WRTextGenerator.h"

// WINDOWS PLATFORM INCLUDES
#include "WindowsGameOS.h"
#include "WindowsGameInput.h"
#include "WindowsGameTimer.h"

// DIRECTX INCLUDES
#include "DirectXGraphics.h"
#include "DirectXTextureManager.h"

// METHODS OTHER THAN WinMain DEFINED BELOW
wchar_t*	constructEmptyWCHAR_TArray(LPCWSTR initChars);
void		initWRgui(Game *game);

/*
	WinMain - This is the application's starting point. In this
	method we will construct a Game object, then construct all the
	custom data for our game, and then initialize the Game with
	our custom data. We'll then start the game loop.
*/
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// USE WINDOWED MODE (ONE LESS HEADACHE)
	bool fullscreen = false;

	// CREATE A GAME
	Game *worldRenderingGame = new Game();

	// SPECIFY THE DIRECTORY WHERE ALL GAME DESIGN FILES
	// ARE TO BE LOADED FROM
	wchar_t *gameDataPathName = constructEmptyWCHAR_TArray(L"design/");

	// WE'RE USING THE WINDOWS PLATFORM, SO MAKE A CUSTOM
	// GameOS OBJECT (WindowsGameOS), FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	wchar_t *gameTitle = constructEmptyWCHAR_TArray(L"Counter Colorist");
	WindowsGameOS *os = new WindowsGameOS(	hInstance, 
											nCmdShow,
											fullscreen,
											gameTitle,
											worldRenderingGame);

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *graphics = new DirectXGraphics(worldRenderingGame);
	graphics->init();
	graphics->initGraphics(os->getWindowHandle(), fullscreen);
	graphics->initTextFont(20);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsGameInput *input = new WindowsGameInput();

	// AND THE TIMER
	WindowsGameTimer *timer = new WindowsGameTimer();

	//AUDIO
	XACT *audio = new XACT();
	audio->PrepareXACT();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	worldRenderingGame->init(	gameDataPathName,
						(GameGraphics*)graphics,
						(GameOS*)os,
						(GameInput*)input,
						(GameTimer*)timer,
						audio);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	WRTextGenerator *textGenerator = new WRTextGenerator();
	textGenerator->initText(worldRenderingGame);
	worldRenderingGame->getText()->setTextGenerator((TextGenerator*)textGenerator);

	// LOAD THE GUI STUFF, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	initWRgui(worldRenderingGame);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	WRButtonEventHandler *eventHandler = new WRButtonEventHandler();
	GameGUI *gui = worldRenderingGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)eventHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	WRKeyEventHandler *keyHandler = new WRKeyEventHandler();
	input->registerKeyHandler((KeyEventHandler*)keyHandler);

	GameWorld *world = worldRenderingGame->getWorld();
	wchar_t *levelFile = constructEmptyWCHAR_TArray(L"Design/Level1/Map.csv");
	world->addLevelFileName(levelFile);
	levelFile = constructEmptyWCHAR_TArray(L"Design/Level2/Map.csv");
	world->addLevelFileName(levelFile);
	levelFile = constructEmptyWCHAR_TArray(L"Design/Level3/Map.csv");
	world->addLevelFileName(levelFile);

	// START THE GAME LOOP
	worldRenderingGame->runGameLoop();

	return 0;
}

/*
	initWRgui - This method builds a GUI for the Empty Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void initWRgui(Game *game)
{
	GameGraphics *graphics = game->getGraphics();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(200, 100, 200);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(162, 142, 137);
	//graphics->setColorKey(252, 176, 64);
	// SETUP THE CURSOR
	vector<int> *imageIDs = new vector<int>();

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	wchar_t *fileName = constructEmptyWCHAR_TArray(L"textures/gui/cursor/cursor.png");
	int imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/cursor/cursor2.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						30,
						32);
	gui->setCursor(cursor);

	// NOW LET'S LOAD A GUI SCREEN, BE CAREFUL TO ADD
	// THESE IN THE PROPER ORDER IF YOU EVER ADD MORE THAN ONE
	ScreenGUI *screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/CounterColorist_Splash.png");
	imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1280;
	imageToAdd->height = 800;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);


	gui->addScreenGUI(screenGUI);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	wchar_t *buttonCommand = constructEmptyWCHAR_TArray(L"Exit");
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Exit.png");
	int normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Exit_MO.png");
	int mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1165,
							747,
							0,
							255,
							87,
							32,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(L"Start");
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/NewGame.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/NewGame_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1053,
							530,
							0,
							255,
							204,
							31,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

		// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(L"Controls");
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Controls.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Controls_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1037,
							590,
							0,
							255,
							230,
							37,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(L"Help");
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Help.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Help_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1142,
							697,
							0,
							255,
							117,
							37,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(L"About");
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/About.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/About_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1115,
							647,
							0,
							255,
							142,
							37,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	
	//screenGUI->addOverlayImage(imageToAdd);
	// IN Game?
	screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/GameOverlay.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 800 - 64;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1280;
	imageToAdd->height = 32;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/HealthIcon.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 160;
	imageToAdd->y = 800 - 46;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 32;
	imageToAdd->height = 32;
	imageToAdd->imageID = imageID;
	screenGUI->addHealthBarImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 160 + 50;
	imageToAdd->y = 800 - 46;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 32;
	imageToAdd->height = 32;
	imageToAdd->imageID = imageID;
	screenGUI->addHealthBarImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 160 + 100;
	imageToAdd->y = 800 - 46;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 32;
	imageToAdd->height = 32;
	imageToAdd->imageID = imageID;
	screenGUI->addHealthBarImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 160 + 150;
	imageToAdd->y = 800 - 46;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 32;
	imageToAdd->height = 32;
	imageToAdd->imageID = imageID;
	screenGUI->addHealthBarImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 160 + 200;
	imageToAdd->y = 800 - 46;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 32;
	imageToAdd->height = 32;
	imageToAdd->imageID = imageID;
	screenGUI->addHealthBarImage(imageToAdd);

	gui->addScreenGUI(screenGUI);

	//BACK
	screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Back.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Back_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"Quit");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1165,
							757,
							0,
							255,
							118,
							36,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
	
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/CounterColorist_Splash.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1280;
	imageToAdd->height = 800;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/Controls.png");
	imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *menuImageToAdd = new OverlayImage();
	menuImageToAdd->x = 390;
	menuImageToAdd->y = 150 + 50;
	menuImageToAdd->z = 0;
	menuImageToAdd->alpha = 255;
	menuImageToAdd->width = 500;
	menuImageToAdd->height = 500;
	menuImageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(menuImageToAdd);

	//About Menu
	gui->addScreenGUI(screenGUI);
	screenGUI = new ScreenGUI();
	screenGUI->addButton(buttonToAdd);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/CounterColorist_Splash.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1280;
	imageToAdd->height = 800;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/About.png");
	imageID = guiTextureManager->loadTexture(fileName);
	menuImageToAdd = new OverlayImage();
	menuImageToAdd->x = 390;
	menuImageToAdd->y = 150 + 50;
	menuImageToAdd->z = 0;
	menuImageToAdd->alpha = 255;
	menuImageToAdd->width = 500;
	menuImageToAdd->height = 500;
	menuImageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(menuImageToAdd);

	//Help Menu
	gui->addScreenGUI(screenGUI);
	screenGUI = new ScreenGUI();
	screenGUI->addButton(buttonToAdd);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/CounterColorist_Splash.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1280;
	imageToAdd->height = 800;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/Help.png");
	imageID = guiTextureManager->loadTexture(fileName);
	menuImageToAdd = new OverlayImage();
	menuImageToAdd->x = 390;
	menuImageToAdd->y = 150 + 50;
	menuImageToAdd->z = 0;
	menuImageToAdd->alpha = 255;
	menuImageToAdd->width = 500;
	menuImageToAdd->height = 500;
	menuImageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(menuImageToAdd);
	gui->addScreenGUI(screenGUI);
	
	//Pause
	screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/Pause.png");
	imageID = guiTextureManager->loadTexture(fileName);
	menuImageToAdd = new OverlayImage();
	menuImageToAdd->x = 340;
	menuImageToAdd->y = 100;
	menuImageToAdd->z = 0;
	menuImageToAdd->alpha = 255;
	menuImageToAdd->width = 600;
	menuImageToAdd->height = 600;
	menuImageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(menuImageToAdd);
	
		//Resume
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Resume.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Resume_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"Start");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							317 + 240,
							71 + 200,
							0,
							255,
							168,
							26,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
		//New Game
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/NewGame.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/NewGame_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"New Game");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							300 + 240,
							131 + 200,
							0,
							255,
							202,
							24,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
		//Controls
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Controls.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Controls_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"Controls");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							293 + 240,
							189 + 200,
							0,
							255,
							217,
							24,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
		//About
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/About.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/About_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"About");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							331 + 240,
							253 + 200,
							0,
							255,
							138,
							24,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
		//Help
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Help.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Help_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"Help");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							345 + 240,
							309 + 200,
							0,
							255,
							109,
							24,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
		//Exit
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Exit.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/Exit_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"Exit");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							356 + 240,
							369 + 200,
							0,
							255,
							89,
							24,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);

	gui->addScreenGUI(screenGUI);

	//End Game
	screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/Game Over.png");
	imageID = guiTextureManager->loadTexture(fileName);
	menuImageToAdd = new OverlayImage();
	menuImageToAdd->x = 0;
	menuImageToAdd->y = 0;
	menuImageToAdd->z = 0;
	menuImageToAdd->alpha = 255;
	menuImageToAdd->width = 800;
	menuImageToAdd->height = 600;
	menuImageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(menuImageToAdd);
	
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/MainMenu.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/MainMenu_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"Quit");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1005,
							737,
							0,
							255,
							216,
							36,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
	gui->addScreenGUI(screenGUI);

	//Game Load
	//screenGUI = new ScreenGUI();
	//fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/Game Loading.png");
	//imageID = guiTextureManager->loadTexture(fileName);
	//menuImageToAdd = new OverlayImage();
	//menuImageToAdd->x = 0;
	//menuImageToAdd->y = 0;
	//menuImageToAdd->z = 0;
	//menuImageToAdd->alpha = 255;
	//menuImageToAdd->width = 800;
	//menuImageToAdd->height = 600;
	//menuImageToAdd->imageID = imageID;
	//screenGUI->addOverlayImage(menuImageToAdd);
	//gui->addScreenGUI(screenGUI);

	//Game Win
	screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/overlays/Game Won.png");
	imageID = guiTextureManager->loadTexture(fileName);
	menuImageToAdd = new OverlayImage();
	menuImageToAdd->x = 0;
	menuImageToAdd->y = 0;
	menuImageToAdd->z = 0;
	menuImageToAdd->alpha = 255;
	menuImageToAdd->width = 1280;
	menuImageToAdd->height = 800;
	menuImageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(menuImageToAdd);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/MainMenu.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"textures/gui/buttons/MainMenu_MO.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(L"Quit");

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1005,
							737,
							0,
							255,
							216,
							36,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
	gui->addScreenGUI(screenGUI);

}