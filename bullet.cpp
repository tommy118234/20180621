/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	バレット処理 [bullet.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "sound.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(int no);
void SetTextureBullet(int no, int cntPattern);
void SetVertexBullet(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;		// テクスチャへのポリゴン
BULLET					bulletWk[BULLET_MAX];			// バレット構造体
LPDIRECTSOUNDBUFFER8	g_pSE;							// SE用バッファ


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = &bulletWk[0];		// エネミーのポインターを初期化
	PLAYER *player = GetPlayer(0);
	for (int i = 0; i < BULLET_MAX; i++, bullet++) {
		g_pSE = LoadSound(SE_00);
		// テクスチャーの初期化を行う？
		if (type == 0)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,			// デバイスのポインタ
				TEXTURE_GAME_BULLET,					// ファイルの名前
				&g_pD3DTextureBullet);					// 読み込むメモリのポインタ
			//g_pSE = LoadSound(SE_00);
		}
		else if (type == 1) {
			//UninitBullet();
		}						
		bullet->use = FALSE;									// 未使用（発射されていない弾）
		bullet->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);			// 座標データを初期化
		bullet->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		bullet->atk = 0;			// ATKを初期化
		bullet->PatternAnim = 0;								// アニメパターン番号をランダムで初期化
		bullet->CountAnim = 0;									// アニメカウントを初期化
		bullet->BaseAngle = atan2f(TEXTURE_BULLET_SIZE_Y, TEXTURE_BULLET_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_BULLET_SIZE_X / 2, TEXTURE_BULLET_SIZE_Y / 2);
		bullet->Radius = D3DXVec2Length(&temp);
		bullet->Texture = g_pD3DTextureBullet;					// テクスチャ情報
		MakeVertexBullet(i);									// 頂点情報の作成
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	if (g_pD3DTextureBullet != NULL)
	{	// テクスチャの開放
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}	
	if (g_pSE != NULL)
	{	// テクスチャの開放
		g_pSE->Release();
		g_pSE = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = bulletWk;			// バレットのポインターを初期化

	float abs = GetPlayer(0)->pos.x / 4.0f;
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{		
		if (bullet->use == true)			// 未使用状態のバレットを見つける
		{
			// バレットの移動処理
			//bullet->pos.x -= BULLET_SPEED * (-sinf(bullet->BaseAngle + bullet->rot.z) + cosf(bullet->BaseAngle + bullet->rot.z));
			//bullet->pos.y -= BULLET_SPEED * (cosf(bullet->BaseAngle + bullet->rot.z) + sinf(bullet->BaseAngle + bullet->rot.z));

			bullet->abs_pos.x -= BULLET_SPEED * (-sinf(bullet->BaseAngle + bullet->rot.z) + cosf(bullet->BaseAngle + bullet->rot.z));
			bullet->abs_pos.y -= BULLET_SPEED * (cosf(bullet->BaseAngle + bullet->rot.z) + sinf(bullet->BaseAngle + bullet->rot.z));
			
			// 画面外まで進んだ？
			if (bullet->abs_pos.y < -TEXTURE_BULLET_SIZE_Y || bullet->abs_pos.y > SCREEN_HEIGHT
				|| bullet->abs_pos.x < -TEXTURE_BULLET_SIZE_X || bullet->abs_pos.x > SCREEN_WIDTH)
			{
				bullet->use = false;
				bullet->Texture = NULL;					// テクスチャ情報2
				bullet->Texture = g_pD3DTextureBullet;	// テクスチャ情報				
				bullet->abs_pos.x = -100.0f;
			}
			SetVertexBullet(i);
		}		
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = bulletWk;				// バレットのポインターを初期化

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{		
		if (bullet->use)					// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, bullet->Texture);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BULLET, bullet->vertexWk, sizeof(VERTEX_2D));
		}
	}	
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBullet(int no)
{	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		
	BULLET *bullet = &bulletWk[no];

	// 頂点座標の設定
	SetVertexBullet(no);
	// rhwの設定
	bullet->vertexWk[0].rhw =
	bullet->vertexWk[1].rhw =
	bullet->vertexWk[2].rhw =
	bullet->vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	bullet->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);	
	// テクスチャ座標の設定
	bullet->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	bullet->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET, 0.0f);
	bullet->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET);
	bullet->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET);
	
	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBullet(int no)
{	
	BULLET *bullet = &bulletWk[no];			// バレットのポインターを初期化
	// 頂点座標の設定	
	bullet->pos.y = bullet->abs_pos.y;
	bullet->pos.x = bullet->abs_pos.x - GetPlayer(0)->pos.x / 4.0f;

	bullet->vertexWk[0].vtx = D3DXVECTOR3(bullet->pos.x - cosf(bullet->BaseAngle + bullet->rot.z) * bullet->Radius,
										  bullet->pos.y - sinf(bullet->BaseAngle + bullet->rot.z) * bullet->Radius,
										  0);

	bullet->vertexWk[1].vtx = D3DXVECTOR3(bullet->pos.x + cosf(bullet->BaseAngle - bullet->rot.z) * bullet->Radius,
										  bullet->pos.y - sinf(bullet->BaseAngle - bullet->rot.z) *bullet->Radius,
										  0);
																	
	bullet->vertexWk[2].vtx = D3DXVECTOR3(bullet->pos.x - cosf(bullet->BaseAngle - bullet->rot.z) * bullet->Radius,
										  bullet->pos.y + sinf(bullet->BaseAngle - bullet->rot.z) * bullet->Radius,
									      0);

	bullet->vertexWk[3].vtx = D3DXVECTOR3(bullet->pos.x + cosf(bullet->BaseAngle + bullet->rot.z) * bullet->Radius,
										  bullet->pos.y + sinf(bullet->BaseAngle + bullet->rot.z) * bullet->Radius,
										  0);
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBullet(int no, int cntPattern)
{	
	BULLET *bullet = &bulletWk[no];			// バレットのポインターを初期化
	
	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_BULLET;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
	// 頂点座標の設定
	bullet->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	bullet->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	bullet->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	bullet->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// バレットの発射設定
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, float rot, int atk)
{
	BULLET *bullet = &bulletWk[0];			// バレットのポインターを初期化
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use == false)			// 未使用状態のバレットを見つける
		{
			bullet->use = true;				// 使用状態へ変更する
			bullet->pos = pos;				// 座標をセット
			bullet->rot.z = rot;
			bullet->atk = atk;
			bullet->abs_pos = pos;
			// 発射音再生
			g_pSE = LoadSound(SE_01);
			if (IsPlaying(g_pSE))
				g_pSE->SetCurrentPosition(0);
			else {
			PlaySound(g_pSE, E_DS8_FLAG_NONE);
			}
			return;							// 1発セットしたので終了する
		}
	}
}

/*******************************************************************************
関数名:	BULLET *GetBulletAdr( int pno )
引数:	int pno : Bullet番号
戻り値:	BULLET *
説明:	Bulletのアドレスを取得する
*******************************************************************************/
BULLET *GetBullet(int pno)
{
	return &bulletWk[pno];
}