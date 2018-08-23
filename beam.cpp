/*******************************************************************************
* タイトル:		DirectXゲーム～はじめての個人作品～
* プログラム名:	Beam処理 [beam.cpp]
* 作成者:		GP11B 16　徐　ワイ延
* 作成開始日:	2018/07/24
********************************************************************************/
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"
#include "beam.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBeam(int no);
void SetTextureBeam(int no, int cntPattern);
void SetVertexBeam(int no);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBeam = NULL;		// テクスチャへのポリゴン
BEAM					beamWk[BEAM_MAX];				// Beam構造体
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBeam(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BEAM *beam = &beamWk[0];							// Beamのポインターを初期化
	PLAYER *player = GetPlayer(0);
	for (int i = 0; i < BEAM_MAX; i++, beam++) {		
		// テクスチャーの初期化を行う？
		if (type == 0)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,			// デバイスのポインタ
			TEXTURE_GAME_BEAM,						// ファイルの名前
			&g_pD3DTextureBeam);					// 読み込むメモリのポインタ			
		}
		else if (type == 1) {
			UninitBeam;
		}
		beam->use = FALSE;								// 未使用（発射されていない弾）
		beam->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);	// 座標データを初期化
		beam->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転データを初期化
		beam->PatternAnim = 0;							// アニメパターン番号をランダムで初期化
		beam->CountAnim = 0;							// アニメカウントを初期化
		beam->BaseAngle = atan2f(TEXTURE_BEAM_SIZE_Y, TEXTURE_BEAM_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_BEAM_SIZE_X / 2, TEXTURE_BEAM_SIZE_Y / 2);
		beam->Radius = D3DXVec2Length(&temp);
		beam->Texture = g_pD3DTextureBeam;				// テクスチャ情報
		MakeVertexBeam(i);								// 頂点情報の作成
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBeam(void)
{
	if (g_pD3DTextureBeam != NULL)	
	{	// テクスチャの開放
		g_pD3DTextureBeam->Release();
		g_pD3DTextureBeam = NULL;
	}	
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBeam(void)
{
	BEAM *beam = beamWk;				// Beamのポインターを初期化
	for (int i = 0; i < BEAM_MAX; i++, beam++)
	{
		if (beam->use == true)			// 未使用状態のBeamを見つける
		{
			// Beamの移動処理
			beam->pos.x -= BEAM_SPEED * (sinf(beam->BaseAngle + beam->rot.z) - cosf(beam->BaseAngle + beam->rot.z));
			beam->pos.y += BEAM_SPEED * (cosf(beam->BaseAngle + beam->rot.z) + sinf(beam->BaseAngle + beam->rot.z));
			// 画面外まで進んだ？
			if (beam->pos.y < -TEXTURE_BEAM_SIZE_Y || beam->pos.y > SCREEN_HEIGHT
			 || beam->pos.x < -TEXTURE_BEAM_SIZE_X || beam->pos.x > SCREEN_WIDTH)
			{
				beam->use = false;
			}
			SetVertexBeam(i);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBeam(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BEAM *beam = beamWk;				// Beamのポインターを初期化
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < BEAM_MAX; i++, beam++)
	{
		if (beam->use)					// 使用している状態なら更新する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, beam->Texture);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BEAM, beam->vertexWk, sizeof(VERTEX_2D));
		}
	}
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBeam(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BEAM *beam = &beamWk[no];
	// 頂点座標の設定
	SetVertexBeam(no);
	// rhwの設定
	beam->vertexWk[0].rhw =
	beam->vertexWk[1].rhw =
	beam->vertexWk[2].rhw =
	beam->vertexWk[3].rhw = 1.0f;
	// 反射光の設定
	beam->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	beam->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	beam->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	beam->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// テクスチャ座標の設定
	beam->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	beam->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BEAM, 0.0f);
	beam->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BEAM);
	beam->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BEAM, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BEAM);
	return S_OK;
}
//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBeam(int no)
{
	BEAM *beam = &beamWk[no];			// Beamのポインターを初期化

	// 頂点座標の設定	
	beam->vertexWk[3].vtx = D3DXVECTOR3(beam->pos.x - cosf(beam->BaseAngle + beam->rot.z) * beam->Radius - GetPlayer(0)->pos.x / 4.0f,
										beam->pos.y - sinf(beam->BaseAngle + beam->rot.z) * beam->Radius,
										0);
	beam->vertexWk[2].vtx = D3DXVECTOR3(beam->pos.x + cosf(beam->BaseAngle - beam->rot.z) * beam->Radius - GetPlayer(0)->pos.x / 4.0f,
										beam->pos.y - sinf(beam->BaseAngle - beam->rot.z) *beam->Radius,
										0);
	beam->vertexWk[1].vtx = D3DXVECTOR3(beam->pos.x - cosf(beam->BaseAngle - beam->rot.z) * beam->Radius - GetPlayer(0)->pos.x / 4.0f,
										beam->pos.y + sinf(beam->BaseAngle - beam->rot.z) * beam->Radius,
										0);
	beam->vertexWk[0].vtx = D3DXVECTOR3(beam->pos.x + cosf(beam->BaseAngle + beam->rot.z) * beam->Radius - GetPlayer(0)->pos.x / 4.0f,
										beam->pos.y + sinf(beam->BaseAngle + beam->rot.z) * beam->Radius,
										0);
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBeam(int no, int cntPattern)
{
	BEAM *beam = &beamWk[no];			// Beamのポインターを初期化

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_BEAM;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_BEAM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BEAM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BEAM;
	// 頂点座標の設定
	beam->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	beam->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	beam->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	beam->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
//=============================================================================
// Beamの発射設定
//=============================================================================
void SetBeam(D3DXVECTOR3 pos, float rot)
{
	BEAM *beam = &beamWk[0];			// Beamのポインターを初期化
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < BEAM_MAX; i++, beam++)
	{
		if (beam->use == false)			// 未使用状態のBeamを見つける
		{
			beam->use = true;				// 使用状態へ変更する
			beam->pos = pos;				// 座標をセット
			beam->rot.z = rot;			
			return;							// 1発セットしたので終了する
		}
	}
}
/*******************************************************************************
関数名:	beam *GetBeamAdr( int pno )
引数:	int pno : beam番号
戻り値:	beam *
説明:	beamのアドレスを取得する
*******************************************************************************/
BEAM *GetBeam(int pno)
{
	return &beamWk[pno];
}