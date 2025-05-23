//=============================================================================
//
// ƒGƒlƒ~[ˆ— [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "enemybullet.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "sound.h"

//*****************************************************************************
// ƒ}ƒNƒ’è‹`
//*****************************************************************************
#define TEXTURE_WIDTH				(200/2)	// ƒLƒƒƒ‰ƒTƒCƒY
#define TEXTURE_HEIGHT				(200/2)	// 
#define TEXTURE_MAX					(2)		// ƒeƒNƒXƒ`ƒƒ‚Ì”

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
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ’¸“_î•ñ
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// ƒeƒNƒXƒ`ƒƒî•ñ

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/enemy00.png",
	
};


static BOOL		g_Load = FALSE;			// ‰Šú‰»‚ðs‚Á‚½‚©‚Ìƒtƒ‰ƒO
static ENEMY	g_Enemy[ENEMY_MAX];		// ƒGƒlƒ~[\‘¢‘Ì

static int		g_EnemyCount = ENEMY_MAX;

static PLAYER		playerhp[PLAYER_MAXHP];

static int shotTimer = 0;

//static INTERPOLATION_DATA g_MoveTbl0[] = {
//	//À•W									‰ñ“]—¦							Šg‘å—¦					ŽžŠÔ
//	{ XMFLOAT3(50.0f,  100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
//	{ XMFLOAT3(250.0f, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 3.14f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	60 },
//	{ XMFLOAT3(50.0f, 350.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(2.0f, 2.0f, 2.0f),	60 },
//	{ XMFLOAT3(150.0f,  100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 5.14f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	60 },
//	{ XMFLOAT3(300.0f,  300.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 3.14f),	XMFLOAT3(-1.0f, -1.0f, -1.0f),	60 },
//
//
//};
//
//
//static INTERPOLATION_DATA g_MoveTbl1[] = {
//	//À•W									‰ñ“]—¦							Šg‘å—¦							ŽžŠÔ
//	{ XMFLOAT3(1700.0f,   0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
//	{ XMFLOAT3(1700.0f,  SCREEN_HEIGHT, 0.0f),XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },
//};
//
//
//static INTERPOLATION_DATA g_MoveTbl2[] = {
//	//À•W									‰ñ“]—¦							Šg‘å—¦							ŽžŠÔ
//	{ XMFLOAT3(3000.0f, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
//	{ XMFLOAT3(3000 + SCREEN_WIDTH, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
//};
//
//
//static INTERPOLATION_DATA* g_MoveTblAdr[] =
//{
//	g_MoveTbl0,
//	g_MoveTbl1,
//	g_MoveTbl2,
//
//};


//=============================================================================
// ‰Šú‰»ˆ—
//=============================================================================
HRESULT InitEnemy(void)
{
	ID3D11Device *pDevice = GetDevice();

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
	g_EnemyCount = 0;
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		
		g_EnemyCount++;
		g_Enemy[i].use = TRUE;
		g_Enemy[i].pos = XMFLOAT3(100.0f + i * 200.0f, 150.0f, 0.0f);	// ’†S“_‚©‚ç•\Ž¦
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].w = TEXTURE_WIDTH;
		g_Enemy[i].h = TEXTURE_HEIGHT;
		g_Enemy[i].texNo = 0;

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].move = XMFLOAT3(4.0f, 0.0f, 0.0f);		// ˆÚ“®—Ê

		g_Enemy[i].time = 0.0f;			// üŒ`•âŠÔ—p‚Ìƒ^ƒCƒ}[‚ðƒNƒŠƒA
		g_Enemy[i].tblNo = 0;			// Ä¶‚·‚és“®ƒf[ƒ^ƒe[ƒuƒ‹No‚ðƒZƒbƒg
		g_Enemy[i].tblMax = 0;			// Ä¶‚·‚és“®ƒf[ƒ^ƒe[ƒuƒ‹‚ÌƒŒƒR[ƒh”‚ðƒZƒbƒg

	}

	// 0”Ô‚¾‚¯üŒ`•âŠÔ‚Å“®‚©‚µ‚Ä‚Ý‚é
	//g_Enemy[0].time = 0.0f;		// üŒ`•âŠÔ—p‚Ìƒ^ƒCƒ}[‚ðƒNƒŠƒA
	//g_Enemy[0].tblNo = 0;		// Ä¶‚·‚éƒAƒjƒƒf[ƒ^‚Ìæ“ªƒAƒhƒŒƒX‚ðƒZƒbƒg
	//g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// Ä¶‚·‚éƒAƒjƒƒf[ƒ^‚ÌƒŒƒR[ƒh”‚ðƒZƒbƒg

	//// 1”Ô‚¾‚¯üŒ`•âŠÔ‚Å“®‚©‚µ‚Ä‚Ý‚é
	//g_Enemy[1].time = 0.0f;		// üŒ`•âŠÔ—p‚Ìƒ^ƒCƒ}[‚ðƒNƒŠƒA
	//g_Enemy[1].tblNo = 1;		// Ä¶‚·‚éƒAƒjƒƒf[ƒ^‚Ìæ“ªƒAƒhƒŒƒX‚ðƒZƒbƒg
	//g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// Ä¶‚·‚éƒAƒjƒƒf[ƒ^‚ÌƒŒƒR[ƒh”‚ðƒZƒbƒg

	//// 2”Ô‚¾‚¯üŒ`•âŠÔ‚Å“®‚©‚µ‚Ä‚Ý‚é
	//g_Enemy[2].time = 0.0f;		// üŒ`•âŠÔ—p‚Ìƒ^ƒCƒ}[‚ðƒNƒŠƒA
	//g_Enemy[2].tblNo = 2;		// Ä¶‚·‚éƒAƒjƒƒf[ƒ^‚Ìæ“ªƒAƒhƒŒƒX‚ðƒZƒbƒg
	//g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// Ä¶‚·‚éƒAƒjƒƒf[ƒ^‚ÌƒŒƒR[ƒh”‚ðƒZƒbƒg

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// I—¹ˆ—
//=============================================================================
void UninitEnemy(void)
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
void UpdateEnemy(void)
{

	if (g_Load == FALSE) return;
	g_EnemyCount = 0;			// ¶‚«‚Ä‚éƒGƒlƒ~[‚Ì”

	

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		float speed = g_Enemy[i].move.x;

		speed = 2;

		

		// ¶‚«‚Ä‚éƒGƒlƒ~[‚¾‚¯ˆ—‚ð‚·‚é
		if (g_Enemy[i].use == TRUE)
		{
			g_EnemyCount++;		// ¶‚«‚Ä‚½“G‚Ì”

		

			// ’nŒ`‚Æ‚Ì“–‚½‚è”»’è—p‚ÉÀ•W‚ÌƒoƒbƒNƒAƒbƒv‚ðŽæ‚Á‚Ä‚¨‚­
			XMFLOAT3 pos_old = g_Enemy[i].pos;

			// ƒAƒjƒ[ƒVƒ‡ƒ“  
			g_Enemy[i].countAnim += 1.0f;
			if (g_Enemy[i].countAnim > ANIM_WAIT)
			{
				g_Enemy[i].countAnim = 0.0f;
				// ƒpƒ^[ƒ“‚ÌØ‚è‘Ö‚¦
				g_Enemy[i].patternAnim = (g_Enemy[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			

				PLAYER* player = GetPlayer();
				// Yön vektörünü hesapla
				XMFLOAT2 direction;
				direction.x = player->pos.x - g_Enemy[i].pos.x;
				direction.y = player->pos.y - g_Enemy[i].pos.y;


				// Yön vektörünü normalize et
				float length = sqrt(direction.x * direction.x + direction.y * direction.y);
				if (length != 0) {
					direction.x /= length;
					direction.y /= length;
				}




				// Oyuncuya bakacak açıyı hesapla
				
				// Düşmanın dönüşünü ayarla (grafik framework'ünüze bağlı olarak bunu uyarlamanız gerekebilir)

				float targetAngle = atan2(direction.y, direction.x);

				direction.x* speed;
				direction.y* speed;

				g_Enemy[i].rot.z = 0.9f * g_Enemy[i].rot.z + 0.1f * targetAngle;


				g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, targetAngle);

				// Pozisyonu güncelle veya diğer düşman mantığı
				// ...
				


					g_Enemy[i].pos.x += g_Enemy[i].move.x; 

					// MAPŠOƒ`ƒFƒbƒN
					BG* bg = GetBG();

					if (g_Enemy[i].pos.x < 0.0f)
					{
						g_Enemy[i].pos.x = 0.0f;
						g_Enemy[i].move.x *= -1.0f;
					}


					if (g_Enemy[i].pos.x > bg->w)
					{
						g_Enemy[i].pos.x = bg->w;
						g_Enemy[i].move.x *= -1.0f;
					}

					if (g_Enemy[i].pos.y < 0.0f)
					{
						g_Enemy[i].pos.y = 0.0f;
					}

					if (g_Enemy[i].pos.y > bg->h)
					{
						g_Enemy[i].pos.y = bg->h;
					}


					const int ENEMY_PER_ROW = 5;
					const float ENEMY_ROW_HEIGHT = 150.0f; // Satırlar arasındaki yükseklik
					int rowIndex = i / ENEMY_PER_ROW; // Düşmanın bulunduğu satır indeksi
					float targetY = 150.0f + rowIndex * ENEMY_ROW_HEIGHT; // Satır yüksekliğine göre y konumu
					if (fabs(g_Enemy[i].pos.y - targetY) > 1.0f)
					{
						g_Enemy[i].pos.y = targetY; // Satır pozisyonuna ayarla
					}

					if (g_Enemy[i].pos.x <= 0.0f || g_Enemy[i].pos.x >= GetBG()->w)
					{

						// Eğer düşman bir satırın sonuna gelirse, bir alt satıra geç
						for (int j = 0; j < ENEMY_MAX; j++)
						{
							if (g_Enemy[j].use == TRUE && g_Enemy[j].pos.x >= g_Enemy[i].pos.x)
							{
								g_Enemy[j].pos.y += ENEMY_ROW_HEIGHT; // Satır yüksekliği kadar aşağı kaydır
							}
						}
					}

				

				
			

			// ˆÚ“®ˆ—
			if (g_Enemy[i].tblMax > 0)	// üŒ`•âŠÔ‚ðŽÀs‚·‚éH
			{	// üŒ`•âŠÔ‚Ìˆ—
				int nowNo = (int)g_Enemy[i].time;			// ®”•ª‚Å‚ ‚éƒe[ƒuƒ‹”Ô†‚ðŽæ‚èo‚µ‚Ä‚¢‚é
				int maxNo = g_Enemy[i].tblMax;				// “o˜^ƒe[ƒuƒ‹”‚ð”‚¦‚Ä‚¢‚é
				int nextNo = (nowNo + 1) % maxNo;			// ˆÚ“®æƒe[ƒuƒ‹‚Ì”Ô†‚ð‹‚ß‚Ä‚¢‚é
				/*INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];*/	// s“®ƒe[ƒuƒ‹‚ÌƒAƒhƒŒƒX‚ðŽæ“¾
				//
				//XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR‚Ö•ÏŠ·
				//XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR‚Ö•ÏŠ·
				//XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR‚Ö•ÏŠ·
				//
				//XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZˆÚ“®—Ê‚ðŒvŽZ‚µ‚Ä‚¢‚é
				//XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ‰ñ“]—Ê‚ðŒvŽZ‚µ‚Ä‚¢‚é
				//XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZŠg‘å—¦‚ðŒvŽZ‚µ‚Ä‚¢‚é

				float nowTime = g_Enemy[i].time - nowNo;	// ŽžŠÔ•”•ª‚Å‚ ‚é­”‚ðŽæ‚èo‚µ‚Ä‚¢‚é

				//Pos *= nowTime;								// Œ»Ý‚ÌˆÚ“®—Ê‚ðŒvŽZ‚µ‚Ä‚¢‚é
				//Rot *= nowTime;								// Œ»Ý‚Ì‰ñ“]—Ê‚ðŒvŽZ‚µ‚Ä‚¢‚é
				//Scl *= nowTime;								// Œ»Ý‚ÌŠg‘å—¦‚ðŒvŽZ‚µ‚Ä‚¢‚é

			//	// ŒvŽZ‚µ‚Ä‹‚ß‚½ˆÚ“®—Ê‚ðŒ»Ý‚ÌˆÚ“®ƒe[ƒuƒ‹XYZ‚É‘«‚µ‚Ä‚¢‚é•\Ž¦À•W‚ð‹‚ß‚Ä‚¢‚é
			//	XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);

			//	// ŒvŽZ‚µ‚Ä‹‚ß‚½‰ñ“]—Ê‚ðŒ»Ý‚ÌˆÚ“®ƒe[ƒuƒ‹‚É‘«‚µ‚Ä‚¢‚é
			//	XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

			//	// ŒvŽZ‚µ‚Ä‹‚ß‚½Šg‘å—¦‚ðŒ»Ý‚ÌˆÚ“®ƒe[ƒuƒ‹‚É‘«‚µ‚Ä‚¢‚é
			//	XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);
			//	g_Enemy[i].w = TEXTURE_WIDTH * g_Enemy[i].scl.x;
			//	g_Enemy[i].h = TEXTURE_HEIGHT * g_Enemy[i].scl.y;

			//	// frame‚ðŽg‚ÄŽžŠÔŒo‰ßˆ—‚ð‚·‚é
			//	g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// ŽžŠÔ‚ði‚ß‚Ä‚¢‚é
			//	if ((int)g_Enemy[i].time >= maxNo)			// “o˜^ƒe[ƒuƒ‹ÅŒã‚Ü‚ÅˆÚ“®‚µ‚½‚©H
			//	{
			//		g_Enemy[i].time -= maxNo;				// ‚O”Ô–Ú‚ÉƒŠƒZƒbƒg‚µ‚Â‚Â‚à¬”•”•ª‚ðˆø‚«Œp‚¢‚Å‚¢‚é
			//	}

			//}

			// ˆÚ“®‚ªI‚í‚Á‚½‚çƒGƒlƒ~[‚Æ‚Ì“–‚½‚è”»’è
				{
					PLAYER* player = GetPlayer();

					// ƒGƒlƒ~[‚Ì”•ª“–‚½‚è”»’è‚ðs‚¤
					for (int j = 0; j < ENEMY_MAX; j++)
					{
						// ¶‚«‚Ä‚éƒGƒlƒ~[‚Æ“–‚½‚è”»’è‚ð‚·‚é
						if (player[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,player[j].pos, player[j].w, player[j].h);
							// “–‚½‚Á‚Ä‚¢‚éH
							if (ans == TRUE)
							{
								player[i].playerhp -= 50;
							}
						}
						if (g_EnemyCount <= 0)
						{
							SetFade(FADE_OUT,MODE_BOSS);
						}
						
					}
				}
			}
		}

		shotTimer++; // Her karede sayaç artırılır

		if (shotTimer > 120) // 60 karede bir mermi atışı yapar (1 saniye için 60 kare varsayımı)
		{
			SetEBullet(g_Enemy[i].pos); // Mermiyi düşmanın bulunduğu pozisyondan ateşle
			
			shotTimer = 0; // Sayaç sıfırlanır
		}


		// ƒGƒlƒ~[‘S–Åƒ`ƒFƒbƒN

#ifdef _DEBUG	// ƒfƒoƒbƒOî•ñ‚ð•\Ž¦‚·‚é


#endif

	}
}

	//=============================================================================
	// •`‰æˆ—
	//=============================================================================
	void DrawEnemy(void)
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

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (g_Enemy[i].use == TRUE)			// ‚±‚ÌƒGƒlƒ~[‚ªŽg‚í‚ê‚Ä‚¢‚éH
			{									// Yes
				// ƒeƒNƒXƒ`ƒƒÝ’è
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].texNo]);

				//ƒGƒlƒ~[‚ÌˆÊ’u‚âƒeƒNƒXƒ`ƒƒ[À•W‚ð”½‰f
				float px = g_Enemy[i].pos.x - bg->pos.x;	// ƒGƒlƒ~[‚Ì•\Ž¦ˆÊ’uX
				float py = g_Enemy[i].pos.y - bg->pos.y;	// ƒGƒlƒ~[‚Ì•\Ž¦ˆÊ’uY
				float pw = g_Enemy[i].w;		// ƒGƒlƒ~[‚Ì•\Ž¦•
				float ph = g_Enemy[i].h;		// ƒGƒlƒ~[‚Ì•\Ž¦‚‚³

				// ƒAƒjƒ[ƒVƒ‡ƒ“—p
				//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// ƒeƒNƒXƒ`ƒƒ‚Ì•
				//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// ƒeƒNƒXƒ`ƒƒ‚Ì‚‚³
				//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// ƒeƒNƒXƒ`ƒƒ‚Ì¶ãXÀ•W
				//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// ƒeƒNƒXƒ`ƒƒ‚Ì¶ãYÀ•W

				float tw = 1.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì•
				float th = 1.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì‚‚³
				float tx = 0.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì¶ãXÀ•W
				float ty = 0.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì¶ãYÀ•W

				// ‚P–‡‚Ìƒ|ƒŠƒSƒ“‚Ì’¸“_‚ÆƒeƒNƒXƒ`ƒƒÀ•W‚ðÝ’è
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					g_Enemy[i].rot.z);

				// ƒ|ƒŠƒSƒ“•`‰æ
				GetDeviceContext()->Draw(4, 0);
			}
		}


		// ƒQ[ƒW‚ÌƒeƒXƒg
		{
			// ‰º•~‚«‚ÌƒQ[ƒWi˜g“I‚È•¨j
			// ƒeƒNƒXƒ`ƒƒÝ’è
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			PLAYER* player = GetPlayer();
			//ƒQ[ƒW‚ÌˆÊ’u‚âƒeƒNƒXƒ`ƒƒ[À•W‚ð”½‰f
			float px = player[0].pos.x + 70;		// ƒQ[ƒW‚Ì•\Ž¦ˆÊ’uX
			float py = player[0].pos.y - 50;		// ƒQ[ƒW‚Ì•\Ž¦ˆÊ’uY
			float pw = 100.0f;		// ƒQ[ƒW‚Ì•\Ž¦•
			float ph = 20.0f;		// ƒQ[ƒW‚Ì•\Ž¦‚‚³

			float tw = 1.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì•
			float th = 1.0f;	// ƒeƒNƒXƒ`ƒƒ‚Ì‚‚³
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
			pw = pw * ((float)player[0].playerhp / PLAYER_MAXHP);

			// ‚P–‡‚Ìƒ|ƒŠƒSƒ“‚Ì’¸“_‚ÆƒeƒNƒXƒ`ƒƒÀ•W‚ðÝ’è
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			// ƒ|ƒŠƒSƒ“•`‰æ
			GetDeviceContext()->Draw(4, 0);


		}




}


//=============================================================================
// Enemy\‘¢‘Ì‚Ìæ“ªƒAƒhƒŒƒX‚ðŽæ“¾
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}


// ¶‚«‚Ä‚éƒGƒlƒ~[‚Ì”
int GetEnemyCount(void)
{
	return g_EnemyCount;
}


