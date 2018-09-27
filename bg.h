/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�w�i���� [bg.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _BG_H_
#define _BG_H_
// �}�N����`
#define TEXTURE_GAME_BG00		_T("data/BG/moon.png")			// �w�i�摜1
#define TEXTURE_GAME_BG01		_T("data/BG/story1.png")		// �w�i�摜2
#define TEXTURE_GAME_BG02		_T("data/BG/story2.png")		// �w�i�摜3
#define TEXTURE_GAME_BG03		_T("data/BG/battleGuide.png")	// �w�i�摜4
#define TEXTURE_GAME_BG04		_T("data/BG/battle_ice5.png")	// �w�i�摜5
#define TEXTURE_GAME_BG05		_T("data/BG/ice_castle.png")	// �w�i�摜6
#define TEXTURE_GAME_BG06		_T("data/BG/moon_red.png")		// �w�i�摜7
#define BG00_SIZE_X				(800)							// �e�N�X�`���T�C�Y
#define BG00_SIZE_Y				(600)							// ����
#define BG00_POS_X				(0)								// �|���S���̏����ʒuX(����)
#define BG00_POS_Y				(0)								// ����
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(int type);
void	UninitBG(void);
void	UpdateBG(void);
void	DrawBG(void);
void	SwitchBG(int type);
void	ChangeBG(void);
#endif
