/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�o���b�g���� [bullet.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(int no);
void SetTextureBullet(int no, int cntPattern);
void SetVertexBullet(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;		// �e�N�X�`���ւ̃|���S��
BULLET					bulletWk[BULLET_MAX];			// �o���b�g�\����
LPDIRECTSOUNDBUFFER8	g_pSE;						// SE�p�o�b�t�@


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = GetBullet(0);		// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer(0);

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_BULLET,			// �t�@�C���̖��O
			&g_pD3DTextureBullet);			// �ǂݍ��ރ������̃|�C���^

		g_pSE = LoadSound(SE_00);

	}

	for (int i = 0; i < BULLET_MAX; i++) {

		BULLET *bullet = GetBullet(i);							
		bullet->use = FALSE;									// ���g�p�i���˂���Ă��Ȃ��e�j
		bullet->pos = D3DXVECTOR3(player->pos.x, player->pos.y, 0.0f); // ���W�f�[�^��������
		bullet->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]�f�[�^��������
		bullet->PatternAnim = 0;								// �A�j���p�^�[���ԍ��������_���ŏ�����
		bullet->CountAnim = 0;
		bullet->BaseAngle = atan2f(TEXTURE_BULLET_SIZE_Y, TEXTURE_BULLET_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_BULLET_SIZE_X / 2, TEXTURE_BULLET_SIZE_Y / 2);
		bullet->Radius = D3DXVec2Length(&temp);

		bullet->Texture = g_pD3DTextureBullet;
		// ���_���̍쐬
		MakeVertexBullet(i);		
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	if (g_pD3DTextureBullet != NULL)	//
	{	// �e�N�X�`���̊J��
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}	

	if (g_pSE != NULL)
	{	// �e�N�X�`���̊J��
		g_pSE->Release();
		g_pSE = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	// �A�j���[�V���� 
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < player->bullet_num + 1; i++) {
		BULLET *bullet = GetBullet(i);
		
		if (bullet->use) {
			// �o���b�g�̈ړ�����
			bullet->pos.y -= bullet->direction * BULLET_SPEED;
			// ��ʊO�܂Ői�񂾁H
			if (bullet->pos.y < (-TEXTURE_BULLET_SIZE_Y) || bullet->pos.x < (-TEXTURE_BULLET_SIZE_X)) {
				bullet->use = FALSE;
				bullet->pos = D3DXVECTOR3(player->pos.x + TEXTURE_BULLET_SIZE_X / 2, player->pos.y, 0.0f);
			}
		}
		else // bullet->use = false
		{
			bullet->pos = D3DXVECTOR3(player->pos.x + TEXTURE_BULLET_SIZE_X / 2, player->pos.y, 0.0f);
		}	

		MakeVertexBullet(i);
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	BULLET *bullet = GetBullet(0);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	for (int i = 0; i < player->bullet_num; i++) {
		BULLET *bullet = GetBullet(i);
		if (bullet->use) {
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, bullet->Texture);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bullet->vertexWk, sizeof(VERTEX_2D));
		}
	}	
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBullet(int no)
{	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		
	BULLET *bullet = &bulletWk[no];

	// ���_���W�̐ݒ�
	SetVertexBullet(no);
	// rhw�̐ݒ�
	bullet->vertexWk[0].rhw =
	bullet->vertexWk[1].rhw =
	bullet->vertexWk[2].rhw =
	bullet->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	bullet->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	bullet->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	SetTextureBullet(no,bullet->PatternAnim);
	
	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBullet(int no)
{	
	BULLET *bullet = GetBullet(no);

	// ���_���W�̐ݒ�
	bullet->vertexWk[0].vtx = D3DXVECTOR3(bullet->pos.x, bullet->pos.y, bullet->pos.z);
	bullet->vertexWk[1].vtx = D3DXVECTOR3(bullet->pos.x + TEXTURE_BULLET_SIZE_X, bullet->pos.y, bullet->pos.z);
	bullet->vertexWk[2].vtx = D3DXVECTOR3(bullet->pos.x, bullet->pos.y + TEXTURE_BULLET_SIZE_Y, bullet->pos.z);
	bullet->vertexWk[3].vtx = D3DXVECTOR3(bullet->pos.x + TEXTURE_BULLET_SIZE_X, bullet->pos.y + TEXTURE_BULLET_SIZE_Y, bullet->pos.z);
	
	

}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureBullet(int no, int cntPattern)
{	
	BULLET *bullet = GetBullet(no);

	//// �e�N�X�`�����W�̐ݒ�	
	//bullet->vertexWk[0].tex = D3DXVECTOR2(0, 0);
	//bullet->vertexWk[1].tex = D3DXVECTOR2(1, 0);
	//bullet->vertexWk[2].tex = D3DXVECTOR2(0, 1);
	//bullet->vertexWk[3].tex = D3DXVECTOR2(1, 1);
	//
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_BULLET;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
	// ���_���W�̐ݒ�
	bullet->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	bullet->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	bullet->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	bullet->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetBullet(D3DXVECTOR3 pos)
{
	BULLET *bullet = &bulletWk[0];			// �o���b�g�̃|�C���^�[��������

											// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use == false)			// ���g�p��Ԃ̃o���b�g��������
		{
			bullet->use = true;				// �g�p��Ԃ֕ύX����
			bullet->pos = pos;				// ���W���Z�b�g

											// ���ˉ��Đ�
			PlaySound(g_pSE, E_DS8_FLAG_NONE);

			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

/*******************************************************************************
�֐���:	BULLET *GetBulletAdr( int pno )
����:	int pno : Bullet�ԍ�
�߂�l:	BULLET *
����:	Bullet�̃A�h���X���擾����
*******************************************************************************/
BULLET *GetBullet(int pno)
{
	return &bulletWk[pno];
}