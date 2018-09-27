/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	背景処理 [bg.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _BG_H_
#define _BG_H_
// マクロ定義
#define TEXTURE_GAME_BG00		_T("data/BG/moon.png")			// 背景画像1
#define TEXTURE_GAME_BG01		_T("data/BG/story1.png")		// 背景画像2
#define TEXTURE_GAME_BG02		_T("data/BG/story2.png")		// 背景画像3
#define TEXTURE_GAME_BG03		_T("data/BG/battleGuide.png")	// 背景画像4
#define TEXTURE_GAME_BG04		_T("data/BG/battle_ice5.png")	// 背景画像5
#define TEXTURE_GAME_BG05		_T("data/BG/ice_castle.png")	// 背景画像6
#define TEXTURE_GAME_BG06		_T("data/BG/moon_red.png")		// 背景画像7
#define BG00_SIZE_X				(800)							// テクスチャサイズ
#define BG00_SIZE_Y				(600)							// 同上
#define BG00_POS_X				(0)								// ポリゴンの初期位置X(左上)
#define BG00_POS_Y				(0)								// 同上
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(int type);
void	UninitBG(void);
void	UpdateBG(void);
void	DrawBG(void);
void	SwitchBG(int type);
void	ChangeBG(void);
#endif
