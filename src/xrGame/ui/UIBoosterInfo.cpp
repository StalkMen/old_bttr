#include "stdafx.h"
#include "UIBoosterInfo.h"
#include "UIStatic.h"
#include "object_broker.h"
#include "../EntityCondition.h"
#include "..\actor.h"
#include "../ActorCondition.h"
#include "UIXmlInit.h"
#include "UIHelper.h"
#include "../string_table.h"

CUIBoosterInfo::CUIBoosterInfo()
{
	for(u32 i = 0; i < eBoostExplImmunity; ++i)
	{
		m_booster_items[i] = NULL;
	}
	m_booster_satiety = NULL;
	m_booster_anabiotic = NULL;
	m_booster_time = NULL;
	m_booster_sleep = NULL;
	m_booster_thirst = NULL;

	if (BttR_mode)
	{
		m_booster_vodka = NULL;
		m_booster_medkit = NULL;
		m_booster_antizombin = NULL;
		m_booster_psyblockade = NULL;
		m_booster_antirad = NULL;
		m_booster_kurr = NULL;
		m_booster_komponent = NULL;
	}
}

CUIBoosterInfo::~CUIBoosterInfo()
{
	delete_data(m_booster_items);
	xr_delete(m_booster_satiety);
	xr_delete(m_booster_anabiotic);
	xr_delete(m_booster_time);
	xr_delete(m_Prop_line);
	xr_delete(m_booster_sleep);
	xr_delete(m_booster_thirst);

	if (BttR_mode)
	{
		xr_delete(m_booster_vodka);
		xr_delete(m_booster_medkit);
		xr_delete(m_booster_antizombin);
		xr_delete(m_booster_psyblockade);
		xr_delete(m_booster_antirad);
		xr_delete(m_booster_kurr);
		xr_delete(m_booster_komponent);
	}
}

LPCSTR boost_influence_caption_bttr[] =
{
	"st_ui_boosterlnfo_1",
	"st_ui_boosterlnfo_2",
	"st_ui_boosterlnfo_3",
	"st_ui_boosterlnfo_4",
	"st_ui_boosterlnfo_5",
	"st_ui_boosterlnfo_6",
	"st_ui_boosterlnfo_7",
	"st_ui_boosterlnfo_8",
	"st_ui_boosterlnfo_9",
	"st_ui_boosterlnfo_10",
	"st_ui_boosterlnfo_11",
	"st_ui_boosterlnfo_12",
	"st_ui_boosterlnfo_13",
	"st_ui_boosterlnfo_14",
	"st_ui_boosterlnfo_15"
};
void CUIBoosterInfo::InitFromXml(CUIXml& xml)
{
	LPCSTR base	= "booster_params";
	XML_NODE* stored_root = xml.GetLocalRoot();
	XML_NODE* base_node   = xml.NavigateToNode( base, 0 );
	if(!base_node)
		return;

	CUIXmlInit::InitWindow(xml, base, 0, this);
	xml.SetLocalRoot(base_node);
	
	m_Prop_line = xr_new<CUIStatic>();
	AttachChild(m_Prop_line);
	m_Prop_line->SetAutoDelete(false);	
	CUIXmlInit::InitStatic(xml, "prop_line", 0, m_Prop_line);

	for(u32 i = 0; i < eBoostExplImmunity; ++i)
	{
		m_booster_items[i] = xr_new<UIBoosterInfoItem>();
		m_booster_items[i]->Init(xml, ef_boosters_section_names[i]);
		m_booster_items[i]->SetAutoDelete(false);

		LPCSTR name = CStringTable().translate(boost_influence_caption_bttr[i]).c_str();
		m_booster_items[i]->SetCaption(name);
		xml.SetLocalRoot(base_node);
	}

	m_booster_satiety = xr_new<UIBoosterInfoItem>();
	m_booster_satiety->Init(xml, "boost_satiety");
	m_booster_satiety->SetAutoDelete(false);
	LPCSTR name = CStringTable().translate("ui_inv_satiety").c_str();
	m_booster_satiety->SetCaption(name);
	xml.SetLocalRoot( base_node );
	
	m_booster_sleep = xr_new<UIBoosterInfoItem>();
	m_booster_sleep->Init(xml, "boost_sleep");
	m_booster_sleep->SetAutoDelete(false);
	name = CStringTable().translate("ui_inv_sleep").c_str();
	m_booster_sleep->SetCaption(name);
	xml.SetLocalRoot(base_node);

	m_booster_thirst = xr_new<UIBoosterInfoItem>();
	m_booster_thirst->Init(xml, "boost_thirst");
	m_booster_thirst->SetAutoDelete(false);
	name = CStringTable().translate("ui_inv_thirst").c_str();
	m_booster_thirst->SetCaption(name);
	xml.SetLocalRoot(base_node);
	
	if (BttR_mode)
	{
		m_booster_vodka = xr_new<UIBoosterInfoItem>();
		m_booster_vodka->Init(xml, "boost_vodka");
		m_booster_vodka->SetAutoDelete(false);
		name = CStringTable().translate("ui_inv_vodka_engine").c_str();
		m_booster_vodka->SetCaption(name);
		xml.SetLocalRoot(base_node);

		m_booster_medkit = xr_new<UIBoosterInfoItem>();
		m_booster_medkit->Init(xml, "boost_medtik_psy");
		m_booster_medkit->SetAutoDelete(false);
		name = CStringTable().translate("ui_inv_medkit_engine").c_str();
		m_booster_medkit->SetCaption(name);
		xml.SetLocalRoot(base_node);

		m_booster_antizombin = xr_new<UIBoosterInfoItem>();
		m_booster_antizombin->Init(xml, "boost_antizombin");
		m_booster_antizombin->SetAutoDelete(false);
		name = CStringTable().translate("ui_inv_antizombin_engine").c_str();
		m_booster_antizombin->SetCaption(name);
		xml.SetLocalRoot(base_node);

		m_booster_psyblockade = xr_new<UIBoosterInfoItem>();
		m_booster_psyblockade->Init(xml, "boost_psyblockade");
		m_booster_psyblockade->SetAutoDelete(false);
		name = CStringTable().translate("ui_inv_psyblockade_engine").c_str();
		m_booster_psyblockade->SetCaption(name);
		xml.SetLocalRoot(base_node);

		m_booster_antirad = xr_new<UIBoosterInfoItem>();
		m_booster_antirad->Init(xml, "boost_antirad");
		m_booster_antirad->SetAutoDelete(false);
		name = CStringTable().translate("ui_inv_antirad").c_str();
		m_booster_antirad->SetCaption(name);
		xml.SetLocalRoot(base_node);

		m_booster_kurr = xr_new<UIBoosterInfoItem>();
		m_booster_kurr->Init(xml, "boost_kurr");
		m_booster_kurr->SetAutoDelete(false);
		name = CStringTable().translate("ui_inv_kurr").c_str();
		m_booster_kurr->SetCaption(name);
		xml.SetLocalRoot(base_node);

		m_booster_komponent = xr_new<UIBoosterInfoItem>();
		m_booster_komponent->Init(xml, "boost_komponent");
		m_booster_komponent->SetAutoDelete(false);
		name = CStringTable().translate("ui_inv_komponent_art_mod").c_str();
		m_booster_komponent->SetCaption(name);
		xml.SetLocalRoot(base_node);
	}

	m_booster_anabiotic = xr_new<UIBoosterInfoItem>();
	m_booster_anabiotic->Init(xml, "boost_anabiotic");
	m_booster_anabiotic->SetAutoDelete(false);
	name = CStringTable().translate("ui_inv_survive_surge").c_str();
	m_booster_anabiotic->SetCaption(name);
	xml.SetLocalRoot( base_node );

	m_booster_time = xr_new<UIBoosterInfoItem>();
	m_booster_time->Init(xml, "boost_time");
	m_booster_time->SetAutoDelete(false);
	name = CStringTable().translate("ui_inv_effect_time").c_str();
	m_booster_time->SetCaption(name);

	xml.SetLocalRoot( stored_root );
}

void CUIBoosterInfo::SetInfo( shared_str const& section )
{
	DetachAll();
	AttachChild( m_Prop_line );

	CActor* actor = smart_cast<CActor*>( Level().CurrentViewEntity() );
	if ( !actor )
	{
		return;
	}

	CEntityCondition::BOOSTER_MAP boosters = actor->conditions().GetCurBoosterInfluences();

	float val = 0.0f, max_val = 1.0f;
	Fvector2 pos;
	float h = m_Prop_line->GetWndPos().y+m_Prop_line->GetWndSize().y;

	for (u32 i = 0; i < eBoostExplImmunity; ++i)
	{
		if(pSettings->line_exist(section.c_str(), ef_boosters_section_names[i]))
		{
			val	= pSettings->r_float(section, ef_boosters_section_names[i]);
			if(fis_zero(val))
				continue;

			EBoostParams type = (EBoostParams)i;
			switch(type)
			{
				case eBoostHpRestore: 
				case eBoostPowerRestore: 
				case eBoostBleedingRestore: 
				case eBoostMaxWeight: 
					max_val = 1.0f;
					break;
				case eBoostRadiationRestore: 
					max_val = -1.0f;
					break;
				case eBoostBurnImmunity: 
					max_val = actor->conditions().GetZoneMaxPower(ALife::infl_fire);
					break;
				case eBoostShockImmunity: 
					max_val = actor->conditions().GetZoneMaxPower(ALife::infl_electra);
					break;
				case eBoostRadiationImmunity: 
				case eBoostRadiationProtection: 
					max_val = actor->conditions().GetZoneMaxPower(ALife::infl_rad);
					break;
				case eBoostTelepaticImmunity: 
				case eBoostTelepaticProtection: 
					max_val = actor->conditions().GetZoneMaxPower(ALife::infl_psi);
					break;
				case eBoostChemicalBurnImmunity: 
				case eBoostChemicalBurnProtection: 
					max_val = actor->conditions().GetZoneMaxPower(ALife::infl_acid);
					break;
			}
			val /= max_val;
			m_booster_items[i]->SetValue(val);

			pos.set(m_booster_items[i]->GetWndPos());
			pos.y = h;
			m_booster_items[i]->SetWndPos(pos);

			h += m_booster_items[i]->GetWndSize().y;
			AttachChild(m_booster_items[i]);
		}
	}

	if(pSettings->line_exist(section.c_str(), "eat_satiety"))
	{
		val	= pSettings->r_float(section, "eat_satiety");
		if(!fis_zero(val))
		{
			m_booster_satiety->SetValue(val);
			pos.set(m_booster_satiety->GetWndPos());
			pos.y = h;
			m_booster_satiety->SetWndPos(pos);

			h += m_booster_satiety->GetWndSize().y;
			AttachChild(m_booster_satiety);
		}
	}

	if(!xr_strcmp(section.c_str(), "drug_anabiotic"))
	{
		pos.set(m_booster_anabiotic->GetWndPos());
		pos.y = h;
		m_booster_anabiotic->SetWndPos(pos);

		h += m_booster_anabiotic->GetWndSize().y;
		AttachChild(m_booster_anabiotic);
	}

	if(pSettings->line_exist(section.c_str(), "boost_time"))
	{
		val	= pSettings->r_float(section, "boost_time");
		if(!fis_zero(val))
		{
			m_booster_time->SetValue(val);
			pos.set(m_booster_time->GetWndPos());
			pos.y = h;
			m_booster_time->SetWndPos(pos);

			h += m_booster_time->GetWndSize().y;
			AttachChild(m_booster_time);
		}
	}

	if (pSettings->line_exist(section.c_str(), "eat_sleep"))
	{
		val = pSettings->r_float(section, "eat_sleep");
		if (!fis_zero(val))
		{
			m_booster_sleep->SetValue(val);
			pos.set(m_booster_sleep->GetWndPos());
			pos.y = h;
			m_booster_sleep->SetWndPos(pos);

			h += m_booster_sleep->GetWndSize().y;
			AttachChild(m_booster_sleep);
		}
	}

	if (BttR_mode)
	{
		if (!xr_strcmp(section.c_str(), "vodka"))
		{
			pos.set(m_booster_vodka->GetWndPos());
			pos.y = h;
			m_booster_vodka->SetWndPos(pos);

			h += m_booster_vodka->GetWndSize().y;
			AttachChild(m_booster_vodka);
		}

		if (!xr_strcmp(section.c_str(), "medkit_scientic"))
		{
			pos.set(m_booster_medkit->GetWndPos());
			pos.y = h;
			m_booster_medkit->SetWndPos(pos);

			h += m_booster_medkit->GetWndSize().y;
			AttachChild(m_booster_medkit);
		}

		if (!xr_strcmp(section.c_str(), "antizombin"))
		{
			pos.set(m_booster_antizombin->GetWndPos());
			pos.y = h;
			m_booster_antizombin->SetWndPos(pos);

			h += m_booster_antizombin->GetWndSize().y;
			AttachChild(m_booster_antizombin);
		}

		if (!xr_strcmp(section.c_str(), "drug_psy_blockade"))
		{
			pos.set(m_booster_psyblockade->GetWndPos());
			pos.y = h;
			m_booster_psyblockade->SetWndPos(pos);

			h += m_booster_psyblockade->GetWndSize().y;
			AttachChild(m_booster_psyblockade);
		}

		if (!xr_strcmp(section.c_str(), "antirad"))
		{
			pos.set(m_booster_antirad->GetWndPos());
			pos.y = h;
			m_booster_antirad->SetWndPos(pos);
			h += m_booster_antirad->GetWndSize().y;
			AttachChild(m_booster_antirad);
		}

		if (pSettings->line_exist(section.c_str(), "eat_kurr"))
		{
			val = pSettings->r_float(section, "eat_kurr");
			if (!fis_zero(val))
			{
				m_booster_kurr->SetValue(val);
				pos.set(m_booster_kurr->GetWndPos());
				pos.y = h;
				m_booster_kurr->SetWndPos(pos);

				h += m_booster_kurr->GetWndSize().y;
				AttachChild(m_booster_kurr);
			}
		}

		if (!xr_strcmp(section.c_str(), "gravi_kapsula"))
		{
			pos.set(m_booster_komponent->GetWndPos());
			pos.y = h;
			m_booster_komponent->SetWndPos(pos);

			h += m_booster_komponent->GetWndSize().y;
			AttachChild(m_booster_komponent);
		}

		if (!xr_strcmp(section.c_str(), "fire_kapsula"))
		{
			pos.set(m_booster_komponent->GetWndPos());
			pos.y = h;
			m_booster_komponent->SetWndPos(pos);

			h += m_booster_komponent->GetWndSize().y;
			AttachChild(m_booster_komponent);
		}

		if (!xr_strcmp(section.c_str(), "chim_kapsula"))
		{
			pos.set(m_booster_komponent->GetWndPos());
			pos.y = h;
			m_booster_komponent->SetWndPos(pos);

			h += m_booster_komponent->GetWndSize().y;
			AttachChild(m_booster_komponent);
		}

		if (!xr_strcmp(section.c_str(), "snow_kapsula"))
		{
			pos.set(m_booster_komponent->GetWndPos());
			pos.y = h;
			m_booster_komponent->SetWndPos(pos);

			h += m_booster_komponent->GetWndSize().y;
			AttachChild(m_booster_komponent);
		}

		if (!xr_strcmp(section.c_str(), "electro_kapsula"))
		{
			pos.set(m_booster_komponent->GetWndPos());
			pos.y = h;
			m_booster_komponent->SetWndPos(pos);

			h += m_booster_komponent->GetWndSize().y;
			AttachChild(m_booster_komponent);
		}

		if (!xr_strcmp(section.c_str(), "radio_kapsula"))
		{
			pos.set(m_booster_komponent->GetWndPos());
			pos.y = h;
			m_booster_komponent->SetWndPos(pos);

			h += m_booster_komponent->GetWndSize().y;
			AttachChild(m_booster_komponent);
		}
	}

	if (pSettings->line_exist(section.c_str(), "eat_thirst"))
	{
		val = pSettings->r_float(section, "eat_thirst");
		if (!fis_zero(val))
		{
			m_booster_thirst->SetValue(val);
			pos.set(m_booster_thirst->GetWndPos());
			pos.y = h;
			m_booster_thirst->SetWndPos(pos);

			h += m_booster_thirst->GetWndSize().y;
			AttachChild(m_booster_thirst);
		}
	}
	SetHeight(h);
}

/// ----------------------------------------------------------------

UIBoosterInfoItem::UIBoosterInfoItem()
{
	m_caption				= NULL;
	m_value					= NULL;
	m_magnitude				= 1.0f;
	m_show_sign				= false;
	
	m_unit_str._set			("");
	m_texture_minus._set	("");
	m_texture_plus._set		("");
}

UIBoosterInfoItem::~UIBoosterInfoItem()
{
}

void UIBoosterInfoItem::Init(CUIXml& xml, LPCSTR section)
{
	CUIXmlInit::InitWindow(xml, section, 0, this);
	xml.SetLocalRoot(xml.NavigateToNode(section));

	m_caption   = UIHelper::CreateStatic(xml, "caption", this);
	m_value     = UIHelper::CreateTextWnd(xml, "value",   this);
	m_magnitude = xml.ReadAttribFlt("value", 0, "magnitude", 1.0f);
	m_show_sign = (xml.ReadAttribInt("value", 0, "show_sign", 1) == 1);
	
	LPCSTR unit_str = xml.ReadAttrib("value", 0, "unit_str", "");
	m_unit_str._set(CStringTable().translate(unit_str));
	
	LPCSTR texture_minus = xml.Read("texture_minus", 0, "");
	if(texture_minus && xr_strlen(texture_minus))
	{
		m_texture_minus._set(texture_minus);
		
		LPCSTR texture_plus = xml.Read("caption:texture", 0, "");
		m_texture_plus._set(texture_plus);
		VERIFY(m_texture_plus.size());
	}
}

void UIBoosterInfoItem::SetCaption(LPCSTR name)
{
	m_caption->TextItemControl()->SetText(name);
}

void UIBoosterInfoItem::SetValue(float value)
{
	value *= m_magnitude;
	string32 buf;
	if(m_show_sign)
		xr_sprintf(buf, "%+.0f", value);
	else
		xr_sprintf(buf, "%.0f", value);
	
	LPSTR str;
	if(m_unit_str.size())
		STRCONCAT(str, buf, " ", m_unit_str.c_str());
	else
		STRCONCAT(str, buf);

	m_value->SetText(str);

	bool positive = (value >= 0.0f);
	m_value->SetTextColor(color_rgba(170,170,170,255));

	if(m_texture_minus.size())
	{
		if(positive)
			m_caption->InitTexture(m_texture_plus.c_str());
		else
			m_caption->InitTexture(m_texture_minus.c_str());
	}
}
