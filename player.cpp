/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名: プレーヤー処理 [player.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "player.h"
#include "input.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(void);
void SetVertexPlayer(int);
void SetTexturePlayer( int cntPattern );	//

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// テクスチャへのポリゴン
PLAYER					player[PLAYER_MAX];				// プレイヤー構造体
LPDIRECTSOUNDBUFFER8	g_pSE2;							// SE用バッファ

int bullet_cooldown = 0;
int moving_cooldown = 0;
int acceleration = 0;
bool jump_cooldown = FALSE;

ID3DXFont*				font2 = NULL;                // 文字のポインタ
const char* strings2[] = { "戦闘演習を始まりますか？" };
D3DCOLOR colors2[] = { D3DCOLOR_ARGB(155, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 255, 0), D3DCOLOR_ARGB(255, 0, 0, 255) };
RECT r2 = { SCREEN_WIDTH / 3 + 20, SCREEN_HEIGHT / 2 ,0,0 }; // starting point
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);	// プレイヤー０番のアドレスを取得する

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_PLAYER,				// ファイルの名前
			&g_pD3DTexturePlayer);				// 読み込むメモリのポインタ
		g_pSE2 = LoadSound(SE_00);
			
	}

	//player->pos = D3DXVECTOR3(SCREEN_CENTER_X + TEXTURE_PLAYER_SIZE_X / 2, SCREEN_CENTER_Y + TEXTURE_PLAYER_SIZE_Y / 2, 0.0f);
	player->pos = D3DXVECTOR3( TEXTURE_PLAYER_SIZE_X / 2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
	player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->CountAnim = 0;
	player->PatternAnim = 0;
	// 頂点情報の作成
	MakeVertexPlayer();
	//// テクスチャの読み込み
	//	D3DXCreateTextureFromFile( pDevice,		// デバイスのポインタ
	//	TEXTURE_GAME_PLAYER,				// ファイルの名前
	//	&g_pD3DTexturePlayer );			// 読み込むメモリのポインタ	



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if( g_pD3DTexturePlayer != NULL )	//
	{	// テクスチャの開放
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{			

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//player->bullet_num = 0;
	if (player->view_mode == 0) {	
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_PLAYER2,				// ファイルの名前
			&g_pD3DTexturePlayer);				// 読み込むメモリのポインタ

	}
	D3DXCreateFont(pDevice,     //D3D Device
		26,						//Font height
		0,						//Font width
		FW_NORMAL,				//Font Weight
		1,						//MipLevels
		false,					//Italic
		DEFAULT_CHARSET,		//CharSet
		OUT_DEFAULT_PRECIS,		//OutputPrecision
		ANTIALIASED_QUALITY,	//Quality
		DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
		"Georgia",				//pFacename,
		&font2);					//ppFont
	//万有引力
	if (!jump_cooldown && player->gravity > 0) {
		player->pos.y += player->gravity + acceleration;
		if (acceleration >= 0) {
			acceleration -= 1;
			
		}
		else if (acceleration < 0)
			acceleration = 0;
	}
	//Jump
	if (jump_cooldown) {
		if (acceleration > 0) {
			player->pos.y -= acceleration;
			acceleration -= 2;
		}
		else {
			jump_cooldown = FALSE;
			acceleration = 20 + acceleration/2;
		}
	}	
	// アニメーション
	//player->CountAnim = (player->CountAnim +1) % TEXTURE_PATTERN_DIVIDE_X;
	if (moving_cooldown > 0) {
		player->PatternAnim = (player->PatternAnim + 1) % ANIM_PATTERN_NUM;
		if (player->PatternAnim == 1 || player->PatternAnim == 6)
		moving_cooldown--;
	}
	// 画面外判定
	if (player->pos.x < 0) 
	{
		player->pos.x =  0;
	}
	if (player->pos.x > SCREEN_WIDTH - TEXTURE_PLAYER_SIZE_X)
	{		
		player->pos.x = SCREEN_WIDTH - TEXTURE_PLAYER_SIZE_X;
	}
	if (player->pos.y < 0)
	{
		player->pos.y = SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y;
	}
	if (player->pos.y > SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y)
	{
		player->pos.y = SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y;
		acceleration = 0;
		jump_cooldown = FALSE;
	}
	
	// 入力対応
	if (GetKeyboardPress(DIK_DOWN) && player->gravity == 0) {
		
		player->pos.y += 5;
	}
	if (GetKeyboardPress(DIK_UP)) {
		if (player->gravity != 0) {
			if (acceleration == 0 && !jump_cooldown) {
				// jump
				acceleration = 20;
				jump_cooldown = TRUE;
			}
		}
		else {
			player->pos.y -= 5;
		}

	}
	if (GetKeyboardPress(DIK_LEFT)) {
		moving_cooldown   =  1;
		player->direction = -1;
		player->pos.x	 -= 5;
	}
	if (GetKeyboardPress(DIK_RIGHT)) {	
		moving_cooldown   = 1;
		player->direction = 1;
		player->pos.x	 += 5;
	}
	if (GetKeyboardPress(DIK_SPACE) && (bullet_cooldown == 0)) {	
		//BULLET *bullet = GetBullet(player->bullet_num % BULLET_MAX);
		//while (!bullet->use) {
			//OutputDebugStringA("\nSPACE\n");			
		BULLET *bullet = GetBullet(player->bullet_num );		
		bullet->use = TRUE;
		
		//PlaySound(g_pSE2, E_DS8_FLAG_NONE);
		//bullet->direction = player->direction;
		player->bullet_num = (++player->bullet_num % (BULLET_MAX));
		bullet_cooldown += 5;
		//switch (player->bullet_num) {
			//case 0:
			//	OutputDebugStringA("0 ")
			//	break;
			//case 1:
			//	OutputDebugStringA("1 ");
			//	break;
			//case 2:
			//	OutputDebugStringA("2 ");
			//	break;
			//case 3:
			//	OutputDebugStringA("3 ");
			//	break;
			//case 4:
			//	OutputDebugStringA("4 ");
			//	break;
			//case 5:
			//	OutputDebugStringA("5 ");
			//	break;
			//case 6:
			//	OutputDebugStringA("6 ");
			//	break;
			//case 7:
			//	OutputDebugStringA("7 ");
			//	break;
			//case 8:
			//	OutputDebugStringA("8 ");
			//	break;
			//case 9:
			//	OutputDebugStringA("9 ");
			//	break;			
			//default:
			//	OutputDebugStringA("Else ");
			//	break;
			//}			
	}
  	MakeVertexPlayer();
	if(bullet_cooldown > 0)
	bullet_cooldown--;	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture( 0, g_pD3DTexturePlayer );
	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, player->vertexWk, sizeof(VERTEX_2D));	
	if (player->ready == 1) {// Tutorial Start
		// 文字の描画
		const char* strings2[] = { "戦闘演習を始まりますか？(Enter)" };
		D3DCOLOR colors2[] = { D3DCOLOR_ARGB(155, 255, 255, 255), D3DCOLOR_ARGB(255, 0, 255, 0), D3DCOLOR_ARGB(255, 0, 0, 255) };
		RECT r2 = {player->pos.x + TEXTURE_PLAYER_SIZE_X/2, player->pos.y - 20 ,0,0 }; // starting point
		for (int i = 0; i < _countof(strings2); ++i)
		{
			font2->DrawText(NULL, strings2[i], -1, &r2, DT_CALCRECT, 0);
			font2->DrawText(NULL, strings2[i], -1, &r2, DT_CENTER, colors2[i]);
			r2.left = r2.right; // offset for next character.
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexPlayer(player->direction);
	// rhwの設定
	player->vertexWk[0].rhw =
	player->vertexWk[1].rhw =
	player->vertexWk[2].rhw =
	player->vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	player->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	SetTexturePlayer(player->PatternAnim);
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer(int i)
{
	// 頂点座標の設定
	switch (i)
	{
	case -1:
		player->vertexWk[0].vtx = D3DXVECTOR3(player->pos.x, player->pos.y, player->pos.z);
		player->vertexWk[1].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X, player->pos.y, player->pos.z);
		player->vertexWk[2].vtx = D3DXVECTOR3(player->pos.x, player->pos.y + TEXTURE_PLAYER_SIZE_Y, player->pos.z);
		player->vertexWk[3].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X, player->pos.y + TEXTURE_PLAYER_SIZE_Y, player->pos.z);		
		break;
	case 1:		
		player->vertexWk[1].vtx = D3DXVECTOR3(player->pos.x, player->pos.y, player->pos.z);
		player->vertexWk[0].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X, player->pos.y, player->pos.z);
		player->vertexWk[3].vtx = D3DXVECTOR3(player->pos.x, player->pos.y + TEXTURE_PLAYER_SIZE_Y, player->pos.z);
		player->vertexWk[2].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X, player->pos.y + TEXTURE_PLAYER_SIZE_Y, player->pos.z);
		break;
	}	
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePlayer( int cntPattern )
{
	// テクスチャ座標の設定
	player->vertexWk[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	player->vertexWk[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X) + 1)/ (float)TEXTURE_PATTERN_DIVIDE_X, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	player->vertexWk[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	player->vertexWk[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X) + 1)/ (float)TEXTURE_PATTERN_DIVIDE_Y);
}
/*******************************************************************************
関数名:	PLAYER *GetMapAdr( int pno )
引数:	int pno : プレイヤー番号
戻り値:	PLAYER *
説明:	プレイヤーのアドレスを取得する
*******************************************************************************/
PLAYER *GetPlayer(int pno)
{
	return &player[pno];
}


void ChangePlayer(void)
{

#undef TEXTURE_GAME_PLAYER	_T	
#undef TEXTURE_PLAYER_SIZE_X	
#undef TEXTURE_PLAYER_SIZE_Y	
#undef TEXTURE_PATTERN_DIVIDE_X	
#undef TEXTURE_PATTERN_DIVIDE_Y	

#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/runningman000.png")	// サンプル用画像
#define TEXTURE_PLAYER_SIZE_X	(100/2) // テクスチャサイズ
#define TEXTURE_PLAYER_SIZE_Y	(200/2) // 同上 
#define TEXTURE_PATTERN_DIVIDE_X	(8)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
}
