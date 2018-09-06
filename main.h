/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	Main���� [main.h]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~

#include "windows.h"
#include "d3dx9.h"
#include <tchar.h>
#include <stdio.h>

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

#define SCREEN_WIDTH	(640)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(480)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(4)					// ���_��
#define	NUM_POLYGON		(2)					// �|���S����

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FPS_RATE			(30)						// FPS
#define SAFE_RELEASE(ptr)	{if(ptr){(ptr)->Release();(ptr) = NULL;}}

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct {
	char name[32];
	int HP;
	int MP;
	int ATK;
	int DEF;
	int LUCK;
	int EXP;
	int LV;
}CHARACTER;

// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
}	VERTEX_2D;

// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	float x, y, z, rhw;
	D3DCOLOR color;
	static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
}	VERTEX_2D_DIF;

// ��ʑJ�ڒ萔
enum E_STAGE
{
	STAGE_TITLE,
	STAGE_TUTOR,
	STAGE_GAME,
	STAGE_GAME_END,
	STAGE_RESULT
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);		// �f�o�C�X�擾�֐�

void SetStage(int stage);
void SetBGM(int no);
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2);
#endif