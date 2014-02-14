/*

Render thread implementation on Windows
*/

#pragma once

#include "stdafx.h"
#include "resource.h"
#include "stdafx.h"
#include <list>
#include <vector>
#include "GameRenderThread.h"

unsigned __stdcall RenderThread(void* pArguments);

class WindowsGameRenderThread: public GameRenderThread{ 

private:
	HANDLE renderThreadHandle;
	DWORD renderThreadID;
	
	bool timeToProcessData;
	bool timeToRender;

	HANDLE timeToProcessDataMutex;
	HANDLE timeToRenderMutex;
	HANDLE renderingRenderListMutex;

public:
	WindowsGameRenderThread();
	~WindowsGameRenderThread();
	void kill();
	void runFromMainThread(Game *game);
	void start(Game *game);
	

	HANDLE getTimeToRenderMutex() { return timeToRenderMutex; }
	HANDLE getTimeToProcessDataMutex() { return timeToProcessDataMutex; }
	HANDLE getRenderingRenderListMutex() { return renderingRenderListMutex; }
	bool isTimeToRender() { return timeToRender; }
	void setTimeToRender(bool a) { timeToRender = a; }
	void setTimeToProcessData(bool a) { timeToProcessData = a; }
	void setRenderThread(HANDLE r){renderThreadHandle =r;}
	void setThreadID(DWORD ID){renderThreadID=ID;}




};