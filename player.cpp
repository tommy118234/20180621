/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名: プレーヤー処理 [player.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "player.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(void);
void SetVertexPlayer(void);
void SetTexturePlayer( int cntPattern );	//

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// テクスチャへのポリゴン
  

int bullet_cooldown = 0;
int moving_cooldown = 0;
int gravity = 5;
int acceleration = 0;
bool jump_cooldown = FALSE;


PLAYER					player[PLAYER_MAX];				// プレイヤー構造体
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
	}

	//player->pos = D3DXVECTOR3(SCREEN_CENTER_X + TEXTURE_PLAYER_SIZE_X / 2, SCREEN_CENTER_Y + TEXTURE_PLAYER_SIZE_Y / 2, 0.0f);
	player->pos = D3DXVECTOR3( TEXTURE_PLAYER_SIZE_X / 2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
	player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->CountAnim = 0;
	player->PatternAnim = 0;
	// 頂点情報の作成
	MakeVertexPlayer();
	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice,		// デバイスのポインタ
		TEXTURE_GAME_PLAYER,				// ファイルの名前
		&g_pD3DTexturePlayer );			// 読み込むメモリのポインタ	
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
	//player->bullet_num = 0;
	


	//万有引力
	if (!jump_cooldown) {
		player->pos.y += gravity + acceleration;
		if (acceleration <= 0) {
			acceleration -= 1;
			if (acceleration < 0)
				acceleration = 0;
		}
	}

	//Jump
	if (jump_cooldown) {
		if (acceleration > 0) {
			player->pos.y -= acceleration;
			acceleration -= 2;
		}
		else {
			jump_cooldown = FALSE;
			acceleration = 20 - acceleration/2;
		}
	}


	// アニメーション
	//player->CountAnim = (player->CountAnim +1) % TEXTURE_PATTERN_DIVIDE_X;
	if (moving_cooldown > 0) {
		player->PatternAnim = (player->PatternAnim + 1) % ANIM_PATTERN_NUM;
		moving_cooldown--;
	}
	if (player->pos.x < 0) 
	{
		player->pos.x = SCREEN_WIDTH - TEXTURE_PLAYER_SIZE_X / 2;
	}
	if (player->pos.x > SCREEN_WIDTH - TEXTURE_PLAYER_SIZE_X / 2)
	{
		player->pos.x = 0;
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
	if (GetKeyboardPress(DIK_DOWN)) {
		
		player->pos.y += 5;
	}
	if (GetKeyboardPress(DIK_UP) && acceleration == 0) {
		// jump
		acceleration = 20;
		jump_cooldown = TRUE;		
	}
	if (GetKeyboardPress(DIK_LEFT)) {
		moving_cooldown ++;
		player->pos.x -= 5;
	}
	if (GetKeyboardPress(DIK_RIGHT)) {	
		moving_cooldown ++;
		player->pos.x += 5;
	}
	if (GetKeyboardPress(DIK_SPACE) && (bullet_cooldown == 0)) {	
		//BULLET *bullet = GetBullet(player->bullet_num % BULLET_MAX);
		//while (!bullet->use) {
			//OutputDebugStringA("\nSPACE\n");			
		BULLET *bullet = GetBullet(player->bullet_num );		
		bullet->use = TRUE;
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
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexPlayer();
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
void SetVertexPlayer(void)
{
	// 頂点座標の設定
	player->vertexWk[0].vtx = D3DXVECTOR3(player->pos.x, player->pos.y, player->pos.z);
	player->vertexWk[1].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X, player->pos.y, player->pos.z);
	player->vertexWk[2].vtx = D3DXVECTOR3(player->pos.x, player->pos.y + TEXTURE_PLAYER_SIZE_Y, player->pos.z);
	player->vertexWk[3].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X, player->pos.y + TEXTURE_PLAYER_SIZE_Y, player->pos.z);
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