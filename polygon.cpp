//=============================================================================
//
// �|���S������ [polygon.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPolygon(void);
void SetVertexPolygon(void);
void SetTexturePolygon( int cntPattern );	//

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePolygon = NULL;		// �e�N�X�`���ւ̃|���S��

VERTEX_2D				g_vertexWk[NUM_VERTEX];				// ���_���i�[���[�N

D3DXVECTOR3				g_posPolygon;						// �|���S���̈ړ���
D3DXVECTOR3				g_rotPolygon;						// �|���S���̉�]��
int						g_nCountAnim;						// �A�j���[�V�����J�E���g
int						g_nPatternAnim;						// �A�j���[�V�����p�^�[���i���o�[

int						direction = -1;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posPolygon = D3DXVECTOR3(SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X/2, SCREEN_CENTER_Y - TEXTURE_SAMPLE00_SIZE_Y/2, 0.0f);
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nCountAnim = 0;
	g_nPatternAnim = 0;

	// ���_���̍쐬
	MakeVertexPolygon();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice,		// �f�o�C�X�̃|�C���^
		TEXTURE_GAME_SAMPLE00,				// �t�@�C���̖��O
		&g_pD3DTexturePolygon );			// �ǂݍ��ރ������̃|�C���^

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPolygon(void)
{
	if( g_pD3DTexturePolygon != NULL )	//
	{	// �e�N�X�`���̊J��
		g_pD3DTexturePolygon->Release();
		g_pD3DTexturePolygon = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePolygon(void)
{
	// �A�j���[�V����
	g_nCountAnim = (g_nCountAnim +1) % ANIM_PATTERN_NUM;
	g_nPatternAnim = (g_nPatternAnim + 1) % ANIM_PATTERN_NUM;	



	g_posPolygon.x = g_posPolygon.x + direction *  (float)  SCREEN_WIDTH / TIME_ANIMATION  ;

	//g_posPolygon.x -= (float)SCREEN_WIDTH / TIME_ANIMATION;
	if (g_posPolygon.x < 0 || g_posPolygon.x > SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X / 2) {
		direction = -direction;
		//g_posPolygon.x = SCREEN_WIDTH - TEXTURE_SAMPLE00_SIZE_X / 2;
	}
	Sleep(66);
	//g_posPolygon.x = ((int)g_posPolygon.x + 3) % SCREEN_WIDTH;	
	//g_posPolygon.y =((int) g_posPolygon.y + 3) % SCREEN_HIEGHT;
	//g_posPolygon.z = ((int)g_posPolygon.z + 3) % SCREEN_WIDTH;		
	MakeVertexPolygon();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0, g_pD3DTexturePolygon );

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexPolygon();
	// rhw�̐ݒ�
	g_vertexWk[0].rhw =
	g_vertexWk[1].rhw =
	g_vertexWk[2].rhw =
	g_vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	g_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	SetTexturePolygon(g_nPatternAnim);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPolygon(void)
{
	// ���_���W�̐ݒ�
	g_vertexWk[0].vtx = D3DXVECTOR3(g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	g_vertexWk[1].vtx = D3DXVECTOR3(g_posPolygon.x + TEXTURE_SAMPLE00_SIZE_X, g_posPolygon.y, g_posPolygon.z);
	g_vertexWk[2].vtx = D3DXVECTOR3(g_posPolygon.x, g_posPolygon.y + TEXTURE_SAMPLE00_SIZE_Y, g_posPolygon.z);
	g_vertexWk[3].vtx = D3DXVECTOR3(g_posPolygon.x + TEXTURE_SAMPLE00_SIZE_X, g_posPolygon.y + TEXTURE_SAMPLE00_SIZE_Y, g_posPolygon.z);
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePolygon( int cntPattern )
{
	// �e�N�X�`�����W�̐ݒ�
	g_vertexWk[0].tex = D3DXVECTOR2(cntPattern * 1 / (float)TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	g_vertexWk[1].tex = D3DXVECTOR2((cntPattern + 1) * 1 / (float)TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	g_vertexWk[2].tex = D3DXVECTOR2(cntPattern * 1 / (float)TEXTURE_PATTERN_DIVIDE_X, 1/(float) TEXTURE_PATTERN_DIVIDE_Y);
	g_vertexWk[3].tex = D3DXVECTOR2((cntPattern + 1) * 1 / (float)TEXTURE_PATTERN_DIVIDE_X, 1 / (float)TEXTURE_PATTERN_DIVIDE_Y);
}
