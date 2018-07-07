//=============================================================================
//
// ポリゴン処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemy(void);
void SetVertexEnemy(void);
void SetTextureEnemy( int cntPattern );	//

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy = NULL;		// テクスチャへのポリゴン

VERTEX_2D				g_vertexWk2[NUM_VERTEX];		// 頂点情報格納ワーク

//D3DXVECTOR3				enemy->g_posEnemy;			// ポリゴンの移動量
//D3DXVECTOR3				enemy->g_rotEnemy;			// ポリゴンの回転量
//int						enemy->g_nCountAnim2 ;		// アニメーションカウント
//int						enemy->g_nPatternAnim2;		// アニメーションパターンナンバー
//
//float					g_fRadiusEnemy;					// ポリゴンの半径
//float					g_fBaseAngleEnemy;				// ポリゴンの角度
//
//int						direction2 = -1;

ENEMY	enemy[ENEMY_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);	

	enemy->g_posEnemy = D3DXVECTOR3(0, SCREEN_CENTER_Y - TEXTURE_SAMPLE00_SIZE_Y2/2, 0.0f);
	enemy->g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	enemy->g_nCountAnim2  = 0;
	enemy->g_nPatternAnim2 = 0;

	// 頂点情報の作成
	MakeVertexEnemy();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice,		// デバイスのポインタ
		TEXTURE_GAME_SAMPLE002,				// ファイルの名前
		&g_pD3DTextureEnemy );			// 読み込むメモリのポインタ

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if( g_pD3DTextureEnemy != NULL )	//
	{	// テクスチャの開放
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	// アニメーション
	enemy->g_nCountAnim2  = (enemy->g_nCountAnim2  +1) % ANIM_PATTERN_NUM2;
	enemy->g_nPatternAnim2 = (enemy->g_nPatternAnim2 + 1) % ANIM_PATTERN_NUM2;	

	//enemy->g_posEnemy.x += direction2 *  (float)  SCREEN_WIDTH / TIME_ANIMATION  ;



	//enemy->g_posEnemy.x -= (float)SCREEN_WIDTH / TIME_ANIMATION;
	//if (enemy->g_posEnemy.x < 0 || enemy->g_posEnemy.x > SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X2 / 2) {
	//	direction2 = -direction2;
	//}
	if (enemy->g_posEnemy.x < 0) 
	{
		enemy->g_posEnemy.x = SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X2 / 2;
	}
	if (enemy->g_posEnemy.x > SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X2 / 2)
	{
		enemy->g_posEnemy.x = 0;
	}
	if (enemy->g_posEnemy.y < 0)
	{
		enemy->g_posEnemy.y = SCREEN_HEIGHT - TEXTURE_SAMPLE00_SIZE_Y2;
	}
	if (enemy->g_posEnemy.y > SCREEN_HEIGHT - TEXTURE_SAMPLE00_SIZE_Y2)
	{
		enemy->g_posEnemy.y = 0;
	}

	 //入力対応
	if (GetKeyboardPress(DIK_DOWN)) {
		enemy->g_posEnemy.y += 2;
	}
	if (GetKeyboardPress(DIK_UP)) {
		enemy->g_posEnemy.y -= 2;
	}
	if (GetKeyboardPress(DIK_LEFT)) {
		enemy->g_posEnemy.x -= 2;
	}
	if (GetKeyboardPress(DIK_RIGHT)) {
		enemy->g_posEnemy.x += 2;
	}

	MakeVertexEnemy();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture( 0, g_pD3DTextureEnemy );

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk2, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexEnemy();
	// rhwの設定
	g_vertexWk2[0].rhw =
	g_vertexWk2[1].rhw =
	g_vertexWk2[2].rhw =
	g_vertexWk2[3].rhw = 1.0f;
	// 反射光の設定
	g_vertexWk2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	SetTextureEnemy(enemy->g_nPatternAnim2);
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemy(void)
{
	// 頂点座標の設定
	g_vertexWk2[0].vtx = D3DXVECTOR3(enemy->g_posEnemy.x, enemy->g_posEnemy.y, enemy->g_posEnemy.z);
	g_vertexWk2[1].vtx = D3DXVECTOR3(enemy->g_posEnemy.x + TEXTURE_SAMPLE00_SIZE_X2, enemy->g_posEnemy.y, enemy->g_posEnemy.z);
	g_vertexWk2[2].vtx = D3DXVECTOR3(enemy->g_posEnemy.x, enemy->g_posEnemy.y + TEXTURE_SAMPLE00_SIZE_Y2, enemy->g_posEnemy.z);
	g_vertexWk2[3].vtx = D3DXVECTOR3(enemy->g_posEnemy.x + TEXTURE_SAMPLE00_SIZE_X2, enemy->g_posEnemy.y + TEXTURE_SAMPLE00_SIZE_Y2, enemy->g_posEnemy.z);
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEnemy( int cntPattern )
{
	// テクスチャ座標の設定
	g_vertexWk2[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X2) / (float)TEXTURE_PATTERN_DIVIDE_X2, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X2) / (float)TEXTURE_PATTERN_DIVIDE_Y2);
	g_vertexWk2[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X2) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X2, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X2) / (float)TEXTURE_PATTERN_DIVIDE_Y2);
	g_vertexWk2[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X2) / (float)TEXTURE_PATTERN_DIVIDE_X2, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X2) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y2);
	g_vertexWk2[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X2) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X2, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X2) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y2);
}
/*******************************************************************************
関数名:	ENEMY *GetMapAdr( int pno )
引数:	int pno : エネミー番号
戻り値:	ENEMY *
説明:	エネミーのアドレスを取得する
*******************************************************************************/
ENEMY *GetEnemy(int pno)
{
	return &enemy[pno];
}