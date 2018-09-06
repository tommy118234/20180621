/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	背景処理 [bg.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "bg.h"
#include "input.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBG(void);
void	SetVertexBG(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBG = NULL;		// テクスチャへのポインタ
VERTEX_2D				vertexWkBG[NUM_VERTEX];		// 頂点情報格納ワーク
D3DXVECTOR3				posBG;						// 背景の位置
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBG(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	
	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		TEXTURE_GAME_BG00,							// ファイルの名前
		&D3DTextureBG);								// 読み込むメモリー
	}
	else if (type == 1) 
	{
		if (D3DTextureBG != NULL)
		{
			// テクスチャの開放
			D3DTextureBG->Release();
			D3DTextureBG = NULL;
		}
	}
	posBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 頂点情報の作成
	MakeVertexBG();
	SetVertexBG();
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void)
{
	if (D3DTextureBG != NULL)
	{	
		// テクスチャの開放
		D3DTextureBG->Release();
		D3DTextureBG = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBG(void)
{
	// 毎フレーム実行する処理
	// スクロール処理
	PLAYER *player = GetPlayer(0);
	//
	posBG.x = -player->pos.x / 4.0f;	
	//
	//
	//posBG.x -= 5 * (-sinf(player->BaseAngle + player->rot.z) + cosf(player->BaseAngle + player->rot.z));
	//posBG.y -= 5 * (cosf(player->BaseAngle + player->rot.z) + sinf(player->BaseAngle + player->rot.z));
	SetVertexBG();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, D3DTextureBG);
	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkBG, sizeof(VERTEX_2D));
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBG(void)
{
	// 頂点座標の設定
	vertexWkBG[0].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y, 0.0f);
	vertexWkBG[1].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y, 0.0f);
	vertexWkBG[2].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f);
	vertexWkBG[3].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f);
	// rhwの設定
	vertexWkBG[0].rhw =
	vertexWkBG[1].rhw =
	vertexWkBG[2].rhw =
	vertexWkBG[3].rhw = 1.0f;
	// 反射光の設定
	vertexWkBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	vertexWkBG[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkBG[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkBG[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkBG[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBG(void)
{
	// 頂点座標の設定
	vertexWkBG[0].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y, 0.0f) + posBG;
	vertexWkBG[1].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y, 0.0f) + posBG;
	vertexWkBG[2].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f) + posBG;
	vertexWkBG[3].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f) + posBG;

}
//=============================================================================
// 背景の設定
//=============================================================================
void SwitchBG(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (D3DTextureBG != NULL)
	{
		// テクスチャの開放
		D3DTextureBG->Release();
		D3DTextureBG = NULL;
	}
	switch (type)
	{
	case 1:		
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_GAME_BG01,								// ファイルの名前
		&D3DTextureBG);									// 読み込むメモリー		
		break;
	case 2:
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_GAME_BG02,								// ファイルの名前
		&D3DTextureBG);									// 読み込むメモリー
		break;
	case 3:
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_GAME_BG03,								// ファイルの名前
		&D3DTextureBG);									// 読み込むメモリー				
		break;
	case 4:
		ChangeBG();
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_GAME_BG04,								// ファイルの名前
		&D3DTextureBG);									// 読み込むメモリー
		break;
	case 5:
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_GAME_BG05,								// ファイルの名前
		&D3DTextureBG);									// 読み込むメモリー
		break;
	case 6:
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_GAME_BG06,								// ファイルの名前
		&D3DTextureBG);									// 読み込むメモリー
		break;	
	}
}
void ChangeBG(void)
{
#undef	 BG00_SIZE_X	
#undef	 BG00_SIZE_Y			
#define  BG00_SIZE_X	(1440)	// テクスチャサイズ
#define  BG00_SIZE_Y	(1080)	// 同上
}