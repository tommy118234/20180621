/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	バレット処理 [SERVANT.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "SERVANT.h"
#include "bullet.h"
#include "sound.h"

#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSERVANT(int no);
void SetTextureSERVANT(int no, int cntPattern);
void SetVertexSERVANT(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureSERVANT = NULL;		// テクスチャへのポリゴン
SERVANT					SERVANTWk[SERVANT_MAX];			// バレット構造体
LPDIRECTSOUNDBUFFER8	g_pSE3;							// SE用バッファ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSERVANT(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *SERVANT = &SERVANTWk[0];		// エネミーのポインターを初期化
	PLAYER *player = GetPlayer(0);



	for (int i = 0; i < SERVANT_MAX; i++, SERVANT++) {
		g_pSE3 = LoadSound(SE_01);
		// テクスチャーの初期化を行う？
		if (type == 0)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
				TEXTURE_GAME_SERVANT,			// ファイルの名前
				&g_pD3DTextureSERVANT);			// 読み込むメモリのポインタ
			//g_pSE3 = LoadSound(SE_00);
		}
		else if (type == 1) {
			UninitSERVANT;
		}

		SERVANT->use = FALSE;									// 未使用（発射されていない弾）
		SERVANT->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);			// 座標データを初期化
		SERVANT->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		SERVANT->PatternAnim = 0;								// アニメパターン番号をランダムで初期化
		SERVANT->CountAnim = 0;									// アニメカウントを初期化

		SERVANT->BaseAngle = atan2f(TEXTURE_SERVANT_SIZE_Y, TEXTURE_SERVANT_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_SERVANT_SIZE_X / 2, TEXTURE_SERVANT_SIZE_Y / 2);
		SERVANT->Radius = D3DXVec2Length(&temp);

		SERVANT->Texture = g_pD3DTextureSERVANT;					// テクスチャ情報
		MakeVertexSERVANT(i);									// 頂点情報の作成
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitSERVANT(void)
{
	if (g_pD3DTextureSERVANT != NULL)	//
	{	// テクスチャの開放
		g_pD3DTextureSERVANT->Release();
		g_pD3DTextureSERVANT = NULL;
	}

	if (g_pSE3 != NULL)
	{	// テクスチャの開放
		g_pSE3->Release();
		g_pSE3 = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateSERVANT(void)
{

	SERVANT *SERVANT = SERVANTWk;			// バレットのポインターを初期化
	ENEMY *enemy = GetEnemy(0);
	for (int i = 0; i < SERVANT_MAX; i++, SERVANT++)
	{
		if (SERVANT->use == true)			// 未使用状態のバレットを見つける
		{
			//// SERVANTの移動処理

			if (enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2 > SERVANT->pos.x + TEXTURE_SERVANT_SIZE_X / 2 &&  SERVANT->rot.z < 1.57 - atan2f(SERVANT->pos.y + TEXTURE_SERVANT_SIZE_Y / 2, BG00_SIZE_X/2 - SERVANT->pos.x ))
				SERVANT->rot.z += 0.1;
			//else
			//	SERVANT->rot.z = 1.57 - atan2f(SERVANT->pos.y - TEXTURE_SERVANT_SIZE_Y / 2, enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2);
			
			if (enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2 < SERVANT->pos.x + TEXTURE_SERVANT_SIZE_X / 2 && SERVANT->rot.z > -(1.57 - atan2f(SERVANT->pos.y + TEXTURE_SERVANT_SIZE_Y / 2,SERVANT->pos.x -  BG00_SIZE_X/2 )))
				SERVANT->rot.z -= 0.1;
			//else
			//	SERVANT->rot.z = 1.57 - atan2f(SERVANT->pos.y - TEXTURE_SERVANT_SIZE_Y / 2, enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2);
			
			if (SERVANT->bullet_cooldown < 1) {				
				SERVANT->bullet_cooldown = 200;
				D3DXVECTOR3 pos = SERVANT->pos;
				//pos.y -= TEXTURE_SERVANT_SIZE_Y;				
				pos.x -= GetPlayer(0)->pos.x / 4.0f;
				if (SERVANT->rot.z> 0)
				SetBullet(pos,SERVANT->rot.z);
				else
				SetBullet(pos,  SERVANT->rot.z);
			}
			else {
				SERVANT->bullet_cooldown--;
			}

			// 画面外まで進んだ？
			if (SERVANT->pos.y < -TEXTURE_SERVANT_SIZE_Y)
			{
				SERVANT->use = FALSE;
				SERVANT->pos.x = -100.0f;
				//SERVANT->pos = D3DXVECTOR3(player->pos.x + TEXTURE_SERVANT_SIZE_X / 2, player->pos.y, 0.0f);
			}
			SetVertexSERVANT(i);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSERVANT(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *SERVANT = SERVANTWk;				// バレットのポインターを初期化

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SERVANT_MAX; i++, SERVANT++)
	{
		if (SERVANT->use)					// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, SERVANT->Texture);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SERVANT, SERVANT->vertexWk, sizeof(VERTEX_2D));
		}
	}
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSERVANT(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *SERVANT = &SERVANTWk[no];

	// 頂点座標の設定
	SetVertexSERVANT(no);
	// rhwの設定
	SERVANT->vertexWk[0].rhw =
		SERVANT->vertexWk[1].rhw =
		SERVANT->vertexWk[2].rhw =
		SERVANT->vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	SERVANT->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	SERVANT->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	SERVANT->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	SERVANT->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	SERVANT->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	SERVANT->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT, 0.0f);
	SERVANT->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT);
	SERVANT->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT);

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexSERVANT(int no)
{
	SERVANT *SERVANT = &SERVANTWk[no];			// バレットのポインターを初期化

	// 頂点座標の設定
	SERVANT->vertexWk[0].vtx = D3DXVECTOR3(SERVANT->pos.x - TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
		SERVANT->pos.y - TEXTURE_SERVANT_SIZE_Y,
		0);
	SERVANT->vertexWk[1].vtx = D3DXVECTOR3(SERVANT->pos.x + TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
		SERVANT->pos.y - TEXTURE_SERVANT_SIZE_Y,
		0);
	SERVANT->vertexWk[2].vtx = D3DXVECTOR3(SERVANT->pos.x - TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
		SERVANT->pos.y + TEXTURE_SERVANT_SIZE_Y,
		0);
	SERVANT->vertexWk[3].vtx = D3DXVECTOR3(SERVANT->pos.x + TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
		SERVANT->pos.y + TEXTURE_SERVANT_SIZE_Y,
		0);

	// 頂点座標の設定
	SERVANT->vertexWk[0].vtx.x = SERVANT->pos.x - cosf(SERVANT->BaseAngle + SERVANT->rot.z) * SERVANT->Radius - GetPlayer(0)->pos.x / 4.0f;
	SERVANT->vertexWk[0].vtx.y = SERVANT->pos.y - sinf(SERVANT->BaseAngle + SERVANT->rot.z) * SERVANT->Radius;
	SERVANT->vertexWk[0].vtx.z = 0.0f;

	SERVANT->vertexWk[1].vtx.x = SERVANT->pos.x + cosf(SERVANT->BaseAngle - SERVANT->rot.z) * SERVANT->Radius - GetPlayer(0)->pos.x / 4.0f;
	SERVANT->vertexWk[1].vtx.y = SERVANT->pos.y - sinf(SERVANT->BaseAngle - SERVANT->rot.z) * SERVANT->Radius;
	SERVANT->vertexWk[1].vtx.z = 0.0f;

	SERVANT->vertexWk[2].vtx.x = SERVANT->pos.x - cosf(SERVANT->BaseAngle - SERVANT->rot.z) * SERVANT->Radius - GetPlayer(0)->pos.x / 4.0f;
	SERVANT->vertexWk[2].vtx.y = SERVANT->pos.y + sinf(SERVANT->BaseAngle - SERVANT->rot.z) * SERVANT->Radius;
	SERVANT->vertexWk[2].vtx.z = 0.0f;

	SERVANT->vertexWk[3].vtx.x = SERVANT->pos.x + cosf(SERVANT->BaseAngle + SERVANT->rot.z) * SERVANT->Radius - GetPlayer(0)->pos.x / 4.0f;
	SERVANT->vertexWk[3].vtx.y = SERVANT->pos.y + sinf(SERVANT->BaseAngle + SERVANT->rot.z) * SERVANT->Radius;
	SERVANT->vertexWk[3].vtx.z = 0.0f;
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureSERVANT(int no, int cntPattern)
{
	SERVANT *SERVANT = &SERVANTWk[no];			// バレットのポインターを初期化

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT;
	// 頂点座標の設定
	SERVANT->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	SERVANT->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	SERVANT->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	SERVANT->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// バレットの発射設定
//=============================================================================
void SetSERVANT(D3DXVECTOR3 pos, int type)
{
	SERVANT *SERVANT = &SERVANTWk[0];			// バレットのポインターを初期化
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < SERVANT_MAX; i++, SERVANT++)
	{
		if (SERVANT->use == false)			// 未使用状態のバレットを見つける
		{

			SERVANT->use = true;				// 使用状態へ変更する

			switch (type) {
			case 1:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT,			// ファイルの名前
					&g_pD3DTextureSERVANT);			// 読み込むメモリのポインタ
				break;
			case 2:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT2,			// ファイルの名前
					&g_pD3DTextureSERVANT);			// 読み込むメモリのポインタ
				break;
			case 3:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT3,			// ファイルの名前
					&g_pD3DTextureSERVANT);			// 読み込むメモリのポインタ
				break;
			}
			SERVANT->Texture = g_pD3DTextureSERVANT;					// テクスチャ情報
			//MakeVertexSERVANT(i);									// 頂点情報の作成
			SERVANT->pos = pos;				// 座標をセット

			g_pSE3 = LoadSound(SE_01);
			// 発射音再生
			if (IsPlaying(g_pSE3))
				g_pSE3->SetCurrentPosition(0);
			else {
				PlaySound(g_pSE3, E_DS8_FLAG_NONE);
			}

			return;							// 1発セットしたので終了する
		}
	}
}

/*******************************************************************************
関数名:	SERVANT *GetSERVANTAdr( int pno )
引数:	int pno : SERVANT番号
戻り値:	SERVANT *
説明:	SERVANTのアドレスを取得する
*******************************************************************************/
SERVANT *GetSERVANT(int pno)
{
	return &SERVANTWk[pno];
}