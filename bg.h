/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�w�i���� [bg.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _BG_H_
#define _BG_H_

// �}�N����`
//#define TEXTURE_GAME_BG00		_T("data/TEXTURE/earth.png")	// �T���v���p�摜
#define TEXTURE_GAME_BG00		_T("data/BG/moon.png")	// �T���v���p�摜
#define BG00_SIZE_X			(800)	// �e�N�X�`���T�C�Y
#define BG00_SIZE_Y			(600)	// ����
#define BG00_POS_X			(0)		// �|���S���̏����ʒuX(����)
#define BG00_POS_Y			(0)		// ����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(int type);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif
