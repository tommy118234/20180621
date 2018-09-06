/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	Servant処理 [servant.cpp]
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
HRESULT MakeVertexServant(int no);
void SetTextureServant(int no, int cntPattern);
void SetVertexServant(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureServant = NULL;		// テクスチャへのポリゴン
SERVANT					ServantWk[SERVANT_MAX];			// Servant構造体
LPDIRECTSOUNDBUFFER8	SE3;							// SE用バッファ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitServant(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *servant = &ServantWk[0];		// エネミーのポインターを初期化
	PLAYER *player = GetPlayer(0);



	for (int i = 0; i < SERVANT_MAX; i++, servant++) {
		//SE3 = LoadSound(SE_01);
		// テクスチャーの初期化を行う？
		if (type == 0)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
				TEXTURE_GAME_SERVANT,			// ファイルの名前
				&D3DTextureServant);			// 読み込むメモリのポインタ
			SE3 = LoadSound(SE_00);
		}
		else if (type == 1) {
			UninitServant();
		}

		servant->use = false;									// 未使用（発射されていない弾）
		servant->pos = D3DXVECTOR3(-1000.0f, 0.0f, 0.0f);		// 座標データを初期化
		servant->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		servant->PatternAnim = 0;								// アニメパターン番号をランダムで初期化
		servant->CountAnim = 0;									// アニメカウントを初期化

		servant->BaseAngle = atan2f(TEXTURE_SERVANT_SIZE_Y, TEXTURE_SERVANT_SIZE_X);
		D3DXVECTOR2 temp   = D3DXVECTOR2(TEXTURE_SERVANT_SIZE_X / 2, TEXTURE_SERVANT_SIZE_Y / 2);
		servant->Radius    = D3DXVec2Length(&temp);

		servant->Texture = D3DTextureServant;					// テクスチャ情報
		MakeVertexServant(i);									// 頂点情報の作成
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitServant(void)
{
	if (D3DTextureServant != NULL)	//
	{	// テクスチャの開放
		D3DTextureServant->Release();
		D3DTextureServant = NULL;
	}
	if (SE3 != NULL)
	{	// テクスチャの開放
		SE3->Release();
		SE3 = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateServant(void)
{
	SERVANT *servant = ServantWk;			// Servantのポインターを初期化
	ENEMY	*enemy = GetEnemy(0);
	for (int i = 0; i < SERVANT_MAX; i++, servant++)
	{
		if (servant->use)			// 未使用状態のServantを見つける
		{
			// SERVANTの移動処理
			if (enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2 > servant->pos.x + TEXTURE_SERVANT_SIZE_X / 2 &&  
				servant->rot.z < 1.57 - atan2f(servant->pos.y + TEXTURE_SERVANT_SIZE_Y / 2, BG00_SIZE_X/2 - servant->pos.x ))
				servant->rot.z += 0.05;			
			if (enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2 < servant->pos.x + TEXTURE_SERVANT_SIZE_X / 2 && 
				servant->rot.z > -(1.57 - atan2f(servant->pos.y + TEXTURE_SERVANT_SIZE_Y / 2,servant->pos.x -  BG00_SIZE_X/2 )))
				servant->rot.z -= 0.05;				
			if (servant->bullet_count < 1)
			{					
				D3DXVECTOR3 pos = servant->pos;							
				//pos.x -= GetPlayer(0)->pos.x / 4.0f;	
				//servant->pos.x = servant->abs_pos.x - GetPlayer(0)->pos.x / 4.0f;
				SetBullet(pos, servant->rot.z,servant->status.ATK);	
				servant->bullet_count = servant->bullet_cooldown;
			}
			else
			{
				servant->bullet_count--;
			}
			// 画面外まで進んだ？
			if (servant->pos.y < -TEXTURE_SERVANT_SIZE_Y)
			{
				servant->use = FALSE;
				servant->pos.x = -100.0f;
			}
			SetVertexServant(i);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawServant(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *servant = ServantWk;				// Servantのポインターを初期化
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < SERVANT_MAX; i++, servant++)
	{
		if (servant->use)						// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, servant->Texture);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SERVANT, servant->vertexWk, sizeof(VERTEX_2D));
		}
	}
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexServant(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *servant = &ServantWk[no];
	// 頂点座標の設定
	SetVertexServant(no);
	// rhwの設定
	servant->vertexWk[0].rhw =
	servant->vertexWk[1].rhw =
	servant->vertexWk[2].rhw =
	servant->vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	servant->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	servant->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	servant->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	servant->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	servant->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	servant->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT, 0.0f);
	servant->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT);
	servant->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT);
	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexServant(int no)
{
	SERVANT *servant = &ServantWk[no];			// Servantのポインターを初期化
	// 頂点座標の設定
	//servant->vertexWk[0].vtx = D3DXVECTOR3(servant->pos.x - TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
	//	servant->pos.y - TEXTURE_SERVANT_SIZE_Y,
	//	0);
	//servant->vertexWk[1].vtx = D3DXVECTOR3(servant->pos.x + TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
	//	servant->pos.y - TEXTURE_SERVANT_SIZE_Y,
	//	0);
	//servant->vertexWk[2].vtx = D3DXVECTOR3(servant->pos.x - TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
	//	servant->pos.y + TEXTURE_SERVANT_SIZE_Y,
	//	0);
	//servant->vertexWk[3].vtx = D3DXVECTOR3(servant->pos.x + TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
	//	servant->pos.y + TEXTURE_SERVANT_SIZE_Y,
	//	0);
	servant->pos.x = servant->abs_pos.x - GetPlayer(0)->pos.x / 4.0f;
	// 頂点座標の設定
	servant->vertexWk[0].vtx.x = servant->pos.x - cosf(servant->BaseAngle + servant->rot.z) * servant->Radius;
	servant->vertexWk[0].vtx.y = servant->pos.y - sinf(servant->BaseAngle + servant->rot.z) * servant->Radius;
	servant->vertexWk[0].vtx.z = 0.0f;
	servant->vertexWk[1].vtx.x = servant->pos.x + cosf(servant->BaseAngle - servant->rot.z) * servant->Radius;
	servant->vertexWk[1].vtx.y = servant->pos.y - sinf(servant->BaseAngle - servant->rot.z) * servant->Radius;
	servant->vertexWk[1].vtx.z = 0.0f;
	servant->vertexWk[2].vtx.x = servant->pos.x - cosf(servant->BaseAngle - servant->rot.z) * servant->Radius;
	servant->vertexWk[2].vtx.y = servant->pos.y + sinf(servant->BaseAngle - servant->rot.z) * servant->Radius;
	servant->vertexWk[2].vtx.z = 0.0f;
	servant->vertexWk[3].vtx.x = servant->pos.x + cosf(servant->BaseAngle + servant->rot.z) * servant->Radius;
	servant->vertexWk[3].vtx.y = servant->pos.y + sinf(servant->BaseAngle + servant->rot.z) * servant->Radius;
	servant->vertexWk[3].vtx.z = 0.0f;
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureServant(int no, int cntPattern)
{
	SERVANT *servant = &ServantWk[no];			// Servantのポインターを初期化
	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT;
	// 頂点座標の設定
	servant->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	servant->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	servant->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	servant->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
//=============================================================================
// Servantの発射設定
//=============================================================================
void SetServant(D3DXVECTOR3 pos, int type)
{
	SERVANT *servant = &ServantWk[0];				// Servantのポインターを初期化
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < SERVANT_MAX; i++, servant++)
	{
		if (!servant->use)							// 未使用状態のServantを見つける
		{
			servant->use = true;					// 使用状態へ変更する
			switch (type) 
			{
			case 1:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT,			// ファイルの名前
					&D3DTextureServant);			// 読み込むメモリのポインタ	
				servant->status.ATK = SERVANT_ATK_A;
				servant->bullet_cooldown = SERVANT_COOLDOWN_A;
				break;
			case 2:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT2,			// ファイルの名前
					&D3DTextureServant);			// 読み込むメモリのポインタ	
				servant->status.ATK = SERVANT_ATK_B;
				servant->bullet_cooldown = SERVANT_COOLDOWN_B;
				break;
			case 3:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT3,			// ファイルの名前
					&D3DTextureServant);			// 読み込むメモリのポインタ
				servant->status.ATK = SERVANT_ATK_C;
				servant->bullet_cooldown = SERVANT_COOLDOWN_C;
				break;		
			}
			// 召喚音再生
			SE3 = LoadSound(18+type);			
			if (IsPlaying(SE3))
				SE3->SetCurrentPosition(0);
			else {
				PlaySound(SE3, E_DS8_FLAG_NONE);
			}
			servant->Texture = D3DTextureServant;	// テクスチャ情報			
			servant->pos = pos;						// 座標をセット		serva
			servant->abs_pos = pos;
			return;									// 1発セットしたので終了する
		}
		if (!servant->use)							// 未使用状態のServantを見つける
		{
			servant->use = true;					// 使用状態へ変更する
			switch (type) 
			{
			case 1:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT,			// ファイルの名前
					&D3DTextureServant);			// 読み込むメモリのポインタ	
				servant->status.ATK = SERVANT_ATK_A;
				servant->bullet_cooldown = SERVANT_COOLDOWN_A;
				break;
			case 2:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT2,			// ファイルの名前
					&D3DTextureServant);			// 読み込むメモリのポインタ	
				servant->status.ATK = SERVANT_ATK_B;
				servant->bullet_cooldown = SERVANT_COOLDOWN_B;
				break;
			case 3:
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_SERVANT3,			// ファイルの名前
					&D3DTextureServant);			// 読み込むメモリのポインタ
				servant->status.ATK = SERVANT_ATK_C;
				servant->bullet_cooldown = SERVANT_COOLDOWN_C;
				break;		
			}
			// 召喚音再生
			SE3 = LoadSound(18+type);			
			if (IsPlaying(SE3))
				SE3->SetCurrentPosition(0);
			else {
				PlaySound(SE3, E_DS8_FLAG_NONE);
			}
			servant->Texture = D3DTextureServant;	// テクスチャ情報			
			servant->pos = pos;						// 座標をセット		serva
			servant->abs_pos = pos;
			return;									// 1発セットしたので終了する
		}
	}
}
/*******************************************************************************
関数名:	SERVANT *GetSERVANTAdr( int pno )
引数:	int pno : SERVANT番号
戻り値:	SERVANT *
説明:	SERVANTのアドレスを取得する
*******************************************************************************/
SERVANT *GetServant(int pno)
{
	return &ServantWk[pno];
}