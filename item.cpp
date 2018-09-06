/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	Item処理 [item.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "item.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexItem(int no);
void SetTextureItem(int no, int cntPattern);
void SetVertexItem(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureItem = NULL;		// テクスチャへのポリゴン
ITEM					itemWk[ITEM_MAX];			// Item構造体


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[0];		// エネミーのポインターを初期化
	PLAYER *player = GetPlayer(0);



	for (int i = 0; i < ITEM_MAX; i++, item++) {
		// テクスチャーの初期化を行う？
		if (type == 0)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
				TEXTURE_GAME_ITEM,			// ファイルの名前
				&g_pD3DTextureItem);			// 読み込むメモリのポインタ			
		}
		else if (type == 1) {
			UninitItem;
		}

		item->use = FALSE;									// 未使用（発射されていない弾）
		item->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);			// 座標データを初期化
		item->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転データを初期化
		item->PatternAnim = 0;								// アニメパターン番号をランダムで初期化
		item->CountAnim = 0;									// アニメカウントを初期化


		item->BaseAngle = atan2f(TEXTURE_ITEM_SIZE_Y, TEXTURE_ITEM_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ITEM_SIZE_X , TEXTURE_ITEM_SIZE_Y);
		item->Radius = D3DXVec2Length(&temp);

		item->Texture = g_pD3DTextureItem;					// テクスチャ情報
		MakeVertexItem(i);									// 頂点情報の作成
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	if (g_pD3DTextureItem != NULL)	//
	{	// テクスチャの開放
		g_pD3DTextureItem->Release();
		g_pD3DTextureItem = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{

	ITEM *item = itemWk;				// Itemのポインターを初期化

	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->use == true)			// 未使用状態のItemを見つける
		{			
			// 画面外まで進んだ？
			if (item->pos.y < -TEXTURE_ITEM_SIZE_Y || item->pos.y > SCREEN_HEIGHT
			|| item->pos.x < -TEXTURE_ITEM_SIZE_X || item->pos.x > SCREEN_WIDTH)
			{
				item->use = false;
				//item->Texture = NULL;					// テクスチャ情報
				//item->Texture = g_pD3DTextureItem;	// テクスチャ情報				
				//item->pos.x = -100.0f;
				//item->pos = D3DXVECTOR3(player->pos.x + TEXTURE_ITEM_SIZE_X / 2, player->pos.y, 0.0f);
			}
			SetVertexItem(i);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = itemWk;				// Itemのポインターを初期化

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->use)					// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, item->Texture);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_ITEM, item->vertexWk, sizeof(VERTEX_2D));
		}
	}
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexItem(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[no];

	// 頂点座標の設定
	SetVertexItem(no);
	// rhwの設定
	item->vertexWk[0].rhw =
		item->vertexWk[1].rhw =
		item->vertexWk[2].rhw =
		item->vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	item->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	item->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	item->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_ITEM, 0.0f);
	item->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ITEM);
	item->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_ITEM, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ITEM);

	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexItem(int no)
{
	ITEM *item = &itemWk[no];			// Itemのポインターを初期化

	// 頂点座標の設定	
	item->pos.x = item->abs_pos.x - GetPlayer(0)->pos.x / 4.0f;

	item->vertexWk[0].vtx.x = item->pos.x - cosf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[0].vtx.y = item->pos.y - sinf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[0].vtx.z = 0.0f;
	
	item->vertexWk[1].vtx.x = item->pos.x + cosf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[1].vtx.y = item->pos.y - sinf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[1].vtx.z = 0.0f;
	
	item->vertexWk[2].vtx.x = item->pos.x - cosf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[2].vtx.y = item->pos.y + sinf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[2].vtx.z = 0.0f;
	
	item->vertexWk[3].vtx.x = item->pos.x + cosf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[3].vtx.y = item->pos.y + sinf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[3].vtx.z = 0.0f;
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureItem(int no, int cntPattern)
{
	ITEM *item = &itemWk[no];			// Itemのポインターを初期化

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_ITEM;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_ITEM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_ITEM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ITEM;
	// 頂点座標の設定
	item->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	item->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	item->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	item->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// Itemの発射設定
//=============================================================================
void SetItem(D3DXVECTOR3 pos, int type)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[0];			// Itemのポインターを初期化

	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->use == false)			// 未使用状態のItemを見つける
		{
			item->use = true;				// 使用状態へ変更する
			item->pos = pos;				// 座標をセット
			item->abs_pos = pos;				// 座標をセット
			switch (type) {
			case 0:
				item->type = 1;
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_ITEM,			// ファイルの名前
					&item->Texture);			// 読み込むメモリのポインタ
				break;
			case 1:
				item->type = 2;
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_ITEM2,			// ファイルの名前
					&item->Texture);			// 読み込むメモリのポインタ
				break;
			case 2:
				item->type = 3;
				D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
					TEXTURE_GAME_ITEM3,			// ファイルの名前
					&item->Texture);			// 読み込むメモリのポインタ
				break;
			}			
			return;							// 1発セットしたので終了する
		}
		
	}
}

/*******************************************************************************
関数名:	ITEM *GetItemAdr( int pno )
引数:	int pno : Item番号
戻り値:	ITEM *
説明:	Itemのアドレスを取得する
*******************************************************************************/
ITEM *GetItem(int pno)
{
	return &itemWk[pno];
}