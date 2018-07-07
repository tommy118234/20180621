//=============================================================================
//
// �|���S������ [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_


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

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);


#endif
