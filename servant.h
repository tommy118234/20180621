/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�G���� [SERVANT.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _SERVANT_H_
#define _SERVANT_H_
// �}�N����`
#define	NUM_SERVANT						(2)							// �|���S����
#define TEXTURE_GAME_SERVANT	_T("data/TEXTURE/servant001.png")	// �T���v���p�摜
#define TEXTURE_GAME_SERVANT2	_T("data/TEXTURE/servant002.png")	// �T���v���p�摜
#define TEXTURE_GAME_SERVANT3	_T("data/TEXTURE/servant003.png")	// �T���v���p�摜
#define TEXTURE_SERVANT_SIZE_X			(50)						// �e�N�X�`���T�C�Y
#define TEXTURE_SERVANT_SIZE_Y			(49)						// ����
#define TEXTURE_PATTERN_DIVIDE_X_SERVANT	(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_SERVANT	(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_SERVANT		(TEXTURE_PATTERN_DIVIDE_X_SERVANT*TEXTURE_PATTERN_DIVIDE_Y_SERVANT)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_SERVANT			(50)						// �A�j���[�V�����̐؂�ւ��J�E���g
#define SERVANT_MAX						(10)						// Servant�̍ő吔


#define SERVANT_ATK_A					(75)							// Servant�̍U����
#define SERVANT_COOLDOWN_A				(50)						// Servant�̃����[�h�X�s�[�h
#define SERVANT_ATK_B					(100)							// Servant�̍U����
#define SERVANT_COOLDOWN_B				(70)						// Servant�̃����[�h�X�s�[�h
#define SERVANT_ATK_C					(250)							// Servant�̍U����
#define SERVANT_COOLDOWN_C				(100)						// Servant�̃����[�h�X�s�[�h
/*******************************************************************************
* �\���̒�`
********************************************************************************/
typedef struct					// Servant�̍\����
{
	CHARACTER				status;			// ���ʃp�����[�^�[
	bool					use;					// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;					// �|���S���̈ړ���
	D3DXVECTOR3				rot;					// �|���S���̉�]��
	int						PatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						CountAnim;				// �A�j���[�V�����J�E���g
	LPDIRECT3DTEXTURE9		Texture = NULL;			// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	float					Radius;					// �|���S���̔��a
	float					BaseAngle;				// �|���S���̊p�x	
	int						bullet_cooldown;
	int						bullet_count;
	D3DXVECTOR3				abs_pos;				// �|���S���̈ړ���
} SERVANT;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitServant(int type);
void UninitServant(void);
void UpdateServant(void);
void DrawServant(void);
void SetServant(D3DXVECTOR3 pos, int type);
SERVANT *GetServant(int pno);	// �v���C���[��(�A�h���X)���擾
#endif