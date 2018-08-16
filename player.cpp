/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������: �v���[���[���� [player.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
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
  

int bullet_cooldown = 0;
int moving_cooldown = 0;
int gravity = 5;
int acceleration = 0;
bool jump_cooldown = FALSE;


PLAYER					player[PLAYER_MAX];				// �v���C���[�\����
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);	// �v���C���[�O�Ԃ̃A�h���X���擾����

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_PLAYER,				// �t�@�C���̖��O
			&g_pD3DTexturePlayer);				// �ǂݍ��ރ������̃|�C���^
	}

	//player->pos = D3DXVECTOR3(SCREEN_CENTER_X + TEXTURE_PLAYER_SIZE_X / 2, SCREEN_CENTER_Y + TEXTURE_PLAYER_SIZE_Y / 2, 0.0f);
	player->pos = D3DXVECTOR3( TEXTURE_PLAYER_SIZE_X / 2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
	player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->CountAnim = 0;
	player->PatternAnim = 0;
	// ���_���̍쐬
	MakeVertexPlayer();
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice,		// �f�o�C�X�̃|�C���^
		TEXTURE_GAME_PLAYER,				// �t�@�C���̖��O
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
	


	//���L����
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


	// �A�j���[�V����
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
	
	// ���͑Ή�
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
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, player->vertexWk, sizeof(VERTEX_2D));	
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
	player->vertexWk[0].rhw =
	player->vertexWk[1].rhw =
	player->vertexWk[2].rhw =
	player->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	player->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	SetTexturePlayer(player->PatternAnim);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPlayer(void)
{
	// ���_���W�̐ݒ�
	player->vertexWk[0].vtx = D3DXVECTOR3(player->pos.x, player->pos.y, player->pos.z);
	player->vertexWk[1].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X, player->pos.y, player->pos.z);
	player->vertexWk[2].vtx = D3DXVECTOR3(player->pos.x, player->pos.y + TEXTURE_PLAYER_SIZE_Y, player->pos.z);
	player->vertexWk[3].vtx = D3DXVECTOR3(player->pos.x + TEXTURE_PLAYER_SIZE_X, player->pos.y + TEXTURE_PLAYER_SIZE_Y, player->pos.z);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePlayer( int cntPattern )
{
	// �e�N�X�`�����W�̐ݒ�
	player->vertexWk[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	player->vertexWk[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X) + 1)/ (float)TEXTURE_PATTERN_DIVIDE_X, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	player->vertexWk[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y);
	player->vertexWk[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X) + 1)/ (float)TEXTURE_PATTERN_DIVIDE_Y);
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