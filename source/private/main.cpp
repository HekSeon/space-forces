//=============================================================================
//
// ƒƒCƒ“ˆ— [main.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "debugproc.h"
#include "input.h"

#include "title.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "enemybullet.h"
#include "score.h"
#include "result.h"
#include "sound.h"
#include "fade.h"
#include "tutorial.h"
#include "boss.h"
#include "bossbullet.h"
#include "bossbullet2.h"

#include "file.h"

#include "effect.h"

//*****************************************************************************
// ƒ}ƒNƒ’è‹`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ƒEƒCƒ“ƒhƒE‚ÌƒNƒ‰ƒX–¼
#define WINDOW_NAME		"DirectX11"			// ƒEƒCƒ“ƒhƒE‚ÌƒLƒƒƒvƒVƒ‡ƒ“–¼

//*****************************************************************************
// ƒvƒƒgƒ^ƒCƒvéŒ¾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


//*****************************************************************************
// ƒOƒ[ƒoƒ‹•Ï”:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;							// FPSƒJƒEƒ“ƒ^
char	g_DebugStr[2048] = WINDOW_NAME;		// ƒfƒoƒbƒO•¶Žš•\Ž¦—p

#endif

int	g_Mode = MODE_TITLE;					// ‹N“®Žž‚Ì‰æ–Ê‚ðÝ’è

BOOL g_LoadGame = FALSE;					// NewGame


//=============================================================================
// ƒƒCƒ“ŠÖ”
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// –³‚­‚Ä‚à—Ç‚¢‚¯‚ÇAŒx‚ªo‚éi–¢Žg—péŒ¾j
	UNREFERENCED_PARAMETER(lpCmdLine);		// –³‚­‚Ä‚à—Ç‚¢‚¯‚ÇAŒx‚ªo‚éi–¢Žg—péŒ¾j

	// ŽžŠÔŒv‘ª—p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// ƒEƒBƒ“ƒhƒEƒNƒ‰ƒX‚Ì“o˜^
	RegisterClassEx(&wcex);

	// ƒEƒBƒ“ƒhƒE‚Ìì¬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// ƒEƒBƒ“ƒhƒE‚Ì¶À•W
		CW_USEDEFAULT,																		// ƒEƒBƒ“ƒhƒE‚ÌãÀ•W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ƒEƒBƒ“ƒhƒE‰¡•
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ƒEƒBƒ“ƒhƒEc•
		NULL,
		NULL,
		hInstance,
		NULL);




	// ƒEƒBƒ“ƒhƒEƒ‚[ƒh‚©ƒtƒ‹ƒXƒNƒŠ[ƒ“ƒ‚[ƒh‚©‚Ìˆ—
	BOOL mode = TRUE;

	int id = MessageBox(NULL, "Windowƒ‚[ƒh‚ÅƒvƒŒƒC‚µ‚Ü‚·‚©H", "‹N“®ƒ‚[ƒh", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:		// Yes‚È‚çWindowƒ‚[ƒh‚Å‹N“®
		mode = TRUE;
		break;
	case IDNO:		// No‚È‚çƒtƒ‹ƒXƒNƒŠ[ƒ“ƒ‚[ƒh‚Å‹N“®
		mode = FALSE;	// ŠÂ‹«‚É‚æ‚Á‚Ä“®‚©‚È‚¢Ž–‚ª‚ ‚é
		break;
	case IDCANCEL:	// CANCEL‚È‚çI—¹
	default:
		return -1;
		break;
	}

	// ‰Šú‰»ˆ—(ƒEƒBƒ“ƒhƒE‚ðì¬‚µ‚Ä‚©‚çs‚¤)
	if(FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// ƒtƒŒ[ƒ€ƒJƒEƒ“ƒg‰Šú‰»
	timeBeginPeriod(1);	// •ª‰ð”\‚ðÝ’è
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// ƒVƒXƒeƒ€Žž‚ðƒ~ƒŠ•b’PˆÊ‚ÅŽæ“¾
	dwCurrentTime = dwFrameCount = 0;

	// ƒEƒCƒ“ƒhƒE‚Ì•\Ž¦(‰Šú‰»ˆ—‚ÌŒã‚ÉŒÄ‚Î‚È‚¢‚Æ‘Ê–Ú)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ƒƒbƒZ[ƒWƒ‹[ƒv
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()‚ªŒÄ‚Î‚ê‚½‚çƒ‹[ƒvI—¹
				break;
			}
			else
			{
				// ƒƒbƒZ[ƒW‚Ì–|–ó‚Æ‘—o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1•b‚²‚Æ‚ÉŽÀs
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS‚ð‘ª’è‚µ‚½Žž‚ð•Û‘¶
				dwFrameCount = 0;							// ƒJƒEƒ“ƒg‚ðƒNƒŠƒA
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60•b‚²‚Æ‚ÉŽÀs
			{
				dwExecLastTime = dwCurrentTime;	// ˆ—‚µ‚½Žž‚ð•Û‘¶

#ifdef _DEBUG	// ƒfƒoƒbƒO”Å‚ÌŽž‚¾‚¯FPS‚ð•\Ž¦‚·‚é
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// XVˆ—
				Draw();				// •`‰æˆ—

#ifdef _DEBUG	// ƒfƒoƒbƒO”Å‚ÌŽž‚¾‚¯•\Ž¦‚·‚é
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// •ª‰ð”\‚ð–ß‚·

	// ƒEƒBƒ“ƒhƒEƒNƒ‰ƒX‚Ì“o˜^‚ð‰ðœ
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// I—¹ˆ—
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// ƒvƒƒV[ƒWƒƒ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ‰Šú‰»ˆ—
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// •`‰æ‚Ì‰Šú‰»
	InitRenderer(hInstance, hWnd, bWindow);

	// ƒJƒƒ‰‚Ì‰Šú‰»
	InitCamera();

	// ƒ‰ƒCƒg‚ð—LŒø‰»
	SetLightEnable(FALSE);

	// ”w–Êƒ|ƒŠƒSƒ“‚ðƒJƒŠƒ“ƒO
	SetCullingMode(CULL_MODE_BACK);

	// “ü—Íˆ—‚Ì‰Šú‰»
	InitInput(hInstance, hWnd);

	// ƒTƒEƒ“ƒhˆ—‚Ì‰Šú‰»
	InitSound(hWnd);

	// ƒtƒF[ƒhˆ—‚Ì‰Šú‰»
	InitFade();


	// Å‰‚Ìƒ‚[ƒh‚ðƒZƒbƒg
	SetMode(g_Mode);	// ‚±‚±‚ÍSetMode‚Ì‚Ü‚Ü‚ÅI

	return S_OK;
}

//=============================================================================
// I—¹ˆ—
//=============================================================================
void Uninit(void)
{
	// I—¹‚Ìƒ‚[ƒh‚ðƒZƒbƒg
	SetMode(MODE_MAX);

	// ƒtƒF[ƒh‚ÌI—¹ˆ—
	UninitFade();

	// ƒTƒEƒ“ƒh‚ÌI—¹ˆ—
	UninitSound();

	// “ü—Í‚ÌI—¹ˆ—
	UninitInput();

	// ƒJƒƒ‰‚ÌI—¹ˆ—
	UninitCamera();

	// ƒŒƒ“ƒ_ƒ‰[‚ÌI—¹ˆ—
	UninitRenderer();
}

//=============================================================================
// XVˆ—
//=============================================================================
void Update(void)
{
	// “ü—Í‚ÌXVˆ—
	UpdateInput();

	// ƒJƒƒ‰XV
	UpdateCamera();

	// ƒ‚[ƒh‚É‚æ‚Á‚Äˆ—‚ð•ª‚¯‚é
	switch (g_Mode)
	{
	case MODE_TITLE:		// ƒ^ƒCƒgƒ‹‰æ–Ê‚ÌXV
		UpdateTitle();
		break;

	case MODE_GAME:			// ƒQ[ƒ€‰æ–Ê‚ÌXV
		UpdateBG();
		UpdatePlayer();
		UpdateEnemy();
		UpdateBullet();
		UpdateEBullet();
		UpdateEffect();
		UpdateScore();

		if(GetFade() == FADE_NONE)
		{	// ‘S–Åƒ`ƒFƒbƒN
			int ans = CheckGameClear();
			if (ans != 0)
			{
				//SetMode(MODE_RESULT);
				SetFade(FADE_OUT, MODE_BOSS);
			}
		}

		break;

	case MODE_BOSS:			// ƒQ[ƒ€‰æ–Ê‚ÌXV
		UpdateBG();
		UpdatePlayer();
		UpdateBoss();
		UpdateBossBullet();
		UpdateBossBullet2();
		UpdateBullet();
		UpdateEffect();
		UpdateScore();

		//if (GetFade() == FADE_NONE)
		//{	// ‘S–Åƒ`ƒFƒbƒN
		//	int ans = CheckGameClear();
		//	if (ans != 0)
		//	{
		//		//SetMode(MODE_RESULT);
		//		SetFade(FADE_OUT, MODE_RESULT);
		//	}
		//}

		break;

	case MODE_RESULT:		// ƒŠƒUƒ‹ƒg‰æ–Ê‚ÌXV
		UpdateResult();
		break;

	case MODE_TUTORIAL:		// ƒŠƒUƒ‹ƒg‰æ–Ê‚ÌXV
		UpdateTutorial();
		break;
	}

	UpdateFade();			// ƒtƒF[ƒh‚ÌXVˆ—
}

//=============================================================================
// •`‰æˆ—
//=============================================================================
void Draw(void)
{
	// ƒoƒbƒNƒoƒbƒtƒ@ƒNƒŠƒA
	Clear();

	SetCamera();

	// 2D‚Ì•¨‚ð•`‰æ‚·‚éˆ—
	SetViewPort(TYPE_FULL_SCREEN);

	// Z”äŠr‚È‚µ
	SetDepthEnable(FALSE);

	// ƒ‰ƒCƒeƒBƒ“ƒO‚ð–³Œø
	SetLightEnable(FALSE);


	// ƒ‚[ƒh‚É‚æ‚Á‚Äˆ—‚ð•ª‚¯‚é
	switch (g_Mode)
	{
	case MODE_TITLE:		// ƒ^ƒCƒgƒ‹‰æ–Ê‚Ì•`‰æ
		DrawTitle();
		break;

	case MODE_GAME:			// ƒQ[ƒ€‰æ–Ê‚Ì•`‰æ
		DrawBG();
		DrawBullet();		// d‚È‚é‡”Ô‚ðˆÓŽ¯‚µ‚Ä‚Ë
		DrawEBullet();
		DrawEnemy();
		DrawPlayer();
		DrawEffect();
		DrawScore();
		break;

	case MODE_BOSS:			// ƒQ[ƒ€‰æ–Ê‚Ì•`‰æ
		DrawBG();
		DrawBoss();
		DrawBullet();		// d‚È‚é‡”Ô‚ðˆÓŽ¯‚µ‚Ä‚Ë
		DrawBossBullet();
		DrawBossBullet2();
		DrawPlayer();
		DrawEffect();
		DrawScore();
		break;

	case MODE_RESULT:		// ƒŠƒUƒ‹ƒg‰æ–Ê‚Ì•`‰æ
		DrawResult();
		break;

	case MODE_TUTORIAL:
		DrawTutorial();
		break;
	}


	DrawFade();				// ƒtƒF[ƒh‰æ–Ê‚Ì•`‰æ


#ifdef _DEBUG
	// ƒfƒoƒbƒO•\Ž¦
	DrawDebugProc();
#endif

	// ƒoƒbƒNƒoƒbƒtƒ@Aƒtƒƒ“ƒgƒoƒbƒtƒ@“ü‚ê‘Ö‚¦
	Present();
}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif



//=============================================================================
// ƒ‚[ƒh‚ÌÝ’è
//=============================================================================
void SetMode(int mode)
{
	// ƒ‚[ƒh‚ð•Ï‚¦‚é‘O‚É‘S•”ƒƒ‚ƒŠ‚ð‰ð•ú‚µ‚¿‚á‚¤
	StopSound();			// ‚Ü‚¸‹È‚ðŽ~‚ß‚é

	// ƒ‚[ƒh‚ð•Ï‚¦‚é‘O‚É‘S•”ƒƒ‚ƒŠ‚ð‰ð•ú‚µ‚¿‚á‚¤

	// ƒ^ƒCƒgƒ‹‰æ–Ê‚ÌI—¹ˆ—
	UninitTitle();

	UninitTutorial();

	// BG‚ÌI—¹ˆ—
	UninitBG();

	// ƒvƒŒƒCƒ„[‚ÌI—¹ˆ—
	UninitPlayer();

	// ƒGƒlƒ~[‚ÌI—¹ˆ—
	UninitEnemy();

	// ƒoƒŒƒbƒg‚ÌI—¹ˆ—
	UninitBullet();

	UninitEBullet();

	UninitBoss();

	UninitBossBullet();
	UninitBossBullet2();

	// ƒXƒRƒA‚ÌI—¹ˆ—
	UninitScore();

	// ƒŠƒUƒ‹ƒg‚ÌI—¹ˆ—
	UninitResult();

	// ƒGƒtƒFƒNƒg‚ÌI—¹ˆ—
	UninitEffect();


	g_Mode = mode;	// ŽŸ‚Ìƒ‚[ƒh‚ðƒZƒbƒg‚µ‚Ä‚¢‚é

	switch (g_Mode)
	{
	case MODE_TITLE:
		// ƒ^ƒCƒgƒ‹‰æ–Ê‚Ì‰Šú‰»
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_maou);
		break;

	case MODE_GAME:
		// ƒQ[ƒ€‰æ–Ê‚Ì‰Šú‰»
		InitBG();
		InitPlayer();
		InitEnemy();
		InitBullet();
		InitEBullet();
		InitEffect();
		InitScore();

		// ƒ[ƒhƒQ[ƒ€‚¾‚Á‚½‚ç‚·‚×‚Ä‚Ì‰Šú‰»‚ªI‚í‚Á‚½Œã‚ÉƒZ[ƒuƒf[ƒ^‚ð“Ç‚Ýž‚Þ
		if (g_LoadGame == TRUE)
		{
			LoadData();
			g_LoadGame = FALSE;		// ƒ[ƒh‚µ‚½‚©‚çƒtƒ‰ƒO‚ðClear‚·‚é
		}

		PlaySound(SOUND_LABEL_BGM_bgm_maoudamashii_neorock73);
		break;

	case MODE_BOSS:
		// ƒQ[ƒ€‰æ–Ê‚Ì‰Šú‰»
		InitBG();
		InitPlayer();
		InitBoss();
		InitBossBullet();
		InitBossBullet2();
		InitBullet();
		InitEffect();
		InitScore();

		// ƒ[ƒhƒQ[ƒ€‚¾‚Á‚½‚ç‚·‚×‚Ä‚Ì‰Šú‰»‚ªI‚í‚Á‚½Œã‚ÉƒZ[ƒuƒf[ƒ^‚ð“Ç‚Ýž‚Þ
		if (g_LoadGame == TRUE)
		{
			LoadData();
			g_LoadGame = FALSE;		// ƒ[ƒh‚µ‚½‚©‚çƒtƒ‰ƒO‚ðClear‚·‚é
		}

		PlaySound(SOUND_LABEL_BGM_bgm_maoudamashii_neorock73);
		break;

	case MODE_RESULT:
		InitResult();
		PlaySound(SOUND_LABEL_BGM_gameends);
		break;

	case MODE_TUTORIAL:
		InitTutorial();
		break;

	case MODE_MAX:
		break;
	}
}

//=============================================================================
// ƒ‚[ƒh‚ÌŽæ“¾
//=============================================================================
int GetMode(void)
{
	return g_Mode;
}


//=============================================================================
// ‘S–Åƒ`ƒFƒbƒN
//=============================================================================
int CheckGameClear(void)
{

	// ƒGƒlƒ~[‚Ì”‚ª‚O‚È‚çƒvƒŒƒCƒ„[‚ÌŸ‚¿‚Æ‚·‚é
	int cnt = GetEnemyCount();
	if(cnt <= 0)
	{
		return 1;	// ƒvƒŒƒCƒ„[‚ÌŸ‚¿
	}

	// ƒvƒŒƒCƒ„[‚Ì”‚ª‚O‚È‚çƒGƒlƒ~[‚ÌŸ‚¿‚Æ‚·‚é
	cnt = GetPlayerCount();
	if (cnt <= 0)
	{
		return 2;	// ƒGƒlƒ~[‚ÌŸ‚¿
	}

	return 0;		// ƒQ[ƒ€Œp‘±
}


//=============================================================================
// ƒjƒ…[ƒQ[ƒ€‚©ƒ[ƒhƒQ[ƒ€‚©‚ðƒZƒbƒg‚·‚é
//=============================================================================
void SetLoadGame(BOOL flg)
{
	g_LoadGame = flg;
}


