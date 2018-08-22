/*******************************************************************************
* タイトル:		DirectXゲーム〜はじめての個人作品〜
* プログラム名:	敵処理 [SERVANT.h]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#ifndef _SERVANT_H_
#define _SERVANT_H_


// マクロ定義
#define	NUM_SERVANT						(2)			// ポリゴン数

#define TEXTURE_GAME_SERVANT				_T("data/TEXTURE/servant001.png")	// サンプル用画像
#define TEXTURE_GAME_SERVANT2				_T("data/TEXTURE/servant002.png")	// サンプル用画像
#define TEXTURE_GAME_SERVANT3				_T("data/TEXTURE/servant003.png")	// サンプル用画像
#define TEXTURE_SERVANT_SIZE_X			(50)		// テクスチャサイズ
#define TEXTURE_SERVANT_SIZE_Y			(49)		// 同上

#define TEXTURE_PATTERN_DIVIDE_X_SERVANT	(1)			// アニメパターンのテクスチャ内分割数（X)											// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_SERVANT	(1)			// アニメパターンのテクスチャ内分割数（Y)											// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_SERVANT			(TEXTURE_PATTERN_DIVIDE_X_SERVANT*TEXTURE_PATTERN_DIVIDE_Y_SERVANT)	// アニメーションパターン数
#define TIME_ANIMATION_SERVANT			(50)			// アニメーションの切り替わるカウント

#define SERVANT_MAX						(10)			// バレットの最大数

#define SERVANT_SPEED					(6.0f)		// バレットの移動スピード

/*******************************************************************************
* 構造体定義
********************************************************************************/

typedef struct					// バレットの構造体
{
	CHARACTER	status;			// 共通パラメーター

	bool					use;					// true:使用  false:未使用
	D3DXVECTOR3				pos;					// ポリゴンの移動量
	D3DXVECTOR3				rot;					// ポリゴンの回転量
	int						PatternAnim;			// アニメーションパターンナンバー
	int						CountAnim;				// アニメーションカウント

	LPDIRECT3DTEXTURE9		Texture = NULL;			// テクスチャへのポリゴン
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク

	float					Radius;					// ポリゴンの半径
	float					BaseAngle = 0.2;				// ポリゴンの角度

	int						direction = 1;
	int						gravity = 5;
	int						view_mode = 1;
	int                     ready = 0;

	int						bullet_cooldown = 50;

} SERVANT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSERVANT(int type);
void UninitSERVANT(void);
void UpdateSERVANT(void);
void DrawSERVANT(void);
void SetSERVANT(D3DXVECTOR3 pos, int type);
SERVANT *GetSERVANT(int pno);	// プレイヤーの(アドレス)を取得


#endif
