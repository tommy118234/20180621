/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	エネミー処理 [enemy.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/

#include "main.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "math.h"
#include "bg.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemy( int no);
void SetVertexEnemy(int no);
void SetTextureEnemy(int no, int cntPattern );	//

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy = NULL;		// テクスチャへのポリゴン		
ENEMY					enemyWk[ENEMY_MAX];				// 頂点情報格納ワーク
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = enemyWk;

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_ENEMY,				// ファイルの名前
			&g_pD3DTextureEnemy);			// 読み込むメモリのポインタ
	}
	else if (type == 1) {
		UninitEnemy;
	}


	// エネミーの初期化処理
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		enemy->use = true;										// 使用
		enemy->pos = D3DXVECTOR3(BG00_SIZE_X/2, TEXTURE_ENEMY_SIZE_Y/2, 0.0f);	 // 座標データを初期化	
		enemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転データを初期化
		enemy->CountAnim = 0;									// アニメパターン番号をランダムで初期化
		enemy->PatternAnim = 0;									// アニメカウントを初期化


		enemy->status.HP = 5000;								// HPを初期化
		enemy->status.MP= 100;									// MPを初期化
		enemy->status.ATK = 300;								// ATKを初期化
		enemy->status.DEF = 50;								// DEFを初期化
		enemy->status.LUCK = 100;								// LUCKを初期化
		strcpy(enemy->status.name,"亡霊の形 ?");				// NAMEを初期化


		//enemy Base Angle, Radius
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X / 2, TEXTURE_ENEMY_SIZE_Y / 2);
		enemy->Radius = D3DXVec2Length(&temp);
		enemy->BaseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);

		enemy->Texture = g_pD3DTextureEnemy;					// テクスチャ情報		
		MakeVertexEnemy(i);										// 頂点情報の作成
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{	
		if (g_pD3DTextureEnemy != NULL)	//
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
	ENEMY *enemy = enemyWk;				// エネミーのポインターを初期化

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == true)			// 使用している状態なら更新する
		{
			// アニメーション
			enemy->CountAnim = (enemy->CountAnim + 1) % ANIM_PATTERN_NUM_ENEMY;
			enemy->PatternAnim = (enemy->PatternAnim + 1) % ANIM_PATTERN_NUM_ENEMY;


			if (enemy->pos.x < 0)
			{
				enemy->pos.x = 0;
			}
			if (enemy->pos.x > SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X)
			{
				enemy->pos.x = SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X;
			}
			if (enemy->pos.y < 0)
			{
				enemy->pos.y = 0;
			}
			if (enemy->pos.y > SCREEN_HEIGHT - TEXTURE_ENEMY_SIZE_Y)
			{
				enemy->pos.y = SCREEN_HEIGHT - TEXTURE_ENEMY_SIZE_Y;
			}
			if (GetKeyboardPress(DIK_UP) )
			{
				if (GetPlayer(0)->pos.x > BG00_SIZE_X / 2 && enemy->rot.z != -GetPlayer(0)->rot.z && enemy->rot.z > -1.57 )
					enemy->rot.z -= 0.1;
				if (GetPlayer(0)->pos.x < BG00_SIZE_X / 2 && enemy->rot.z != -GetPlayer(0)->rot.z&& enemy->rot.z < 1.57)
					enemy->rot.z += 0.1;

			}
			if (enemy->rot.z > 2 *3.14)
				enemy->rot.z = enemy->rot.z - 2 * 3.14;

			if (enemy->rot.z < -2 * 3.14)
				enemy->rot.z = -enemy->rot.z - 2 * 3.14;

			SetVertexEnemy(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = enemyWk;				// エネミーのポインターを初期化

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == true)			// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, enemy->Texture);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, enemy->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEnemy(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	
	ENEMY *enemy = &enemyWk[no];			// エネミーのポインターを初期化

	// 頂点座標の設定
	SetVertexEnemy(no);
	// rhwの設定
	enemy->vertexWk[0].rhw =
	enemy->vertexWk[1].rhw =
	enemy->vertexWk[2].rhw =
	enemy->vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	enemy->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	enemy->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	enemy->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMY, 0.0f);
	enemy->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	enemy->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMY, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMY);


	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEnemy(int no)
{
	ENEMY *enemy = &enemyWk[no];			// エネミーのポインターを初期化
	
	// 頂点座標の設定
	enemy->vertexWk[0].vtx.x = enemy->pos.x - cosf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius - GetPlayer(0)->pos.x / 4.0f;
	enemy->vertexWk[0].vtx.y = enemy->pos.y - sinf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[0].vtx.z = 0.0f;

	enemy->vertexWk[1].vtx.x = enemy->pos.x + cosf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius - GetPlayer(0)->pos.x / 4.0f;
	enemy->vertexWk[1].vtx.y = enemy->pos.y - sinf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[1].vtx.z = 0.0f;

	enemy->vertexWk[2].vtx.x = enemy->pos.x - cosf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius - GetPlayer(0)->pos.x / 4.0f;
	enemy->vertexWk[2].vtx.y = enemy->pos.y + sinf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[2].vtx.z = 0.0f;

	enemy->vertexWk[3].vtx.x = enemy->pos.x + cosf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius - GetPlayer(0)->pos.x / 4.0f;
	enemy->vertexWk[3].vtx.y = enemy->pos.y + sinf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[3].vtx.z = 0.0f;
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEnemy( int no, int cntPattern )
{
	ENEMY *enemy = &enemyWk[no];			// エネミーのポインターを初期化

	// テクスチャ座標の設定
	enemy->vertexWk[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, 
						(float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	enemy->vertexWk[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY,
						(float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	enemy->vertexWk[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY,
						(float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	enemy->vertexWk[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY,
						(float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
}
/*******************************************************************************
関数名:	ENEMY *GetMapAdr( int pno )
引数:	int pno : エネミー番号
戻り値:	ENEMY *
説明:	エネミーのアドレスを取得する
*******************************************************************************/
ENEMY *GetEnemy(int pno)
{
	return &enemyWk[pno];
}