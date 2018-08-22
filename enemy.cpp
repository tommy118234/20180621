/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�G�l�~�[���� [enemy.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/

#include "main.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "math.h"
#include "bg.h"

#include "beam.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEnemy( int no);
void SetVertexEnemy(int no);
void SetTextureEnemy(int no, int cntPattern );	//

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemy = NULL;		// �e�N�X�`���ւ̃|���S��		
ENEMY					enemyWk[ENEMY_MAX];				// ���_���i�[���[�N
int beam_cooldown = 0;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = enemyWk;

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_ENEMY,				// �t�@�C���̖��O
			&g_pD3DTextureEnemy);			// �ǂݍ��ރ������̃|�C���^
	}
	else if (type == 1) {
		UninitEnemy;
	}


	// �G�l�~�[�̏���������
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		enemy->use = true;										// �g�p
		enemy->pos = D3DXVECTOR3(BG00_SIZE_X/2, TEXTURE_ENEMY_SIZE_Y/2, 0.0f);	 // ���W�f�[�^��������	
		enemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ��]�f�[�^��������
		enemy->CountAnim = 0;									// �A�j���p�^�[���ԍ��������_���ŏ�����
		enemy->PatternAnim = 0;									// �A�j���J�E���g��������


		enemy->status.HP = 5000;								// HP��������
		enemy->status.MP= 100;									// MP��������
		enemy->status.ATK = 300;								// ATK��������
		enemy->status.DEF = 50;								// DEF��������
		enemy->status.LUCK = 100;								// LUCK��������
		strcpy(enemy->status.name,"�S��̌` ?");				// NAME��������


		//enemy Base Angle, Radius
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X / 2, TEXTURE_ENEMY_SIZE_Y / 2);
		enemy->Radius = D3DXVec2Length(&temp);
		enemy->BaseAngle = atan2f(TEXTURE_ENEMY_SIZE_Y, TEXTURE_ENEMY_SIZE_X);

		enemy->Texture = g_pD3DTextureEnemy;					// �e�N�X�`�����		
		MakeVertexEnemy(i);										// ���_���̍쐬
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{	
		if (g_pD3DTextureEnemy != NULL)	//
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
	ENEMY *enemy = enemyWk;				// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer(0);					// �v���C���[�O�Ԃ̃A�h���X���擾����

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == true)			// �g�p���Ă����ԂȂ�X�V����
		{
			// �A�j���[�V����
			enemy->CountAnim = (enemy->CountAnim + 1) % ANIM_PATTERN_NUM_ENEMY;
			enemy->PatternAnim = (enemy->PatternAnim + 1) % ANIM_PATTERN_NUM_ENEMY;

			if (enemy->pos.x < 0)
			{
				enemy->pos.x = 0;
			}
			if (enemy->pos.x > SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X)
			{
				enemy->pos.x = SCREEN_WIDTH - TEXTURE_ENEMY_SIZE_X;
			}
			if (enemy->pos.y < 0)
			{
				enemy->pos.y = 0;
			}
			if (enemy->pos.y > SCREEN_HEIGHT - TEXTURE_ENEMY_SIZE_Y)
			{
				enemy->pos.y = SCREEN_HEIGHT - TEXTURE_ENEMY_SIZE_Y;
			}
			
			if (player->pos.x + TEXTURE_PLAYER_SIZE_X / 2 > BG00_SIZE_X / 2 && enemy->rot.z >-( 1.57 - atan2f(player->pos.y + TEXTURE_PLAYER_SIZE_Y / 2, player->pos.x - BG00_SIZE_X / 2)))
				enemy->rot.z -= 0.02;
			if (player->pos.x + TEXTURE_ENEMY_SIZE_X / 2 < BG00_SIZE_X / 2 && enemy->rot.z < 1.57 - atan2f(player->pos.y + TEXTURE_PLAYER_SIZE_Y / 2, BG00_SIZE_X / 2 - player->pos.x))
				enemy->rot.z += 0.02;

			if (enemy->status.HP < 4500) {
				if (beam_cooldown == 0) 
				{
					BEAM *beam = GetBeam(0);		// �G�l�~�[�̃|�C���^�[��������
					D3DXVECTOR3 pos = enemy->pos;
					//pos.y += TEXTURE_ENEMY_SIZE_Y/2;				
					//pos.x += TEXTURE_ENEMY_SIZE_X/2;
					pos.x -= GetPlayer(0)->pos.x / 4.0f;	
					// Hard Mode
					//for (int i = 0; i < 50; i++) {
					//	pos.x -= i * (sinf(beam->BaseAngle + beam->rot.z) - cosf(beam->BaseAngle + beam->rot.z));
					//	pos.y += i * (cosf(beam->BaseAngle + beam->rot.z) + sinf(beam->BaseAngle + beam->rot.z));
					//	SetBeam(pos, enemy->rot.z);
					//}
					SetBeam(pos, enemy->rot.z);
					beam_cooldown = 50;
				}
				beam_cooldown--;
			}			
			SetVertexEnemy(i);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = enemyWk;				// �G�l�~�[�̃|�C���^�[��������

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == true)			// �g�p���Ă����ԂȂ�`�悷��
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, enemy->Texture);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, enemy->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEnemy(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	
	ENEMY *enemy = &enemyWk[no];			// �G�l�~�[�̃|�C���^�[��������

	// ���_���W�̐ݒ�
	SetVertexEnemy(no);
	// rhw�̐ݒ�
	enemy->vertexWk[0].rhw =
	enemy->vertexWk[1].rhw =
	enemy->vertexWk[2].rhw =
	enemy->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	enemy->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	enemy->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	enemy->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMY, 0.0f);
	enemy->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	enemy->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMY, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMY);


	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEnemy(int no)
{
	ENEMY *enemy = &enemyWk[no];			// �G�l�~�[�̃|�C���^�[��������
	
	// ���_���W�̐ݒ�
	enemy->vertexWk[0].vtx.x = enemy->pos.x - cosf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius - GetPlayer(0)->pos.x / 4.0f;
	enemy->vertexWk[0].vtx.y = enemy->pos.y - sinf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[0].vtx.z = 0.0f;

	enemy->vertexWk[1].vtx.x = enemy->pos.x + cosf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius - GetPlayer(0)->pos.x / 4.0f;
	enemy->vertexWk[1].vtx.y = enemy->pos.y - sinf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[1].vtx.z = 0.0f;

	enemy->vertexWk[2].vtx.x = enemy->pos.x - cosf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius - GetPlayer(0)->pos.x / 4.0f;
	enemy->vertexWk[2].vtx.y = enemy->pos.y + sinf(enemy->BaseAngle - enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[2].vtx.z = 0.0f;

	enemy->vertexWk[3].vtx.x = enemy->pos.x + cosf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius - GetPlayer(0)->pos.x / 4.0f;
	enemy->vertexWk[3].vtx.y = enemy->pos.y + sinf(enemy->BaseAngle + enemy->rot.z) * enemy->Radius;
	enemy->vertexWk[3].vtx.z = 0.0f;
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEnemy( int no, int cntPattern )
{
	ENEMY *enemy = &enemyWk[no];			// �G�l�~�[�̃|�C���^�[��������

	// �e�N�X�`�����W�̐ݒ�
	enemy->vertexWk[0].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY, 
						(float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	enemy->vertexWk[1].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY,
						(float)(cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	enemy->vertexWk[2].tex = D3DXVECTOR2((float)(cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY,
						(float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
	enemy->vertexWk[3].tex = D3DXVECTOR2((float)((cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_X_ENEMY,
						(float)((cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY) + 1) / (float)TEXTURE_PATTERN_DIVIDE_Y_ENEMY);
}
/*******************************************************************************
�֐���:	ENEMY *GetMapAdr( int pno )
����:	int pno : �G�l�~�[�ԍ�
�߂�l:	ENEMY *
����:	�G�l�~�[�̃A�h���X���擾����
*******************************************************************************/
ENEMY *GetEnemy(int pno)
{
	return &enemyWk[pno];
}