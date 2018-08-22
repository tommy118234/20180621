/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名: プレーヤー処理 [player.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "servant.h"
#include "sound.h"

#define M_PI 3.14159265358979323846

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(int no);
void	SetVertexPlayer(int no, int dir);
void	SetTexturePlayer(int no, int cntPattern );	//

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// テクスチャへのポリゴン
PLAYER					playerWk[PLAYER_MAX];			// プレイヤー構造体
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
	PLAYER *player = &playerWk[0];					// プレイヤー０番のアドレスを取得する
	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスのポインタ
			TEXTURE_GAME_PLAYER,					// ファイルの名前
			&g_pD3DTexturePlayer);		//g_pSE2 = LoadSound(SE_00);		
	}
		
	else if (type == 1) {
		UninitPlayer;
	}
	// プレイヤーの初期化処理
	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		player->use = true;							// 使用
		player->pos = D3DXVECTOR3(TEXTURE_PLAYER_SIZE_X / 2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
		player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player->CountAnim = 0;
		player->PatternAnim = 0;
		player->direction = -1;
		player->gravity = 5;
		player->view_mode = 1;
		player->ready = 0;

		player->status.HP = 500;								// HPを初期化
		player->status.MP = 100;								// MPを初期化
		player->status.ATK = 100;								// ATKを初期化
		player->status.DEF = 100;								// DEFを初期化
		player->status.LUCK = 100;								// LUCKを初期化
		strcpy(player->status.name, "ヴァルキリー ?");			// NAMEを初期化

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_Y);
		player->Radius = D3DXVec2Length(&temp);				// プレイヤーの半径を初期化
		player->BaseAngle = atan2f(TEXTURE_PLAYER_SIZE_Y, TEXTURE_PLAYER_SIZE_X);	// プレイヤーの角度を初期化

		player->Texture = g_pD3DTexturePlayer;				// テクスチャ情報
		MakeVertexPlayer(i);						// 頂点情報の作成

	}
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
	PLAYER *player = &playerWk[0];	
	if (player->view_mode == 0) {
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_PLAYER2,				// ファイルの名前
			&g_pD3DTexturePlayer);				// 読み込むメモリのポインタ
		Side_to_TopView();
		player->gravity = 0;
		player->Texture = g_pD3DTexturePlayer;				// テクスチャ情報
		player->pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - TEXTURE_PLAYER_SIZE_X/2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
		player->view_mode = 2;
	}
	else if (player->view_mode == 1) {
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_PLAYER,				// ファイルの名前
			&g_pD3DTexturePlayer);				// 読み込むメモリのポインタ
		Top_to_SideView();
		player->Texture = g_pD3DTexturePlayer;	// テクスチャ情報
		player->view_mode = 2;
	}
	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
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
				acceleration = 20 + acceleration / 2;
			}
		}
		if (player->use == true)			// 使用している状態なら更新する
		{
			// アニメーション
			player->CountAnim++;
			if (moving_cooldown > 0) {
				player->PatternAnim = (player->PatternAnim + 1) % ANIM_PATTERN_NUM;
				// テクスチャ座標を設定
				SetTexturePlayer(i, player->PatternAnim);
				if (player->PatternAnim == 1 || player->PatternAnim == 6)
					moving_cooldown--;
			}
			// 画面外判定
			if (player->pos.x < 0)
			{
				player->pos.x = 0;
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

			// キーボード入力で移動
			if (GetKeyboardPress(DIK_DOWN) && player->gravity == 0) {
				if (player->gravity == 0)
					moving_cooldown = 1;
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
					moving_cooldown = 1;
					player->pos.x -= 5 * (-sinf(player->BaseAngle + player->rot.z) + cosf(player->BaseAngle + player->rot.z));
					player->pos.y -= 5 * (cosf(player->BaseAngle + player->rot.z) + sinf(player->BaseAngle + player->rot.z));							
				}
			}
			if (GetKeyboardPress(DIK_LEFT)) {
				if (player->gravity != 0) {
					moving_cooldown = 1;
					player->direction = -1;

					player->pos.x -= 5;
				}
				else {
					player->rot.z -= 0.1f;
				}
			}
			if (GetKeyboardPress(DIK_RIGHT)) {
				if (player->gravity != 0) {
					moving_cooldown = 1;
					player->direction = 1;

					player->pos.x += 5;
				}
				else {
					player->rot.z += 0.1f;
				}
			}
			if (player->status.MP > 20) {
				// 召喚
				if (GetKeyboardPress(DIK_Q) && (bullet_cooldown == 0)) {
					bullet_cooldown += 5;
					player->status.MP -= 20;
					D3DXVECTOR3 pos = player->pos;
					pos.y -= TEXTURE_PLAYER_SIZE_Y;
					pos.x += TEXTURE_PLAYER_SIZE_X + player->pos.x / 4.0f;
					SetSERVANT(pos, 1);

				}
				// 召喚
				if (GetKeyboardPress(DIK_W) && (bullet_cooldown == 0)) {
					bullet_cooldown += 5;
					player->status.MP -= 20;
					D3DXVECTOR3 pos = player->pos;
					pos.y -= TEXTURE_PLAYER_SIZE_Y;
					pos.x += TEXTURE_PLAYER_SIZE_X + player->pos.x / 4.0f;
					SetSERVANT(pos, 2);
				}
				// 召喚
				if (GetKeyboardPress(DIK_E) && (bullet_cooldown == 0)) {
					bullet_cooldown += 5;
					player->status.MP -= 20;
					D3DXVECTOR3 pos = player->pos;
					pos.y -= TEXTURE_PLAYER_SIZE_Y;
					pos.x += TEXTURE_PLAYER_SIZE_X + player->pos.x / 4.0f;
					SetSERVANT(pos, 3);
				}
				// スキル
				if (GetKeyboardPress(DIK_A)) {
					player->status.ATK += 50;
					player->status.MP -= 20;
				}
				// スキル
				if (GetKeyboardPress(DIK_S)) {
					player->status.DEF += 50;
					player->status.MP -= 20;
				}
				// スキル
				if (GetKeyboardPress(DIK_D)) {
					player->status.ATK += 100;
					player->status.DEF += 100;
					player->status.HP -= 20;
				}
			}
			// 攻撃モードスウィッチ
			if (GetKeyboardPress(DIK_F))
			{
				//player->rot.z += 0.1f;
				player->direction = 1;
			}
			if (GetKeyboardPress(DIK_G))
			{
				//player->rot.z -= 0.1f;
				player->direction = -1;
			}

			// ゲームパッドでで移動処理
			if (IsButtonPressed(0, BUTTON_DOWN))
			{
				player->pos.y += 2.0f;
			}
			else if (IsButtonPressed(0, BUTTON_UP))
			{
				player->pos.y -= 2.0f;
			}

			if (IsButtonPressed(0, BUTTON_RIGHT))
			{
				player->pos.x += 2.0f;
			}
			else if (IsButtonPressed(0, BUTTON_LEFT))
			{
				player->pos.x -= 2.0f;
			}
			//弾発射
			if (GetKeyboardPress(DIK_SPACE) && (bullet_cooldown == 0) && player->direction == 1)
			{
				bullet_cooldown += 5;
				D3DXVECTOR3 pos = player->pos;
				SetBullet(pos,player->rot.z);
			}
			else if (IsButtonTriggered(0, BUTTON_B))
			{
				bullet_cooldown += 5;
				D3DXVECTOR3 pos = player->pos;
				SetBullet(pos, player->rot.z);
			}
					   

			// 移動後の座標で頂点を設定
			SetVertexPlayer(i, player->direction);
			if (bullet_cooldown > 0)
			bullet_cooldown --;		
			
			if (player->rot.z > 2* M_PI)
				player->rot.z = player->rot.z - 2 * M_PI;
			
			if (player->rot.z < - 2 *M_PI)
				player->rot.z = -player->rot.z - 2 * M_PI;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use == true)			// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, player->Texture);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, player->vertexWk, sizeof(VERTEX_2D));
			if (player->ready == 1) {// Tutorial Start
				if (font2 == NULL)
					// 文字の描画
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
						&font2);				//ppFont
				const char* strings2[] = { "戦闘演習を始まりますか？(Enter)" };
				D3DCOLOR colors2[] = { D3DCOLOR_ARGB(255, 255, 255, 255) };
				RECT r2 = { 0  , 0 ,SCREEN_WIDTH,SCREEN_HEIGHT/2 }; // starting point
				font2->DrawText(NULL, strings2[0], -1, &r2, DT_CENTER|DT_VCENTER, colors2[0]);
				//for (int i = 0; i < _countof(strings2); ++i)
				//{
				//	font2->DrawText(NULL, strings2[i], -1, &r2, DT_CALCRECT, 0);
				//	font2->DrawText(NULL, strings2[i], -1, &r2, DT_CENTER, colors2[i]);
				//	r2.left = r2.right; // offset for next character.
				//}
			}
		}
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk[no];

	// 頂点座標の設定
	SetVertexPlayer(no, player->direction);
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
	player->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	player->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	player->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	player->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	// テクスチャ座標の設定
	SetTexturePlayer(no,player->PatternAnim);
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayer( int no, int dir)
{
	
	PLAYER *player = &playerWk[no];

	// 頂点座標の設定
	switch (dir)
	{
	case -1:
		player->vertexWk[0].vtx = D3DXVECTOR3(player->pos.x - cosf(player->BaseAngle + player->rot.z) * player->Radius,
											  player->pos.y - sinf(player->BaseAngle + player->rot.z) * player->Radius,
											  0);

		player->vertexWk[1].vtx = D3DXVECTOR3(player->pos.x + cosf(player->BaseAngle - player->rot.z) * player->Radius,
											  player->pos.y - sinf(player->BaseAngle - player->rot.z) * player->Radius,
											  0);

		player->vertexWk[2].vtx = D3DXVECTOR3(player->pos.x - cosf(player->BaseAngle - player->rot.z) * player->Radius,
											  player->pos.y + sinf(player->BaseAngle - player->rot.z) * player->Radius,
											 0);

		player->vertexWk[3].vtx = D3DXVECTOR3(player->pos.x + cosf(player->BaseAngle + player->rot.z) * player->Radius,
											  player->pos.y + sinf(player->BaseAngle + player->rot.z) * player->Radius,
											 0);		
		break;
	case 1:		
		player->vertexWk[1].vtx = D3DXVECTOR3(player->pos.x - cosf(player->BaseAngle + player->rot.z) * player->Radius,
											  player->pos.y - sinf(player->BaseAngle + player->rot.z) * player->Radius,
											  0);

		player->vertexWk[0].vtx = D3DXVECTOR3(player->pos.x + cosf(player->BaseAngle - player->rot.z) * player->Radius,
											  player->pos.y - sinf(player->BaseAngle - player->rot.z) * player->Radius,
									          0);

		player->vertexWk[3].vtx = D3DXVECTOR3(player->pos.x - cosf(player->BaseAngle - player->rot.z) * player->Radius,
											  player->pos.y + sinf(player->BaseAngle - player->rot.z) * player->Radius,
											  0);

		player->vertexWk[2].vtx = D3DXVECTOR3(player->pos.x + cosf(player->BaseAngle + player->rot.z) * player->Radius,
											  player->pos.y + sinf(player->BaseAngle + player->rot.z) * player->Radius,
											  0);
		break;	
	}	
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePlayer( int no, int cntPattern )
{
	PLAYER *player = &playerWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;

	player->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	player->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	player->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	player->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
/*******************************************************************************
関数名:	PLAYER *GetMapAdr( int pno )
引数:	int pno : プレイヤー番号
戻り値:	PLAYER *
説明:	プレイヤーのアドレスを取得する
*******************************************************************************/
PLAYER *GetPlayer(int pno)
{
	return &playerWk[pno];
}


void Side_to_TopView(void)
{

//#undef TEXTURE_GAME_PLAYER	_T	
#undef TEXTURE_PLAYER_SIZE_X	
#undef TEXTURE_PLAYER_SIZE_Y	
#undef TEXTURE_PATTERN_DIVIDE_X	
#undef TEXTURE_PATTERN_DIVIDE_Y	

//#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/runningman000.png")	// サンプル用画像
#define TEXTURE_PLAYER_SIZE_X	(545/5/2) // テクスチャサイズ
#define TEXTURE_PLAYER_SIZE_Y	(242/2/2) // 同上 
//#define TEXTURE_PATTERN_DIVIDE_X	(8)	// アニメパターンのテクスチャ内分割数（X)
//#define TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
}

void Top_to_SideView(void)
{

//#undef TEXTURE_GAME_PLAYER	_T	
#undef TEXTURE_PLAYER_SIZE_X	
#undef TEXTURE_PLAYER_SIZE_Y	
#undef TEXTURE_PATTERN_DIVIDE_X	
#undef TEXTURE_PATTERN_DIVIDE_Y	

//#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/runningman001.png")	// サンプル用画像
#define TEXTURE_PLAYER_SIZE_X	(140/5) // テクスチャサイズ
#define TEXTURE_PLAYER_SIZE_Y	(200/5) // 同上 
#define TEXTURE_PATTERN_DIVIDE_X	(5)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
}
