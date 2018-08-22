/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�G���� [SERVANT.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _SERVANT_H_
#define _SERVANT_H_


// �}�N����`
#define	NUM_SERVANT						(2)			// �|���S����

#define TEXTURE_GAME_SERVANT				_T("data/TEXTURE/servant001.png")	// �T���v���p�摜
#define TEXTURE_GAME_SERVANT2				_T("data/TEXTURE/servant002.png")	// �T���v���p�摜
#define TEXTURE_GAME_SERVANT3				_T("data/TEXTURE/servant003.png")	// �T���v���p�摜
#define TEXTURE_SERVANT_SIZE_X			(50)		// �e�N�X�`���T�C�Y
#define TEXTURE_SERVANT_SIZE_Y			(49)		// ����

#define TEXTURE_PATTERN_DIVIDE_X_SERVANT	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iX)											// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_SERVANT	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iY)											// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_SERVANT			(TEXTURE_PATTERN_DIVIDE_X_SERVANT*TEXTURE_PATTERN_DIVIDE_Y_SERVANT)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_SERVANT			(50)			// �A�j���[�V�����̐؂�ւ��J�E���g

#define SERVANT_MAX						(10)			// �o���b�g�̍ő吔

#define SERVANT_SPEED					(6.0f)		// �o���b�g�̈ړ��X�s�[�h

/*******************************************************************************
* �\���̒�`
********************************************************************************/

typedef struct					// �o���b�g�̍\����
{
	CHARACTER	status;			// ���ʃp�����[�^�[

	bool					use;					// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;					// �|���S���̈ړ���
	D3DXVECTOR3				rot;					// �|���S���̉�]��
	int						PatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						CountAnim;				// �A�j���[�V�����J�E���g

	LPDIRECT3DTEXTURE9		Texture = NULL;			// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N

	float					Radius;					// �|���S���̔��a
	float					BaseAngle = 0.2;				// �|���S���̊p�x

	int						direction = 1;
	int						gravity = 5;
	int						view_mode = 1;
	int                     ready = 0;

	int						bullet_cooldown = 50;

} SERVANT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSERVANT(int type);
void UninitSERVANT(void);
void UpdateSERVANT(void);
void DrawSERVANT(void);
void SetSERVANT(D3DXVECTOR3 pos, int type);
SERVANT *GetSERVANT(int pno);	// �v���C���[��(�A�h���X)���擾


#endif