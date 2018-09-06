/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	タイトル画面処理 [title.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "title.h"
#include "player.h"
#include "input.h"
#include "bg.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// テクスチャへのポインタ

VERTEX_2D				g_vertexWkTitle[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkTitleLogo[NUM_VERTEX];		// 頂点情報格納ワーク

ID3DXFont*				font = NULL;                // 文字のポインタ
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_TITLE,				// ファイルの名前
		&g_pD3DTextureTitle);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_TITLE_LOGO,			// ファイルの名前
		&g_pD3DTextureTitleLogo);	// 読み込むメモリー

									// 頂点情報の作成
	MakeVertexTitle();


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
		&font);         //ppFont	
	

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_pD3DTextureTitle != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if (g_pD3DTextureTitleLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitleLogo->Release();
		g_pD3DTextureTitleLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) )
	{// Enter押したら、ステージを切り替える	
	
		SetStage(1);		

		//SetStage(STAGE_GAME);
		SetBGM(2);
	}
	if (GetKeyboardTrigger(DIK_N))
	{// N押したら、ステージを切り替える		
		//SetStage(STAGE_TUTOR);
		GetPlayer(0)->ready = 2;

		GetPlayer(0)->view_mode = 0;
		SwitchBG(4);
		SetBGM(3);
		SetStage(STAGE_GAME);
	}
	// ゲームパッドでで移動処理
	else if (IsButtonTriggered(0, BUTTON_START))
	{
		SetStage(STAGE_TUTOR);
		//SetStage(STAGE_GAME);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetStage(STAGE_TUTOR);
		//SetStage(STAGE_GAME);
	}	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitle);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitle, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleLogo, sizeof(VERTEX_2D));	

	// 文字の描画		

	const char* strings[] = { "'Enter'で始まる,\n'N'で教学スキップ","制作：TSUI\n音楽:Music is VFR	魔王魂" };
	D3DCOLOR colors[] = { D3DCOLOR_ARGB(255, 120, 20, 70), D3DCOLOR_ARGB(255, 237, 200, 50)};
	RECT r = { 0  , SCREEN_HEIGHT / 4 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point
	RECT r2 = { 0 , 2* SCREEN_HEIGHT/3 ,SCREEN_WIDTH/2,SCREEN_HEIGHT}; // starting point	
	font->DrawText(NULL, strings[0], -1, &r, DT_CENTER | DT_VCENTER, colors[0]);
	//font->DrawText(NULL, strings[1], -1, &r2, DT_CENTER | DT_VCENTER, colors[1]);


	
	
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(void)
{

	// 頂点座標の設定
	g_vertexWkTitle[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	g_vertexWkTitle[1].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y, 0.0f);
	g_vertexWkTitle[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);
	g_vertexWkTitle[3].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_SIZE_X, TITLE_POS_Y + TITLE_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitle[0].rhw =
		g_vertexWkTitle[1].rhw =
		g_vertexWkTitle[2].rhw =
		g_vertexWkTitle[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitle[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTitle[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitle[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitle[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitle[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkTitleLogo[0].vtx = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y, 0.0f);
	g_vertexWkTitleLogo[1].vtx = D3DXVECTOR3(TITLELOGO_POS_X + TITLELOGO_SIZE_X, TITLELOGO_POS_Y, 0.0f);
	g_vertexWkTitleLogo[2].vtx = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y + TITLELOGO_SIZE_Y, 0.0f);
	g_vertexWkTitleLogo[3].vtx = D3DXVECTOR3(TITLELOGO_POS_X + TITLELOGO_SIZE_X, TITLELOGO_POS_Y + TITLELOGO_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleLogo[0].rhw =
		g_vertexWkTitleLogo[1].rhw =
		g_vertexWkTitleLogo[2].rhw =
		g_vertexWkTitleLogo[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTitleLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

