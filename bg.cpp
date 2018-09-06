/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�w�i���� [bg.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#include "main.h"
#include "bg.h"
#include "input.h"
#include "player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBG(void);
void	SetVertexBG(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureBG = NULL;		// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				vertexWkBG[NUM_VERTEX];		// ���_���i�[���[�N
D3DXVECTOR3				posBG;						// �w�i�̈ʒu
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBG(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	
	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BG00,							// �t�@�C���̖��O
		&D3DTextureBG);								// �ǂݍ��ރ������[
	}
	else if (type == 1) 
	{
		if (D3DTextureBG != NULL)
		{
			// �e�N�X�`���̊J��
			D3DTextureBG->Release();
			D3DTextureBG = NULL;
		}
	}
	posBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// ���_���̍쐬
	MakeVertexBG();
	SetVertexBG();
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
{
	if (D3DTextureBG != NULL)
	{	
		// �e�N�X�`���̊J��
		D3DTextureBG->Release();
		D3DTextureBG = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateBG(void)
{
	// ���t���[�����s���鏈��
	// �X�N���[������
	PLAYER *player = GetPlayer(0);
	//
	posBG.x = -player->pos.x / 4.0f;	
	//
	//
	//posBG.x -= 5 * (-sinf(player->BaseAngle + player->rot.z) + cosf(player->BaseAngle + player->rot.z));
	//posBG.y -= 5 * (cosf(player->BaseAngle + player->rot.z) + sinf(player->BaseAngle + player->rot.z));
	SetVertexBG();
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, D3DTextureBG);
	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWkBG, sizeof(VERTEX_2D));
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBG(void)
{
	// ���_���W�̐ݒ�
	vertexWkBG[0].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y, 0.0f);
	vertexWkBG[1].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y, 0.0f);
	vertexWkBG[2].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f);
	vertexWkBG[3].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f);
	// rhw�̐ݒ�
	vertexWkBG[0].rhw =
	vertexWkBG[1].rhw =
	vertexWkBG[2].rhw =
	vertexWkBG[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	vertexWkBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWkBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	vertexWkBG[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWkBG[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWkBG[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWkBG[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBG(void)
{
	// ���_���W�̐ݒ�
	vertexWkBG[0].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y, 0.0f) + posBG;
	vertexWkBG[1].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y, 0.0f) + posBG;
	vertexWkBG[2].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f) + posBG;
	vertexWkBG[3].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f) + posBG;

}
//=============================================================================
// �w�i�̐ݒ�
//=============================================================================
void SwitchBG(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (D3DTextureBG != NULL)
	{
		// �e�N�X�`���̊J��
		D3DTextureBG->Release();
		D3DTextureBG = NULL;
	}
	switch (type)
	{
	case 1:		
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BG01,								// �t�@�C���̖��O
		&D3DTextureBG);									// �ǂݍ��ރ������[		
		break;
	case 2:
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BG02,								// �t�@�C���̖��O
		&D3DTextureBG);									// �ǂݍ��ރ������[
		break;
	case 3:
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BG03,								// �t�@�C���̖��O
		&D3DTextureBG);									// �ǂݍ��ރ������[				
		break;
	case 4:
		ChangeBG();
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BG04,								// �t�@�C���̖��O
		&D3DTextureBG);									// �ǂݍ��ރ������[
		break;
	case 5:
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BG05,								// �t�@�C���̖��O
		&D3DTextureBG);									// �ǂݍ��ރ������[
		break;
	case 6:
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_GAME_BG06,								// �t�@�C���̖��O
		&D3DTextureBG);									// �ǂݍ��ރ������[
		break;	
	}
}
void ChangeBG(void)
{
#undef	 BG00_SIZE_X	
#undef	 BG00_SIZE_Y			
#define  BG00_SIZE_X	(1440)	// �e�N�X�`���T�C�Y
#define  BG00_SIZE_Y	(1080)	// ����
}