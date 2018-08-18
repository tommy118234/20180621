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
void SetVertexBG(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBG = NULL;		// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				g_vertexWkBG[NUM_VERTEX];	// ���_���i�[���[�N

D3DXVECTOR3				g_posBG;					// �w�i�̈ʒu

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
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAME_BG00,		// �t�@�C���̖��O
			&g_pD3DTextureBG);		// �ǂݍ��ރ������[
	}

	g_posBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_���̍쐬
	MakeVertexBG();

	//g_posBG.x = -GetPlayer(0)->pos.x / 4.0f;
	//g_posBG.y = -GetPlayer(0)->pos.y / 4.0f;
	//
	//if (BG00_POS_X - g_posBG.x < 0)
	//	g_posBG.x = 0;
	//if (BG00_POS_Y - g_posBG.y < 0)
	//	g_posBG.y = 0;

	SetVertexBG();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
{
	if (g_pD3DTextureBG != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBG->Release();
		g_pD3DTextureBG = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBG(void)
{
	// ���t���[�����s���鏈��

	// �X�N���[������
	g_posBG.x = -GetPlayer(0)->pos.x / 4.0f;
	g_posBG.y = -GetPlayer(0)->pos.y / 4.0f;

	if (BG00_POS_X - g_posBG.x < 0)
		g_posBG.x = 0;
	if (BG00_POS_Y - g_posBG.y < 0)
		g_posBG.y = 0;

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
	pDevice->SetTexture(0, g_pD3DTextureBG);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkBG, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBG(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkBG[0].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y, 0.0f);
	g_vertexWkBG[1].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y, 0.0f);
	g_vertexWkBG[2].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f);
	g_vertexWkBG[3].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	g_vertexWkBG[0].rhw =
		g_vertexWkBG[1].rhw =
		g_vertexWkBG[2].rhw =
		g_vertexWkBG[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkBG[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBG[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBG[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBG[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkBG[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkBG[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkBG[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkBG[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBG(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkBG[0].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y, 0.0f) + g_posBG;
	g_vertexWkBG[1].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y, 0.0f) + g_posBG;
	g_vertexWkBG[2].vtx = D3DXVECTOR3(BG00_POS_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f) + g_posBG;
	g_vertexWkBG[3].vtx = D3DXVECTOR3(BG00_POS_X + BG00_SIZE_X, BG00_POS_Y + BG00_SIZE_Y, 0.0f) + g_posBG;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SwitchBG(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	switch (type)
	{
	case 1:
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAME_BG01,								// �t�@�C���̖��O
			&g_pD3DTextureBG);								// �ǂݍ��ރ������[
		break;

	case 2:
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAME_BG02,								// �t�@�C���̖��O
			&g_pD3DTextureBG);								// �ǂݍ��ރ������[
		break;

	case 3:
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAME_BG03,								// �t�@�C���̖��O
			&g_pD3DTextureBG);								// �ǂݍ��ރ������[				
		break;

	case 4:
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GAME_BG04,								// �t�@�C���̖��O
			&g_pD3DTextureBG);								// �ǂݍ��ރ������[
		break;

	}
	
}