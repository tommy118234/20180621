/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	�^�C�g����ʏ��� [title.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _TITLE_H_
#define _TITLE_H_
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	TEXTURE_TITLE		_T("data/TEXTURE/bg000.jpg")			// �ǂݍ��ރe�N�X�`���t�@�C����
//#define	TEXTURE_TITLE_LOGO	_T("data/TEXTURE/title_logo.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE		_T("data/BG/moon.png")				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_LOGO	_T("data/TEXTURE/title_moon.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TITLE_POS_X			(0)										// �^�C�g���̕\���ʒu
#define	TITLE_POS_Y			(0)										// �^�C�g���̕\���ʒu
#define	TITLE_SIZE_X		(800)									// �^�C�g���̕�
#define	TITLE_SIZE_Y		(600)									// �^�C�g���̍���
#define	TITLELOGO_POS_X		(200)									// �^�C�g�����S�̕\���ʒu
#define	TITLELOGO_POS_Y		(50)									// �^�C�g�����S�̕\���ʒu
#define	TITLELOGO_SIZE_X	(480/2)									// �^�C�g�����S�̕�
#define	TITLELOGO_SIZE_Y	(246/2)//80)									// �^�C�g�����S�̍���
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
#endif