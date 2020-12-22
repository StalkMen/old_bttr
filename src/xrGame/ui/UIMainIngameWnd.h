#pragma once
#include "UIGameLog.h"

#include "../hudsound.h"
#include "../../xrServerEntities/alife_space.h"
#include "../EntityCondition.h"

class	CUIPdaMsgListItem;
class	CLAItem;
class	CUIZoneMap;
class	CUIScrollView;
struct	GAME_NEWS_DATA;
class	CMissile;
class	CInventoryItem;
class	CUIHudStatesWnd;
class	CUIMotionIcon;
class	CUIArtefactPanel;

class CUIMainIngameWnd: public CUIWindow  
{
public:
			CUIMainIngameWnd();
	virtual ~CUIMainIngameWnd();

	virtual void Init();
	virtual void Draw();
			void KurrStatic();
	virtual void Update();

	CUIArtefactPanel* m_artefactPanel;

protected:
	
	CUIStatic*			UIStaticDiskIO;
	CUIStatic*          UIStaticKurr;

	CUITextWnd*			UIStaticQuickHelp;
	CUIMotionIcon*		UIMotionIcon;
	CUIZoneMap*			UIZoneMap;

	CUIHudStatesWnd*	m_ui_hud_states;

//	CUIStatic			UIStaticTorch;
//	CUIProgressBar		UIFlashlightBar;

	////////////////////////////////////////////////////////////////////////////////////////////////	
//*Индикаторы худа Зов Чернобыля
	CUIStatic* m_ind_bleeding_coc;
	CUIStatic* m_ind_radiation_coc;
	CUIStatic* m_ind_starvation_coc;
	CUIStatic* m_ind_thirst_coc;
	CUIStatic* m_ind_slepping_coc;
	CUIStatic* m_ind_weapon_broken_coc;
	CUIStatic* m_ind_helmet_broken_coc;
	CUIStatic* m_ind_outfit_broken_coc;
	CUIStatic* m_ind_overweight_coc;
	////////////////////////////////////////////////////////////////////////////////////////////////	

	////////////////////////////////////////////////////////////////////////////////////////////////	
	//*Индикаторы худа Зов Припяти
	CUIStatic* m_ind_bleeding_cop;
	CUIStatic* m_ind_radiation_cop;
	CUIStatic* m_ind_starvation_cop;
	CUIStatic* m_ind_thirst_cop;
	CUIStatic* m_ind_slepping_cop;
	CUIStatic* m_ind_weapon_broken_cop;
	CUIStatic* m_ind_helmet_broken_cop;
	CUIStatic* m_ind_outfit_broken_cop;
	CUIStatic* m_ind_overweight_cop;
	////////////////////////////////////////////////////////////////////////////////////////////////	

	//*Индикаторы 
	//	CUIStatic* m_ind_bleeding_st;
	CUIStatic* m_ind_radiation_st;
	CUIStatic* m_ind_starvation_st;
	CUIStatic* m_ind_thirst_st;
	//	CUIStatic* m_ind_slepping_cop;
	CUIStatic* m_ind_weapon_broken_st;
	CUIStatic* m_ind_helmet_broken_st;
	CUIStatic* m_ind_outfit_broken_st;
	CUIStatic* m_ind_overweight_st;

	CUIStatic* m_ind_bleeding_cmp;
	CUIStatic* m_ind_radiation_cmp;
	CUIStatic* m_ind_starvation_cmp;
	//	CUIStatic* m_ind_slepping_cop;
	CUIStatic* m_ind_weapon_broken_cmp;
	CUIStatic* m_ind_helmet_broken_cmp;
	CUIStatic* m_ind_outfit_broken_cmp;
	CUIStatic* m_ind_overweight_cmp;

	////////////////////////////////////////////////////////////////////////////////////////////////
		//*Индикаторы худа Ветер Времени
	CUIStatic* m_ind_radiation_vv;
	CUIStatic* m_ind_starvation_vv;
	CUIStatic* m_ind_thirst_vv;
	CUIStatic* m_ind_slepping_vv;
	CUIStatic* m_ind_weapon_broken_vv;
	CUIStatic* m_ind_helmet_broken_vv;
	CUIStatic* m_ind_overweight_vv;
	////////////////////////////////////////////////////////////////////////////////////////////////	

	CUIStatic* m_ind_starvation_cs;
	CUIStatic* m_ind_thirst_cs;
	CUIStatic* m_ind_slepping_cs;
	CUIStatic* m_ind_weapon_broken_cs;
	CUIStatic* m_ind_helmet_broken_cs;

public:
	CUIStatic*			m_ind_boost_psy;
	CUIStatic*			m_ind_boost_radia;
	CUIStatic*			m_ind_boost_chem;
	CUIStatic*			m_ind_boost_wound;
	CUIStatic*			m_ind_boost_weight;
	CUIStatic*			m_ind_boost_health;
	CUIStatic*			m_ind_boost_power;
	CUIStatic*			m_ind_boost_rad;

public:
		void			ShowZoneMap(bool status);
		void			DrawZoneMap();
		void			UpdateZoneMap();

		void			DrawMainIndicatorsForInventory();
	
	CUIHudStatesWnd*	get_hud_states() { return m_ui_hud_states; } //temp
	void				OnSectorChanged			(int sector);

	xr_vector<CUIStatic* > m_quick_slots_icons;
	CUITextWnd*			m_QuickSlotText1;
	CUITextWnd*			m_QuickSlotText2;
	CUITextWnd*			m_QuickSlotText3;
	CUITextWnd*			m_QuickSlotText4;

protected:

	// 5 статиков для отображения иконок:
	// - сломанного оружия(only mp)
	// - радиации
	// - ранения
	// - голода
	// - усталости
	CUIStatic*			UIWeaponJammedIcon;
//	CUIStatic			UIRadiaitionIcon;
//	CUIStatic			UIWoundIcon;
//	CUIStatic			UIStarvationIcon;
//	CUIStatic			UIPsyHealthIcon;
	CUIStatic*			UIInvincibleIcon;
//	CUIStatic			UISleepIcon;
	CUIStatic*			UIArtefactIcon;

	CUIScrollView*		m_UIIcons;
	CUIWindow*			m_pMPChatWnd;
	CUIWindow*			m_pMPLogWnd;
public:
	
	// Енумы соответсвующие предупреждающим иконкам 
	enum EWarningIcons
	{
		ewiAll				= 0,
		ewiWeaponJammed,
//		ewiRadiation,
//		ewiWound,
//		ewiStarvation,
//		ewiPsyHealth,
//		ewiSleep,
		ewiInvincible,
		ewiArtefact,
	};

	void				SetMPChatLog					(CUIWindow* pChat, CUIWindow* pLog);

	// Задаем цвет соответствующей иконке
	void				SetWarningIconColor				(EWarningIcons icon, const u32 cl);
	void				TurnOffWarningIcon				(EWarningIcons icon);

	// Пороги изменения цвета индикаторов, загружаемые из system.ltx
	typedef				xr_map<EWarningIcons, xr_vector<float> >	Thresholds;
	typedef				Thresholds::iterator						Thresholds_it;
	Thresholds			m_Thresholds;

	// Енум перечисления возможных мигающих иконок
	enum EFlashingIcons
	{
		efiPdaTask	= 0,
		efiMail
	};
	
	void				SetFlashIconState_				(EFlashingIcons type, bool enable);

	void				AnimateContacts					(bool b_snd);
	HUD_SOUND_ITEM		m_contactSnd;

	void				ReceiveNews						(GAME_NEWS_DATA* news);
	void				UpdateMainIndicators			();
	void				UpdateBoosterIndicators			(const xr_map<EBoostParams, SBooster> influences);

protected:
	void				UpdateQuickSlots				();
	void				SetWarningIconColorUI			(CUIStatic* s, const u32 cl);
	void				InitFlashingIcons				(CUIXml* node);
	void				DestroyFlashingIcons			();
	void				UpdateFlashingIcons				();
//	void				UpdateActiveItemInfo			();

//	void				SetAmmoIcon						(const shared_str& seсt_name);

	// first - иконка, second - анимация
	DEF_MAP				(FlashingIcons, EFlashingIcons, CUIStatic*);
	FlashingIcons		m_FlashingIcons;

//	CMissile*			m_pGrenade;
//	CInventoryItem*		m_pItem;

	// Отображение подсказок при наведении прицела на объект
	void				RenderQuickInfos();

public:
	CUIMotionIcon*		MotionIcon							(){return UIMotionIcon;}
	void				OnConnected							();
	void				reset_ui							();

protected:
	CInventoryItem*		m_pPickUpItem;
	CUIStatic*			UIPickUpItemIcon;

	float				m_iPickUpItemIconX;
	float				m_iPickUpItemIconY;
	float				m_iPickUpItemIconWidth;
	float				m_iPickUpItemIconHeight;

	void				UpdatePickUpItem();
public:
	void				SetPickUpItem	(CInventoryItem* PickUpItem);
	CInventoryItem*		GetPickUpItem() { return m_pPickUpItem; }
#ifdef DEBUG
	void				draw_adjust_mode					();
#endif
};
