/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	エネミー処理 [enemy.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/

#ifndef _ENEMY_H_
#define _ENEMY_H_


// マクロ定義
#define	NUM_ENEMY					(2)			// ポリゴン数

#define TEXTURE_GAME_ENEMY			_T("data/TEXTURE/runningman003.png")	// サンプル用画像
#define TEXTURE_ENEMY_SIZE_X		(140)		// テクスチャサイズ
#define TEXTURE_ENEMY_SIZE_Y		(200)		// 同上

#define TEXTURE_PATTERN_DIVIDE_X_ENEMY	(5)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_ENEMY	(2)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_ENEMY			(TEXTURE_PATTERN_DIVIDE_X_ENEMY*TEXTURE_PATTERN_DIVIDE_Y_ENEMY)	// アニメーションパターン数
#define TIME_ANIMATION_ENEMY			(50)	// アニメーションの切り替わるカウント

#define ENEMY_MAX						(4)		// 敵の最大数

/*******************************************************************************
* 構造体定義
********************************************************************************/
typedef struct					// エネミー構造体
{
	CHARACTER	status;			// 共通パラメーター

	bool					use = TRUE;						// true:使用  false:未使用
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	D3DXVECTOR3				rot;							// ポリゴンの回転量	
	int						PatternAnim;					// アニメーションパターンナンバー
	int						CountAnim;						// アニメーションカウント

	LPDIRECT3DTEXTURE9		Texture;						// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク

	float					Radius;					// ポリゴンの半径
	float					BaseAngle = 0.2;		// ポリゴンの角度

} ENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int pno);


#endif
