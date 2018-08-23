/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	Beam処理 [bullet.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _BEAM_H_
#define _BEAM_H_

// マクロ定義
#define	NUM_BEAM						(2)			// ポリゴン数
#define TEXTURE_GAME_BEAM				_T("data/TEXTURE/beam.png")	// サンプル用画像
#define TEXTURE_BEAM_SIZE_X				(20)		// テクスチャサイズ
#define TEXTURE_BEAM_SIZE_Y				(20)		// 同上
#define TEXTURE_PATTERN_DIVIDE_X_BEAM	(1)			// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_BEAM	(1)			// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_BEAM			(TEXTURE_PATTERN_DIVIDE_X_BEAM*TEXTURE_PATTERN_DIVIDE_Y_BEAM)	// アニメーションパターン数
#define TIME_ANIMATION_BEAM				(50)		// アニメーションの切り替わるカウント

#define BEAM_MAX						(50)		// Beamの最大数
#define BEAM_SPEED						(5.0f)		// Beamの移動スピード
/*******************************************************************************
* 構造体定義
********************************************************************************/
typedef struct										// Beamの構造体
{
	bool					use;					// true:使用  false:未使用
	D3DXVECTOR3				pos;					// ポリゴンの移動量
	D3DXVECTOR3				rot;					// ポリゴンの回転量
	int						PatternAnim;			// アニメーションパターンナンバー
	int						CountAnim;				// アニメーションカウント
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャへのポリゴン
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	float					Radius;					// ポリゴンの半径
	float					BaseAngle;				// ポリゴンの角度	
	int						atk;
} BEAM;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBeam(int type);
void	UninitBeam(void);
void	UpdateBeam(void);
void	DrawBeam(void);
void	SetBeam(D3DXVECTOR3 pos, float rot, int ATK);
BEAM	*GetBeam(int pno);							// Beamの(アドレス)を取得
#endif