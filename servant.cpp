/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�o���b�g���� [SERVANT.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "SERVANT.h"
#include "bullet.h"
#include "sound.h"

#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexSERVANT(int no);
void SetTextureSERVANT(int no, int cntPattern);
void SetVertexSERVANT(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureSERVANT = NULL;		// �e�N�X�`���ւ̃|���S��
SERVANT					SERVANTWk[SERVANT_MAX];			// �o���b�g�\����
LPDIRECTSOUNDBUFFER8	g_pSE3;							// SE�p�o�b�t�@

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSERVANT(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *SERVANT = &SERVANTWk[0];		// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer(0);



	for (int i = 0; i < SERVANT_MAX; i++, SERVANT++) {
		g_pSE3 = LoadSound(SE_01);
		// �e�N�X�`���[�̏��������s���H
		if (type == 0)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
				TEXTURE_GAME_SERVANT,			// �t�@�C���̖��O
				&g_pD3DTextureSERVANT);			// �ǂݍ��ރ������̃|�C���^
			//g_pSE3 = LoadSound(SE_00);
		}
		else if (type == 1) {
			UninitSERVANT;
		}

		SERVANT->use = FALSE;									// ���g�p�i���˂���Ă��Ȃ��e�j
		SERVANT->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);			// ���W�f�[�^��������
		SERVANT->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]�f�[�^��������
		SERVANT->PatternAnim = 0;								// �A�j���p�^�[���ԍ��������_���ŏ�����
		SERVANT->CountAnim = 0;									// �A�j���J�E���g��������

		SERVANT->BaseAngle = atan2f(TEXTURE_SERVANT_SIZE_Y, TEXTURE_SERVANT_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_SERVANT_SIZE_X / 2, TEXTURE_SERVANT_SIZE_Y / 2);
		SERVANT->Radius = D3DXVec2Length(&temp);

		SERVANT->Texture = g_pD3DTextureSERVANT;					// �e�N�X�`�����
		MakeVertexSERVANT(i);									// ���_���̍쐬
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitSERVANT(void)
{
	if (g_pD3DTextureSERVANT != NULL)	//
	{	// �e�N�X�`���̊J��
		g_pD3DTextureSERVANT->Release();
		g_pD3DTextureSERVANT = NULL;
	}

	if (g_pSE3 != NULL)
	{	// �e�N�X�`���̊J��
		g_pSE3->Release();
		g_pSE3 = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateSERVANT(void)
{

	SERVANT *SERVANT = SERVANTWk;			// �o���b�g�̃|�C���^�[��������
	ENEMY *enemy = GetEnemy(0);
	for (int i = 0; i < SERVANT_MAX; i++, SERVANT++)
	{
		if (SERVANT->use == true)			// ���g�p��Ԃ̃o���b�g��������
		{
			//// SERVANT�̈ړ�����

			if (enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2 > SERVANT->pos.x + TEXTURE_SERVANT_SIZE_X / 2 &&  SERVANT->rot.z < 1.57 - atan2f(SERVANT->pos.y + TEXTURE_SERVANT_SIZE_Y / 2, BG00_SIZE_X/2 - SERVANT->pos.x ))
				SERVANT->rot.z += 0.1;
			//else
			//	SERVANT->rot.z = 1.57 - atan2f(SERVANT->pos.y - TEXTURE_SERVANT_SIZE_Y / 2, enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2);
			
			if (enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2 < SERVANT->pos.x + TEXTURE_SERVANT_SIZE_X / 2 && SERVANT->rot.z > -(1.57 - atan2f(SERVANT->pos.y + TEXTURE_SERVANT_SIZE_Y / 2,SERVANT->pos.x -  BG00_SIZE_X/2 )))
				SERVANT->rot.z -= 0.1;
			//else
			//	SERVANT->rot.z = 1.57 - atan2f(SERVANT->pos.y - TEXTURE_SERVANT_SIZE_Y / 2, enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2);
			
			if (SERVANT->bullet_cooldown < 1) {				
				SERVANT->bullet_cooldown = 200;
				D3DXVECTOR3 pos = SERVANT->pos;
				//pos.y -= TEXTURE_SERVANT_SIZE_Y;				
				pos.x -= GetPlayer(0)->pos.x / 4.0f;
				if (SERVANT->rot.z> 0)
				SetBullet(pos,SERVANT->rot.z);
				else
				SetBullet(pos,  SERVANT->rot.z);
			}
			else {
				SERVANT->bullet_cooldown--;
			}

			// ��ʊO�܂Ői�񂾁H
			if (SERVANT->pos.y < -TEXTURE_SERVANT_SIZE_Y)
			{
				SERVANT->use = FALSE;
				SERVANT->pos.x = -100.0f;
				//SERVANT->pos = D3DXVECTOR3(player->pos.x + TEXTURE_SERVANT_SIZE_X / 2, player->pos.y, 0.0f);
			}
			SetVertexSERVANT(i);
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawSERVANT(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *SERVANT = SERVANTWk;				// �o���b�g�̃|�C���^�[��������

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SERVANT_MAX; i++, SERVANT++)
	{
		if (SERVANT->use)					// �g�p���Ă����ԂȂ�X�V����
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, SERVANT->Texture);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SERVANT, SERVANT->vertexWk, sizeof(VERTEX_2D));
		}
	}
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexSERVANT(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *SERVANT = &SERVANTWk[no];

	// ���_���W�̐ݒ�
	SetVertexSERVANT(no);
	// rhw�̐ݒ�
	SERVANT->vertexWk[0].rhw =
		SERVANT->vertexWk[1].rhw =
		SERVANT->vertexWk[2].rhw =
		SERVANT->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	SERVANT->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	SERVANT->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	SERVANT->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	SERVANT->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	SERVANT->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	SERVANT->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT, 0.0f);
	SERVANT->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT);
	SERVANT->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT);

	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexSERVANT(int no)
{
	SERVANT *SERVANT = &SERVANTWk[no];			// �o���b�g�̃|�C���^�[��������

	// ���_���W�̐ݒ�
	SERVANT->vertexWk[0].vtx = D3DXVECTOR3(SERVANT->pos.x - TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
		SERVANT->pos.y - TEXTURE_SERVANT_SIZE_Y,
		0);
	SERVANT->vertexWk[1].vtx = D3DXVECTOR3(SERVANT->pos.x + TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
		SERVANT->pos.y - TEXTURE_SERVANT_SIZE_Y,
		0);
	SERVANT->vertexWk[2].vtx = D3DXVECTOR3(SERVANT->pos.x - TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
		SERVANT->pos.y + TEXTURE_SERVANT_SIZE_Y,
		0);
	SERVANT->vertexWk[3].vtx = D3DXVECTOR3(SERVANT->pos.x + TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
		SERVANT->pos.y + TEXTURE_SERVANT_SIZE_Y,
		0);

	// ���_���W�̐ݒ�
	SERVANT->vertexWk[0].vtx.x = SERVANT->pos.x - cosf(SERVANT->BaseAngle + SERVANT->rot.z) * SERVANT->Radius - GetPlayer(0)->pos.x / 4.0f;
	SERVANT->vertexWk[0].vtx.y = SERVANT->pos.y - sinf(SERVANT->BaseAngle + SERVANT->rot.z) * SERVANT->Radius;
	SERVANT->vertexWk[0].vtx.z = 0.0f;

	SERVANT->vertexWk[1].vtx.x = SERVANT->pos.x + cosf(SERVANT->BaseAngle - SERVANT->rot.z) * SERVANT->Radius - GetPlayer(0)->pos.x / 4.0f;
	SERVANT->vertexWk[1].vtx.y = SERVANT->pos.y - sinf(SERVANT->BaseAngle - SERVANT->rot.z) * SERVANT->Radius;
	SERVANT->vertexWk[1].vtx.z = 0.0f;

	SERVANT->vertexWk[2].vtx.x = SERVANT->pos.x - cosf(SERVANT->BaseAngle - SERVANT->rot.z) * SERVANT->Radius - GetPlayer(0)->pos.x / 4.0f;
	SERVANT->vertexWk[2].vtx.y = SERVANT->pos.y + sinf(SERVANT->BaseAngle - SERVANT->rot.z) * SERVANT->Radius;
	SERVANT->vertexWk[2].vtx.z = 0.0f;

	SERVANT->vertexWk[3].vtx.x = SERVANT->pos.x + cosf(SERVANT->BaseAngle + SERVANT->rot.z) * SERVANT->Radius - GetPlayer(0)->pos.x / 4.0f;
	SERVANT->vertexWk[3].vtx.y = SERVANT->pos.y + sinf(SERVANT->BaseAngle + SERVANT->rot.z) * SERVANT->Radius;
	SERVANT->vertexWk[3].vtx.z = 0.0f;
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureSERVANT(int no, int cntPattern)
{
	SERVANT *SERVANT = &SERVANTWk[no];			// �o���b�g�̃|�C���^�[��������

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT;
	// ���_���W�̐ݒ�
	SERVANT->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	SERVANT->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	SERVANT->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	SERVANT->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetSERVANT(D3DXVECTOR3 pos, int type)
{
	SERVANT *SERVANT = &SERVANTWk[0];			// �o���b�g�̃|�C���^�[��������
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < SERVANT_MAX; i++, SERVANT++)
	{
		if (SERVANT->use == false)			// ���g�p��Ԃ̃o���b�g��������
		{

			SERVANT->use = true;				// �g�p��Ԃ֕ύX����

			switch (type) {
			case 1:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT,			// �t�@�C���̖��O
					&g_pD3DTextureSERVANT);			// �ǂݍ��ރ������̃|�C���^
				break;
			case 2:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT2,			// �t�@�C���̖��O
					&g_pD3DTextureSERVANT);			// �ǂݍ��ރ������̃|�C���^
				break;
			case 3:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT3,			// �t�@�C���̖��O
					&g_pD3DTextureSERVANT);			// �ǂݍ��ރ������̃|�C���^
				break;
			}
			SERVANT->Texture = g_pD3DTextureSERVANT;					// �e�N�X�`�����
			//MakeVertexSERVANT(i);									// ���_���̍쐬
			SERVANT->pos = pos;				// ���W���Z�b�g

			g_pSE3 = LoadSound(SE_01);
			// ���ˉ��Đ�
			if (IsPlaying(g_pSE3))
				g_pSE3->SetCurrentPosition(0);
			else {
				PlaySound(g_pSE3, E_DS8_FLAG_NONE);
			}

			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

/*******************************************************************************
�֐���:	SERVANT *GetSERVANTAdr( int pno )
����:	int pno : SERVANT�ԍ�
�߂�l:	SERVANT *
����:	SERVANT�̃A�h���X���擾����
*******************************************************************************/
SERVANT *GetSERVANT(int pno)
{
	return &SERVANTWk[pno];
}