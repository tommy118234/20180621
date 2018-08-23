/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	Beam���� [bullet.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _BEAM_H_
#define _BEAM_H_

// �}�N����`
#define	NUM_BEAM						(2)			// �|���S����
#define TEXTURE_GAME_BEAM				_T("data/TEXTURE/beam.png")	// �T���v���p�摜
#define TEXTURE_BEAM_SIZE_X				(20)		// �e�N�X�`���T�C�Y
#define TEXTURE_BEAM_SIZE_Y				(20)		// ����
#define TEXTURE_PATTERN_DIVIDE_X_BEAM	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_BEAM	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_BEAM			(TEXTURE_PATTERN_DIVIDE_X_BEAM*TEXTURE_PATTERN_DIVIDE_Y_BEAM)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_BEAM				(50)		// �A�j���[�V�����̐؂�ւ��J�E���g

#define BEAM_MAX						(50)		// Beam�̍ő吔
#define BEAM_SPEED						(5.0f)		// Beam�̈ړ��X�s�[�h
/*******************************************************************************
* �\���̒�`
********************************************************************************/
typedef struct										// Beam�̍\����
{
	bool					use;					// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;					// �|���S���̈ړ���
	D3DXVECTOR3				rot;					// �|���S���̉�]��
	int						PatternAnim;			// �A�j���[�V�����p�^�[���i���o�[
	int						CountAnim;				// �A�j���[�V�����J�E���g
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	float					Radius;					// �|���S���̔��a
	float					BaseAngle;				// �|���S���̊p�x	
	int						atk;
} BEAM;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBeam(int type);
void	UninitBeam(void);
void	UpdateBeam(void);
void	DrawBeam(void);
void	SetBeam(D3DXVECTOR3 pos, float rot, int ATK);
BEAM	*GetBeam(int pno);							// Beam��(�A�h���X)���擾
#endif