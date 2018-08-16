/*******************************************************************************
* タイトル:		DirectXゲーム〜はじめての個人作品〜
* プログラム名:	背景処理 [bg.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _BG_H_
#define _BG_H_

// マクロ定義
//#define TEXTURE_GAME_BG00		_T("data/TEXTURE/earth.png")	// サンプル用画像
#define TEXTURE_GAME_BG00		_T("data/BG/moon.png")	// サンプル用画像
#define BG00_SIZE_X			(800)	// テクスチャサイズ
#define BG00_SIZE_Y			(600)	// 同上
#define BG00_POS_X			(0)		// ポリゴンの初期位置X(左上)
#define BG00_POS_Y			(0)		// 同上

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(int type);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif
