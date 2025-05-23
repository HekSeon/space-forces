//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(6)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(480)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(80)			// 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg.jpg",
	"data/TEXTURE/start.png",
	"data/TEXTURE/logo4.png",
	"data/TEXTURE/help.png",
	"data/TEXTURE/player.png",
	"data/TEXTURE/quit.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号


static BOOL						g_startUse;						// TRUE:使っている  FALSE:未使用
static float					g_startw, g_starth;					// 幅と高さ
static XMFLOAT3					g_startPos;						// ポリゴンの座標

float	alpha;
BOOL	flag_alpha;

static BOOL						g_Load = FALSE;

static int menuseelect;

static float	effect_dx;
static float	effect_dy;

static float ax;
static float ay;




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 変数の初期化
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w, g_h, 0.0f);
	g_TexNo = 0;


	g_startUse   = TRUE;	
	g_startw = TEXTURE_WIDTH / 4;
	g_starth = TEXTURE_HEIGHT / 4;
	g_startPos = XMFLOAT3(g_startw / 2, g_starth / 2, 0.0f);
	g_TexNo = 1;


	alpha = 1.0f;
	flag_alpha = TRUE;

	effect_dx = 960.0f;
	effect_dy = 200.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{


	if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0,BUTTON_DOWN))
	{
		menuseelect += 1;
	}
	else if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0,BUTTON_UP))
	{
		menuseelect -=1;
	}
	menuseelect  %= 3;

	switch (menuseelect) {
	case 0:

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{// Enter押したら、ステージを切り替える
			SetFade(FADE_OUT, MODE_GAME);
		}
		// ゲームパッドで入力処理
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			SetFade(FADE_OUT, MODE_GAME);
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			SetFade(FADE_OUT, MODE_GAME);
		}
		break;

	case 1:

		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			SetFade(FADE_OUT, MODE_TUTORIAL);
		}
		break;

	case 2:

		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			exit(-1);
		}

		break;

	default:
		break;

		

		// セーブデータをロードする？
		if (GetKeyboardTrigger(DIK_L))
		{
			SetLoadGame(TRUE);
			SetFade(FADE_OUT, MODE_GAME);
		}

	}

	//// テストでエフェクトの発生場所を移動させる
	//float speed = 4.0f;

	//if (GetKeyboardPress(DIK_DOWN))
	//{
	//	effect_dy += speed;
	//}
	//else if (GetKeyboardPress(DIK_UP))
	//{
	//	effect_dy -= speed;
	//}

	//if (GetKeyboardPress(DIK_RIGHT))
	//{
	//	effect_dx += speed;
	//}
	//else if (GetKeyboardPress(DIK_LEFT))
	//{
	//	effect_dx -= speed;
	//}




#ifdef _DEBUG	// デバッグ情報を表示する
	//PrintDebugProc("Player:↑ → ↓ ←　Space\n");

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
		for (int i = 0; i < 30; i++)
		{

			float sx = (float)(rand() % 10);
			float sy = (float)(rand() % 10);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, 960 + sx, 500 + sy, 900, 100, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 0.3f, 1.0f, 0.5f));



			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}


		// 加減算のテスト
		SetBlendState(BLEND_MODE_ADD);		// 加算合成
		//	SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成

			// テクスチャ設定

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		for (int i = 0; i < 30; i++)
		{
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			float dx = effect_dx;
			float dy = effect_dy;
			float sx = (float)(rand() % 10);
			float sy = (float)(rand() % 10);

			SetSpriteColor(g_VertexBuffer, dx + sx, dy + sy, 1000, 250, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 0.3f, 1.0f, 0.5f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
		SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		for (int i = 0; i < 30; i++)
		{
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			float sx = (float)(rand() % 10);
			float sy = (float)(rand() % 10);

			SetSpriteColor(g_VertexBuffer, 960 + sx, 650 + sy, 500, 100, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 0.3f, 1.0f, 0.5f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
		SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す
	}

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

	for (int i = 0; i < 30; i++)
	{
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		float ax = 400;
		float ay = 500;

		float sx = (float)(rand() % 10);
		float sy = (float)(rand() % 10);

		switch (menuseelect) {

		case 0:
			ax = 400;
			ay = 500;
			break;

		case 1:
			ax = 600;
			ay = 650;
			break;
		case 2:
			ax = 700;
			ay = 800;
			break;

		default:
			break;
		}

		SetSpriteColor(g_VertexBuffer, ax + sx, ay + sy, 150, 150, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);

	for (int i = 0; i < 30; i++)
	{
		// １枚のポリゴンの頂点とテクスチャ座標を設定

		float sx = (float)(rand() % 10);
		float sy = (float)(rand() % 10);

		SetSpriteColor(g_VertexBuffer, 960+ sx, 800 + sy, 250, 100, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
	SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す

}

