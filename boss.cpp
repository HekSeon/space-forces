//=============================================================================
//
// ƒGƒlƒ~[ˆ— [enemy.cpp]
// Author : 
//
//=============================================================================
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "sound.h"
#include "boss.h"
#include "bossbullet.h"
#include "bossbullet2.h"

//*****************************************************************************
// ƒ}ƒNƒ’è‹`
//*****************************************************************************
#define TEXTURE_WIDTH				(650)	// ƒLƒƒƒ‰ƒTƒCƒY
#define TEXTURE_HEIGHT				(450)	
#define TEXTURE_MAX					(2)		// ƒeƒNƒXƒ`ƒƒ‚Ì”

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// ƒAƒjƒƒpƒ^[ƒ“‚ÌƒeƒNƒXƒ`ƒƒ“à•ªŠ„”iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// ƒAƒjƒƒpƒ^[ƒ“‚ÌƒeƒNƒXƒ`ƒƒ“à•ªŠ„”iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒpƒ^[ƒ“”
#define ANIM_WAIT					(4)		// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌØ‚è‘Ö‚í‚éWait’l


//*****************************************************************************
// ƒvƒƒgƒ^ƒCƒvéŒ¾
//*****************************************************************************
int shotTimer = 0;                // Ateş için zamanlayıcı (kare sayısı)
const int FIRE_RATE = 10;        // Her 600 karede bir ateş (10 saniye, çünkü 60 FPS * 10)
const int SHORT_FIRE_RATE = 120;

//*****************************************************************************
// ƒOƒ[ƒoƒ‹•Ï”
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ’¸“_î•ñ
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// ƒeƒNƒXƒ`ƒƒî•ñ

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/boss.png",
	"data/TEXTURE/bar_white.png",

};


static BOOL		g_Load = FALSE;			// ‰Šú‰»‚ðs‚Á‚½‚©‚Ìƒtƒ‰ƒO
static BOSS	g_Boss[BOSS_MAX];		// ƒGƒlƒ~[\‘¢‘Ì

static int		g_BossCount = BOSS_MAX;



//=============================================================================
// ‰Šú‰»ˆ—
//=============================================================================
HRESULT InitBoss(void)
{
	ID3D11Device* pDevice = GetDevice();


	//ƒeƒNƒXƒ`ƒƒ¶¬
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


	// ’¸“_ƒoƒbƒtƒ@¶¬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// ƒGƒlƒ~[\‘¢‘Ì‚Ì‰Šú‰»
	g_BossCount = 0;
	for (int i = 0; i < BOSS_MAX; i++)
	{


		if (g_Texture[i] == NULL) {
			OutputDebugString("Failed to load texture\n");
		}

		g_BossCount++;
		g_Boss[i].use = TRUE;
		g_Boss[i].pos = XMFLOAT3(960.0f, 350.0f, 0.0f);	// ’†S“_‚©‚ç•\Ž¦
		g_Boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Boss[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Boss[i].w = TEXTURE_WIDTH;
		g_Boss[i].h = TEXTURE_HEIGHT;
		g_Boss[i].texNo = 0;

		g_Boss[i].countAnim = 0;
		g_Boss[i].patternAnim = 0;

		g_Boss[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ˆÚ“®—Ê

		g_Boss[i].bosshp = BOSS_MAXHP;

	}



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// I—¹ˆ—
//=============================================================================
void UninitBoss(void)
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
// XVˆ—
//=============================================================================
void UpdateBoss(void)
{

	if (g_Load == FALSE) return;
	g_BossCount = 0;			// ¶‚«‚Ä‚éƒGƒlƒ~[‚Ì”

	for (int i = 0; i < BOSS_MAX; i++)
	{

		// ¶‚«‚Ä‚éƒGƒlƒ~[‚¾‚¯ˆ—‚ð‚·‚é
		if (g_Boss[i].use == TRUE)
		{

			// ’nŒ`‚Æ‚Ì“–‚½‚è”»’è—p‚ÉÀ•W‚ÌƒoƒbƒNƒAƒbƒv‚ðŽæ‚Á‚Ä‚¨‚­
			XMFLOAT3 pos_old = g_Boss[i].pos;



			// ˆÚ“®‚ªI‚í‚Á‚½‚çƒGƒlƒ~[‚Æ‚Ì“–‚½‚è”»’è
			{
				PLAYER* player = GetPlayer();


				// ƒGƒlƒ~[‚Ì”•ª“–‚½‚è”»’è‚ðs‚¤
				for (int j = 0; j < BOSS_MAX; j++)
				{
					// ¶‚«‚Ä‚éƒGƒlƒ~[‚Æ“–‚½‚è”»’è‚ð‚·‚é
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Boss[i].pos, g_Boss[i].w, g_Boss[i].h, player[j].pos, player[j].w, player[j].h);
						// “–‚½‚Á‚Ä‚¢‚éH
						if (ans == TRUE)
						{
							player[i].playerhp -= 50;
							g_Boss[i].bosshp -= 100;

							
						}
					}

				}
			}
		}

		if (g_Boss[i].bosshp <= 100)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}

	

	shotTimer++;

	// Eğer bosshp 5000'den küçük veya eşitse, daha sık ateş et
	if (g_Boss[0].bosshp <= 5000)
	{
		// Eğer 120 kare geçmişse (2 saniye) ateş et
		if (shotTimer >= SHORT_FIRE_RATE) // Changed to SHORT_FIRE_RATE
		{
			// Mermiyi ateşle
			SetBossBullet(g_Boss[0].pos);
			shotTimer = 0; // Zamanlayıcıyı sıfırla
		}

	}


	else if (g_Boss[0].bosshp <= 5000)
	{
		shotTimer++;
		if (shotTimer > 100) // 60 karede bir mermi atışı yapar (1 saniye için 60 kare varsayımı)
		{
			SetBossBullet2(g_Boss[0].pos); // Mermiyi düşmanın bulunduğu pozisyondan ateşle

			shotTimer = 0; // Sayaç sıfırlanır
		}

	}

	else
	{
		// Eğer 600 kare geçmişse (10 saniye) ateş et
		shotTimer++; // Her karede sayaç artırılır

		if (shotTimer > 90) // 60 karede bir mermi atışı yapar (1 saniye için 60 kare varsayımı)
		{
			SetBossBullet2(g_Boss[0].pos); // Mermiyi düşmanın bulunduğu pozisyondan ateşle

			shotTimer = 0; // Sayaç sıfırlanır
		}
	}


	

	// Her karede sayaç art?r?l?r

	//if (shotTimer >= 600) // 60 karede bir mermi at??? yapar (1 saniye için 60 kare varsay?m?)
	//{
	//	SetBossBullet(g_Boss[0].pos);
	//	shotTimer = 0;
	//}
	//shotTimer++;

	// ƒGƒlƒ~[‘S–Åƒ`ƒFƒbƒN

#ifdef _DEBUG	// ƒfƒoƒbƒOî•ñ‚ð•\Ž¦‚·‚é


#endif

}

//=============================================================================
// •`‰æˆ—
//=============================================================================
void DrawBoss(void)
{

	OutputDebugString("Drawing boss\n");

	// ’¸“_ƒoƒbƒtƒ@Ý’è
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);
	SetWorldViewProjection2D(); // Bu fonksiyonun çalıştığından emin olun

	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	for (int i = 0; i < BOSS_MAX; i++) {
		if (g_Boss[i].use == TRUE) {
			OutputDebugString("Drawing boss...\n");

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Boss[i].texNo]);

			float px = g_Boss[i].pos.x; // Burada bg'yi çıkardım, daha basit bir kontrol için
			float py = g_Boss[i].pos.y;
			float pw = g_Boss[i].w;
			float ph = g_Boss[i].h;

			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph,
				1.0f, 1.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Boss[i].rot.z);
			GetDeviceContext()->Draw(4, 0);
		}

		{
			// ‰º•~‚«‚ÌƒQ[ƒWi˜g“I‚È•¨j
			// ƒeƒNƒXƒ`ƒƒÝ’è
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			//ƒQ[ƒW‚ÌˆÊ’u‚âƒeƒNƒXƒ`ƒƒ[À•W‚ð”½‰f
			float px = g_Boss[0].pos.x +100;		// ƒQ[ƒW‚Ì•\Ž¦ˆÊ’uX
			float py = g_Boss[0].pos.y -250;		// ƒQ[ƒW‚Ì•\Ž¦ˆÊ’uY
			float pw = 100.0f;		// ƒQ[ƒW‚Ì•\Ž¦•
			float ph = 20.0f;		// ƒQ[ƒW‚Ì•\Ž¦‚‚³

			float tw = 25.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì•
			float th = 25.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì‚‚³
			float tx = 0.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì¶ãXÀ•W
			float ty = 0.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì¶ãYÀ•W

			// ‚P–‡‚Ìƒ|ƒŠƒSƒ“‚Ì’¸“_‚ÆƒeƒNƒXƒ`ƒƒÀ•W‚ðÝ’è
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

			// ƒ|ƒŠƒSƒ“•`‰æ
			GetDeviceContext()->Draw(4, 0);



			//ƒGƒlƒ~[‚Ì”‚É]‚Á‚ÄƒQ[ƒW‚Ì’·‚³‚ð•\Ž¦‚µ‚Ä‚Ý‚é
			//ƒeƒNƒXƒ`ƒƒÝ’è
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			//ƒQ[ƒW‚ÌˆÊ’u‚âƒeƒNƒXƒ`ƒƒ[À•W‚ð”½‰f
			pw = pw * ((float)g_Boss[0].bosshp / BOSS_MAXHP);

			// ‚P–‡‚Ìƒ|ƒŠƒSƒ“‚Ì’¸“_‚ÆƒeƒNƒXƒ`ƒƒÀ•W‚ðÝ’è
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			// ƒ|ƒŠƒSƒ“•`‰æ
			GetDeviceContext()->Draw(4, 0);


		}
	}
}
//=============================================================================
// Enemy\‘¢‘Ì‚Ìæ“ªƒAƒhƒŒƒX‚ðŽæ“¾
//=============================================================================
BOSS* GetBoss(void)
{
	return &g_Boss[0];
}

int GetBossCount(void)
{
	return g_BossCount;
}