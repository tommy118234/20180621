//=============================================================================
//
// �|���S������ [bullet.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bullet.h"
#include "input.h"
//#include <Windows.h>
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(void);
void SetVertexBullet(void);
void SetTextureBullet(int cntPattern);
BULLET	 bullet[BULLET_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	for (int i = 0; i < BULLET_MAX; i++) {
		BULLET *bullet = GetBullet(i);
		bullet->g_posBullet = D3DXVECTOR3(player->g_posPlayer.x , player->g_posPlayer.y, 0.0f);
		bullet->use = FALSE;
		// ���_���̍쐬
		MakeVertexBullet();
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
		TEXTURE_GAME_BULLET,				// �t�@�C���̖��O
		&bullet->g_pD3DTextureBullet);		// �ǂݍ��ރ������̃|�C���^			
	}
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	if (bullet->g_pD3DTextureBullet != NULL)	//
	{	// �e�N�X�`���̊J��
		bullet->g_pD3DTextureBullet->Release();
		bullet->g_pD3DTextureBullet = NULL;
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
		if (bullet->g_posBullet.y < (-TEXTURE_SAMPLE00_SIZE_Y3) || bullet->g_posBullet.x < (-TEXTURE_SAMPLE00_SIZE_X3)) {
			bullet->g_posBullet = D3DXVECTOR3(player->g_posPlayer.x + TEXTURE_SAMPLE00_SIZE_X / 2, player->g_posPlayer.y, 0.0f);
			bullet->use = FALSE;
		}
		if (bullet->use) {
			//OutputDebugStringA("B ");
			bullet->g_posBullet.x -= 8;
		}
		else
		{
			bullet->g_posBullet = D3DXVECTOR3(player->g_posPlayer.x + TEXTURE_SAMPLE00_SIZE_X / 2, player->g_posPlayer.y, 0.0f);
		}	
	}
	MakeVertexBullet();
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
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, bullet->g_pD3DTextureBullet);	
	for (int i = 0; i < player->bullet_num; i++) {
		BULLET *bullet = GetBullet(i);
		if (bullet->use) {
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bullet->g_vertexWk, sizeof(VERTEX_2D));
		}
	}	
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBullet(void)
{	
	for (int i = 0; i < BULLET_MAX; i++) {		
		BULLET *bullet = GetBullet(i);
		// ���_���W�̐ݒ�
		SetVertexBullet();
		// rhw�̐ݒ�
		bullet->g_vertexWk[0].rhw =
		bullet->g_vertexWk[1].rhw =
		bullet->g_vertexWk[2].rhw =
		bullet->g_vertexWk[3].rhw = 1.0f;
		// ���ˌ��̐ݒ�
		bullet->g_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		bullet->g_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		bullet->g_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		bullet->g_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		// �e�N�X�`�����W�̐ݒ�
		SetTextureBullet(bullet->g_nPatternAnim);
	}
	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		BULLET *bullet = GetBullet(i);
		// ���_���W�̐ݒ�
		bullet->g_vertexWk[0].vtx = D3DXVECTOR3(bullet->g_posBullet.x, bullet->g_posBullet.y, bullet->g_posBullet.z);
		bullet->g_vertexWk[1].vtx = D3DXVECTOR3(bullet->g_posBullet.x + TEXTURE_SAMPLE00_SIZE_X3, bullet->g_posBullet.y, bullet->g_posBullet.z);
		bullet->g_vertexWk[2].vtx = D3DXVECTOR3(bullet->g_posBullet.x, bullet->g_posBullet.y + TEXTURE_SAMPLE00_SIZE_Y3, bullet->g_posBullet.z);
		bullet->g_vertexWk[3].vtx = D3DXVECTOR3(bullet->g_posBullet.x + TEXTURE_SAMPLE00_SIZE_X3, bullet->g_posBullet.y + TEXTURE_SAMPLE00_SIZE_Y3, bullet->g_posBullet.z);
	}	
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureBullet(int cntPattern)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		BULLET *bullet = GetBullet(i);
		// �e�N�X�`�����W�̐ݒ�	
		bullet->g_vertexWk[0].tex = D3DXVECTOR2(0, 0);
		bullet->g_vertexWk[1].tex = D3DXVECTOR2(1, 0);
		bullet->g_vertexWk[2].tex = D3DXVECTOR2(0, 1);
		bullet->g_vertexWk[3].tex = D3DXVECTOR2(1, 1);
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
	return &bullet[pno];
}