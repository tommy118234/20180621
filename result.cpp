/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名: リザルト画面処理 [result.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "result.h"
#include "input.h"
#include "score.h"

#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo = NULL;	// テクスチャへのポインタ

VERTEX_2D				g_vertexWkResult[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultLogo[NUM_VERTEX];		// 頂点情報格納ワーク



ID3DXFont*				font3 = NULL;                // 文字のポインタ
bool					good_end = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RESULT,				// ファイルの名前
		&g_pD3DTextureResult);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RESULT_LOGO,		// ファイルの名前
		&g_pD3DTextureResultLogo);	// 読み込むメモリー
	
	// 頂点情報の作成
	MakeVertexResult();
	// 文字の読み込み
	D3DXCreateFont(pDevice,     //D3D Device
		30,               //Font height
		0,                //Font width
		FW_NORMAL,        //Font Weight
		1,                //MipLevels
		false,            //Italic
		DEFAULT_CHARSET,  //CharSet
		OUT_DEFAULT_PRECIS, //OutputPrecision
		ANTIALIASED_QUALITY, //Quality
		DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
		"HGS行書体",          //pFacename,
		&font3);         //ppFont	

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if(g_pD3DTextureResult != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if(g_pD3DTextureResultLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultLogo->Release();
		g_pD3DTextureResultLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	if (GetScore() <= 1 && GetEnemy(0)->status.HP < 4500) {

		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT2,				// ファイルの名前
			&g_pD3DTextureResult);		// 読み込むメモリー

		SetBGM(10);
		InitScore(1);

		InitEnemy(1);
		//Good End
		good_end = true;
	}
	if(GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、ステージを切り替える

		good_end = false;

		SetStage( STAGE_TITLE );
		SetBGM(1);
		InitScore(1);		// スコアの再初期化	
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetStage(STAGE_TITLE);
		SetBGM(1);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResult, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultLogo, sizeof(VERTEX_2D));

	// 文字の描画		
	if (good_end) {
		const char* strings[] = { "残ったの魂も、安寧の赤月に戻る\n あぁー、君(あたし)も。。。\nGOOD END?","制作：TSUI\n音楽:Music is VFR, 魔王魂" };
		D3DCOLOR colors[] = { D3DCOLOR_ARGB(255, 120, 20, 70), D3DCOLOR_ARGB(255, 237, 200, 50) };
		RECT r = { 0  , SCREEN_HEIGHT / 4 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point
		RECT r2 = { 0 , 2 * SCREEN_HEIGHT / 3 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point	
		font3->DrawText(NULL, strings[0], -1, &r, DT_CENTER | DT_VCENTER, colors[1]);
		font3->DrawText(NULL, strings[1], -1, &r2, DT_CENTER | DT_VCENTER, colors[1]);
	}
	else {
		const char* strings[] = { "始まる, また終わらなかった\n TRY AGAIN?","制作：TSUI\n音楽:Music is VFR,	魔王魂" };
		D3DCOLOR colors[] = { D3DCOLOR_ARGB(255, 120, 20, 70), D3DCOLOR_ARGB(255, 237, 200, 50) };
		RECT r = { 0  , SCREEN_HEIGHT / 4 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point
		RECT r2 = { 0 , 2 * SCREEN_HEIGHT / 3 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point	
		font3->DrawText(NULL, strings[0], -1, &r, DT_CENTER | DT_VCENTER, colors[0]);
		font3->DrawText(NULL, strings[1], -1, &r2, DT_CENTER | DT_VCENTER, colors[1]);
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{	
	// 頂点座標の設定
	g_vertexWkResult[0].vtx = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	g_vertexWkResult[1].vtx = D3DXVECTOR3(RESULT_POS_X + RESULT_SIZE_X, RESULT_POS_Y, 0.0f);
	g_vertexWkResult[2].vtx = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y + RESULT_SIZE_Y, 0.0f);
	g_vertexWkResult[3].vtx = D3DXVECTOR3(RESULT_POS_X + RESULT_SIZE_X, RESULT_POS_Y + RESULT_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResult[0].rhw =
	g_vertexWkResult[1].rhw =
	g_vertexWkResult[2].rhw =
	g_vertexWkResult[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResult[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResult[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResultLogo[0].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultLogo[1].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultLogo[2].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
	g_vertexWkResultLogo[3].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultLogo[0].rhw =
	g_vertexWkResultLogo[1].rhw =
	g_vertexWkResultLogo[2].rhw =
	g_vertexWkResultLogo[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResultLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}
