/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�G�l�~�[���� [enemy.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/

#ifndef _ENEMY_H_
#define _ENEMY_H_


// �}�N����`
#define	NUM_ENEMY					(2)			// �|���S����

#define TEXTURE_GAME_ENEMY			_T("data/TEXTURE/runningman003.png")	// �T���v���p�摜
#define TEXTURE_ENEMY_SIZE_X		(140)		// �e�N�X�`���T�C�Y
#define TEXTURE_ENEMY_SIZE_Y		(200)		// ����

#define TEXTURE_PATTERN_DIVIDE_X_ENEMY	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_ENEMY	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_ENEMY			(TEXTURE_PATTERN_DIVIDE_X_ENEMY*TEXTURE_PATTERN_DIVIDE_Y_ENEMY)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_ENEMY			(50)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define ENEMY_MAX						(4)		// �G�̍ő吔

/*******************************************************************************
* �\���̒�`
********************************************************************************/
typedef struct					// �G�l�~�[�\����
{
	CHARACTER	status;			// ���ʃp�����[�^�[

	bool					use = TRUE;						// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;							// �|���S���̈ړ���
	D3DXVECTOR3				rot;							// �|���S���̉�]��	
	int						PatternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	int						CountAnim;						// �A�j���[�V�����J�E���g

	LPDIRECT3DTEXTURE9		Texture;						// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];			// ���_���i�[���[�N

	float					Radius;					// �|���S���̔��a
	float					BaseAngle = 0.2;		// �|���S���̊p�x

} ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int pno);


#endif
