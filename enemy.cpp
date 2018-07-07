//=============================================================================
//
// �|���S������ [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEnemy(void);
void SetVertexEnemy(void);
void SetTextureEnemy( int cntPattern );	//

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy = NULL;		// �e�N�X�`���ւ̃|���S��

VERTEX_2D				g_vertexWk2[NUM_VERTEX];		// ���_���i�[���[�N

//D3DXVECTOR3				enemy->g_posEnemy;			// �|���S���̈ړ���
//D3DXVECTOR3				enemy->g_rotEnemy;			// �|���S���̉�]��
//int						enemy->g_nCountAnim2 ;		// �A�j���[�V�����J�E���g
//int						enemy->g_nPatternAnim2;		// �A�j���[�V�����p�^�[���i���o�[
//
//float					g_fRadiusEnemy;					// �|���S���̔��a
//float					g_fBaseAngleEnemy;				// �|���S���̊p�x
//
//int						direction2 = -1;

ENEMY	enemy[ENEMY_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);	

	enemy->g_posEnemy = D3DXVECTOR3(0, SCREEN_CENTER_Y - TEXTURE_SAMPLE00_SIZE_Y2/2, 0.0f);
	enemy->g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	enemy->g_nCountAnim2  = 0;
	enemy->g_nPatternAnim2 = 0;

	// ���_���̍쐬
	MakeVertexEnemy();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice,		// �f�o�C�X�̃|�C���^
		TEXTURE_GAME_SAMPLE002,				// �t�@�C���̖��O
		&g_pD3DTextureEnemy );			// �ǂݍ��ރ������̃|�C���^

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if( g_pD3DTextureEnemy != NULL )	//
	{	// �e�N�X�`���̊J��
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	// �A�j���[�V����
	enemy->g_nCountAnim2  = (enemy->g_nCountAnim2  +1) % ANIM_PATTERN_NUM2;
	enemy->g_nPatternAnim2 = (enemy->g_nPatternAnim2 + 1) % ANIM_PATTERN_NUM2;	

	//enemy->g_posEnemy.x += direction2 *  (float)  SCREEN_WIDTH / TIME_ANIMATION  ;



	//enemy->g_posEnemy.x -= (float)SCREEN_WIDTH / TIME_ANIMATION;
	//if (enemy->g_posEnemy.x < 0 || enemy->g_posEnemy.x > SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X2 / 2) {
	//	direction2 = -direction2;
	//}
	if (enemy->g_posEnemy.x < 0) 
	{
		enemy->g_posEnemy.x = SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X2 / 2;
	}
	if (enemy->g_posEnemy.x > SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X2 / 2)
	{
		enemy->g_posEnemy.x = 0;
	}
	if (enemy->g_posEnemy.y < 0)
	{
		enemy->g_posEnemy.y = SCREEN_HEIGHT - TEXTURE_SAMPLE00_SIZE_Y2;
	}
	if (enemy->g_posEnemy.y > SCREEN_HEIGHT - TEXTURE_SAMPLE00_SIZE_Y2)
	{
		enemy->g_posEnemy.y = 0;
	}

	 //���͑Ή�
	if (GetKeyboardPress(DIK_DOWN)) {
		enemy->g_posEnemy.y += 2;
	}
	if (GetKeyboardPress(DIK_UP)) {
		enemy->g_posEnemy.y -= 2;
	}
	if (GetKeyboardPress(DIK_LEFT)) {
		enemy->g_posEnemy.x -= 2;
	}
	if (GetKeyboardPress(DIK_RIGHT)) {
		enemy->g_posEnemy.x += 2;
	}

	MakeVertexEnemy();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pD3DTextureEnemy );

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk2, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexEnemy();
	// rhw�̐ݒ�
	g_vertexWk2[0].rhw =
	g_vertexWk2[1].rhw =
	g_vertexWk2[2].rhw =
	g_vertexWk2[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	g_vertexWk2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	SetTextureEnemy(enemy->g_nPatternAnim2);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEnemy(void)
{
	// ���_���W�̐ݒ�
	g_vertexWk2[0].vtx = D3DXVECTOR3(enemy->g_posEnemy.x, enemy->g_posEnemy.y, enemy->g_posEnemy.z);
	g_vertexWk2[1].vtx = D3DXVECTOR3(enemy->g_posEnemy.x + TEXTURE_SAMPLE00_SIZE_X2, enemy->g_posEnemy.y, enemy->g_posEnemy.z);
	g_vertexWk2[2].vtx = D3DXVECTOR3(enemy->g_posEnemy.x, enemy->g_posEnemy.y + TEXTURE_SAMPLE00_SIZE_Y2, enemy->g_posEnemy.z);
	g_vertexWk2[3].vtx = D3DXVECTOR3(enemy->g_posEnemy.x + TEXTURE_SAMPLE00_SIZE_X2, enemy->g_posEnemy.y + TEXTURE_SAMPLE00_SIZE_Y2, enemy->g_posEnemy.z);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEnemy( int cntPattern )
{
	// �e�N�X�`�����W�̐ݒ�
	g_vertexWk2[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X2) / (float)TEXTURE_PATTERN_DIVIDE_X2, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X2) / (float)TEXTURE_PATTERN_DIVIDE_Y2);
	g_vertexWk2[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X2) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X2, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X2) / (float)TEXTURE_PATTERN_DIVIDE_Y2);
	g_vertexWk2[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X2) / (float)TEXTURE_PATTERN_DIVIDE_X2, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X2) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y2);
	g_vertexWk2[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X2) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X2, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X2) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y2);
}
/*******************************************************************************
�֐���:	ENEMY *GetMapAdr( int pno )
����:	int pno : �G�l�~�[�ԍ�
�߂�l:	ENEMY *
����:	�G�l�~�[�̃A�h���X���擾����
*******************************************************************************/
ENEMY *GetEnemy(int pno)
{
	return &enemy[pno];
}