//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_sample000,	// BGM0
	SOUND_LABEL_BGM_bgm_maoudamashii_neorock73,	//
	SOUND_LABEL_SE_lazer,		// 爆発音
	SOUND_LABEL_SE_explosion,
	SOUND_LABEL_BGM_gameends,	// 	// 

	SOUND_LABEL_BGM_maou,		// BGM Maou

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

