/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������: �v���[���[���� [player.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "servant.h"
#include "sound.h"

#define M_PI 3.14159265358979323846

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPlayer(int no);
void	SetVertexPlayer(int no, int dir);
void	SetTexturePlayer(int no, int cntPattern );	//

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePlayer = NULL;		// �e�N�X�`���ւ̃|���S��
PLAYER					playerWk[PLAYER_MAX];			// �v���C���[�\����
LPDIRECTSOUNDBUFFER8	g_pSE2;							// SE�p�o�b�t�@

bool jump_cooldown = FALSE;
float post_x = 0;
float post_y = 0;
bool overlap = false;

ID3DXFont*				font2 = NULL;                // �����̃|�C���^
const char* strings2[] = { "�퓬���K���n�܂�܂����H" };
D3DCOLOR colors2[] = { D3DCOLOR_ARGB(155, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 255, 0), D3DCOLOR_ARGB(255, 0, 0, 255) };
RECT r2 = { SCREEN_WIDTH / 3 + 20, SCREEN_HEIGHT / 2 ,0,0 }; // starting point

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk[0];					// �v���C���[�O�Ԃ̃A�h���X���擾����
	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_PLAYER,					// �t�@�C���̖��O
			&g_pD3DTexturePlayer);		
		g_pSE2 = LoadSound(SE_00);		
	}
		
	else if (type == 1) {
		UninitPlayer;
	}
	// �v���C���[�̏���������
	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		player->use = true;							// �g�p
		player->pos = D3DXVECTOR3(TEXTURE_PLAYER_SIZE_X / 2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
		player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player->CountAnim = 0;
		player->PatternAnim = 0;
		player->direction = -1;
		player->gravity = 5;
		player->view_mode = 1;
		player->ready = 0;

		player->status.HP = 2000;								// HP��������
		player->status.MP = 200;								// MP��������
		player->status.ATK = 100;								// ATK��������
		player->status.DEF = 100;								// DEF��������
		player->status.LUCK = 100;								// LUCK��������
		strcpy(player->status.name, "���@���L���[ ?");			// NAME��������

		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_Y);
		player->Radius = D3DXVec2Length(&temp);				// �v���C���[�̔��a��������
		player->BaseAngle = atan2f(TEXTURE_PLAYER_SIZE_Y, TEXTURE_PLAYER_SIZE_X);	// �v���C���[�̊p�x��������

		player->Texture = g_pD3DTexturePlayer;				// �e�N�X�`�����
		MakeVertexPlayer(i);								// ���_���̍쐬

	}
	return S_OK;	
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if( g_pD3DTexturePlayer != NULL )	//
	{	// �e�N�X�`���̊J��
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{			
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk[0];	
	if (player->view_mode == 0) {
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_PLAYER2,				// �t�@�C���̖��O
			&g_pD3DTexturePlayer);				// �ǂݍ��ރ������̃|�C���^
		Side_to_TopView();
		player->gravity = 0;
		player->Texture = g_pD3DTexturePlayer;				// �e�N�X�`�����
		player->pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - TEXTURE_PLAYER_SIZE_X/2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
		player->view_mode = 2;
	}
	else if (player->view_mode == 1) {
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_PLAYER,				// �t�@�C���̖��O
			&g_pD3DTexturePlayer);				// �ǂݍ��ރ������̃|�C���^
		Top_to_SideView();
		player->gravity = 5;
		player->Texture = g_pD3DTexturePlayer;	// �e�N�X�`�����
		player->pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - TEXTURE_PLAYER_SIZE_X / 2, SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y, 0.0f);
		player->view_mode = 2;
	}
	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		//���L����
		if (!jump_cooldown && player->gravity > 0)
		{
			player->pos.y += player->gravity + player->acceleration;
			if (player->acceleration >= 0) {
				player->acceleration -= 1;

			}
			else if (player->acceleration < 0)
				player->acceleration = 0;
		}
		//Jump
		if (jump_cooldown) 
		{
			if (player->acceleration > 0) {
				player->pos.y -= player->acceleration;
				player->acceleration -= 2;
			}
			else {
				jump_cooldown = FALSE;
				player->acceleration = 20 + player->acceleration / 2;
			}
		}
		if (player->use == true)			// �g�p���Ă����ԂȂ�X�V����
		{
			// �A�j���[�V����
			player->CountAnim++;
			if (player->moving_cooldown > 0) {
				player->PatternAnim = (player->PatternAnim + 1) % ANIM_PATTERN_NUM;
				// �e�N�X�`�����W��ݒ�
				SetTexturePlayer(i, player->PatternAnim);
				if (player->PatternAnim == 1 || player->PatternAnim == 6)
					player->moving_cooldown--;
			}
			

			// �L�[�{�[�h���͂ňړ�
			if (GetKeyboardPress(DIK_DOWN) && player->gravity == 0) {
				if (player->gravity == 0)
				{
					player->moving_cooldown = 1;
					post_x = player->pos.x;
					post_y = player->pos.y;
					player->pos.x -= 5 * (-sinf(player->BaseAngle + player->rot.z) + cosf(player->BaseAngle + player->rot.z));
					player->pos.y -= 5 * (cosf(player->BaseAngle + player->rot.z) + sinf(player->BaseAngle + player->rot.z));
				}
				player->pos.y += 5;
			}
			if (GetKeyboardPress(DIK_UP)) 
			{
				if (player->gravity != 0 && !jump_cooldown && player->acceleration == 0)
				{
						// jump
						player->acceleration = 20;
						jump_cooldown = TRUE;
				}
				else 
				{
					// pos update
					player->moving_cooldown = 1;
					post_x = player->pos.x;
					post_y = player->pos.y;
					player->pos.x -= 5 * (-sinf(player->BaseAngle + player->rot.z) + cosf(player->BaseAngle + player->rot.z));
					player->pos.y -= 5 * (cosf(player->BaseAngle + player->rot.z) + sinf(player->BaseAngle + player->rot.z));							
				}
			}

			// ��ʊO����
			if (player->pos.x < 0 + TEXTURE_PLAYER_SIZE_X)
			{
				player->pos.x = 0 + TEXTURE_PLAYER_SIZE_X;

				if (player->gravity == 0)
				player->pos.y = post_y;
			}
			if (player->pos.x > SCREEN_WIDTH - TEXTURE_PLAYER_SIZE_X)
			{
				player->pos.x = SCREEN_WIDTH - TEXTURE_PLAYER_SIZE_X;

				if (player->gravity == 0)
				player->pos.y = post_y;
			}
			if (player->pos.y < 0)
			{
				player->pos.y = 0;
				if (player->gravity == 0)
				player->pos.x = post_x;
			}
			if (player->pos.y > SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y)
			{
				player->pos.y = SCREEN_HEIGHT - TEXTURE_PLAYER_SIZE_Y;

				if (player->gravity == 0) {
					player->pos.x = post_x;
					player->acceleration = 0;
					jump_cooldown = FALSE;
				}
			}

			if (GetKeyboardPress(DIK_LEFT)) {
				if (player->gravity != 0) {
					player->moving_cooldown = 1;
					player->direction = -1;
					player->pos.x -= 5;
				}
				else {
					player->rot.z -= 0.1f;
				}
			}
			if (GetKeyboardPress(DIK_RIGHT)) {
				if (player->gravity != 0) {
					player->moving_cooldown = 1;
					player->direction = 1;

					player->pos.x += 5;
				}
				else {
					player->rot.z += 0.1f;
				}
			}
			// MP�����
			if (player->gravity == 0)
			{				

				SERVANT *servant = GetServant(0);
				for (int i = 0; i < SERVANT_MAX; i++, servant++)
				{
					if (servant->use)
					{
						if (CheckHitBC(servant->pos, player->pos, servant->Radius, servant->Radius))
							overlap = true;
					}
				}

				// ���� 1
				if (GetKeyboardPress(DIK_Q) && (player->bullet_cooldown == 0) && !overlap)
				{
					player->bullet_cooldown += 5;
					if (player->status.MP > 20 )
					{
						player->status.MP -= 20;
						D3DXVECTOR3 pos = player->pos;
						pos.y -= TEXTURE_PLAYER_SIZE_Y;
						pos.x += TEXTURE_PLAYER_SIZE_X + player->pos.x / 4.0f;						
						SetServant(pos, 1);
					}
				}
				// ���� 2
				if (GetKeyboardPress(DIK_W) && (player->bullet_cooldown == 0) && !overlap)
				{
					player->bullet_cooldown += 5;
					if (player->status.MP > 40)
					{
						player->status.MP -= 40;
						D3DXVECTOR3 pos = player->pos;
						pos.y -= TEXTURE_PLAYER_SIZE_Y;
						pos.x += TEXTURE_PLAYER_SIZE_X + player->pos.x / 4.0f;
						SetServant(pos, 2);
					}
				}
				// ���� 3
				if (GetKeyboardPress(DIK_E) && (player->bullet_cooldown == 0) && !overlap)
				{
					player->bullet_cooldown += 5;
					if (player->status.MP > 60)
					{
						player->status.MP -= 60;
						D3DXVECTOR3 pos = player->pos;
						pos.y -= TEXTURE_PLAYER_SIZE_Y;
						pos.x += TEXTURE_PLAYER_SIZE_X + player->pos.x / 4.0f;
						SetServant(pos, 3);
					}
				}
				
			}
			// �X�L�� 1
			if (GetKeyboardPress(DIK_A) && (player->skill1_cooldown == 0) && player->status.MP >= 20 && player->status.ATK <= 500)
			{

				// �X�L�����Đ�
				g_pSE2 = LoadSound(16);
				if (IsPlaying(g_pSE2))
					g_pSE2->SetCurrentPosition(0);
				else
				{
					PlaySound(g_pSE2, E_DS8_FLAG_NONE);
				}
				player->status.ATK += 50;

				SERVANT *servant = GetServant(0);
				for (int i = 0; i < SERVANT_MAX; i++, servant++)
				{
					if (servant->use)
					{
						servant->status.ATK += 25;
					}
				}
				player->status.MP -= 20;
				player->skill1_cooldown = 100;
			}
			// �X�L�� 2
			if (GetKeyboardPress(DIK_S) && (player->skill2_cooldown == 0) && player->status.MP >= 50 && player->status.DEF <= 250)
			{
				// �X�L�����Đ�
				g_pSE2 = LoadSound(17);
				if (IsPlaying(g_pSE2))
					g_pSE2->SetCurrentPosition(0);
				else
				{
					PlaySound(g_pSE2, E_DS8_FLAG_NONE);
				}
				player->status.DEF += 50;
				player->status.MP -= 50;
				player->status.HP -= 50;
				player->skill2_cooldown = 100;
			}
			// �X�L�� 3
			if (GetKeyboardPress(DIK_D) && (player->skill3_cooldown == 0) && player->status.HP > 50)
			{
				g_pSE2 = LoadSound(18);
				// �X�L�����Đ�
				if (IsPlaying(g_pSE2))
					g_pSE2->SetCurrentPosition(0);
				else 
				{
					PlaySound(g_pSE2, E_DS8_FLAG_NONE);
				}
				player->status.HP -= 50;
				player->status.MP += 50;

				player->skill3_cooldown = 20;
			}

			// �U�����[�h�X�E�B�b�`
			//if (GetKeyboardPress(DIK_F))
			//{
			//	// �U�����[�h�ɕϊ�				
			//	player->direction = 1;
			//}
			//if (GetKeyboardPress(DIK_T))
			//{
			//	// �������[�h�ɕϊ�					
			//	player->direction = -1;
			//}
			// �Q�[���p�b�h�łňړ�����
			//if (IsButtonPressed(0, BUTTON_DOWN))
			//{
			//	player->pos.y += 2.0f;
			//}
			//else if (IsButtonPressed(0, BUTTON_UP))
			//{
			//	player->pos.y -= 2.0f;
			//}
			//
			//if (IsButtonPressed(0, BUTTON_RIGHT))
			//{
			//	player->pos.x += 2.0f;
			//}
			//else if (IsButtonPressed(0, BUTTON_LEFT))
			//{
			//	player->pos.x -= 2.0f;
			//}

			//�e����
			if (GetKeyboardPress(DIK_SPACE) && (player->bullet_cooldown == 0) && player->direction == 1)
			{
				player->bullet_cooldown += 20;
				D3DXVECTOR3 pos = player->pos;
				//pos.x += TEXTURE_PLAYER_SIZE_X / 2;
				//pos.y -= TEXTURE_BULLET_SIZE_Y;
				pos.y -= TEXTURE_PLAYER_SIZE_Y;
				pos.x += TEXTURE_PLAYER_SIZE_X/2 + player->pos.x / 4.0f;
				SetBullet(pos, player->rot.z, player->status.ATK);
			}			
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexPlayer(i, player->direction);
			if (player->bullet_cooldown > 0)
				player->bullet_cooldown--;
			if (player->skill1_cooldown > 0)
				player->skill1_cooldown--;
			if (player->skill2_cooldown > 0)
				player->skill2_cooldown--;
			if (player->skill3_cooldown > 0)
				player->skill3_cooldown--;
			overlap = false;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->use == true)			// �g�p���Ă����ԂȂ�`�悷��
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, player->Texture);
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, player->vertexWk, sizeof(VERTEX_2D));
			if (player->ready == 1) {// Tutorial Start
				if (font2 == NULL)
					// �����̕`��
					D3DXCreateFont(pDevice,     //D3D Device
						26,						//Font height
						0,						//Font width
						FW_NORMAL,				//Font Weight
						1,						//MipLevels
						false,					//Italic
						DEFAULT_CHARSET,		//CharSet
						OUT_DEFAULT_PRECIS,		//OutputPrecision
						ANTIALIASED_QUALITY,	//Quality
						DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
						"Georgia",				//pFacename,
						&font2);				//ppFont
				const char* strings2[] = { "�퓬���K���n�܂�܂����H(Enter)" };
				D3DCOLOR colors2[] = { D3DCOLOR_ARGB(255, 255, 255, 255) };
				RECT r2 = { 0  , 0 ,SCREEN_WIDTH,SCREEN_HEIGHT/2 }; // starting point
				font2->DrawText(NULL, strings2[0], -1, &r2, DT_CENTER|DT_VCENTER, colors2[0]);
				//for (int i = 0; i < _countof(strings2); ++i)
				//{
				//	font2->DrawText(NULL, strings2[i], -1, &r2, DT_CALCRECT, 0);
				//	font2->DrawText(NULL, strings2[i], -1, &r2, DT_CENTER, colors2[i]);
				//	r2.left = r2.right; // offset for next character.
				//}
			}
		}
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = &playerWk[no];

	// ���_���W�̐ݒ�
	SetVertexPlayer(no, player->direction);
	// rhw�̐ݒ�
	player->vertexWk[0].rhw =
	player->vertexWk[1].rhw =
	player->vertexWk[2].rhw =
	player->vertexWk[3].rhw = 1.0f;
	// ���ˌ��̐ݒ�
	player->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	player->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	player->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	player->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	player->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	player->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X, 1.0f / TEXTURE_PATTERN_DIVIDE_Y);
	// �e�N�X�`�����W�̐ݒ�
	SetTexturePlayer(no,player->PatternAnim);
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPlayer( int no, int dir)
{
	
	PLAYER *player = &playerWk[no];

	// ���_���W�̐ݒ�
	switch (dir)
	{
	case -1:
		player->vertexWk[0].vtx = D3DXVECTOR3(player->pos.x - cosf(player->BaseAngle + player->rot.z) * player->Radius,
											  player->pos.y - sinf(player->BaseAngle + player->rot.z) * player->Radius,
											  0);

		player->vertexWk[1].vtx = D3DXVECTOR3(player->pos.x + cosf(player->BaseAngle - player->rot.z) * player->Radius,
											  player->pos.y - sinf(player->BaseAngle - player->rot.z) * player->Radius,
											  0);

		player->vertexWk[2].vtx = D3DXVECTOR3(player->pos.x - cosf(player->BaseAngle - player->rot.z) * player->Radius,
											  player->pos.y + sinf(player->BaseAngle - player->rot.z) * player->Radius,
											 0);

		player->vertexWk[3].vtx = D3DXVECTOR3(player->pos.x + cosf(player->BaseAngle + player->rot.z) * player->Radius,
											  player->pos.y + sinf(player->BaseAngle + player->rot.z) * player->Radius,
											 0);		
		break;
	case 1:		
		player->vertexWk[1].vtx = D3DXVECTOR3(player->pos.x - cosf(player->BaseAngle + player->rot.z) * player->Radius,
											  player->pos.y - sinf(player->BaseAngle + player->rot.z) * player->Radius,
											  0);

		player->vertexWk[0].vtx = D3DXVECTOR3(player->pos.x + cosf(player->BaseAngle - player->rot.z) * player->Radius,
											  player->pos.y - sinf(player->BaseAngle - player->rot.z) * player->Radius,
									          0);

		player->vertexWk[3].vtx = D3DXVECTOR3(player->pos.x - cosf(player->BaseAngle - player->rot.z) * player->Radius,
											  player->pos.y + sinf(player->BaseAngle - player->rot.z) * player->Radius,
											  0);

		player->vertexWk[2].vtx = D3DXVECTOR3(player->pos.x + cosf(player->BaseAngle + player->rot.z) * player->Radius,
											  player->pos.y + sinf(player->BaseAngle + player->rot.z) * player->Radius,
											  0);
		break;	
	}	
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePlayer( int no, int cntPattern )
{
	PLAYER *player = &playerWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;

	player->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	player->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	player->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	player->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}
/*******************************************************************************
�֐���:	PLAYER *GetMapAdr( int pno )
����:	int pno : �v���C���[�ԍ�
�߂�l:	PLAYER *
����:	�v���C���[�̃A�h���X���擾����
*******************************************************************************/
PLAYER *GetPlayer(int pno)
{
	return &playerWk[pno];
}


void Side_to_TopView(void)
{

//#undef TEXTURE_GAME_PLAYER	_T	
#undef TEXTURE_PLAYER_SIZE_X	
#undef TEXTURE_PLAYER_SIZE_Y	
#undef TEXTURE_PATTERN_DIVIDE_X	
#undef TEXTURE_PATTERN_DIVIDE_Y	

//#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/runningman000.png")	// �T���v���p�摜
#define TEXTURE_PLAYER_SIZE_X	(545/5/2) // �e�N�X�`���T�C�Y
#define TEXTURE_PLAYER_SIZE_Y	(242/2/2) // ���� 
//#define TEXTURE_PATTERN_DIVIDE_X	(8)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
//#define TEXTURE_PATTERN_DIVIDE_Y	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
}

void Top_to_SideView(void)
{

//#undef TEXTURE_GAME_PLAYER	_T	
#undef TEXTURE_PLAYER_SIZE_X	
#undef TEXTURE_PLAYER_SIZE_Y	
#undef TEXTURE_PATTERN_DIVIDE_X	
#undef TEXTURE_PATTERN_DIVIDE_Y	

//#define TEXTURE_GAME_PLAYER	_T("data/TEXTURE/runningman001.png")	// �T���v���p�摜
#define TEXTURE_PLAYER_SIZE_X	(140/5) // �e�N�X�`���T�C�Y
#define TEXTURE_PLAYER_SIZE_Y	(200/5) // ���� 
#define TEXTURE_PATTERN_DIVIDE_X	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
}
