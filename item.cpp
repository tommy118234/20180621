/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	Item���� [item.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "item.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexItem(int no);
void SetTextureItem(int no, int cntPattern);
void SetVertexItem(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureItem = NULL;		// �e�N�X�`���ւ̃|���S��
ITEM					itemWk[ITEM_MAX];			// Item�\����


//=============================================================================
// ����������
//=============================================================================
HRESULT InitItem(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[0];		// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer(0);



	for (int i = 0; i < ITEM_MAX; i++, item++) {
		// �e�N�X�`���[�̏��������s���H
		if (type == 0)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
				TEXTURE_GAME_ITEM,			// �t�@�C���̖��O
				&g_pD3DTextureItem);			// �ǂݍ��ރ������̃|�C���^			
		}
		else if (type == 1) {
			UninitItem;
		}

		item->use = FALSE;									// ���g�p�i���˂���Ă��Ȃ��e�j
		item->pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);			// ���W�f�[�^��������
		item->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]�f�[�^��������
		item->PatternAnim = 0;								// �A�j���p�^�[���ԍ��������_���ŏ�����
		item->CountAnim = 0;									// �A�j���J�E���g��������


		item->BaseAngle = atan2f(TEXTURE_ITEM_SIZE_Y, TEXTURE_ITEM_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_ITEM_SIZE_X , TEXTURE_ITEM_SIZE_Y);
		item->Radius = D3DXVec2Length(&temp);

		item->Texture = g_pD3DTextureItem;					// �e�N�X�`�����
		MakeVertexItem(i);									// ���_���̍쐬
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	if (g_pD3DTextureItem != NULL)	//
	{	// �e�N�X�`���̊J��
		g_pD3DTextureItem->Release();
		g_pD3DTextureItem = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{

	ITEM *item = itemWk;				// Item�̃|�C���^�[��������

	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->use == true)			// ���g�p��Ԃ�Item��������
		{			
			// ��ʊO�܂Ői�񂾁H
			if (item->pos.y < -TEXTURE_ITEM_SIZE_Y || item->pos.y > SCREEN_HEIGHT
			|| item->pos.x < -TEXTURE_ITEM_SIZE_X || item->pos.x > SCREEN_WIDTH)
			{
				item->use = false;
				//item->Texture = NULL;					// �e�N�X�`�����
				//item->Texture = g_pD3DTextureItem;	// �e�N�X�`�����				
				//item->pos.x = -100.0f;
				//item->pos = D3DXVECTOR3(player->pos.x + TEXTURE_ITEM_SIZE_X / 2, player->pos.y, 0.0f);
			}
			SetVertexItem(i);
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = itemWk;				// Item�̃|�C���^�[��������

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->use)					// �g�p���Ă����ԂȂ�X�V����
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, item->Texture);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_ITEM, item->vertexWk, sizeof(VERTEX_2D));
		}
	}
}
//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexItem(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[no];

	// ���_���W�̐ݒ�
	SetVertexItem(no);
	// rhw�̐ݒ�
	item->vertexWk[0].rhw =
		item->vertexWk[1].rhw =
		item->vertexWk[2].rhw =
		item->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	item->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	item->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	// �e�N�X�`�����W�̐ݒ�
	item->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	item->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_ITEM, 0.0f);
	item->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ITEM);
	item->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_ITEM, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ITEM);

	return S_OK;
}
//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexItem(int no)
{
	ITEM *item = &itemWk[no];			// Item�̃|�C���^�[��������

	// ���_���W�̐ݒ�	
	item->pos.x = item->abs_pos.x - GetPlayer(0)->pos.x / 4.0f;

	item->vertexWk[0].vtx.x = item->pos.x - cosf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[0].vtx.y = item->pos.y - sinf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[0].vtx.z = 0.0f;
	
	item->vertexWk[1].vtx.x = item->pos.x + cosf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[1].vtx.y = item->pos.y - sinf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[1].vtx.z = 0.0f;
	
	item->vertexWk[2].vtx.x = item->pos.x - cosf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[2].vtx.y = item->pos.y + sinf(item->BaseAngle - item->rot.z) * item->Radius;
	item->vertexWk[2].vtx.z = 0.0f;
	
	item->vertexWk[3].vtx.x = item->pos.x + cosf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[3].vtx.y = item->pos.y + sinf(item->BaseAngle + item->rot.z) * item->Radius;
	item->vertexWk[3].vtx.z = 0.0f;
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureItem(int no, int cntPattern)
{
	ITEM *item = &itemWk[no];			// Item�̃|�C���^�[��������

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_ITEM;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_ITEM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_ITEM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ITEM;
	// ���_���W�̐ݒ�
	item->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	item->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	item->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	item->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// Item�̔��ːݒ�
//=============================================================================
void SetItem(D3DXVECTOR3 pos, int type)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ITEM *item = &itemWk[0];			// Item�̃|�C���^�[��������

	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < ITEM_MAX; i++, item++)
	{
		if (item->use == false)			// ���g�p��Ԃ�Item��������
		{
			item->use = true;				// �g�p��Ԃ֕ύX����
			item->pos = pos;				// ���W���Z�b�g
			item->abs_pos = pos;				// ���W���Z�b�g
			switch (type) {
			case 0:
				item->type = 1;
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_ITEM,			// �t�@�C���̖��O
					&item->Texture);			// �ǂݍ��ރ������̃|�C���^
				break;
			case 1:
				item->type = 2;
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_ITEM2,			// �t�@�C���̖��O
					&item->Texture);			// �ǂݍ��ރ������̃|�C���^
				break;
			case 2:
				item->type = 3;
				D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
					TEXTURE_GAME_ITEM3,			// �t�@�C���̖��O
					&item->Texture);			// �ǂݍ��ރ������̃|�C���^
				break;
			}			
			return;							// 1���Z�b�g�����̂ŏI������
		}
		
	}
}

/*******************************************************************************
�֐���:	ITEM *GetItemAdr( int pno )
����:	int pno : Item�ԍ�
�߂�l:	ITEM *
����:	Item�̃A�h���X���擾����
*******************************************************************************/
ITEM *GetItem(int pno)
{
	return &itemWk[pno];
}