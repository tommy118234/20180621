/*******************************************************************************
* �^�C�g��:		DirectX�Q�[���`�͂��߂Ă̌l��i�`
* �v���O������:	Main���� [main.cpp]
* �쐬��:		GP11B 16�@���@���C��
* �쐬�J�n��:	2018/07/24
********************************************************************************/
#include "main.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "bg.h"
#include "title.h"
#include "result.h"
#include "score.h"
#include "servant.h"
#include "sound.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")			// �E�C���h�E�̃N���X��
#define WINDOW_NAME			_T("VOID FUNC")			// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HWND hWnd, BOOL bWindow);
void	Uninit(void);
void	Update(void);
void	Draw(void);
void	CheckHit(void);
bool	CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);
bool	CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);

void InitGame(void);

#ifdef _DEBUG
void DrawDebugFont(void);
#endif

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;				// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Device�I�u�W�F�N�g(�`��ɕK�v)

#ifdef _DEBUG
LPD3DXFONT				g_pD3DXFont = NULL;			// �t�H���g�ւ̃|�C���^
int						g_nCountFPS;				// FPS�J�E���^
#endif

int						g_nStage = 0;				// �X�e�[�W�ԍ�
LPDIRECTSOUNDBUFFER8	g_pBGM;						// BGM�p�o�b�t�@
int                     page = 1;					// �X�e�[�W�T�u�ԍ�

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	
	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	
	MSG			msg;
	HWND		hWnd;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,																		// �E�B���h�E�̍����W
						CW_USEDEFAULT,																		// �E�B���h�E�̏���W
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,									// �E�B���h�E����
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
						NULL,
						NULL,
						hInstance,
						NULL);

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hWnd, true)))
	{
		return -1;
	}

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�  1 = 1000����1�b
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;
	// �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);	
	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //Message ���邩�H
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount;					//�\�����邽�߂ɕۑ�����
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS_RATE))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

				Update();			// �X�V����
				Draw();				// �`�揈��

				dwFrameCount++;		// �����񐔂̃J�E���g�����Z
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;


	srand((unsigned)time(NULL));

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�͌��ݐݒ肳��Ă�����̂��g��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �J���[���[�h�̎w��

	if(bWindow)
	{// �E�B���h�E���[�h
		d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.BackBufferFormat			 = D3DFMT_R5G6B5;					// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// �f�B�X�v���C�A�_�v�^
									D3DDEVTYPE_HAL,								// �f�B�X�v���C�^�C�v
									hWnd,										// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
									D3DCREATE_HARDWARE_VERTEXPROCESSING,		// �f�o�C�X�쐬����̑g�ݍ��킹
									&d3dpp,										// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
									&g_pD3DDevice)))							// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp,
										&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp,
											&g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���s��Ȃ�
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�


#ifdef _DEBUG
																			// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pD3DXFont);
#endif

	// �T�E���h������
	InitSound(hWnd);
	// ���[�h�ƍĐ�
	g_pBGM = LoadSound(BGM_00);
	PlaySound(g_pBGM, E_DS8_FLAG_LOOP);
	// �v���C���[�̏���������
	InitPlayer(0);
	// �G�l�~�[�̏���������
	InitEnemy(0);
	// �o���b�g�̏���������
	InitBullet(0);
	// SERVANT�̏���������
	InitSERVANT(0);
	// BG������
	InitBG(0);
	// �^�C�g��������
	InitTitle();
	// ���U���g������
	InitResult();
	// �X�R�A������
	InitScore(0);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	// ���͏����̏I������
	UninitInput();
	// �v���C���[�̏I������
	UninitPlayer();
	// �G�l�~�[�̏I������
	UninitEnemy();
	// �o���b�g�̏I������
	UninitBullet();
	// SERVANT�̏I������
	UninitSERVANT();
	// BG�I������
	UninitBG();
	// �^�C�g���I������
	UninitTitle();
	// ���U���g�I������
	UninitResult();
	// �X�R�A�I������
	UninitScore();
	if(g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
	}	
	if (g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
	}
	// �T�E���h�����̏I������
	if (g_pD3D != NULL)
	{
		g_pBGM->Release();
		g_pBGM = NULL;
	}
	UninitSound();
	
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͂̍X�V����
	UpdateInput();
	// ��ʑJ��
	switch (g_nStage)
	{
	case STAGE_TITLE:
		if (GetKeyboardTrigger(DIK_RETURN))
		SwitchBG(1);
		UpdateTitle();		
		break;
	case STAGE_TUTOR:		
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter��������A�X�e�[�W��؂�ւ���
			switch (page){
			case 1:
				SwitchBG(2);
				page++;
				break;
			case 2:
				GetPlayer(0)->ready = 1;
				SwitchBG(3);
				page++;
				break;
			case 3:
				GetPlayer(0)->ready = 2;
				GetPlayer(0)->view_mode = 0;
				SwitchBG(4);
				page = 1;
				SetStage(STAGE_GAME);
				break;
			}
		}
		// �Q�[���p�b�h�łňړ�����
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			SetStage(STAGE_GAME);
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			SetStage(STAGE_GAME);
		}
		// �v���C���[�̍X�V����
		UpdatePlayer();		
		// BG�̍X�V����
		UpdateBG();
		break;
	case STAGE_GAME:		
		// BG�̍X�V����
		UpdateBG();
		// �G�l�~�[�̍X�V����
		UpdateEnemy();
		// �v���C���[�̍X�V����
		UpdatePlayer();
		// �o���b�g�̍X�V����
		UpdateBullet();
		// SERVANT�̍X�V����
		UpdateSERVANT();
		// �X�R�A�̍X�V����
		UpdateScore();
		// �����蔻��
		CheckHit();
		break;
	case STAGE_GAME_END:		
		// �X�R�A�̍X�V����
		UpdateScore();
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter��������A�X�e�[�W��؂�ւ���
			InitGame();				// �Q�[���̍ď���������
			//SwitchBG(5);
			SetStage(STAGE_RESULT);
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			InitGame();				// �Q�[���̍ď���������
			//SwitchBG(5);
			SetStage(STAGE_RESULT);
		}
		break;
	case STAGE_RESULT:
		UpdateResult();	
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(128, 128, 255, 0), 1.0f, 0);	
	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// ��ʑJ��
		switch (g_nStage)
		{
		case STAGE_TITLE:
			DrawTitle();
			break;
		case STAGE_TUTOR:				
			// BG�̕`�揈��
			DrawBG();
			// �v���C���[�̕`�揈��
			DrawPlayer();
			break;
		case STAGE_GAME:
			// BG�̕`�揈��
			DrawBG();
			// �G�l�~�[�̕`�揈��
			DrawEnemy();
			// �v���C���[�̕`�揈��
			DrawPlayer();
			// �o���b�g�̕`�揈��
			DrawBullet();
			// SERVANT�̕`�揈��
			DrawSERVANT();
			// �X�R�A�̕`�揈��
			DrawScore();
			break;
		case STAGE_GAME_END:
			// BG�̕`�揈��
			DrawBG();			
			// �X�R�A�̕`�揈��
			DrawScore();
			break;
		case STAGE_RESULT:
			DrawResult();
			break;
		}
#ifdef _DEBUG
		// �o�b�O�\��
		DrawDebugFont();
#endif
		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�擾�֐�
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}

#ifdef _DEBUG
//=============================================================================
// FPS�\������
//=============================================================================
void DrawDebugFont(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];
	PLAYER *player = GetPlayer(0);	

	ENEMY *enemy = GetEnemy(0);

	BULLET *bullet = GetBullet(0);
	SERVANT *servant = GetSERVANT(0);
	
	// �e�L�X�g�`��
	sprintf(str, _T("FPS:%d"), g_nCountFPS);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 20;
	sprintf(str, "PX:%f  PY:%f", player->pos.x, player->pos.y);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));


	rect.top = 40;
	sprintf(str, "A:%f B:%f C:%f D:%f", enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2, servant->pos.x + TEXTURE_SERVANT_SIZE_X / 2, servant->rot.z, atan2f(servant->pos.y + TEXTURE_SERVANT_SIZE_Y / 2, enemy->pos.x + TEXTURE_ENEMY_SIZE_X / 2));
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif
//=============================================================================
// ��ʑJ��
//=============================================================================
void SetStage(int stage)
{
	//if( state < 0 || state >= STATE_MAX ) return;		
	g_nStage = stage;		
	if (!IsPlaying(LoadSound(stage))) 
	{
		StopSound(g_pBGM);
		g_pBGM->Release();
		g_pBGM = NULL;
		g_pBGM = LoadSound(stage);
		PlaySound(g_pBGM, E_DS8_FLAG_NONE);
	}
}
//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	PLAYER *player = GetPlayer(0);				// �G�l�~�[�̃|�C���^�[��������
	ENEMY *enemy = GetEnemy(0);				// �G�l�~�[�̃|�C���^�[��������
	BULLET *bullet = GetBullet(0);			// �o���b�g�̃|�C���^�[��������
	int cnt = 0;							// �G�̐��𐔂���
	// �G�Ƒ���L����(BB)
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == false)	continue;
		if (CheckHitBB(player->pos, enemy->pos, D3DXVECTOR2(TEXTURE_PLAYER_SIZE_X, TEXTURE_PLAYER_SIZE_Y), D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y)))
		{
			// ����L�����N�^�[�͎���
			player->use = false;
			// �G�L�����N�^�[�͓|�����
			enemy->use = false;
			// HP��������
			player->status.HP -= 10;
		}
	}
	// �e�ƓG(BC)
	enemy = GetEnemy(0);					// �G�l�~�[�̃|�C���^�[��������
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->use == false) continue;
		for (int j = 0; j < ENEMY_MAX; j++, enemy++)
		{
			if (enemy->use == false) continue;
			//if (CheckHitBC(bullet->pos, enemy->pos, TEXTURE_BULLET_SIZE_X, TEXTURE_ENEMY_SIZE_X))
			if (CheckHitBB(bullet->pos, enemy->pos, D3DXVECTOR2(TEXTURE_BULLET_SIZE_X, TEXTURE_BULLET_SIZE_Y), D3DXVECTOR2(TEXTURE_ENEMY_SIZE_X, TEXTURE_ENEMY_SIZE_Y)))
			{
				bullet->use = false;		// �e�̏��ŏ������s��
				enemy->status.HP--;			// �GHP��������
				if (enemy->status.HP < 1){
					enemy->use = false;		// �G�͓|�����					
					AddScore(100);			// �X�R�A�v�Z					
				}
			}
		}
	}
	// �{�X�ƒe(BC)

	// �����ƓG�̒e(BC)

	// �G���S�ł������ʑJ��
	enemy = GetEnemy(0);					// �G�l�~�[�̃|�C���^�[��������
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{
		if (enemy->use == true)
		{
			cnt++;
		}
	}
	// ���������ʓG���S�ŁH
	if (cnt == 0)
	{
		SetStage(STAGE_GAME_END);
		SwitchBG(5);
	}
}


//=============================================================================
// BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ� 
// �߂�l�F�������Ă���true
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{
	size1 /= 2.0f;	// ���T�C�Y�ɂ���
	size2 /= 2.0f;	// ����

	if (pos1.x + size1.x > pos2.x - size2.x && pos2.x + size2.x > pos1.x - size1.x &&
		pos1.y + size1.y > pos2.y - size2.y && pos2.y + size2.y > pos1.y - size1.y)
	{
		return true;
	}

	return false;
}

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// pos�͉~�̒��S�Aradius�͔��a
// �߂�l�F�������Ă���true
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
{
	if ((radius1 + radius2) * (radius1 + radius2) >
		(pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y))
	{
		return true;
	}
	return false;
}

//=============================================================================
// �Q�[���̍ď�������������
// �߂�l�F����
//=============================================================================
void InitGame(void)
{
	InitBG(1);			// BG�̍ď�����
	InitBullet(1);		// �o���b�g�̍ď�����
	InitSERVANT(1);		// �o���b�g�̍ď�����
	InitEnemy(1);		// �G�l�~�[�̍ď�����
	InitPlayer(1);		// �v���C���[�̍ď�����	
	InitResult();	
	InitScore(1);		// �X�R�A�̍ď�����		
	InitTitle();		// �v���C���[�̍ď�����	
	page = 1;
}