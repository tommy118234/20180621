/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������: ���U���g��ʏ��� [result.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#include "main.h"
#include "result.h"
#include "input.h"
#include "score.h"

#include "enemy.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo = NULL;	// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkResult[NUM_VERTEX];			// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResultLogo[NUM_VERTEX];		// ���_���i�[���[�N



ID3DXFont*				font3 = NULL;                // �����̃|�C���^
bool					good_end = false;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RESULT,				// �t�@�C���̖��O
		&g_pD3DTextureResult);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RESULT_LOGO,		// �t�@�C���̖��O
		&g_pD3DTextureResultLogo);	// �ǂݍ��ރ������[
	
	// ���_���̍쐬
	MakeVertexResult();
	// �����̓ǂݍ���
	D3DXCreateFont(pDevice,     //D3D Device
		30,               //Font height
		0,                //Font width
		FW_NORMAL,        //Font Weight
		1,                //MipLevels
		false,            //Italic
		DEFAULT_CHARSET,  //CharSet
		OUT_DEFAULT_PRECIS, //OutputPrecision
		ANTIALIASED_QUALITY, //Quality
		DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
		"HGS�s����",          //pFacename,
		&font3);         //ppFont	

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	if(g_pD3DTextureResult != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if(g_pD3DTextureResultLogo != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultLogo->Release();
		g_pD3DTextureResultLogo = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	if (GetScore() <= 1 && GetEnemy(0)->status.HP < 4500) {

		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESULT2,				// �t�@�C���̖��O
			&g_pD3DTextureResult);		// �ǂݍ��ރ������[

		SetBGM(10);
		InitScore(1);

		InitEnemy(1);
		//Good End
		good_end = true;
	}
	if(GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���

		good_end = false;

		SetStage( STAGE_TITLE );
		SetBGM(1);
		InitScore(1);		// �X�R�A�̍ď�����	
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetStage(STAGE_TITLE);
		SetBGM(1);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResult, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResultLogo);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultLogo, sizeof(VERTEX_2D));

	// �����̕`��		
	if (good_end) {
		const char* strings[] = { "�c�����̍����A���J�̐Ԍ��ɖ߂�\n �����[�A�N(������)���B�B�B\nGOOD END?","����FTSUI\n���y:Music is VFR, ������" };
		D3DCOLOR colors[] = { D3DCOLOR_ARGB(255, 120, 20, 70), D3DCOLOR_ARGB(255, 237, 200, 50) };
		RECT r = { 0  , SCREEN_HEIGHT / 4 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point
		RECT r2 = { 0 , 2 * SCREEN_HEIGHT / 3 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point	
		font3->DrawText(NULL, strings[0], -1, &r, DT_CENTER | DT_VCENTER, colors[1]);
		font3->DrawText(NULL, strings[1], -1, &r2, DT_CENTER | DT_VCENTER, colors[1]);
	}
	else {
		const char* strings[] = { "�n�܂�, �܂��I���Ȃ�����\n TRY AGAIN?","����FTSUI\n���y:Music is VFR,	������" };
		D3DCOLOR colors[] = { D3DCOLOR_ARGB(255, 120, 20, 70), D3DCOLOR_ARGB(255, 237, 200, 50) };
		RECT r = { 0  , SCREEN_HEIGHT / 4 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point
		RECT r2 = { 0 , 2 * SCREEN_HEIGHT / 3 ,SCREEN_WIDTH,SCREEN_HEIGHT }; // starting point	
		font3->DrawText(NULL, strings[0], -1, &r, DT_CENTER | DT_VCENTER, colors[0]);
		font3->DrawText(NULL, strings[1], -1, &r2, DT_CENTER | DT_VCENTER, colors[1]);
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(void)
{	
	// ���_���W�̐ݒ�
	g_vertexWkResult[0].vtx = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	g_vertexWkResult[1].vtx = D3DXVECTOR3(RESULT_POS_X + RESULT_SIZE_X, RESULT_POS_Y, 0.0f);
	g_vertexWkResult[2].vtx = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y + RESULT_SIZE_Y, 0.0f);
	g_vertexWkResult[3].vtx = D3DXVECTOR3(RESULT_POS_X + RESULT_SIZE_X, RESULT_POS_Y + RESULT_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResult[0].rhw =
	g_vertexWkResult[1].rhw =
	g_vertexWkResult[2].rhw =
	g_vertexWkResult[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResult[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResult[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkResultLogo[0].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultLogo[1].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultLogo[2].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
	g_vertexWkResultLogo[3].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResultLogo[0].rhw =
	g_vertexWkResultLogo[1].rhw =
	g_vertexWkResultLogo[2].rhw =
	g_vertexWkResultLogo[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResultLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResultLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}
