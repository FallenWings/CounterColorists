/*
XACT for handling game audio
*/


#pragma once
#include <windows.h>
#include <commdlg.h>
#include <xact3.h>
#include <shellapi.h>
#include "resource.h"
#include "stdafx.h"
#include <list>
#include <vector>
#include <strsafe.h>

class XACT{
public:
	struct AUDIO_STATE
	{
		IXACT3Engine* pEngine;
		IXACT3WaveBank* pWaveBank;
		IXACT3SoundBank* pSoundBank;
		bool bInitialized;

		VOID* pbWaveBank; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
		VOID* pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed

		//cue
		XACTINDEX MENU;
		XACTINDEX BGM_STAGE1;
		XACTINDEX BGM_STAGE2;
		XACTINDEX BGM_STAGE3;

		XACTINDEX PLAYER_JUMP;
		XACTINDEX PLAYER_GRAB;
		XACTINDEX PLAYER_THROW;
		XACTINDEX PLAYER_HURT;
		XACTINDEX PLAYER_DIE;

		XACTINDEX GAME_PAUSE;

		XACTINDEX GOT_ITEM;

		XACTINDEX ALARM;
		XACTINDEX AI_HURT;
		XACTINDEX AI_DIE;
		XACTINDEX WIN;
		
	};

	AUDIO_STATE g_audioState;
	AUDIO_STATE getAudioState(){return g_audioState;}

	//-----------------------------------------------------------------------------
	HRESULT  PlayAudioCue( XACTINDEX iCueIndex );
	HRESULT PrepareXACT();
	VOID CleanupXACT();
	void update();
	

};