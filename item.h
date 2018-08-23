/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	敵処理 [item.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _ITEM_H_
#define _ITEM_H_


// マクロ定義
#define	NUM_ITEM						(2)			// ポリゴン数

#define TEXTURE_GAME_ITEM				_T("data/TEXTURE/item1.png")	// サンプル用画像
#define TEXTURE_GAME_ITEM2				_T("data/TEXTURE/item2.png")	// サンプル用画像
#define TEXTURE_GAME_ITEM3				_T("data/TEXTURE/item3.png")	// サンプル用画像
#define TEXTURE_ITEM_SIZE_X			(100/2)		// テクスチャサイズ
#define TEXTURE_ITEM_SIZE_Y			(100/2)		// 同上
#define TEXTURE_PATTERN_DIVIDE_X_ITEM	(1)			// アニメパターンのテクスチャ内分割数（X)											// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_ITEM	(1)			// アニメパターンのテクスチャ内分割数（Y)											// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_ITEM			(TEXTURE_PATTERN_DIVIDE_X_ITEM*TEXTURE_PATTERN_DIVIDE_Y_ITEM)	// アニメーションパターン数
#define TIME_ANIMATION_ITEM			(50)			// アニメーションの切り替わるカウント
#define ITEM_MAX						(20)			// アイテムの最大数
#define ITEM_SPEED					(6.0f)		// アイテムの移動スピード
/*******************************************************************************
* 構造体定義
********************************************************************************/
typedef struct											// アイテムの構造体
{
	bool					use;						// true:使用  false:未使用
	D3DXVECTOR3				pos;						// ポリゴンの移動量
	D3DXVECTOR3				rot;						// ポリゴンの回転量
	int						PatternAnim;				// アニメーションパターンナンバー
	int						CountAnim;					// アニメーションカウン
	LPDIRECT3DTEXTURE9		Texture;					// テクスチャへのポリゴン
	VERTEX_2D				vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	float					Radius;						// ポリゴンの半径
	float					BaseAngle = 0.0;			// ポリゴンの角度
	int						type;
} ITEM;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(int type);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, int type);
ITEM *GetItem(int pno);									// アイテムの(アドレス)を取得
#endif