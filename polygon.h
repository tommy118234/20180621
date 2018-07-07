//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_


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


#define TEXTURE_GAME_SAMPLE00	_T("data/TEXTURE/runningman001.png")	// サンプル用画像
#define TEXTURE_SAMPLE00_SIZE_X	(140) // テクスチャサイズ
#define TEXTURE_SAMPLE00_SIZE_Y	(200) // 同上

#define TEXTURE_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION				(50)	// アニメーションの切り替わるカウント

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);


#endif
