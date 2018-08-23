/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�G���� [item.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _ITEM_H_
#define _ITEM_H_


// �}�N����`
#define	NUM_ITEM						(2)			// �|���S����

#define TEXTURE_GAME_ITEM				_T("data/TEXTURE/item1.png")	// �T���v���p�摜
#define TEXTURE_GAME_ITEM2				_T("data/TEXTURE/item2.png")	// �T���v���p�摜
#define TEXTURE_GAME_ITEM3				_T("data/TEXTURE/item3.png")	// �T���v���p�摜
#define TEXTURE_ITEM_SIZE_X			(100/2)		// �e�N�X�`���T�C�Y
#define TEXTURE_ITEM_SIZE_Y			(100/2)		// ����
#define TEXTURE_PATTERN_DIVIDE_X_ITEM	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iX)											// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_ITEM	(1)			// �A�j���p�^�[���̃e�N�X�`�����������iY)											// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_ITEM			(TEXTURE_PATTERN_DIVIDE_X_ITEM*TEXTURE_PATTERN_DIVIDE_Y_ITEM)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_ITEM			(50)			// �A�j���[�V�����̐؂�ւ��J�E���g
#define ITEM_MAX						(20)			// �A�C�e���̍ő吔
#define ITEM_SPEED					(6.0f)		// �A�C�e���̈ړ��X�s�[�h
/*******************************************************************************
* �\���̒�`
********************************************************************************/
typedef struct											// �A�C�e���̍\����
{
	bool					use;						// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;						// �|���S���̈ړ���
	D3DXVECTOR3				rot;						// �|���S���̉�]��
	int						PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int						CountAnim;					// �A�j���[�V�����J�E��
	LPDIRECT3DTEXTURE9		Texture;					// �e�N�X�`���ւ̃|���S��
	VERTEX_2D				vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	float					Radius;						// �|���S���̔��a
	float					BaseAngle = 0.0;			// �|���S���̊p�x
	int						type;
} ITEM;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(int type);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, int type);
ITEM *GetItem(int pno);									// �A�C�e����(�A�h���X)���擾
#endif