//=============================================================================
//
// バレット処理 [bullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BossBullet_MAX		(100)		// バレットのMax数
#define BossBullet_SPEED	(6.0f)		// バレットの移動スピード


// バレット構造体
struct BossBullet
{
	BOOL				use;				// true:使っている  false:未使用
	float				w, h;				// 幅と高さ
	XMFLOAT3			pos;				// バレットの座標
	XMFLOAT3			rot;				// バレットの回転量
	XMFLOAT3			move;				// バレットの移動量
	int					countAnim;			// アニメーションカウント
	int					patternAnim;		// アニメーションパターンナンバー
	int					texNo;				// 何番目のテクスチャーを使用するのか
	int			dir;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBossBullet(void);
void UninitBossBullet(void);
void UpdateBossBullet(void);
void DrawBossBullet(void);

BossBullet* GetBossBullet(void);
void SetBossBullet(XMFLOAT3 enemyPos);


