/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	Main処理 [main.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止

#include "windows.h"
#include "d3dx9.h"
#include <tchar.h>
#include <stdio.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

#define SCREEN_WIDTH	(640)				// ウインドウの幅
#define SCREEN_HEIGHT	(480)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)					// 頂点数
#define	NUM_POLYGON		(2)					// ポリゴン数

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FPS_RATE			(30)						// FPS
#define SAFE_RELEASE(ptr)	{if(ptr){(ptr)->Release();(ptr) = NULL;}}

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct {
	char name[32];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int LUCK;
	int EXP;
	int LV;
}CHARACTER;

// 上記頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
}	VERTEX_2D;

// 上記頂点フォーマットに合わせた構造体を定義
typedef struct
{
	float x, y, z, rhw;
	D3DCOLOR color;
	static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
}	VERTEX_2D_DIF;

// 画面遷移定数
enum E_STAGE
{
	STAGE_TITLE,
	STAGE_TUTOR,
	STAGE_GAME,
	STAGE_GAME_END,
	STAGE_RESULT
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);		// デバイス取得関数

void SetStage(int stage);
void SetBGM(int no);
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2);
#endif