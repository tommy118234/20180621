/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	Servant���� [servant.cpp]
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
HRESULT MakeVertexServant(int no);
void SetTextureServant(int no, int cntPattern);
void SetVertexServant(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureServant = NULL;		// �e�N�X�`���ւ̃|���S��
SERVANT					ServantWk[SERVANT_MAX];			// Servant�\����
LPDIRECTSOUNDBUFFER8	SE3;							// SE�p�o�b�t�@

//=============================================================================
// ����������
//=============================================================================
HRESULT InitServant(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *servant = &ServantWk[0];		// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer(0);



	for (int i = 0; i < SERVANT_MAX; i++, servant++) {
		//SE3 = LoadSound(SE_01);
		// �e�N�X�`���[�̏��������s���H
		if (type == 0)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
				TEXTURE_GAME_SERVANT,			// �t�@�C���̖��O
				&D3DTextureServant);			// �ǂݍ��ރ������̃|�C���^
			SE3 = LoadSound(SE_00);
		}
		else if (type == 1) {
			UninitServant();
		}

		servant->use = false;									// ���g�p�i���˂���Ă��Ȃ��e�j
		servant->pos = D3DXVECTOR3(-1000.0f, 0.0f, 0.0f);		// ���W�f�[�^��������
		servant->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]�f�[�^��������
		servant->PatternAnim = 0;								// �A�j���p�^�[���ԍ��������_���ŏ�����
		servant->CountAnim = 0;									// �A�j���J�E���g��������

		servant->BaseAngle = atan2f(TEXTURE_SERVANT_SIZE_Y, TEXTURE_SERVANT_SIZE_X);
		D3DXVECTOR2 temp   = D3DXVECTOR2(TEXTURE_SERVANT_SIZE_X / 2, TEXTURE_SERVANT_SIZE_Y / 2);
		servant->Radius    = D3DXVec2Length(&temp);

		servant->Texture = D3DTextureServant;					// �e�N�X�`�����
		MakeVertexServant(i);									// ���_���̍쐬
	}
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitServant(void)
{
	if (D3DTextureServant != NULL)	//
	{	// �e�N�X�`���̊J��
		D3DTextureServant->Release();
		D3DTextureServant = NULL;
	}
	if (SE3 != NULL)
	{	// �e�N�X�`���̊J��
		SE3->Release();
		SE3 = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateServant(void)
{
	SERVANT *servant = ServantWk;			// Servant�̃|�C���^�[��������
	ENEMY	*enemy = GetEnemy(0);
	for (int i = 0; i < SERVANT_MAX; i++, servant++)
	{
		if (servant->use)			// ���g�p��Ԃ�Servant��������
		{
			// SERVANT�̈ړ�����
			if (enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2 > servant->pos.x + TEXTURE_SERVANT_SIZE_X / 2 &&  
				servant->rot.z < 1.57 - atan2f(servant->pos.y + TEXTURE_SERVANT_SIZE_Y / 2, BG00_SIZE_X/2 - servant->pos.x ))
				servant->rot.z += 0.05;			
			if (enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2 < servant->pos.x + TEXTURE_SERVANT_SIZE_X / 2 && 
				servant->rot.z > -(1.57 - atan2f(servant->pos.y + TEXTURE_SERVANT_SIZE_Y / 2,servant->pos.x -  BG00_SIZE_X/2 )))
				servant->rot.z -= 0.05;				
			if (servant->bullet_count < 1)
			{					
				D3DXVECTOR3 pos = servant->pos;							
				//pos.x -= GetPlayer(0)->pos.x / 4.0f;	
				//servant->pos.x = servant->abs_pos.x - GetPlayer(0)->pos.x / 4.0f;
				SetBullet(pos, servant->rot.z,servant->status.ATK);	
				servant->bullet_count = servant->bullet_cooldown;
			}
			else
			{
				servant->bullet_count--;
			}
			// ��ʊO�܂Ői�񂾁H
			if (servant->pos.y < -TEXTURE_SERVANT_SIZE_Y)
			{
				servant->use = FALSE;
				servant->pos.x = -100.0f;
			}
			SetVertexServant(i);
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawServant(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *servant = ServantWk;				// Servant�̃|�C���^�[��������
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < SERVANT_MAX; i++, servant++)
	{
		if (servant->use)						// �g�p���Ă����ԂȂ�X�V����
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, servant->Texture);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SERVANT, servant->vertexWk, sizeof(VERTEX_2D));
		}
	}
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexServant(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SERVANT *servant = &ServantWk[no];
	// ���_���W�̐ݒ�
	SetVertexServant(no);
	// rhw�̐ݒ�
	servant->vertexWk[0].rhw =
	servant->vertexWk[1].rhw =
	servant->vertexWk[2].rhw =
	servant->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	servant->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	servant->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	servant->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	servant->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	servant->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	servant->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT, 0.0f);
	servant->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT);
	servant->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT);
	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexServant(int no)
{
	SERVANT *servant = &ServantWk[no];			// Servant�̃|�C���^�[��������
	// ���_���W�̐ݒ�
	//servant->vertexWk[0].vtx = D3DXVECTOR3(servant->pos.x - TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
	//	servant->pos.y - TEXTURE_SERVANT_SIZE_Y,
	//	0);
	//servant->vertexWk[1].vtx = D3DXVECTOR3(servant->pos.x + TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
	//	servant->pos.y - TEXTURE_SERVANT_SIZE_Y,
	//	0);
	//servant->vertexWk[2].vtx = D3DXVECTOR3(servant->pos.x - TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
	//	servant->pos.y + TEXTURE_SERVANT_SIZE_Y,
	//	0);
	//servant->vertexWk[3].vtx = D3DXVECTOR3(servant->pos.x + TEXTURE_SERVANT_SIZE_X - GetPlayer(0)->pos.x / 4.0f,
	//	servant->pos.y + TEXTURE_SERVANT_SIZE_Y,
	//	0);
	servant->pos.x = servant->abs_pos.x - GetPlayer(0)->pos.x / 4.0f;
	// ���_���W�̐ݒ�
	servant->vertexWk[0].vtx.x = servant->pos.x - cosf(servant->BaseAngle + servant->rot.z) * servant->Radius;
	servant->vertexWk[0].vtx.y = servant->pos.y - sinf(servant->BaseAngle + servant->rot.z) * servant->Radius;
	servant->vertexWk[0].vtx.z = 0.0f;
	servant->vertexWk[1].vtx.x = servant->pos.x + cosf(servant->BaseAngle - servant->rot.z) * servant->Radius;
	servant->vertexWk[1].vtx.y = servant->pos.y - sinf(servant->BaseAngle - servant->rot.z) * servant->Radius;
	servant->vertexWk[1].vtx.z = 0.0f;
	servant->vertexWk[2].vtx.x = servant->pos.x - cosf(servant->BaseAngle - servant->rot.z) * servant->Radius;
	servant->vertexWk[2].vtx.y = servant->pos.y + sinf(servant->BaseAngle - servant->rot.z) * servant->Radius;
	servant->vertexWk[2].vtx.z = 0.0f;
	servant->vertexWk[3].vtx.x = servant->pos.x + cosf(servant->BaseAngle + servant->rot.z) * servant->Radius;
	servant->vertexWk[3].vtx.y = servant->pos.y + sinf(servant->BaseAngle + servant->rot.z) * servant->Radius;
	servant->vertexWk[3].vtx.z = 0.0f;
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureServant(int no, int cntPattern)
{
	SERVANT *servant = &ServantWk[no];			// Servant�̃|�C���^�[��������
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_SERVANT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SERVANT;
	// ���_���W�̐ݒ�
	servant->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	servant->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	servant->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	servant->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
//=============================================================================
// Servant�̔��ːݒ�
//=============================================================================
void SetServant(D3DXVECTOR3 pos, int type)
{
	SERVANT *servant = &ServantWk[0];				// Servant�̃|�C���^�[��������
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < SERVANT_MAX; i++, servant++)
	{
		if (!servant->use)							// ���g�p��Ԃ�Servant��������
		{
			servant->use = true;					// �g�p��Ԃ֕ύX����
			switch (type) 
			{
			case 1:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT,			// �t�@�C���̖��O
					&D3DTextureServant);			// �ǂݍ��ރ������̃|�C���^	
				servant->status.ATK = SERVANT_ATK_A;
				servant->bullet_cooldown = SERVANT_COOLDOWN_A;
				break;
			case 2:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT2,			// �t�@�C���̖��O
					&D3DTextureServant);			// �ǂݍ��ރ������̃|�C���^	
				servant->status.ATK = SERVANT_ATK_B;
				servant->bullet_cooldown = SERVANT_COOLDOWN_B;
				break;
			case 3:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT3,			// �t�@�C���̖��O
					&D3DTextureServant);			// �ǂݍ��ރ������̃|�C���^
				servant->status.ATK = SERVANT_ATK_C;
				servant->bullet_cooldown = SERVANT_COOLDOWN_C;
				break;		
			}
			// �������Đ�
			SE3 = LoadSound(18+type);			
			if (IsPlaying(SE3))
				SE3->SetCurrentPosition(0);
			else {
				PlaySound(SE3, E_DS8_FLAG_NONE);
			}
			servant->Texture = D3DTextureServant;	// �e�N�X�`�����			
			servant->pos = pos;						// ���W���Z�b�g		serva
			servant->abs_pos = pos;
			return;									// 1���Z�b�g�����̂ŏI������
		}
		if (!servant->use)							// ���g�p��Ԃ�Servant��������
		{
			servant->use = true;					// �g�p��Ԃ֕ύX����
			switch (type) 
			{
			case 1:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT,			// �t�@�C���̖��O
					&D3DTextureServant);			// �ǂݍ��ރ������̃|�C���^	
				servant->status.ATK = SERVANT_ATK_A;
				servant->bullet_cooldown = SERVANT_COOLDOWN_A;
				break;
			case 2:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT2,			// �t�@�C���̖��O
					&D3DTextureServant);			// �ǂݍ��ރ������̃|�C���^	
				servant->status.ATK = SERVANT_ATK_B;
				servant->bullet_cooldown = SERVANT_COOLDOWN_B;
				break;
			case 3:
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_SERVANT3,			// �t�@�C���̖��O
					&D3DTextureServant);			// �ǂݍ��ރ������̃|�C���^
				servant->status.ATK = SERVANT_ATK_C;
				servant->bullet_cooldown = SERVANT_COOLDOWN_C;
				break;		
			}
			// �������Đ�
			SE3 = LoadSound(18+type);			
			if (IsPlaying(SE3))
				SE3->SetCurrentPosition(0);
			else {
				PlaySound(SE3, E_DS8_FLAG_NONE);
			}
			servant->Texture = D3DTextureServant;	// �e�N�X�`�����			
			servant->pos = pos;						// ���W���Z�b�g		serva
			servant->abs_pos = pos;
			return;									// 1���Z�b�g�����̂ŏI������
		}
	}
}
/*******************************************************************************
�֐���:	SERVANT *GetSERVANTAdr( int pno )
����:	int pno : SERVANT�ԍ�
�߂�l:	SERVANT *
����:	SERVANT�̃A�h���X���擾����
*******************************************************************************/
SERVANT *GetServant(int pno)
{
	return &ServantWk[pno];
}