/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	Main処理 [main.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "bg.h"
#include "title.h"
#include "result.h"
#include "score.h"
#include "sound.h"
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")			// ウインドウのクラス名
#define WINDOW_NAME			_T("VOID FUNC")		// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HWND hWnd, BOOL bWindow);
void	Uninit(void);
void	Update(void);
void	Draw(void);
void CheckHit(void);
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);

void InitGame(void);

#ifdef _DEBUG
void DrawFPS(void);
#endif

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;				// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)

#ifdef _DEBUG
LPD3DXFONT				g_pD3DXFont = NULL;			// フォントへのポインタ
int						g_nCountFPS;				// FPSカウンタ
#endif

int						g_nStage = 0;				// ステージ番号
LPDIRECTSOUNDBUFFER8	g_pBGM;						// BGM用バッファ
int                     page = 1;					// ステージサブ番号
//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）
	
	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// ウィンドウの左座標
						CW_USEDEFAULT,																		// ウィンドウの上座標
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// ウィンドウ横幅
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
						NULL,
						NULL,
						hInstance,
						NULL);

	// DirectXの初期化(ウィンドウを作成してから行う)
	if(FAILED(Init(hWnd, true)))
	{
		return -1;
	}

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定  1 = 1000分の1秒
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //Message あるか？
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount;					//表示するために保存する
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS_RATE))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

				Update();			// 更新処理
				Draw();				// 描画処理

				dwFrameCount++;		// 処理回数のカウントを加算
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;


	srand((unsigned)time(NULL));

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定

	if(bWindow)
	{// ウィンドウモード
		d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.BackBufferFormat			 = D3DFMT_R5G6B5;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// ディスプレイアダプタ
									D3DDEVTYPE_HAL,								// ディスプレイタイプ
									hWnd,										// フォーカスするウインドウへのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,		// デバイス作成制御の組み合わせ
									&d3dpp,										// デバイスのプレゼンテーションパラメータ
									&g_pD3DDevice)))							// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp,
										&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp,
											&g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定


#ifdef _DEBUG
																			// 情報表示用フォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pD3DXFont);
#endif

	// サウンド初期化
	InitSound(hWnd);
	// ロードと再生
	g_pBGM = LoadSound(BGM_00);
	PlaySound(g_pBGM, E_DS8_FLAG_LOOP);
	// プレイヤーの初期化処理
	InitPlayer(0);
	// エネミーの初期化処理
	InitEnemy(0);
	// バレットの初期化処理
	InitBullet(0);
	// BG初期化
	InitBG(0);
	// タイトル初期化
	InitTitle();
	// リザルト初期化
	InitResult();
	// スコア初期化
	InitScore(0);	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// 入力処理の終了処理
	UninitInput();
	// プレイヤーの終了処理
	UninitPlayer();
	// エネミーの終了処理
	UninitEnemy();
	// バレットの終了処理
	UninitBullet();
	// BG終了処理
	UninitBG();
	// タイトル終了処理
	UninitTitle();
	// リザルト終了処理
	UninitResult();
	// スコア終了処理
	UninitScore();
	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
	}	
	if (g_pD3D != NULL)
	{	// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		// サウンド処理の終了処理
		SAFE_RELEASE(g_pBGM);
	}
	UninitSound();
	
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力の更新処理
	UpdateInput();
	// 画面遷移
	switch (g_nStage)
	{
	case STAGE_TITLE:
		UpdateTitle();
		break;
	case STAGE_TUTOR:		
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			switch (page){
			case 1:
				SwitchBG(2);
				break;
			case 2:
				GetPlayer(0)->ready = 1;
				break;
			case 3:
				GetPlayer(0)->ready = 2;
				SetStage(STAGE_GAME);
			}
			page++;
		}
		// ゲームパッドでで移動処理
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			SetStage(STAGE_GAME);
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			SetStage(STAGE_GAME);
		}
		// プレイヤーの更新処理
		UpdatePlayer();		
		// BGの更新処理
		UpdateBG();
		break;
	case STAGE_GAME:
		// プレイヤーの更新処理
		UpdatePlayer();
		// エネミーの更新処理
		UpdateEnemy();
		// バレットの更新処理
		UpdateBullet();
		// BGの更新処理
		UpdateBG();
		// スコアの更新処理
		UpdateScore();
		// 当たり判定
		CheckHit();
		break;
	case STAGE_GAME_END:
		// スコアの更新処理
		UpdateScore();

		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			InitGame();				// ゲームの再初期化処理
			SetStage(STAGE_RESULT);
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			InitGame();				// ゲームの再初期化処理
			SetStage(STAGE_RESULT);
		}
		break;
	case STAGE_RESULT:
		UpdateResult();
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(128, 128, 255, 0), 1.0f, 0);	
	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// 画面遷移
		switch (g_nStage)
		{
		case STAGE_TITLE:
			DrawTitle();
			break;
		case STAGE_TUTOR:				
			// BGの描画処理
			DrawBG();
			// プレイヤーの描画処理
			DrawPlayer();
			break;
		case STAGE_GAME:
		case STAGE_GAME_END:
			// BGの描画処理
			DrawBG();
			// エネミーの描画処理
			DrawEnemy();
			// プレイヤーの描画処理
			DrawPlayer();
			// バレットの描画処理
			DrawBullet();
			// スコアの描画処理
			DrawScore();
			break;
		case STAGE_RESULT:
			DrawResult();
			break;
		}
#ifdef _DEBUG
		// FPS表示
		DrawFPS();
#endif
		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイス取得関数
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}

#ifdef _DEBUG
//=============================================================================
// FPS表示処理
//=============================================================================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];
	wsprintf(str, _T("FPS:%d\nENEMY HP:%d\n"), g_nCountFPS, GetEnemy(0)->status.HP);
	//wsprintf(str, _T("ENEMY HP:%d\n"));
	// テキスト描画
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif
//=============================================================================
// 当たり判定関数
//=============================================================================
//void HitTest(void)
//{
//	PLAYER *player = GetPlayer(0);
//	ENEMY  *enemy = GetEnemy(0);
//	//SCORE *score = GetScore(0);
//	for (int i = 0; i < player->bullet_num + 1; i++) {
//		BULLET *bullet = GetBullet(i);		
//		if (bullet->use) {
//			//OutputDebugStringA("B ");
//			/*
//			if (((bullet->pos.x + TEXTURE_SAMPLE00_SIZE_X3) > enemy->pos.x) &&
//				((bullet->pos.x) < enemy->pos.x + TEXTURE_SAMPLE00_SIZE_X2))  //横だけ当たり
//			{
//				if (((bullet->pos.y + TEXTURE_SAMPLE00_SIZE_Y3) > enemy->pos.y) &&
//					((bullet->pos.y) < enemy->pos.y + TEXTURE_SAMPLE00_SIZE_Y2))  //横と縱当たり
//				{				
//					//スコア UP
// 					player->score = player->score + 1;
//					bullet->use = FALSE;
//					//enemy->use = FALSE;
//				}
//			}*/
//			D3DXVECTOR2 temp = D3DXVECTOR2(bullet->pos.x - enemy->pos.x, bullet->pos.y - enemy->pos.y);
//			if ((enemy->Radius + bullet->Radius ) > D3DXVec2Length(&temp))
//			{				
//				//スコア UP
//				player->score = player->score + 1;
//				bullet->use = FALSE;
//				bullet->pos = D3DXVECTOR3(player->pos.x, player->pos.y, 0.0f);
//				//enemy->use = FALSE;
//			}
//		}
//	}
//}

//=============================================================================
// 画面遷移
//=============================================================================
void SetStage(int stage)
{
	//if( state < 0 || state >= STATE_MAX ) return;		
	g_nStage = stage;	
	StopSound(g_pBGM);
	switch (g_nStage)
	{
	case STAGE_TITLE:
		g_pBGM = LoadSound(BGM_00);
		break;
	case STAGE_TUTOR:
		SwitchBG(1);
		g_pBGM = LoadSound(BGM_01);
		break;
	case STAGE_GAME:
		g_pBGM = LoadSound(BGM_02);
		SwitchBG(4);
		GetPlayer(0)->gravity = 0;
		GetPlayer(0)->view_mode = 0;
		break;
	case STAGE_GAME_END:
		g_pBGM = LoadSound(BGM_03);
		SwitchBG(5);
		break;
	case STAGE_RESULT:
		g_pBGM = LoadSound(BGM_04);
		SwitchBG(6);
		break;
	}
	PlaySound(g_pBGM, E_DS8_FLAG_NONE);
}
//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	ENEMY *enemy = GetEnemy(0);				// エネミーのポインターを初期化
	BULLET *bullet = GetBullet(0);			// バレットのポインターを初期化
	int cnt = 0;							// 敵の数を数える
	// 敵と操作キャラ(BB)
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == false)	continue;
		if (CheckHitBB(GetPlayer(0)->pos, enemy->pos, D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_Y), D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y)))
		{
			// 操作キャラクターは死に

			// 敵キャラクターは倒される
			enemy->use = false;
			// HP減少処理
		}
	}
	// 弾と敵(BC)
	enemy = GetEnemy(0);					// エネミーのポインターを初期化
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use == false) continue;
		for (int j = 0; j < ENEMY_MAX; j++, enemy++)
		{
			if (enemy->use == false) continue;
			//if (CheckHitBC(bullet->pos, enemy->pos, TEXTURE_BULLET_SIZE_X, TEXTURE_ENEMY_SIZE_X))
			if (CheckHitBB(bullet->pos, enemy->pos, D3DXVECTOR2(TEXTURE_BULLET_SIZE_X, TEXTURE_BULLET_SIZE_Y), D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y)))
			{
				bullet->use = false;		// 弾の消滅処理を行い
				enemy->status.HP--;			// HP = 10
				if (enemy->status.HP < 1){
					enemy->use = false;		// 敵は倒される					
					AddScore(100);			// スコア計算					
				}
			}
		}
	}
	// ボスと弾(BC)
	// 自分と敵の弾(BC)
	// 敵が全滅したら画面遷移
	enemy = GetEnemy(0);					// エネミーのポインターを初期化
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == true)
		{
			cnt++;
		}
	}
	// 数えた結果敵が全滅？
	if (cnt == 0)
	{
		SetStage(STAGE_GAME_END);
	}
}


//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない 
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{
	size1 /= 2.0f;	// 半サイズにする
	size2 /= 2.0f;	// 同上

	if (pos1.x + size1.x > pos2.x - size2.x && pos2.x + size2.x > pos1.x - size1.x &&
		pos1.y + size1.y > pos2.y - size2.y && pos2.y + size2.y > pos1.y - size1.y)
	{
		return true;
	}

	return false;
}

//=============================================================================
// BCによる当たり判定処理
// posは円の中心、radiusは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
{
	if ((radius1 + radius2) * (radius1 + radius2) >
		(pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y))
	{
		return true;
	}
	return false;
}

//=============================================================================
// ゲームの再初期化処理処理
// 戻り値：無し
//=============================================================================
void InitGame(void)
{
	InitPlayer(1);		// プレイヤーの再初期化
	InitEnemy(1);		// エネミーの再初期化
	InitBullet(1);		// バレットの再初期化
	InitScore(1);		// スコアの再初期化
	InitBG(1);			// BGの再初期化
}