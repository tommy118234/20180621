/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	エネミー処理 [enemy.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/

#include "main.h"
#include "enemy.h"
#include "input.h"
#include "math.h"

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
ENEMY	enemy[ENEMY_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);	

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_ENEMY,				// ファイルの名前
			&g_pD3DTextureEnemy);			// 読み込むメモリのポインタ
	}


	// エネミーの初期化処理
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		enemy->use = true;
		//enemy->pos = D3DXVECTOR3(TEXTURE_ENEMY_SIZE_X, SCREEN_CENTER_Y - TEXTURE_ENEMY_SIZE_Y / 2, 0.0f);
		enemy->pos = D3DXVECTOR3(SCREEN_CENTER_X,  TEXTURE_ENEMY_SIZE_Y, 0.0f);
		
		enemy->rot = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		enemy->CountAnim = 0;
		enemy->PatternAnim = 0;
		enemy->status.HP = 10;

		//enemy Base Angle, Radius
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X / 2, TEXTURE_ENEMY_SIZE_Y / 2);
		enemy->Radius = D3DXVec2Length(&temp);
		enemy->BaseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);

		// 頂点情報の作成
		MakeVertexEnemy();			// 読み込むメモリのポインタ
	}
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
	enemy->CountAnim  = (enemy->CountAnim  +1) % ANIM_PATTERN_NUM_ENEMY;
	enemy->PatternAnim = (enemy->PatternAnim + 1) % ANIM_PATTERN_NUM_ENEMY;

	if (enemy->pos.x < 0) 
	{
		enemy->pos.x = SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X / 2;
	}
	if (enemy->pos.x > SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X / 2)
	{
		enemy->pos.x = 0;
	}
	if (enemy->pos.y < 0)
	{
		enemy->pos.y = SCREEN_HEIGHT - TEXTURE_ENEMY_SIZE_Y;
	}
	if (enemy->pos.y > SCREEN_HEIGHT - TEXTURE_ENEMY_SIZE_Y)
	{
		enemy->pos.y = 0;
	}
	
	MakeVertexEnemy();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	ENEMY  *enemy = GetEnemy(0);
	if (enemy->use) {
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureEnemy);
		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk2, sizeof(VERTEX_2D));
	}
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
	SetTextureEnemy(enemy->PatternAnim);
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemy(void)
{
	//// 頂点座標の設定
	//g_vertexWk2[0].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y, enemy->pos.z);
	//g_vertexWk2[1].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y, enemy->pos.z);
	//g_vertexWk2[2].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, enemy->pos.z);
	//g_vertexWk2[3].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, enemy->pos.z);
	
	// 回転頂点座標の設定
	enemy->rot.z = enemy->rot.z - 0.1;

	g_vertexWk2[0].vtx.x = enemy->pos.x - cos(enemy->BaseAngle + enemy->rot.z)* enemy->Radius;
	g_vertexWk2[0].vtx.y = enemy->pos.y - sin(enemy->BaseAngle + enemy->rot.z)* enemy->Radius;
	
	g_vertexWk2[1].vtx.x = enemy->pos.x + cos(enemy->BaseAngle - enemy->rot.z)* enemy->Radius;
	g_vertexWk2[1].vtx.y = enemy->pos.y - sin(enemy->BaseAngle - enemy->rot.z)* enemy->Radius;
	
	g_vertexWk2[2].vtx.x = enemy->pos.x - cos(enemy->BaseAngle - enemy->rot.z)* enemy->Radius;
	g_vertexWk2[2].vtx.y = enemy->pos.y + sin(enemy->BaseAngle - enemy->rot.z)* enemy->Radius;
	
	g_vertexWk2[3].vtx.x = enemy->pos.x + cos(enemy->BaseAngle + enemy->rot.z)* enemy->Radius;
	g_vertexWk2[3].vtx.y = enemy->pos.y + sin(enemy->BaseAngle + enemy->rot.z)* enemy->Radius;
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEnemy( int cntPattern )
{
	// テクスチャ座標の設定
	g_vertexWk2[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	g_vertexWk2[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	g_vertexWk2[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	g_vertexWk2[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
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