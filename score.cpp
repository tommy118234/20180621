//=============================================================================
//
// ポリゴン処理 [score.cpp]
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(void);
void SetVertexScore(void);
void SetTextureScore(int cntPattern);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
SCORE	score[SCORE_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(void)
{	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	for (int i = 0; i < SCORE_MAX; i++) {		
		SCORE *score = GetScore(i);
		score->g_posScore = D3DXVECTOR3(SCREEN_WIDTH - (i+1) * TEXTURE_SAMPLE00_SIZE_X4, 0.0f, 0.0f);
		// 頂点情報の作成
		MakeVertexScore();
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
		TEXTURE_GAME_SAMPLE003,					// ファイルの名前
		&score->g_pD3DTextureScore);			// 読み込むメモリのポインタ
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	if (score->g_pD3DTextureScore != NULL)	//
	{	// テクスチャの開放
		score->g_pD3DTextureScore->Release();
		score->g_pD3DTextureScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	PLAYER *player = GetPlayer(0);
	SCORE * score = GetScore(0);
	for (int i = 0; i < SCORE_MAX; i++) {
		score = GetScore(i);
		int k = player->score;
		switch (i) {		
		case 1:
			k = player->score/10;
			break;
		case 2:
			k = player->score / 100;
			break;
		case 3:
			k = player->score / 1000;
			break;
		case 4:
			k = player->score / 10000;
			break;			
		}
		score->g_nPatternAnim3 = k % ANIM_PATTERN_NUM4;
	}	
	MakeVertexScore();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = GetBullet(0);
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < SCORE_MAX; i++) {
		SCORE *score = GetScore(i);	
		// テクスチャの設定
		pDevice->SetTexture(0, score->g_pD3DTextureScore);
		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, score->g_vertexWk3, sizeof(VERTEX_2D));
	}
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(void)
{
	for (int i = 0; i < SCORE_MAX; i++) {
		SCORE *score = GetScore(i);
		// 頂点座標の設定
		SetVertexScore();
		// rhwの設定
		score->g_vertexWk3[0].rhw =
		score->g_vertexWk3[1].rhw =
		score->g_vertexWk3[2].rhw =
		score->g_vertexWk3[3].rhw = 1.0f;
		// 反射光の設定
		score->g_vertexWk3[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		score->g_vertexWk3[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		score->g_vertexWk3[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		score->g_vertexWk3[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		// テクスチャ座標の設定
		SetTextureScore(i);
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexScore(void)
{
	for (int i = 0; i < SCORE_MAX; i++) {
		SCORE *score = GetScore(i);
		// 頂点座標の設定
		score->g_vertexWk3[0].vtx = D3DXVECTOR3(score->g_posScore.x, score->g_posScore.y, score->g_posScore.z);
		score->g_vertexWk3[1].vtx = D3DXVECTOR3(score->g_posScore.x + TEXTURE_SAMPLE00_SIZE_X4, score->g_posScore.y, score->g_posScore.z);
		score->g_vertexWk3[2].vtx = D3DXVECTOR3(score->g_posScore.x, score->g_posScore.y + TEXTURE_SAMPLE00_SIZE_Y4, score->g_posScore.z);
		score->g_vertexWk3[3].vtx = D3DXVECTOR3(score->g_posScore.x + TEXTURE_SAMPLE00_SIZE_X4, score->g_posScore.y + TEXTURE_SAMPLE00_SIZE_Y4, score->g_posScore.z);
	}

}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureScore(int cntPattern)
{	
	SCORE *score = GetScore(cntPattern);
	// テクスチャ座標の設定
	score->g_vertexWk3[0].tex = D3DXVECTOR2((float)(score->g_nPatternAnim3 % TEXTURE_PATTERN_DIVIDE_X4) / TEXTURE_PATTERN_DIVIDE_X4, (float)(score->g_nPatternAnim3 / TEXTURE_PATTERN_DIVIDE_X4) / TEXTURE_PATTERN_DIVIDE_Y4);
	score->g_vertexWk3[1].tex = D3DXVECTOR2((float)((score->g_nPatternAnim3 % TEXTURE_PATTERN_DIVIDE_X4) + 1) /TEXTURE_PATTERN_DIVIDE_X4, (float)(score->g_nPatternAnim3 / TEXTURE_PATTERN_DIVIDE_X4) / TEXTURE_PATTERN_DIVIDE_Y4);
	score->g_vertexWk3[2].tex = D3DXVECTOR2((float)(score->g_nPatternAnim3 % TEXTURE_PATTERN_DIVIDE_X4) / TEXTURE_PATTERN_DIVIDE_X4, (float)((score->g_nPatternAnim3 / TEXTURE_PATTERN_DIVIDE_X4) + 1) / TEXTURE_PATTERN_DIVIDE_Y4);
	score->g_vertexWk3[3].tex = D3DXVECTOR2((float)((score->g_nPatternAnim3 % TEXTURE_PATTERN_DIVIDE_X4) + 1) / TEXTURE_PATTERN_DIVIDE_X4, (float)((score->g_nPatternAnim3 / TEXTURE_PATTERN_DIVIDE_X4) + 1) /TEXTURE_PATTERN_DIVIDE_Y4);
}
/*******************************************************************************
関数名:	SCORE *GetMapAdr( int pno )
引数:	int pno : エネミー番号
戻り値:	SCORE *
説明:	エネミーのアドレスを取得する
*******************************************************************************/
SCORE *GetScore(int pno)
{
	return &score[pno];
}