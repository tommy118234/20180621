//=============================================================================
//
// ポリゴン表示Mainヘッダー [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止

#include "windows.h"
#include "d3dx9.h"
#include <tchar.h>


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

// フィールド用
#define FH_MAX		(20)		// 縦幅
#define FW_MAX		(60)		// 横幅
// 共通パラメータ用
#define	NAME_MAX	(256)		// 名前BufのMAX
#define PLAYER_MAX	(1)			// プレイヤーの数
#define ENEMY_MAX	(1)			// 敵の数
#define BULLET_MAX	(100)			// Bulletの数
#define MAP_MAX     (1)			// Mapの数

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
} VERTEX_2D;


LPDIRECT3DDEVICE9 GetDevice(void);		// デバイス取得関数


#endif