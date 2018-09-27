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
#include <math.h>
#include "bg.h"
#include "item.h"
#include "beam.h"
#include "sound.h"
#include "servant.h"
#include <time.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemy( int no);
void SetVertexEnemy(int no);
void SetTextureEnemy(int no, int cntPattern );
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy = NULL;		// テクスチャへのポリゴン		
ENEMY					enemyWk[ENEMY_MAX];				// 頂点情報格納ワーク
int beam_cooldown = 0;

D3DXVECTOR3 last_pos = D3DXVECTOR3(0,0, 0.0f);

float abs_pos_x = 0;
float abs_pos_y = 0;

bool overlap2=false;

LPDIRECTSOUNDBUFFER8	g_pSE4;							// SE用バッファ
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
		g_pSE4 = LoadSound(SE_00);
	}
	else if (type == 1) {
		UninitEnemy;
	}
	// エネミーの初期化処理
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		enemy->use = true;										// 使用
		enemy->pos = D3DXVECTOR3(BG00_SIZE_X/2 - TEXTURE_ENEMY_SIZE_X / 2, TEXTURE_ENEMY_SIZE_Y/2, 0.0f);	 // 座標データを初期化	
		enemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転データを初期化
		enemy->CountAnim = 0;									// アニメパターン番号をランダムで初期化
		enemy->PatternAnim = 0;									// アニメカウントを初期化

		enemy->status.HP = 5000;								// HPを初期化
		enemy->status.MP= 100;									// MPを初期化
		enemy->status.ATK = 150;								// ATKを初期化
		enemy->status.DEF = 50;									// DEFを初期化
		enemy->status.LUCK = 100;								// LUCKを初期化
		enemy->mode = 0;										// modeを初期化
		strcpy(enemy->status.name,"亡霊の形 ?");				// NAMEを初期化

		enemy->abs_pos = enemy->pos;

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
		if (g_pSE4 != NULL)
		{	// テクスチャの開放
			g_pSE4->Release();
			g_pSE4 = NULL;
		}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	ENEMY *enemy = enemyWk;				// エネミーのポインターを初期化
	PLAYER *player = GetPlayer(0);		// プレイヤー０番のアドレスを取得する


	/* ランダム用初期設定 */
	srand((unsigned)time(NULL));

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == true)			// 使用している状態なら更新する
		{
			// アニメーション
			enemy->CountAnim = (enemy->CountAnim + 1) % ANIM_PATTERN_NUM_ENEMY;
			enemy->PatternAnim = (enemy->PatternAnim +1) % ANIM_PATTERN_NUM_ENEMY;

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
			
			if (player->pos.x + TEXTURE_PLAYER_SIZE_X / 2 > BG00_SIZE_X / 2 && enemy->rot.z > -(D3DX_PI/2 - atan2f(player->pos.y + TEXTURE_PLAYER_SIZE_Y / 2, player->pos.x - BG00_SIZE_X / 2)))
				enemy->rot.z -= 0.02;
			if (player->pos.x + TEXTURE_PLAYER_SIZE_X / 2 < BG00_SIZE_X / 2 && enemy->rot.z < D3DX_PI/2 - atan2f(player->pos.y + TEXTURE_PLAYER_SIZE_Y / 2, BG00_SIZE_X / 2 - player->pos.x))
				enemy->rot.z += 0.02;


			LPDIRECT3DDEVICE9 pDevice = GetDevice();
			if (enemy->status.HP < 4500 && enemy->mode == 0) {
				enemy->mode = 1;					// Easy Mode
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_ENEMY,				// ファイルの名前
					&g_pD3DTextureEnemy);			// 読み込むメモリのポインタ
				enemy->Texture = g_pD3DTextureEnemy;// テクスチャ情報					
				SetBGM(4);
				enemy->status.DEF = 100;
			}		
			else if (enemy->status.HP < 3000 && enemy->mode == 1)
			{
				enemy->mode = 2;					// Normal Mode
				enemy->PatternAnim = 2;
				// テクスチャ座標を設定
				SetTextureEnemy(i, player->PatternAnim+1);				
				SetBGM(5);
				enemy->status.ATK = 200;
				enemy->status.DEF = 150;
			}
			else if (enemy->status.HP < 1500 && enemy->mode == 2)
			{
				enemy->mode = 3;					// Hard Mode
				enemy->PatternAnim = 3;
				// テクスチャ座標を設定
				SetTextureEnemy(i, player->PatternAnim+1);				
				SetBGM(6);
				enemy->status.ATK = 250;
				enemy->status.DEF = 200;
			}

			if (beam_cooldown == 0)
			{	
				BEAM *beam = GetBeam(0);		// エネミーのポインターを初期化

				D3DXVECTOR3 pos = enemy->abs_pos ;

				
				//pos.x +=  GetPlayer(0)->pos.x / 4.0f;				
				switch (enemy ->mode)
				{
				case 1:	// Easy Mode
					// 発射音再生
					g_pSE4 = LoadSound(SE_03);
					if (IsPlaying(g_pSE4))
						g_pSE4->SetCurrentPosition(0);
					else {
						PlaySound(g_pSE4, E_DS8_FLAG_NONE);
					}
					pos = enemy->pos;
					if (enemy->rot.z >0)
					pos.x -= TEXTURE_ENEMY_SIZE_X / 2;
					else
					pos.x += TEXTURE_ENEMY_SIZE_X ;
					//pos.y += TEXTURE_ENEMY_SIZE_Y / 2;
					for (int i = 0; i < BEAM_MAX/8 +1; i++, beam++) //Side wave(circle) Bullet						
					{
						D3DXVECTOR3 bullet_pos = pos;											   					
						bullet_pos.x += enemy->Radius/2 * cos(D3DX_PI*(i / (BEAM_MAX / 8.0f)));//Max = cos pi/8, 
						bullet_pos.y += enemy->Radius/2 * sin(D3DX_PI*(i / (BEAM_MAX / 8.0f)));	
						//SetBeam(bullet_pos, enemy->rot.z, enemy->status.ATK);

						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI/4, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI/4, enemy->status.ATK);

						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI/8, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI/8, enemy->status.ATK);
					}
					beam_cooldown = 100;
					break;

				case 2: // Normal Mode
					// 発射音再生
					g_pSE4 = LoadSound(SE_03);
					if (IsPlaying(g_pSE4))
						g_pSE4->SetCurrentPosition(0);
					else {
						PlaySound(g_pSE4, E_DS8_FLAG_NONE);
					}
					enemy->vertexWk->diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
					pos = enemy->pos;
					if (enemy->rot.z > 0)
						pos.x -= TEXTURE_ENEMY_SIZE_X / 2;
					else
						pos.x += TEXTURE_ENEMY_SIZE_X;
					pos.y += TEXTURE_ENEMY_SIZE_Y / 2;
					for (int i = 0; i < BEAM_MAX / 16 + 1; i++, beam++) // Ring Bullet						
					{
						D3DXVECTOR3 bullet_pos = pos;
						bullet_pos.x += enemy->Radius / 2 * cos(D3DX_PI*(i / (BEAM_MAX / 8.0f)));
						bullet_pos.y += enemy->Radius / 2 * sin(D3DX_PI*(i / (BEAM_MAX / 8.0f)));



						SetBeam(bullet_pos, enemy->rot.z, enemy->status.ATK);

						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 3, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 3, enemy->status.ATK);

						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 6, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 6, enemy->status.ATK);


						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 9, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 9, enemy->status.ATK);
					}
					beam_cooldown = 150;
					break;

				case 3:	// Hard Mode	
					// 発射音再生
					g_pSE4 = LoadSound(SE_03);
					if (IsPlaying(g_pSE4))
						g_pSE4->SetCurrentPosition(0);
					else {
						PlaySound(g_pSE4, E_DS8_FLAG_NONE);
					}
					for (int i = 0; i < BEAM_MAX / 16 + 1; i++, beam++) // Ring Bullet						
					{
						D3DXVECTOR3 bullet_pos = pos;
						bullet_pos.x += enemy->Radius  * cos(D3DX_PI*(i / (BEAM_MAX / 8.0f)));
						bullet_pos.y += enemy->Radius  * sin(D3DX_PI*(i / (BEAM_MAX / 8.0f)));
						

						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 3, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 3, enemy->status.ATK);


						bullet_pos = pos;
						bullet_pos.x += enemy->Radius /2 * cos(D3DX_PI*(i / (BEAM_MAX / 8.0f)));
						bullet_pos.y += enemy->Radius /2 * sin(D3DX_PI*(i / (BEAM_MAX / 8.0f)));


						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 4, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 4, enemy->status.ATK);


						bullet_pos = pos;
						bullet_pos.x += enemy->Radius / 3 * cos(D3DX_PI*(i / (BEAM_MAX / 8.0f)));
						bullet_pos.y += enemy->Radius / 3 * sin(D3DX_PI*(i / (BEAM_MAX / 8.0f)));

						//bullet_pos.x += - 2 * TEXTURE_ENEMY_SIZE_X;
						//bullet_pos.y += - TEXTURE_ENEMY_SIZE_Y / 2;
						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 5, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 5, enemy->status.ATK);
					}
					for (int i = 0; i < BEAM_MAX / 16 + 1; i++, beam++) // Ring Bullet						
					{
						D3DXVECTOR3 bullet_pos = pos;
						bullet_pos.x += enemy->Radius / 2 * cos(D3DX_PI*(i / (BEAM_MAX / 8.0f)));
						bullet_pos.y += enemy->Radius / 2 * sin(D3DX_PI*(i / (BEAM_MAX / 8.0f)));



						SetBeam(bullet_pos, enemy->rot.z, enemy->status.ATK);

						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 3, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 3, enemy->status.ATK);

						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 6, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 6, enemy->status.ATK);


						SetBeam(bullet_pos, enemy->rot.z - D3DX_PI / 9, enemy->status.ATK);
						SetBeam(bullet_pos, enemy->rot.z + D3DX_PI / 9, enemy->status.ATK);
					}
					beam_cooldown = 100;
					break;

				default:
					beam_cooldown = 50;
					break;
				}

				// Item Random Set


				pos.x = rand() % (SCREEN_CENTER_X - TEXTURE_ITEM_SIZE_X) * 2 + TEXTURE_ITEM_SIZE_X;
				pos.y = rand() % (SCREEN_CENTER_Y)+TEXTURE_ITEM_SIZE_Y + TEXTURE_ENEMY_SIZE_Y;

				ITEM *item = GetItem(0);
				for (int i = 0; i < ITEM_MAX; i++, item++)
				{
					if (item->use)
					{
						if (CheckHitBC(item->pos, pos, item->Radius, item->Radius))
							overlap2 = true;
					}
				}
				SERVANT *servant = GetServant(0);
				for (int i = 0; i < SERVANT_MAX; i++, servant++)
				{
					if (servant->use)
					{
						if (CheckHitBC(servant->pos, pos, servant->Radius, item->Radius))
							overlap2 = true;
					}
				}

				if (!overlap2)
				SetItem(pos, rand() % 3);

				srand((unsigned)time(NULL));
				//last_pos = pos;
			}

			beam_cooldown --;
			//beam_cooldown -= recharge ;
			
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
	enemy->pos.x  = enemy->abs_pos.x - GetPlayer(0)->pos.x / 4.0f;

	enemy->vertexWk[0].vtx.x = enemy->pos.x - cosf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[0].vtx.y = enemy->pos.y - sinf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[0].vtx.z = 0.0f;

	enemy->vertexWk[1].vtx.x = enemy->pos.x + cosf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[1].vtx.y = enemy->pos.y - sinf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[1].vtx.z = 0.0f;

	enemy->vertexWk[2].vtx.x = enemy->pos.x - cosf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[2].vtx.y = enemy->pos.y + sinf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[2].vtx.z = 0.0f;

	enemy->vertexWk[3].vtx.x = enemy->pos.x + cosf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
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