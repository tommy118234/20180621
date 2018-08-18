/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�G�l�~�[���� [enemy.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/

#include "main.h"
#include "enemy.h"
#include "input.h"
#include "math.h"

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
ENEMY	enemy[ENEMY_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);	

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_ENEMY,				// �t�@�C���̖��O
			&g_pD3DTextureEnemy);			// �ǂݍ��ރ������̃|�C���^
	}


	// �G�l�~�[�̏���������
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		enemy->use = true;
		//enemy->pos = D3DXVECTOR3(TEXTURE_ENEMY_SIZE_X, SCREEN_CENTER_Y - TEXTURE_ENEMY_SIZE_Y / 2, 0.0f);
		enemy->pos = D3DXVECTOR3(SCREEN_CENTER_X,  TEXTURE_ENEMY_SIZE_Y, 0.0f);
		
		enemy->rot = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		enemy->CountAnim = 0;
		enemy->PatternAnim = 0;
		enemy->status.HP = 10;

		//enemy Base Angle, Radius
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X / 2, TEXTURE_ENEMY_SIZE_Y / 2);
		enemy->Radius = D3DXVec2Length(&temp);
		enemy->BaseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);

		// ���_���̍쐬
		MakeVertexEnemy();			// �ǂݍ��ރ������̃|�C���^
	}
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
	enemy->CountAnim  = (enemy->CountAnim  +1) % ANIM_PATTERN_NUM_ENEMY;
	enemy->PatternAnim = (enemy->PatternAnim + 1) % ANIM_PATTERN_NUM_ENEMY;

	if (enemy->pos.x < 0) 
	{
		enemy->pos.x = SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X / 2;
	}
	if (enemy->pos.x > SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X / 2)
	{
		enemy->pos.x = 0;
	}
	if (enemy->pos.y < 0)
	{
		enemy->pos.y = SCREEN_HEIGHT - TEXTURE_ENEMY_SIZE_Y;
	}
	if (enemy->pos.y > SCREEN_HEIGHT - TEXTURE_ENEMY_SIZE_Y)
	{
		enemy->pos.y = 0;
	}
	
	MakeVertexEnemy();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	ENEMY  *enemy = GetEnemy(0);
	if (enemy->use) {
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureEnemy);
		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk2, sizeof(VERTEX_2D));
	}
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
	SetTextureEnemy(enemy->PatternAnim);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEnemy(void)
{
	//// ���_���W�̐ݒ�
	//g_vertexWk2[0].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y, enemy->pos.z);
	//g_vertexWk2[1].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y, enemy->pos.z);
	//g_vertexWk2[2].vtx = D3DXVECTOR3(enemy->pos.x, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, enemy->pos.z);
	//g_vertexWk2[3].vtx = D3DXVECTOR3(enemy->pos.x + TEXTURE_ENEMY_SIZE_X, enemy->pos.y + TEXTURE_ENEMY_SIZE_Y, enemy->pos.z);
	
	// ��]���_���W�̐ݒ�
	enemy->rot.z = enemy->rot.z - 0.1;

	g_vertexWk2[0].vtx.x = enemy->pos.x - cos(enemy->BaseAngle + enemy->rot.z)* enemy->Radius;
	g_vertexWk2[0].vtx.y = enemy->pos.y - sin(enemy->BaseAngle + enemy->rot.z)* enemy->Radius;
	
	g_vertexWk2[1].vtx.x = enemy->pos.x + cos(enemy->BaseAngle - enemy->rot.z)* enemy->Radius;
	g_vertexWk2[1].vtx.y = enemy->pos.y - sin(enemy->BaseAngle - enemy->rot.z)* enemy->Radius;
	
	g_vertexWk2[2].vtx.x = enemy->pos.x - cos(enemy->BaseAngle - enemy->rot.z)* enemy->Radius;
	g_vertexWk2[2].vtx.y = enemy->pos.y + sin(enemy->BaseAngle - enemy->rot.z)* enemy->Radius;
	
	g_vertexWk2[3].vtx.x = enemy->pos.x + cos(enemy->BaseAngle + enemy->rot.z)* enemy->Radius;
	g_vertexWk2[3].vtx.y = enemy->pos.y + sin(enemy->BaseAngle + enemy->rot.z)* enemy->Radius;
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEnemy( int cntPattern )
{
	// �e�N�X�`�����W�̐ݒ�
	g_vertexWk2[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	g_vertexWk2[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, (float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	g_vertexWk2[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	g_vertexWk2[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, (float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
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