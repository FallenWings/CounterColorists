#include "resource.h"
#include "stdafx.h"
#include <xact3.h>
#include "XACT.h"
#include "Game.h"

//HANDLE notifications
void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION* pNotification)
{
   if( pNotification->type == XACTNOTIFICATIONTYPE_CUESTOP )
   {
       // Handle the cue stop notification 
   }
  
}
//init the audio

HRESULT XACT::PrepareXACT(){
    HRESULT hr;
	DWORD dwFileSize;
	HANDLE hMapFile;
	HANDLE hFile;

 
    hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );  // COINIT_APARTMENTTHREADED will work too
	hr = XACT3CreateEngine( XACT_FLAG_API_AUDITION_MODE, &g_audioState.pEngine );

	XACT_RUNTIME_PARAMETERS xrParams = {0};
	xrParams.fnNotificationCallback = XACTNotificationCallback;
	xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	hr = g_audioState.pEngine->Initialize( &xrParams );

    //create wave banks
	wchar_t *str = constructEmptyWCHAR_TArray(L"audio/Win/Wave Bank.xwb");
	hFile = CreateFile( str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	dwFileSize = GetFileSize( hFile, NULL );
	hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
	g_audioState.pbWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );    
	hr = g_audioState.pEngine->CreateInMemoryWaveBank( g_audioState.pbWaveBank, dwFileSize, 0,
                                                                       0,&g_audioState.pWaveBank );
	CloseHandle( hFile );

    //create sound banks
    str = constructEmptyWCHAR_TArray(L"audio/Win/Sound Bank.xsb");
	hFile = CreateFile( str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
	dwFileSize = GetFileSize( hFile, NULL );
	hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
	g_audioState.pbSoundBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );    
	hr = g_audioState.pEngine->CreateSoundBank( g_audioState.pbSoundBank, dwFileSize, 0,
                                                                       0,&g_audioState.pSoundBank );
	CloseHandle( hFile );
    
	g_audioState.MENU = g_audioState.pSoundBank->GetCueIndex( "Menu" );
    g_audioState.BGM_STAGE1 = g_audioState.pSoundBank->GetCueIndex( "stage1" );
	g_audioState.BGM_STAGE2 = g_audioState.pSoundBank->GetCueIndex( "stage2" );
	g_audioState.BGM_STAGE3 = g_audioState.pSoundBank->GetCueIndex( "stage3" );
	g_audioState.PLAYER_JUMP = g_audioState.pSoundBank->GetCueIndex( "playerjump" );
	g_audioState.PLAYER_GRAB = g_audioState.pSoundBank->GetCueIndex( "playergrab" );
	g_audioState.PLAYER_THROW = g_audioState.pSoundBank->GetCueIndex( "playerthrow" );
	g_audioState.PLAYER_HURT = g_audioState.pSoundBank->GetCueIndex( "playerhurt" );
	g_audioState.PLAYER_DIE = g_audioState.pSoundBank->GetCueIndex( "playerdie" );
	g_audioState.GAME_PAUSE = g_audioState.pSoundBank->GetCueIndex( "gamepause" );
	g_audioState.GOT_ITEM = g_audioState.pSoundBank->GetCueIndex( "item" );

	g_audioState.ALARM = g_audioState.pSoundBank->GetCueIndex( "alarm" );
    g_audioState.AI_HURT = g_audioState.pSoundBank->GetCueIndex( "enemyhurt" );
    g_audioState.AI_DIE = g_audioState.pSoundBank->GetCueIndex( "enemydie" );

	g_audioState.WIN = g_audioState.pSoundBank->GetCueIndex( "win" );
	




    
	g_audioState.bInitialized = true;

	return 0;

}

//play a cue in the bank.
//-----------------------------------------------------------------------------
HRESULT  XACT::PlayAudioCue(XACTINDEX iCueIndex){
	if( !g_audioState.bInitialized ){}
			

    return g_audioState.pSoundBank->Play( iCueIndex, 0, 0, NULL );
}
//shut down the audio.
VOID XACT::CleanupXACT(){
	    // Shutdown XACT
    //
    // Note that pEngine->ShutDown is synchronous and will take some time to complete 
    // if there are still playing cues.  Also pEngine->ShutDown() is generally only 
    // called when a game exits and is not the preferred method of changing audio 
    // resources. To know when it is safe to free wave/sound bank data without 
    // shutting down the XACT engine, use the XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED 
    // or XACTNOTIFICATIONTYPE_WAVEBANKDESTROYED notifications 
    // Release XACT interfaces
	if( !g_audioState.bInitialized )
        return;
    
    // Shut down XACT  
	if( g_audioState.pEngine )
    {
        g_audioState.pEngine->ShutDown();
        g_audioState.pEngine->Release();
    }    
    
    // Release memory mapped files
    if( g_audioState.pbSoundBank )
        UnmapViewOfFile( g_audioState.pbSoundBank );
	    
    if( g_audioState.pbWaveBank )
        UnmapViewOfFile( g_audioState.pbWaveBank );
	
	CoUninitialize();	

}

//audio update, called by main game loop per frame
void XACT::update(){
	if( g_audioState.pEngine )
        g_audioState.pEngine->DoWork();
}

