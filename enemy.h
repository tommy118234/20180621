//=============================================================================
//
// ポリゴン処理 [enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_


// マクロ定義
//#define TEXTURE_GAME_SAMPLE00	_T("data/TEXTURE/runningman000.png")	// サンプル用画像
//#define TEXTURE_SAMPLE00_SIZE_X	(200) // テクスチャサイズ
//#define TEXTURE_SAMPLE00_SIZE_Y	(200) // 同上
//
//
//#define TEXTURE_PATTERN_DIVIDE_X	(8)	// アニメパターンのテクスチャ内分割数（X)
//#define TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
//#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
//#define TIME_ANIMATION				(100)	// アニメーションの切り替わるカウント


#define TEXTURE_GAME_SAMPLE002 _T("data/TEXTURE/runningman003.png")	// サンプル用画像
#define TEXTURE_SAMPLE00_SIZE_X2	(140) // テクスチャサイズ
#define TEXTURE_SAMPLE00_SIZE_Y2	(200) // 同上

#define TEXTURE_PATTERN_DIVIDE_X2	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y2	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM2			(TEXTURE_PATTERN_DIVIDE_X2*TEXTURE_PATTERN_DIVIDE_Y2)	// アニメーションパターン数
#define TIME_ANIMATION2				(50)	// アニメーションの切り替わるカウント

/*******************************************************************************
* 構造体定義
********************************************************************************/
typedef struct					// キャラクターの基本構造体
{
	CHARACTER	status;			// 共通パラメーター

								//LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// テクスチャへのポリゴン
								//
								//VERTEX_2D				g_vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
	D3DXVECTOR3				g_posEnemy;						// ポリゴンの移動量
	D3DXVECTOR3				g_rotEnemy;						// ポリゴンの回転量
	int						g_nCountAnim2;						// アニメーションカウント
	int						g_nPatternAnim2;						// アニメーションパターンナンバー

	float					g_fRadiusEnemy;					// ポリゴンの半径
	float					g_fBaseAngleEnemy;					// ポリゴンの角度
	int						direction2 = -1;
} ENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int pno);	// プレイヤーの(アドレス)を取得


#endif
