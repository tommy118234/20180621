//=============================================================================
//
// �|���S������ [bullet.h]
// Author : 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_


// �}�N����`
#define TEXTURE_GAME_BULLET _T("data/TEXTURE/bullet.png")	// �T���v���p�摜
#define TEXTURE_SAMPLE00_SIZE_X3	(22) // �e�N�X�`���T�C�Y
#define TEXTURE_SAMPLE00_SIZE_Y3	(60) // ����

#define TEXTURE_PATTERN_DIVIDE_X3	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y3	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM3			(TEXTURE_PATTERN_DIVIDE_X3*TEXTURE_PATTERN_DIVIDE_Y3)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION				(50)	// �A�j���[�V�����̐؂�ւ��J�E���g

/*******************************************************************************
* �\���̒�`
********************************************************************************/
typedef struct					// �L�����N�^�[�̊�{�\����
{
	CHARACTER	status;			// ���ʃp�����[�^�[

								//LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// �e�N�X�`���ւ̃|���S��
								//
								//VERTEX_2D				g_vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
	D3DXVECTOR3				g_posBullet;						// �|���S���̈ړ���
	D3DXVECTOR3				g_rotBullet;						// �|���S���̉�]��
	int						g_nCountAnim;						// �A�j���[�V�����J�E���g
	int						g_nPatternAnim;						// �A�j���[�V�����p�^�[���i���o�[

	float					g_fRadiusBullet;					// �|���S���̔��a
	float					g_fBaseAngleBullet;					// �|���S���̊p�x

	LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;		// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				g_vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	bool					use = FALSE;
} BULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
BULLET *GetBullet(int pno);	// �v���C���[��(�A�h���X)���擾


#endif
