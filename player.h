//=============================================================================
//
// �|���S������ [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_




#include "bullet.h"
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


#define TEXTURE_GAME_SAMPLE00	_T("data/TEXTURE/runningman001.png")	// �T���v���p�摜
#define TEXTURE_SAMPLE00_SIZE_X	(140) // �e�N�X�`���T�C�Y
#define TEXTURE_SAMPLE00_SIZE_Y	(200) // ����

#define TEXTURE_PATTERN_DIVIDE_X	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
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
	D3DXVECTOR3				g_posPlayer;						// �|���S���̈ړ���
	D3DXVECTOR3				g_rotPlayer;						// �|���S���̉�]��
	int						g_nCountAnim;						// �A�j���[�V�����J�E���g
	int						g_nPatternAnim;						// �A�j���[�V�����p�^�[���i���o�[

	float					g_fRadiusPlayer;					// �|���S���̔��a
	float					g_fBaseAnglePlayer;					// �|���S���̊p�x

	/*
	g_fBaseAnglePolygon = Atan2f(120,80)
	D3DXVECTOR2 temp= D3DXVECTOR2()
	*/

	int						direction = -1;
	int						bullet_num = 0;
	int						score = 0;
} PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(int pno);	// �v���C���[��(�A�h���X)���擾


#endif
