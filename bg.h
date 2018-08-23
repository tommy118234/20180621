/*******************************************************************************
* ^Cg:		DirectXQ[`อถ฿ฤฬยl์i`
* vOผ:	wi [bg.h]
* ์ฌา:		GP11B 16@@C
* ์ฌJn๚:	2018/07/24
********************************************************************************/
#ifndef _BG_H_
#define _BG_H_

// }N่`
#define TEXTURE_GAME_BG00		_T("data/BG/moon.png")			// wiๆ
#define TEXTURE_GAME_BG01		_T("data/BG/story1.png")		// wiๆ
#define TEXTURE_GAME_BG02		_T("data/BG/story2.png")		// wiๆ
#define TEXTURE_GAME_BG03		_T("data/BG/battleGuide.png")	// wiๆ
#define TEXTURE_GAME_BG04		_T("data/BG/battle_ice.png")	// wiๆ
#define TEXTURE_GAME_BG05		_T("data/BG/ice_castle.png")	// wiๆ
#define TEXTURE_GAME_BG06		_T("data/BG/moon_red.png")		// wiๆ

#define BG00_SIZE_X			(800)	// eNX`TCY
#define BG00_SIZE_Y			(600)	// ฏใ
#define BG00_POS_X			(0)		// |Sฬ๚สuX(ถใ)
#define BG00_POS_Y			(0)		// ฏใ
//*****************************************************************************
// vg^Cv้พ
//*****************************************************************************
HRESULT InitBG(int type);
void	 UninitBG(void);
void	UpdateBG(void);
void	DrawBG(void);
void	SwitchBG(int type);

void	ChangeBG(void);
#endif
