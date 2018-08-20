/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������: �v���[���[���� [player.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "bullet.h"
// �}�N����`
#define	NUM_PLAYER		(2)					// �|���S����

#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/runningman001.png")	// �T���v���p�摜
#define TEXTURE_PLAYER_SIZE_X	(140/2) // �e�N�X�`���T�C�Y
#define TEXTURE_PLAYER_SIZE_Y	(200/2) // ���� 

#define TEXTURE_PATTERN_DIVIDE_X	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����


#define TEXTURE_GAME_PLAYER2	_T("data/TEXTURE/runningman000.png")	// �T���v���p�摜
#define TEXTURE_PLAYER_SIZE_X2	(100/2) // �e�N�X�`���T�C�Y
#define TEXTURE_PLAYER_SIZE_Y2	(200/2) // ���� 
#define TEXTURE_PATTERN_DIVIDE_X2	(8)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y2	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)



#define TIME_ANIMATION				(50)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define PLAYER_MAX					(1) // �v���C���[�̍ő吔
/*******************************************************************************
* �\���̒�`
********************************************************************************/
typedef struct					// �L�����N�^�[�̊�{�\����
{
	CHARACTER	status;			// ���ʃp�����[�^�[
	
	bool					use;						// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;				// �|���S���̈ړ���
	D3DXVECTOR3				rot;				// �|���S���̉�]��
	int						PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int						CountAnim;				// �A�j���[�V�����J�E���g
	
	LPDIRECT3DTEXTURE9		Texture = NULL;	// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	float					g_fRadiusPlayer;			// �|���S���̔��a
	float					g_fBaseAnglePlayer;			// �|���S���̊p�x
	
	int						direction = -1;
	int						bullet_num = 0;
	int						score = 0;
	int						gravity = 5;
	int						view_mode = 1;
	int                     ready = 0;
} PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void ChangePlayer(void);
PLAYER *GetPlayer(int pno);	// �v���C���[��(�A�h���X)���擾


#endif
