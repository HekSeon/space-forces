//=============================================================================
//
// ƒXƒvƒ‰ƒCƒgˆ— [sprite.cpp]
// Author : GP11B132 99 ŠO‰ª‚–¾
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "sprite.h"

//*****************************************************************************
// ƒ}ƒNƒ’è‹`
//*****************************************************************************


//*****************************************************************************
// ƒvƒƒgƒ^ƒCƒvéŒ¾
//*****************************************************************************
#define TEXTURE_WIDTH				(1920)			// (SCREEN_WIDTH)	// ”wŒiƒTƒCƒY
#define TEXTURE_HEIGHT				(1080)			// (SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(3)				// ƒeƒNƒXƒ`ƒƒ‚Ì”

#define TEXTURE_WIDTH_LOGO			(480)			// ƒƒSƒTƒCƒY
#define TEXTURE_HEIGHT_LOGO			(80)

//*****************************************************************************
// ƒOƒ[ƒoƒ‹•Ï”
//*****************************************************************************

static ID3D11Buffer* g_VertexBuffer = NULL;

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// ƒeƒNƒXƒ`ƒƒî•ñ

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/start.png",
	"data/TEXTURE/sky000.png",
	"data/TEXTURE/sky001.png",
};

static MENU	g_menu;

//=============================================================================
// ’¸“_ƒf[ƒ^Ý’è
//=============================================================================

void SetSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// ƒRƒ“ƒsƒ…[ƒ^[‚ÍŠ„‚èŽZ‚ª‹êŽè
	hh = Height * 0.5f;		// Š|‚¯ŽZ‚Ì•û‚ªˆ—‚ª‘¬‚¢

	// Žw’è‚³‚ê‚½À•W‚ð’†S‚ÉÝ’è‚·‚éƒvƒƒOƒ‰ƒ€

	// ’¸“_‚O”Ôi¶ã‚Ì’¸“_j
	vertex[0].Position = XMFLOAT3(X - hw, Y - hh, 0.0f);
	vertex[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(U, V);

	// ’¸“_‚P”Ôi‰Eã‚Ì’¸“_j
	vertex[1].Position = XMFLOAT3(X + hw, Y - hh, 0.0f);
	vertex[1].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	// ’¸“_‚Q”Ôi¶‰º‚Ì’¸“_j
	vertex[2].Position = XMFLOAT3(X - hw, Y + hh, 0.0f);
	vertex[2].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	// ’¸“_‚R”Ôi‰E‰º‚Ì’¸“_j
	vertex[3].Position = XMFLOAT3(X + hw, Y + hh, 0.0f);
	vertex[3].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	// /*¶ã‚ðŒ´“_‚Æ‚µ‚ÄÝ’è‚·‚éƒvƒƒOƒ‰ƒ€*/
	//vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	//vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[0].TexCoord = D3DXVECTOR2(U, V);

	//vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	//vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[1].TexCoord = D3DXVECTOR2(U + UW, V);

	//vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	//vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[2].TexCoord = D3DXVECTOR2(U, V + VH);

	//vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	//vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[3].TexCoord = D3DXVECTOR2(U + UW, V + VH);



	GetDeviceContext()->Unmap(buf, 0);

}


void SetSpriteLeftTop(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	// ¶ã‚ðŒ´“_‚Æ‚µ‚ÄÝ’è‚·‚éƒvƒƒOƒ‰ƒ€
	vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	vertex[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(U, V);

	vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	vertex[1].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	vertex[2].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	vertex[3].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);



	GetDeviceContext()->Unmap(buf, 0);

}


// Žw’èÀ•W‚ð¶ã‚Æ‚µ‚Ä•`‰æ‚·‚é
void SetSpriteLTColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// ¶ã‚ðŒ´“_‚Æ‚µ‚ÄÝ’è‚·‚éƒvƒƒOƒ‰ƒ€
	vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}


void SetSpriteColor(ID3D11Buffer *buf, float X, float Y, float Width, float Height,
		float U, float V, float UW, float VH,
		XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// ƒRƒ“ƒsƒ…[ƒ^[‚ÍŠ„‚èŽZ‚ª‹êŽè
	hh = Height * 0.5f;		// Š|‚¯ŽZ‚Ì•û‚ªˆ—‚ª‘¬‚¢

	// Žw’è‚³‚ê‚½À•W‚ð’†S‚ÉÝ’è‚·‚éƒvƒƒOƒ‰ƒ€

	// ’¸“_‚O”Ôi¶ã‚Ì’¸“_j
	vertex[0].Position = XMFLOAT3(X - hw, Y - hh, 0.0f);
	vertex[0].Diffuse  = color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	// ’¸“_‚P”Ôi‰Eã‚Ì’¸“_j
	vertex[1].Position = XMFLOAT3(X + hw, Y - hh, 0.0f);
	vertex[1].Diffuse  = color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	// ’¸“_‚Q”Ôi¶‰º‚Ì’¸“_j
	vertex[2].Position = XMFLOAT3(X - hw, Y + hh, 0.0f);
	vertex[2].Diffuse  = color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	// ’¸“_‚R”Ôi‰E‰º‚Ì’¸“_j
	vertex[3].Position = XMFLOAT3(X + hw, Y + hh, 0.0f);
	vertex[3].Diffuse  = color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}


void SetSpriteColorRotation(ID3D11Buffer *buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = Width * 0.5f;		// ƒRƒ“ƒsƒ…[ƒ^[‚ÍŠ„‚èŽZ‚ª‹êŽè
	hh = Height * 0.5f;		// Š|‚¯ŽZ‚Ì•û‚ªˆ—‚ª‘¬‚¢

	// Žw’è‚³‚ê‚½À•W‚ð’†S‚É‰ñ“]‚·‚éƒvƒƒOƒ‰ƒ€
	float BaseAngle = atan2f(hh, hw);			// ’†S“_‚©‚ç’¸“_‚É‘Î‚·‚éŠp“x
	XMVECTOR temp = { hw, hh, 0.0f, 0.0f };
	temp = XMVector2Length(temp);				// ’†S“_‚©‚ç’¸“_‚É‘Î‚·‚é‹——£
	float Radius = 0.0f;
	XMStoreFloat(&Radius, temp);

	// ‚±‚±‚ÅƒAƒtƒBƒ“•ÏŠ·isincos‚Ì‚â‚Âj‚ðŽg‚Á‚Ä4’¸“_‚ð‰ñ“]‚³‚¹‚é
	float x = X - cosf(BaseAngle + Rot) * Radius;
	float y = Y - sinf(BaseAngle + Rot) * Radius;
	vertex[0].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle - Rot) * Radius;
	y = Y - sinf(BaseAngle - Rot) * Radius;
	vertex[1].Position = XMFLOAT3(x, y, 0.0f);

	x = X - cosf(BaseAngle - Rot) * Radius;
	y = Y + sinf(BaseAngle - Rot) * Radius;
	vertex[2].Position = XMFLOAT3(x, y, 0.0f);

	x = X + cosf(BaseAngle + Rot) * Radius;
	y = Y + sinf(BaseAngle + Rot) * Radius;
	vertex[3].Position = XMFLOAT3(x, y, 0.0f);

	vertex[0].Diffuse = Color;
	vertex[1].Diffuse = Color;
	vertex[2].Diffuse = Color;
	vertex[3].Diffuse = Color;

	vertex[0].TexCoord = XMFLOAT2(U, V);
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);

	GetDeviceContext()->Unmap(buf, 0);

}

void ShowMainMenuOptions() {

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

	g_menu.w = TEXTURE_WIDTH/4;
	g_menu.h = TEXTURE_HEIGHT/4;
	g_menu.pos = XMFLOAT3(960.0f, 200.0f, 0.0f);
	g_menu.texNo = 0;
	
}

MENU* GetMENU(void)
{
	return &g_menu;
}