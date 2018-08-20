/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名: プレーヤー処理 [player.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "bullet.h"
// マクロ定義
#define	NUM_PLAYER		(2)					// ポリゴン数

#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/runningman001.png")	// サンプル用画像
#define TEXTURE_PLAYER_SIZE_X	(140/2) // テクスチャサイズ
#define TEXTURE_PLAYER_SIZE_Y	(200/2) // 同上 

#define TEXTURE_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数


#define TEXTURE_GAME_PLAYER2	_T("data/TEXTURE/runningman000.png")	// サンプル用画像
#define TEXTURE_PLAYER_SIZE_X2	(100/2) // テクスチャサイズ
#define TEXTURE_PLAYER_SIZE_Y2	(200/2) // 同上 
#define TEXTURE_PATTERN_DIVIDE_X2	(8)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y2	(1)	// アニメパターンのテクスチャ内分割数（Y)



#define TIME_ANIMATION				(50)	// アニメーションの切り替わるカウント

#define PLAYER_MAX					(1) // プレイヤーの最大数
/*******************************************************************************
* 構造体定義
********************************************************************************/
typedef struct					// キャラクターの基本構造体
{
	CHARACTER	status;			// 共通パラメーター
	
	bool					use;						// true:使用  false:未使用
	D3DXVECTOR3				pos;				// ポリゴンの移動量
	D3DXVECTOR3				rot;				// ポリゴンの回転量
	int						PatternAnim;				// アニメーションパターンナンバー
	int						CountAnim;				// アニメーションカウント
	
	LPDIRECT3DTEXTURE9		Texture = NULL;	// テクスチャへのポリゴン
	VERTEX_2D				vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float					g_fRadiusPlayer;			// ポリゴンの半径
	float					g_fBaseAnglePlayer;			// ポリゴンの角度
	
	int						direction = -1;
	int						bullet_num = 0;
	int						score = 0;
	int						gravity = 5;
	int						view_mode = 1;
	int                     ready = 0;
} PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void ChangePlayer(void);
PLAYER *GetPlayer(int pno);	// プレイヤーの(アドレス)を取得


#endif
