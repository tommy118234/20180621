//=============================================================================
//
// �|���S������ [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPlayer(void);
void SetVertexPlayer(void);
void SetTexturePlayer( int cntPattern );	//

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
  LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// �e�N�X�`���ւ̃|���S��
  
  VERTEX_2D					g_vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
  
//D3DXVECTOR3				g_posPlayer;						// �|���S���̈ړ���
//D3DXVECTOR3				g_rotPlayer;						// �|���S���̉�]��
//int						g_nCountAnim;						// �A�j���[�V�����J�E���g
//int						g_nPatternAnim;						// �A�j���[�V�����p�^�[���i���o�[
//
//float						g_fRadiusPlayer;					// �|���S���̔��a
//float						g_fBaseAnglePlayer;					// �|���S���̊p�x//
//int						direction = -1;

PLAYER	player[PLAYER_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);	// �v���C���[�O�Ԃ̃A�h���X���擾����	
	//player->g_posPlayer = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X/2, SCREEN_CENTER_Y - TEXTURE_SAMPLE00_SIZE_Y/2, 0.0f);
	player->g_posPlayer = D3DXVECTOR3(SCREEN_CENTER_X - TEXTURE_SAMPLE00_SIZE_X / 2, SCREEN_CENTER_Y - TEXTURE_SAMPLE00_SIZE_Y / 2, 0.0f);
	player->g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->g_nCountAnim = 0;
	player->g_nPatternAnim = 0;
	// ���_���̍쐬
	MakeVertexPlayer();
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice,		// �f�o�C�X�̃|�C���^
		TEXTURE_GAME_SAMPLE00,				// �t�@�C���̖��O
		&g_pD3DTexturePlayer );			// �ǂݍ��ރ������̃|�C���^	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if( g_pD3DTexturePlayer != NULL )	//
	{	// �e�N�X�`���̊J��
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{			
	//player->bullet_num = 0;
	
	// �A�j���[�V����
	player->g_nCountAnim = (player->g_nCountAnim +1) % TEXTURE_PATTERN_DIVIDE_X;
	player->g_nPatternAnim = (player->g_nPatternAnim + 1) % ANIM_PATTERN_NUM;
	if (player->g_posPlayer.x < 0) 
	{
		player->g_posPlayer.x = SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X / 2;
	}
	if (player->g_posPlayer.x > SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X / 2)
	{
		player->g_posPlayer.x = 0;
	}
	if (player->g_posPlayer.y < 0)
	{
		player->g_posPlayer.y = SCREEN_HEIGHT - TEXTURE_SAMPLE00_SIZE_Y;
	}
	if (player->g_posPlayer.y > SCREEN_HEIGHT - TEXTURE_SAMPLE00_SIZE_Y)
	{
		player->g_posPlayer.y = 0;
	}

	// ���͑Ή�
	if (GetKeyboardPress(DIK_DOWN)) {
		player->g_posPlayer.y += 5;
	}
	if (GetKeyboardPress(DIK_UP)) {
		player->g_posPlayer.y -= 5;
	}
	if (GetKeyboardPress(DIK_LEFT)) {
		player->g_posPlayer.x -= 5;
	}
	if (GetKeyboardPress(DIK_RIGHT)) {		
		player->g_posPlayer.x += 5;
	}
	if (GetKeyboardPress(DIK_SPACE) ) {	
		//BULLET *bullet = GetBullet(player->bullet_num % BULLET_MAX);
		//while (!bullet->use) {
			//OutputDebugStringA("\nSPACE\n");			
			BULLET *bullet = GetBullet(player->bullet_num );
			//Sleep(100);
			bullet->use = TRUE;
			player->bullet_num = ++player->bullet_num % BULLET_MAX;
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
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pD3DTexturePlayer );

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk, sizeof(VERTEX_2D));


	
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexPlayer();
	// rhw�̐ݒ�
	g_vertexWk[0].rhw =
	g_vertexWk[1].rhw =
	g_vertexWk[2].rhw =
	g_vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	g_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	SetTexturePlayer(player->g_nPatternAnim);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPlayer(void)
{
	// ���_���W�̐ݒ�
	g_vertexWk[0].vtx = D3DXVECTOR3(player->g_posPlayer.x, player->g_posPlayer.y, player->g_posPlayer.z);
	g_vertexWk[1].vtx = D3DXVECTOR3(player->g_posPlayer.x + TEXTURE_SAMPLE00_SIZE_X, player->g_posPlayer.y, player->g_posPlayer.z);
	g_vertexWk[2].vtx = D3DXVECTOR3(player->g_posPlayer.x, player->g_posPlayer.y + TEXTURE_SAMPLE00_SIZE_Y, player->g_posPlayer.z);
	g_vertexWk[3].vtx = D3DXVECTOR3(player->g_posPlayer.x + TEXTURE_SAMPLE00_SIZE_X, player->g_posPlayer.y + TEXTURE_SAMPLE00_SIZE_Y, player->g_posPlayer.z);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePlayer( int cntPattern )
{
	// �e�N�X�`�����W�̐ݒ�
	g_vertexWk[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	g_vertexWk[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X) + 1)/ (float)TEXTURE_PATTERN_DIVIDE_X, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	g_vertexWk[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	g_vertexWk[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X) + 1)/ (float)TEXTURE_PATTERN_DIVIDE_Y);
}
/*******************************************************************************
�֐���:	PLAYER *GetMapAdr( int pno )
����:	int pno : �v���C���[�ԍ�
�߂�l:	PLAYER *
����:	�v���C���[�̃A�h���X���擾����
*******************************************************************************/
PLAYER *GetPlayer(int pno)
{
	return &player[pno];
}