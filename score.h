//=============================================================================
//
// ポリゴン処理 [score.h]
// Author : 
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

// マクロ定義
#define TEXTURE_GAME_SAMPLE003 _T("data/TEXTURE/score.png")	// サンプル用画像
#define TEXTURE_SAMPLE00_SIZE_X4	(30) // テクスチャサイズ
#define TEXTURE_SAMPLE00_SIZE_Y4	(50) // 同上

#define TEXTURE_PATTERN_DIVIDE_X4	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y4	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM4			(TEXTURE_PATTERN_DIVIDE_X4 * TEXTURE_PATTERN_DIVIDE_Y4)	// アニメーションパターン数
/*******************************************************************************
* 構造体定義
********************************************************************************/
typedef struct					// キャラクターの基本構造体
{
	int						value;
	D3DXVECTOR3				g_posScore;						// ポリゴンの移動量
	int						g_nPatternAnim3;				// アニメーションパターンナンバー
	LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;		// テクスチャへのポリゴン
	VERTEX_2D				g_vertexWk3[NUM_VERTEX];		// 頂点情報格納
}SCORE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
SCORE *GetScore(int pno);	// プレイヤーの(アドレス)を取得


#endif
