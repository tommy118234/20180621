//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPolygon(void);
void SetVertexPolygon(void);
void SetTexturePolygon( int cntPattern );	//

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePolygon = NULL;		// テクスチャへのポリゴン

VERTEX_2D				g_vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク

D3DXVECTOR3				g_posPolygon;						// ポリゴンの移動量
D3DXVECTOR3				g_rotPolygon;						// ポリゴンの回転量
int						g_nCountAnim;						// アニメーションカウント
int						g_nPatternAnim;						// アニメーションパターンナンバー

int						direction = -1;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posPolygon = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X/2, SCREEN_CENTER_Y - TEXTURE_SAMPLE00_SIZE_Y/2, 0.0f);
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nCountAnim = 0;
	g_nPatternAnim = 0;

	// 頂点情報の作成
	MakeVertexPolygon();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice,		// デバイスのポインタ
		TEXTURE_GAME_SAMPLE00,				// ファイルの名前
		&g_pD3DTexturePolygon );			// 読み込むメモリのポインタ

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPolygon(void)
{
	if( g_pD3DTexturePolygon != NULL )	//
	{	// テクスチャの開放
		g_pD3DTexturePolygon->Release();
		g_pD3DTexturePolygon = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePolygon(void)
{
	// アニメーション
	g_nCountAnim = (g_nCountAnim +1) % ANIM_PATTERN_NUM;
	g_nPatternAnim = (g_nPatternAnim + 1) % ANIM_PATTERN_NUM;	



	g_posPolygon.x = g_posPolygon.x + direction *  (float)  SCREEN_WIDTH / TIME_ANIMATION  ;

	//g_posPolygon.x -= (float)SCREEN_WIDTH / TIME_ANIMATION;
	if (g_posPolygon.x < 0 || g_posPolygon.x > SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X / 2) {
		direction = -direction;
		//g_posPolygon.x = SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X / 2;
	}
	Sleep(66);
	//g_posPolygon.x = ((int)g_posPolygon.x + 3) % SCREEN_WIDTH;	
	//g_posPolygon.y =((int) g_posPolygon.y + 3) % SCREEN_HIEGHT;
	//g_posPolygon.z = ((int)g_posPolygon.z + 3) % SCREEN_WIDTH;		
	MakeVertexPolygon();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pD3DTexturePolygon );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexPolygon();
	// rhwの設定
	g_vertexWk[0].rhw =
	g_vertexWk[1].rhw =
	g_vertexWk[2].rhw =
	g_vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	g_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	SetTexturePolygon(g_nPatternAnim);
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPolygon(void)
{
	// 頂点座標の設定
	g_vertexWk[0].vtx = D3DXVECTOR3(g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	g_vertexWk[1].vtx = D3DXVECTOR3(g_posPolygon.x + TEXTURE_SAMPLE00_SIZE_X, g_posPolygon.y, g_posPolygon.z);
	g_vertexWk[2].vtx = D3DXVECTOR3(g_posPolygon.x, g_posPolygon.y + TEXTURE_SAMPLE00_SIZE_Y, g_posPolygon.z);
	g_vertexWk[3].vtx = D3DXVECTOR3(g_posPolygon.x + TEXTURE_SAMPLE00_SIZE_X, g_posPolygon.y + TEXTURE_SAMPLE00_SIZE_Y, g_posPolygon.z);
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePolygon( int cntPattern )
{
	// テクスチャ座標の設定
	g_vertexWk[0].tex = D3DXVECTOR2(cntPattern * 1 / (float)TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	g_vertexWk[1].tex = D3DXVECTOR2((cntPattern + 1) * 1 / (float)TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	g_vertexWk[2].tex = D3DXVECTOR2(cntPattern * 1 / (float)TEXTURE_PATTERN_DIVIDE_X, 1/(float) TEXTURE_PATTERN_DIVIDE_Y);
	g_vertexWk[3].tex = D3DXVECTOR2((cntPattern + 1) * 1 / (float)TEXTURE_PATTERN_DIVIDE_X, 1 / (float)TEXTURE_PATTERN_DIVIDE_Y);
}
