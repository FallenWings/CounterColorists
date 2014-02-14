#include "GameRenderThread.h"
#include "Game.h"
#include "WindowsGameRenderThread.h"
#include "GameGraphics.h"
WindowsGameRenderThread::WindowsGameRenderThread(){
	renderThreadHandle = NULL;
    timeToProcessData = true;
	timeToRender = false;

	timeToProcessDataMutex = CreateMutex(0,NULL,
		    L"timeToProcessDataMutex");
	timeToRenderMutex = CreateMutex(0, NULL,
			L"timeToRenderMutex");
	renderingRenderListMutex = CreateMutex(0, NULL,
			L"renderingRenderListMutex");

}

WindowsGameRenderThread::~WindowsGameRenderThread(){

}

void WindowsGameRenderThread::start(Game *game){
	//start the render thread
	HANDLE renderThreadHandle;
	unsigned int renderThreadID;
	renderThreadHandle = (HANDLE)_beginthreadex(NULL,0,
		&RenderThread,game,0,&renderThreadID);
	setRenderThread(renderThreadHandle);
	setThreadID(renderThreadID);
}

void WindowsGameRenderThread::runFromMainThread(Game *game)
{
	game->processGameData();
	WaitForSingleObject(timeToProcessDataMutex, INFINITE);	
	
	if (timeToProcessData)
	{	
		
		timeToProcessData = false;		
		ReleaseMutex(timeToProcessDataMutex);
		// NOW LET'S SWAP THE RENDER LISTS
		WaitForSingleObject(renderingRenderListMutex, INFINITE);		
		
		GameGraphics *graphics = game->getGraphics();		
		graphics->swapRenderLists();
		ReleaseMutex(renderingRenderListMutex);
		WaitForSingleObject(timeToRenderMutex, INFINITE);
		timeToRender = true;
		ReleaseMutex(timeToRenderMutex);
	}
	else
		ReleaseMutex(timeToProcessDataMutex);
}


unsigned __stdcall RenderThread( void* pArguments )
{
	Game *game = (Game*)pArguments;
	GameGraphics *graphics = game->getGraphics();
	WindowsGameRenderThread *thread = (WindowsGameRenderThread*)game->getRenderThread();

	while (game->isGameActive())
	{
		WaitForSingleObject(thread->getTimeToRenderMutex(), INFINITE);

		if (thread->isTimeToRender())
		{
			thread->setTimeToRender(false);
			ReleaseMutex(thread->getTimeToRenderMutex());

			WaitForSingleObject(thread->getRenderingRenderListMutex(), INFINITE);
			graphics->renderGame(game);
			ReleaseMutex(thread->getRenderingRenderListMutex());

			WaitForSingleObject(thread->getTimeToProcessDataMutex(), INFINITE);
			thread->setTimeToProcessData(true);
			ReleaseMutex(thread->getTimeToProcessDataMutex());
		}
		else
		{
			ReleaseMutex(thread->getTimeToRenderMutex());
		}
	}
	_endthreadex( 0 );

	return (26);
}



		
void WindowsGameRenderThread::kill(){}

