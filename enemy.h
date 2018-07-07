//=============================================================================
//
// �|���S������ [enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_


// �}�N����`
//#define TEXTURE_GAME_SAMPLE00	_T("data/TEXTURE/runningman000.png")	// �T���v���p�摜
//#define TEXTURE_SAMPLE00_SIZE_X	(200) // �e�N�X�`���T�C�Y
//#define TEXTURE_SAMPLE00_SIZE_Y	(200) // ����
//
//
//#define TEXTURE_PATTERN_DIVIDE_X	(8)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
//#define TEXTURE_PATTERN_DIVIDE_Y	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
//#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
//#define TIME_ANIMATION				(100)	// �A�j���[�V�����̐؂�ւ��J�E���g


#define TEXTURE_GAME_SAMPLE002 _T("data/TEXTURE/runningman003.png")	// �T���v���p�摜
#define TEXTURE_SAMPLE00_SIZE_X2	(140) // �e�N�X�`���T�C�Y
#define TEXTURE_SAMPLE00_SIZE_Y2	(200) // ����

#define TEXTURE_PATTERN_DIVIDE_X2	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y2	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM2			(TEXTURE_PATTERN_DIVIDE_X2*TEXTURE_PATTERN_DIVIDE_Y2)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION2				(50)	// �A�j���[�V�����̐؂�ւ��J�E���g

/*******************************************************************************
* �\���̒�`
********************************************************************************/
typedef struct					// �L�����N�^�[�̊�{�\����
{
	CHARACTER	status;			// ���ʃp�����[�^�[

								//LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// �e�N�X�`���ւ̃|���S��
								//
								//VERTEX_2D				g_vertexWk[NUM_VERTEX];				// ���_���i�[���[�N
	D3DXVECTOR3				g_posEnemy;						// �|���S���̈ړ���
	D3DXVECTOR3				g_rotEnemy;						// �|���S���̉�]��
	int						g_nCountAnim2;						// �A�j���[�V�����J�E���g
	int						g_nPatternAnim2;						// �A�j���[�V�����p�^�[���i���o�[

	float					g_fRadiusEnemy;					// �|���S���̔��a
	float					g_fBaseAngleEnemy;					// �|���S���̊p�x
	int						direction2 = -1;
} ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int pno);	// �v���C���[��(�A�h���X)���擾


#endif
