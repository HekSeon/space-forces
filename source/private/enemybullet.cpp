//=============================================================================
//
// ƒoƒŒƒbƒgˆ— [bullet.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "enemybullet.h"
#include "collision.h"
#include "score.h"
#include "bg.h"
#include "effect.h"
#include "enemy.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// ƒ}ƒNƒ’è‹`
//*****************************************************************************
#define TEXTURE_WIDTH				(100/5)	// ƒLƒƒƒ‰ƒTƒCƒY
#define TEXTURE_HEIGHT				(100/5)	// 
#define TEXTURE_MAX					(1)		// ƒeƒNƒXƒ`ƒƒ‚Ì”

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// ƒAƒjƒƒpƒ^[ƒ“‚ÌƒeƒNƒXƒ`ƒƒ“à•ªŠ„”iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// ƒAƒjƒƒpƒ^[ƒ“‚ÌƒeƒNƒXƒ`ƒƒ“à•ªŠ„”iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒpƒ^[ƒ“”
#define ANIM_WAIT					(4)		// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌØ‚è‘Ö‚í‚éWait’l


//*****************************************************************************
// ƒvƒƒgƒ^ƒCƒvéŒ¾
//*****************************************************************************


//*****************************************************************************
// ƒOƒ[ƒoƒ‹•Ï”
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ’¸“_î•ñ
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// ƒeƒNƒXƒ`ƒƒî•ñ

static char* g_TexturName[] = {
	"data/TEXTURE/enemybullet.png",
};

static BOOL		g_Load = FALSE;			// ‰Šú‰»‚ðs‚Á‚½‚©‚Ìƒtƒ‰ƒO
static EBULLET	g_EBullet[EBULLET_MAX];	// ƒoƒŒƒbƒg\‘¢‘Ì





//=============================================================================
// ‰Šú‰»ˆ—
//=============================================================================
HRESULT InitEBullet(void)
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


	// ƒoƒŒƒbƒg\‘¢‘Ì‚Ì‰Šú‰»
	for (int i = 0; i < EBULLET_MAX; i++)
	{
		g_EBullet[i].use = FALSE;			// –¢Žg—pi”­ŽË‚³‚ê‚Ä‚¢‚È‚¢’ej
		g_EBullet[i].w = TEXTURE_WIDTH;
		g_EBullet[i].h = TEXTURE_HEIGHT;
		g_EBullet[i].pos = XMFLOAT3(300, 300.0f, 0.0f);
		g_EBullet[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_EBullet[i].texNo = 0;

		g_EBullet[i].countAnim = 0;
		g_EBullet[i].patternAnim = 0;

		g_EBullet[i].move = XMFLOAT3(0.0f, -EBULLET_SPEED, 0.0f);	// ˆÚ“®—Ê‚ð‰Šú‰»
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// I—¹ˆ—
//=============================================================================
void UninitEBullet(void)
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

}

//=============================================================================
// XVˆ—
//=============================================================================
void UpdateEBullet(void)
{

	if (g_Load == FALSE) return;
	int bulletCount = 0;				// ˆ—‚µ‚½ƒoƒŒƒbƒg‚Ì”

	for (int i = 0; i < EBULLET_MAX; i++)
	{
		if (g_EBullet[i].use == TRUE)	// ‚±‚ÌƒoƒŒƒbƒg‚ªŽg‚í‚ê‚Ä‚¢‚éH
		{								// Yes
			// ƒAƒjƒ[ƒVƒ‡ƒ“  
			g_EBullet[i].countAnim++;
			if ((g_EBullet[i].countAnim % ANIM_WAIT) == 0)
			{
				// ƒpƒ^[ƒ“‚ÌØ‚è‘Ö‚¦
				g_EBullet[i].patternAnim = (g_EBullet[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			// ƒoƒŒƒbƒg‚ÌˆÚ“®ˆ—
			XMVECTOR pos = XMLoadFloat3(&g_EBullet[i].pos);
			XMVECTOR move = XMLoadFloat3(&g_EBullet[i].move);
			pos += move;
			XMStoreFloat3(&g_EBullet[i].pos, pos);
		

			// ‰æ–ÊŠO‚Ü‚Åi‚ñ‚¾H
			BG* bg = GetBG();
			if (g_EBullet[i].pos.y < (-g_EBullet[i].h / 2))		// Ž©•ª‚Ì‘å‚«‚³‚ðl—¶‚µ‚Ä‰æ–ÊŠO‚©”»’è‚µ‚Ä‚¢‚é
			{
				g_EBullet[i].use = false;
			}
			if (g_EBullet[i].pos.y > (bg->h - g_EBullet[i].h / 2))	// Ž©•ª‚Ì‘å‚«‚³‚ðl—¶‚µ‚Ä‰æ–ÊŠO‚©”»’è‚µ‚Ä‚¢‚é
			{
				g_EBullet[i].use = false;
			}

			

			// “–‚½‚è”»’èˆ—
			{
				PLAYER* player = GetPlayer();

				// ƒGƒlƒ~[‚Ì”•ª“–‚½‚è”»’è‚ðs‚¤
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// ¶‚«‚Ä‚éƒGƒlƒ~[‚Æ“–‚½‚è”»’è‚ð‚·‚é
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_EBullet[i].pos, g_EBullet[i].w, g_EBullet[i].h,
							player[j].pos, player[j].w, player[j].h);
						// “–‚½‚Á‚Ä‚¢‚éH
						if (ans == TRUE)
						{
							g_EBullet[i].use = FALSE;
							g_EBullet[j].use = FALSE;


							player[j].playerhp -= 10;

							if (player[j].playerhp <= 0)
							{
								player[j].use = FALSE;
								
							} 
							// “–‚½‚Á‚½Žž‚Ìˆ—
							if (player[j].use == FALSE)
							{
								SetFade(FADE_OUT,MODE_RESULT);
							}

			

							// ƒGƒtƒFƒNƒg”­¶
							SetEffect(player[j].pos.x, player[j].pos.y, 30);
							
						}
					}
					
				}
			}

			PlaySound(SOUND_LABEL_SE_lazer);
			bulletCount++;
		}
	}


}

//=============================================================================
// •`‰æˆ—
//=============================================================================
void DrawEBullet(void)
{
	// ’¸“_ƒoƒbƒtƒ@Ý’è
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// ƒ}ƒgƒŠƒNƒXÝ’è
	SetWorldViewProjection2D();

	// ƒvƒŠƒ~ƒeƒBƒuƒgƒ|ƒƒWÝ’è
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ƒ}ƒeƒŠƒAƒ‹Ý’è
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG* bg = GetBG();

	for (int i = 0; i < EBULLET_MAX; i++)
	{
		if (g_EBullet[i].use == TRUE)		// ‚±‚ÌƒoƒŒƒbƒg‚ªŽg‚í‚ê‚Ä‚¢‚éH
		{									// Yes
			// ƒeƒNƒXƒ`ƒƒÝ’è
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_EBullet[i].texNo]);

			//ƒoƒŒƒbƒg‚ÌˆÊ’u‚âƒeƒNƒXƒ`ƒƒ[À•W‚ð”½‰f
			float px = g_EBullet[i].pos.x - bg->pos.x;	// ƒoƒŒƒbƒg‚Ì•\Ž¦ˆÊ’uX
			float py = g_EBullet[i].pos.y - bg->pos.y;	// ƒoƒŒƒbƒg‚Ì•\Ž¦ˆÊ’uY
			float pw = g_EBullet[i].w;		// ƒoƒŒƒbƒg‚Ì•\Ž¦•
			float ph = g_EBullet[i].h;		// ƒoƒŒƒbƒg‚Ì•\Ž¦‚‚³

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// ƒeƒNƒXƒ`ƒƒ‚Ì•
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// ƒeƒNƒXƒ`ƒƒ‚Ì‚‚³
			float tx = (float)(g_EBullet[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// ƒeƒNƒXƒ`ƒƒ‚Ì¶ãXÀ•W
			float ty = (float)(g_EBullet[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// ƒeƒNƒXƒ`ƒƒ‚Ì¶ãYÀ•W

			// ‚P–‡‚Ìƒ|ƒŠƒSƒ“‚Ì’¸“_‚ÆƒeƒNƒXƒ`ƒƒÀ•W‚ðÝ’è
			SetSpriteColorRotation(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_EBullet[i].rot.z);

			// ƒ|ƒŠƒSƒ“•`‰æ
			GetDeviceContext()->Draw(4, 0);
		}
	}

}


//=============================================================================
// ƒoƒŒƒbƒg\‘¢‘Ì‚Ìæ“ªƒAƒhƒŒƒX‚ðŽæ“¾
//=============================================================================
EBULLET* GetEBullet(void)
{
	return &g_EBullet[0];
}


//=============================================================================
// ƒoƒŒƒbƒg‚Ì”­ŽËÝ’è
//=============================================================================
//void SetEBullet(XMFLOAT3 pos)
//{
//	// ‚à‚µ–¢Žg—p‚Ì’e‚ª–³‚©‚Á‚½‚ç”­ŽË‚µ‚È‚¢( =‚±‚êˆÈãŒ‚‚Ä‚È‚¢‚Á‚ÄŽ– )
//	for (int i = 0; i < EBULLET_MAX; i++)
//	{
//		if (g_EBullet[i].use == FALSE)		// –¢Žg—pó‘Ô‚ÌƒoƒŒƒbƒg‚ðŒ©‚Â‚¯‚é
//		{
//			g_EBullet[i].use = TRUE;			// Žg—pó‘Ô‚Ö•ÏX‚·‚é
//			g_EBullet[i].pos = pos;			// À•W‚ðƒZƒbƒg
//			return;							// 1”­ƒZƒbƒg‚µ‚½‚Ì‚ÅI—¹‚·‚é
//		}
//	}
//}

void SetEBullet(XMFLOAT3 enemyPos)
{
	PLAYER* player = GetPlayer();

	// İlk boş mermiyi bul
	for (int i = 0; i < EBULLET_MAX; i++)
	{
		if (g_EBullet[i].use == FALSE) // Boş mermi bulduysanız
		{
			g_EBullet[i].use = TRUE;   // Mermiyi aktif hale getir
			g_EBullet[i].pos = enemyPos; // Düşmanın konumunu mermiye ata

			// Yön vektörünü hesapla
			XMFLOAT3 direction;
			direction.x = player->pos.x - enemyPos.x;
			direction.y = player->pos.y - enemyPos.y;
			direction.z = 0.0f; // 2D oyunda z eksenini kullanmıyorsanız 0 yapın

			// Yön vektörünü normalize et
			XMVECTOR dirVec = XMLoadFloat3(&direction);
			dirVec = XMVector3Normalize(dirVec);
			XMStoreFloat3(&direction, dirVec);

			// Mermi hızını ayarla
			float bulletSpeed = EBULLET_SPEED;
			g_EBullet[i].move.x = direction.x * bulletSpeed;
			g_EBullet[i].move.y = direction.y * bulletSpeed;
			g_EBullet[i].move.z = direction.z * bulletSpeed;

			return; // Mermi ateşlendikten sonra işlemi bitir
		}
	}
}
