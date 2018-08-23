/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	Beam���� [beam.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"
#include "beam.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBeam(int no);
void SetTextureBeam(int no, int cntPattern);
void SetVertexBeam(int no);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBeam = NULL;		// �e�N�X�`���ւ̃|���S��
BEAM					beamWk[BEAM_MAX];				// Beam�\����
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBeam(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BEAM *beam = &beamWk[0];							// Beam�̃|�C���^�[��������
	PLAYER *player = GetPlayer(0);
	for (int i = 0; i < BEAM_MAX; i++, beam++) {		
		// �e�N�X�`���[�̏��������s���H
		if (type == 0)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_BEAM,						// �t�@�C���̖��O
			&g_pD3DTextureBeam);					// �ǂݍ��ރ������̃|�C���^			
		}
		else if (type == 1) {
			UninitBeam;
		}
		beam->use = FALSE;								// ���g�p�i���˂���Ă��Ȃ��e�j
		beam->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);	// ���W�f�[�^��������
		beam->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�f�[�^��������
		beam->PatternAnim = 0;							// �A�j���p�^�[���ԍ��������_���ŏ�����
		beam->CountAnim = 0;							// �A�j���J�E���g��������
		beam->BaseAngle = atan2f(TEXTURE_BEAM_SIZE_Y, TEXTURE_BEAM_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_BEAM_SIZE_X / 2, TEXTURE_BEAM_SIZE_Y / 2);
		beam->Radius = D3DXVec2Length(&temp);
		beam->Texture = g_pD3DTextureBeam;				// �e�N�X�`�����
		MakeVertexBeam(i);								// ���_���̍쐬
	}
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitBeam(void)
{
	if (g_pD3DTextureBeam != NULL)	
	{	// �e�N�X�`���̊J��
		g_pD3DTextureBeam->Release();
		g_pD3DTextureBeam = NULL;
	}	
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateBeam(void)
{
	BEAM *beam = beamWk;				// Beam�̃|�C���^�[��������
	for (int i = 0; i < BEAM_MAX; i++, beam++)
	{
		if (beam->use == true)			// ���g�p��Ԃ�Beam��������
		{
			// Beam�̈ړ�����
			beam->pos.x -= BEAM_SPEED * (sinf(beam->BaseAngle + beam->rot.z) - cosf(beam->BaseAngle + beam->rot.z));
			beam->pos.y += BEAM_SPEED * (cosf(beam->BaseAngle + beam->rot.z) + sinf(beam->BaseAngle + beam->rot.z));
			// ��ʊO�܂Ői�񂾁H
			if (beam->pos.y < -TEXTURE_BEAM_SIZE_Y || beam->pos.y > SCREEN_HEIGHT
			 || beam->pos.x < -TEXTURE_BEAM_SIZE_X || beam->pos.x > SCREEN_WIDTH)
			{
				beam->use = false;
			}
			SetVertexBeam(i);
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawBeam(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BEAM *beam = beamWk;				// Beam�̃|�C���^�[��������
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < BEAM_MAX; i++, beam++)
	{
		if (beam->use)					// �g�p���Ă����ԂȂ�X�V����
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, beam->Texture);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BEAM, beam->vertexWk, sizeof(VERTEX_2D));
		}
	}
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBeam(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BEAM *beam = &beamWk[no];
	// ���_���W�̐ݒ�
	SetVertexBeam(no);
	// rhw�̐ݒ�
	beam->vertexWk[0].rhw =
	beam->vertexWk[1].rhw =
	beam->vertexWk[2].rhw =
	beam->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	beam->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	beam->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	beam->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	beam->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	beam->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	beam->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BEAM, 0.0f);
	beam->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BEAM);
	beam->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_BEAM, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BEAM);
	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBeam(int no)
{
	BEAM *beam = &beamWk[no];			// Beam�̃|�C���^�[��������

	// ���_���W�̐ݒ�	
	beam->vertexWk[3].vtx = D3DXVECTOR3(beam->pos.x - cosf(beam->BaseAngle + beam->rot.z) * beam->Radius - GetPlayer(0)->pos.x / 4.0f,
										beam->pos.y - sinf(beam->BaseAngle + beam->rot.z) * beam->Radius,
										0);
	beam->vertexWk[2].vtx = D3DXVECTOR3(beam->pos.x + cosf(beam->BaseAngle - beam->rot.z) * beam->Radius - GetPlayer(0)->pos.x / 4.0f,
										beam->pos.y - sinf(beam->BaseAngle - beam->rot.z) *beam->Radius,
										0);
	beam->vertexWk[1].vtx = D3DXVECTOR3(beam->pos.x - cosf(beam->BaseAngle - beam->rot.z) * beam->Radius - GetPlayer(0)->pos.x / 4.0f,
										beam->pos.y + sinf(beam->BaseAngle - beam->rot.z) * beam->Radius,
										0);
	beam->vertexWk[0].vtx = D3DXVECTOR3(beam->pos.x + cosf(beam->BaseAngle + beam->rot.z) * beam->Radius - GetPlayer(0)->pos.x / 4.0f,
										beam->pos.y + sinf(beam->BaseAngle + beam->rot.z) * beam->Radius,
										0);
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureBeam(int no, int cntPattern)
{
	BEAM *beam = &beamWk[no];			// Beam�̃|�C���^�[��������

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_BEAM;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_BEAM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BEAM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BEAM;
	// ���_���W�̐ݒ�
	beam->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	beam->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	beam->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	beam->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
//=============================================================================
// Beam�̔��ːݒ�
//=============================================================================
void SetBeam(D3DXVECTOR3 pos, float rot)
{
	BEAM *beam = &beamWk[0];			// Beam�̃|�C���^�[��������
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < BEAM_MAX; i++, beam++)
	{
		if (beam->use == false)			// ���g�p��Ԃ�Beam��������
		{
			beam->use = true;				// �g�p��Ԃ֕ύX����
			beam->pos = pos;				// ���W���Z�b�g
			beam->rot.z = rot;			
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}
/*******************************************************************************
�֐���:	beam *GetBeamAdr( int pno )
����:	int pno : beam�ԍ�
�߂�l:	beam *
����:	beam�̃A�h���X���擾����
*******************************************************************************/
BEAM *GetBeam(int pno)
{
	return &beamWk[pno];
}