/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名: リザルト画面処理 [result.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _RESULT_H_
#define _RESULT_H_
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT		_T("data/TEXTURE/bg002.jpg")			// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO	_T("data/TEXTURE/result_logo.png")		// 読み込むテクスチャファイル名
#define	RESULT_POS_X			(0)							// タイトルの表示位置
#define	RESULT_POS_Y			(0)							// タイトルの表示位置
#define	RESULT_SIZE_X			(800)						// タイトルの幅
#define	RESULT_SIZE_Y			(600)						// タイトルの高さ
#define	RESULTLOGO_POS_X		(160)						// タイトルロゴの表示位置
#define	RESULTLOGO_POS_Y		(200)						// タイトルロゴの表示位置
#define	RESULTLOGO_SIZE_X		(480)						// タイトルロゴの幅
#define	RESULTLOGO_SIZE_Y		(80)						// タイトルロゴの高さ
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
