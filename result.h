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
#define	TEXTURE_RESULT		_T("data/TEXTURE/bg002.jpg")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_LOGO	_T("data/TEXTURE/result_logo.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RESULT_POS_X			(0)							// �^�C�g���̕\���ʒu
#define	RESULT_POS_Y			(0)							// �^�C�g���̕\���ʒu
#define	RESULT_SIZE_X			(800)						// �^�C�g���̕�
#define	RESULT_SIZE_Y			(600)						// �^�C�g���̍���
#define	RESULTLOGO_POS_X		(160)						// �^�C�g�����S�̕\���ʒu
#define	RESULTLOGO_POS_Y		(200)						// �^�C�g�����S�̕\���ʒu
#define	RESULTLOGO_SIZE_X		(480)						// �^�C�g�����S�̕�
#define	RESULTLOGO_SIZE_Y		(80)						// �^�C�g�����S�̍���
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
