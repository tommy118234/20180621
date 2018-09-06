/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������: ���U���g��ʏ��� [result.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _RESULT_H_
#define _RESULT_H_
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT		_T	("data/BG/moon_red.png")				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT2		_T	("data/BG/moon_red2.png")				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT3		_T	("data/BG/moon_ice.png")				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_LOGO	_T	("data/TEXTURE/title_ice.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RESULT_POS_X			(0)										// �^�C�g���̕\���ʒu
#define	RESULT_POS_Y			(0)										// �^�C�g���̕\���ʒu
#define	RESULT_SIZE_X			(800)									// �^�C�g���̕�
#define	RESULT_SIZE_Y			(600)									// �^�C�g���̍���
#define	RESULTLOGO_POS_X		(200)									// �^�C�g�����S�̕\���ʒu
#define	RESULTLOGO_POS_Y		(50)									// �^�C�g�����S�̕\���ʒu
#define	RESULTLOGO_SIZE_X		(480/2)									// �^�C�g�����S�̕�
#define	RESULTLOGO_SIZE_Y		(246/2)//80)							// �^�C�g�����S�̍���
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
#endif
