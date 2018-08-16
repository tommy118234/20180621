/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	タイトル画面処理 [title.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _TITLE_H_
#define _TITLE_H_
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	TEXTURE_TITLE		_T("data/TEXTURE/bg000.jpg")			// 読み込むテクスチャファイル名
//#define	TEXTURE_TITLE_LOGO	_T("data/TEXTURE/title_logo.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE		_T("data/BG/moon.png")				// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO	_T("data/TEXTURE/title_moon.png")		// 読み込むテクスチャファイル名
#define	TITLE_POS_X			(0)										// タイトルの表示位置
#define	TITLE_POS_Y			(0)										// タイトルの表示位置
#define	TITLE_SIZE_X		(800)									// タイトルの幅
#define	TITLE_SIZE_Y		(600)									// タイトルの高さ
#define	TITLELOGO_POS_X		(200)									// タイトルロゴの表示位置
#define	TITLELOGO_POS_Y		(50)									// タイトルロゴの表示位置
#define	TITLELOGO_SIZE_X	(480/2)									// タイトルロゴの幅
#define	TITLELOGO_SIZE_Y	(246/2)//80)									// タイトルロゴの高さ
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
#endif