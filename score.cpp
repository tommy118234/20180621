//=============================================================================
//
// �|���S������ [score.cpp]
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexScore(void);
void SetVertexScore(void);
void SetTextureScore(int cntPattern);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
SCORE	score[SCORE_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(void)
{	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);
	for (int i = 0; i < SCORE_MAX; i++) {		
		SCORE *score = GetScore(i);
		score->g_posScore = D3DXVECTOR3(SCREEN_WIDTH - (i+1) * TEXTURE_SAMPLE00_SIZE_X4, 0.0f, 0.0f);
		// ���_���̍쐬
		MakeVertexScore();
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
		TEXTURE_GAME_SAMPLE003,					// �t�@�C���̖��O
		&score->g_pD3DTextureScore);			// �ǂݍ��ރ������̃|�C���^
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	if (score->g_pD3DTextureScore != NULL)	//
	{	// �e�N�X�`���̊J��
		score->g_pD3DTextureScore->Release();
		score->g_pD3DTextureScore = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	PLAYER *player = GetPlayer(0);
	SCORE * score = GetScore(0);
	for (int i = 0; i < SCORE_MAX; i++) {
		score = GetScore(i);
		int k = player->score;
		switch (i) {		
		case 1:
			k = player->score/10;
			break;
		case 2:
			k = player->score / 100;
			break;
		case 3:
			k = player->score / 1000;
			break;
		case 4:
			k = player->score / 10000;
			break;			
		}
		score->g_nPatternAnim3 = k % ANIM_PATTERN_NUM4;
	}	
	MakeVertexScore();
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET *bullet = GetBullet(0);
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < SCORE_MAX; i++) {
		SCORE *score = GetScore(i);	
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, score->g_pD3DTextureScore);
		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, score->g_vertexWk3, sizeof(VERTEX_2D));
	}
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexScore(void)
{
	for (int i = 0; i < SCORE_MAX; i++) {
		SCORE *score = GetScore(i);
		// ���_���W�̐ݒ�
		SetVertexScore();
		// rhw�̐ݒ�
		score->g_vertexWk3[0].rhw =
		score->g_vertexWk3[1].rhw =
		score->g_vertexWk3[2].rhw =
		score->g_vertexWk3[3].rhw = 1.0f;
		// ���ˌ��̐ݒ�
		score->g_vertexWk3[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		score->g_vertexWk3[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		score->g_vertexWk3[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		score->g_vertexWk3[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		// �e�N�X�`�����W�̐ݒ�
		SetTextureScore(i);
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexScore(void)
{
	for (int i = 0; i < SCORE_MAX; i++) {
		SCORE *score = GetScore(i);
		// ���_���W�̐ݒ�
		score->g_vertexWk3[0].vtx = D3DXVECTOR3(score->g_posScore.x, score->g_posScore.y, score->g_posScore.z);
		score->g_vertexWk3[1].vtx = D3DXVECTOR3(score->g_posScore.x + TEXTURE_SAMPLE00_SIZE_X4, score->g_posScore.y, score->g_posScore.z);
		score->g_vertexWk3[2].vtx = D3DXVECTOR3(score->g_posScore.x, score->g_posScore.y + TEXTURE_SAMPLE00_SIZE_Y4, score->g_posScore.z);
		score->g_vertexWk3[3].vtx = D3DXVECTOR3(score->g_posScore.x + TEXTURE_SAMPLE00_SIZE_X4, score->g_posScore.y + TEXTURE_SAMPLE00_SIZE_Y4, score->g_posScore.z);
	}

}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureScore(int cntPattern)
{	
	SCORE *score = GetScore(cntPattern);
	// �e�N�X�`�����W�̐ݒ�
	score->g_vertexWk3[0].tex = D3DXVECTOR2((float)(score->g_nPatternAnim3 % TEXTURE_PATTERN_DIVIDE_X4) / TEXTURE_PATTERN_DIVIDE_X4, (float)(score->g_nPatternAnim3 / TEXTURE_PATTERN_DIVIDE_X4) / TEXTURE_PATTERN_DIVIDE_Y4);
	score->g_vertexWk3[1].tex = D3DXVECTOR2((float)((score->g_nPatternAnim3 % TEXTURE_PATTERN_DIVIDE_X4) + 1) /TEXTURE_PATTERN_DIVIDE_X4, (float)(score->g_nPatternAnim3 / TEXTURE_PATTERN_DIVIDE_X4) / TEXTURE_PATTERN_DIVIDE_Y4);
	score->g_vertexWk3[2].tex = D3DXVECTOR2((float)(score->g_nPatternAnim3 % TEXTURE_PATTERN_DIVIDE_X4) / TEXTURE_PATTERN_DIVIDE_X4, (float)((score->g_nPatternAnim3 / TEXTURE_PATTERN_DIVIDE_X4) + 1) / TEXTURE_PATTERN_DIVIDE_Y4);
	score->g_vertexWk3[3].tex = D3DXVECTOR2((float)((score->g_nPatternAnim3 % TEXTURE_PATTERN_DIVIDE_X4) + 1) / TEXTURE_PATTERN_DIVIDE_X4, (float)((score->g_nPatternAnim3 / TEXTURE_PATTERN_DIVIDE_X4) + 1) /TEXTURE_PATTERN_DIVIDE_Y4);
}
/*******************************************************************************
�֐���:	SCORE *GetMapAdr( int pno )
����:	int pno : �G�l�~�[�ԍ�
�߂�l:	SCORE *
����:	�G�l�~�[�̃A�h���X���擾����
*******************************************************************************/
SCORE *GetScore(int pno)
{
	return &score[pno];
}