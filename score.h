//=============================================================================
//
// �|���S������ [score.h]
// Author : 
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

// �}�N����`
#define TEXTURE_GAME_SAMPLE003 _T("data/TEXTURE/score.png")	// �T���v���p�摜
#define TEXTURE_SAMPLE00_SIZE_X4	(30) // �e�N�X�`���T�C�Y
#define TEXTURE_SAMPLE00_SIZE_Y4	(50) // ����

#define TEXTURE_PATTERN_DIVIDE_X4	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y4	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM4			(TEXTURE_PATTERN_DIVIDE_X4 * TEXTURE_PATTERN_DIVIDE_Y4)	// �A�j���[�V�����p�^�[����
/*******************************************************************************
* �\���̒�`
********************************************************************************/
typedef struct					// �L�����N�^�[�̊�{�\����
{
	int						value;
	D3DXVECTOR3				g_posScore;						// �|���S���̈ړ���
	int						g_nPatternAnim3;				// �A�j���[�V�����p�^�[���i���o�[
	LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;		// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				g_vertexWk3[NUM_VERTEX];		// ���_���i�[
}SCORE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
SCORE *GetScore(int pno);	// �v���C���[��(�A�h���X)���擾


#endif
