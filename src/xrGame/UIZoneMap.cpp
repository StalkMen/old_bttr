#include "stdafx.h"
#include "uizonemap.h"

#include "InfoPortion.h"
#include "Pda.h"

#include "Grenade.h"
#include "level.h"
#include "game_cl_base.h"

#include "actor.h"
#include "ai_space.h"
#include "game_graph.h"

#include "ui/UIMap.h"
#include "ui/UIXmlInit.h"
#include "ui/UIHelper.h"
#include "ui/UIInventoryUtilities.h"
//////////////////////////////////////////////////////////////////////////
extern u32 type_hud_token;
extern float minimap_zoom_factor;

#define ZONE_MAP_XML_DEF "zone_map.xml"

#define ZONE_MAP_XML_COP "zone_map\\maingame_zone_map_cop.xml"
#define ZONE_MAP_XML_CMP "zone_map\\maingame_zone_map_cmp.xml"
#define ZONE_MAP_XML_STS "zone_map\\maingame_zone_map_stason.xml"
#define ZONE_MAP_XML_COC "zone_map\\maingame_zone_map_coc.xml"
#define ZONE_MAP_XML_VV  "zone_map\\maingame_zone_map_vv.xml"
#define ZONE_MAP_XML_CS	 "zone_map\\maingame_zone_map_cs.xml"
#define ZONE_MAP_XML_SOC "zone_map\\maingame_zone_map_soc.xml"
#define ZONE_MAP_XML_LA  "zone_map\\maingame_zone_map_la.xml"
#define ZONE_MAP_XML_MIS "zone_map\\maingame_zone_map_mis.xml"
#define ZONE_MAP_XML_PCH "zone_map\\maingame_zone_map_pch.xml"

CUIZoneMap::CUIZoneMap()
:m_current_map_idx(u8(-1)),
visible(true)
{	
}

CUIZoneMap::~CUIZoneMap()
{
}

void CUIZoneMap::Init()
{
    CUIXml uiXml;

    if (type_hud_token == 0)
        if (BttR_mode)
            uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_COC);
        else
            uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_DEF);

    if (type_hud_token == 1)
        uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_COP);

    if (type_hud_token == 2)
        uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_CMP);

    if (type_hud_token == 3)
        uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_STS);

    if (type_hud_token == 8)
        uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_MIS);

    if (type_hud_token == 6 || type_hud_token == 5 || type_hud_token == 7 || type_hud_token == 4 || type_hud_token == 9)
    {
        if (type_hud_token == 6)
            uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_VV);

        if (type_hud_token == 4)
            uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_CS);

        if (type_hud_token == 5)
            uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_SOC);

        if (type_hud_token == 7)
            uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_LA);

        if (type_hud_token == 9)
            uiXml.Load(CONFIG_PATH, UI_PATH, ZONE_MAP_XML_PCH);

        CUIXmlInit xml_init;
        xml_init.InitStatic(uiXml, "minimap:background", 0, &m_background);
        xml_init.InitWindow(uiXml, "minimap:level_frame", 0, &m_clipFrame);

        xml_init.InitStatic(uiXml, "minimap:center", 0, &m_center);

        m_clock_wnd = UIHelper::CreateStatic(uiXml, "minimap:clock_wnd", &m_background);

        if (IsGameTypeSingle())
        {
            if (UI().is_widescreen())
            {
                m_pointerDistanceText_16_9 =
                    UIHelper::CreateStatic(uiXml, "minimap:background:dist_text_map_16_9", &m_background);
            }
            else
            {
                m_pointerDistanceText =
                    UIHelper::CreateStatic(uiXml, "minimap:background:dist_text_map_4_3", &m_background);
            }
        }

        m_activeMap = xr_new<CUIMiniMap>();
        m_clipFrame.AttachChild(m_activeMap);
        m_activeMap->SetAutoDelete(true);

        m_activeMap->EnableHeading(true);
        xml_init.InitStatic(uiXml, "minimap:compass", 0, &m_compass);
        m_background.AttachChild(&m_compass);

        m_clipFrame.AttachChild(&m_center);

        m_Counter_text.SetText("");
        visible = true;

        xml_init.InitStatic(uiXml, "minimap:static_counter", 0, &m_Counter);
        m_background.AttachChild(&m_Counter);
        xml_init.InitTextWnd(uiXml, "minimap:static_counter:text_static", 0, &m_Counter_text);
        m_Counter.AttachChild(&m_Counter_text);

        m_center.SetWndPos(Fvector2().set(m_clipFrame.GetWidth() / 2.0f, m_clipFrame.GetHeight() / 2.0f));
    }
    else
    {
        CUIXmlInit xml_init;
        xml_init.InitStatic(uiXml, "minimap:background", 0, &m_background);
        xml_init.InitWindow(uiXml, "minimap:level_frame", 0, &m_clipFrame);
        xml_init.InitStatic(uiXml, "minimap:center", 0, &m_center);

        m_clock_wnd = UIHelper::CreateStatic(uiXml, "minimap:clock_wnd", &m_background);

        m_activeMap = xr_new<CUIMiniMap>();
        m_clipFrame.AttachChild(m_activeMap);
        m_activeMap->SetAutoDelete(true);

        m_activeMap->EnableHeading(true);
        xml_init.InitStatic(uiXml, "minimap:compass", 0, &m_compass);
        m_background.AttachChild(&m_compass);

        m_clipFrame.AttachChild(&m_center);

        m_Counter_text.SetText("");
        visible = true;

        Fvector2 sz_k = m_clipFrame.GetWndSize();
        Fvector2 sz = sz_k;
        {
            float k = UI().get_current_kx();

            sz.y *= UI_BASE_HEIGHT * k;
            sz.x = sz.y / k;

            m_clipFrame.SetWndSize(sz);

            Fvector2 p = m_clipFrame.GetWndPos();
            p.mul(UI_BASE_HEIGHT);
            m_clipFrame.SetWndPos(p);

            m_background.SetHeight(m_background.GetHeight() * UI_BASE_HEIGHT);
            m_background.SetWidth(m_background.GetHeight() * k);
        }

        Fvector2 map_center;
        m_clipFrame.GetWndRect().getcenter(map_center);
        m_background.SetWndPos(map_center);

        Fvector2 cp;
        cp.x = m_clipFrame.GetWidth() / 2.0f;
        cp.y = m_clipFrame.GetHeight() / 2.0f;
        m_center.SetWndPos(cp);

        Fvector2 rel_pos = m_compass.GetWndPos();
        rel_pos.mul(m_background.GetWndSize());
        m_compass.SetWndPos(rel_pos);

        rel_pos = m_clock_wnd->GetWndPos();
        rel_pos.mul(m_background.GetWndSize());
        m_clock_wnd->SetWndPos(rel_pos);

        if (IsGameTypeSingle())
        {
            xml_init.InitStatic(uiXml, "minimap:static_counter", 0, &m_Counter);
            m_background.AttachChild(&m_Counter);
            xml_init.InitTextWnd(uiXml, "minimap:static_counter:text_static", 0, &m_Counter_text);
            m_Counter.AttachChild(&m_Counter_text);

            rel_pos = m_Counter.GetWndPos();
            rel_pos.mul(m_background.GetWndSize());
            m_Counter.SetWndPos(rel_pos);
        }
    }
}

void CUIZoneMap::Render			()
{
	if ( !visible )
		return;

    if (psActorFlags.test(AF_DISABLE_MINIMAP))
        return;

	m_clipFrame.Draw	();
	m_background.Draw	();
}

void CUIZoneMap::Update()
{
    if (psActorFlags.test(AF_DISABLE_MINIMAP))
        return;

	CActor* pActor = smart_cast<CActor*>( Level().CurrentViewEntity() );
	if ( !pActor ) return;

	if ( !( Device.dwFrame % 20 ) && IsGameTypeSingle() )
	{
		string16	text_str;
		xr_strcpy( text_str, sizeof(text_str), "" );

		CPda* pda = pActor->GetPDA();
		if ( pda )
		{
			u32 cn = pda->ActiveContactsNum();
			if ( cn > 0 )
			{
				xr_sprintf( text_str, sizeof(text_str), "%d", cn );
			}
		}
		m_Counter_text.SetText( text_str );
	}

	UpdateRadar( Device.vCameraPosition );
	float h, p;
	Device.vCameraDirection.getHP( h, p );
	SetHeading( -h );

	m_clock_wnd->TextItemControl()->SetText( InventoryUtilities::GetGameTimeAsString( InventoryUtilities::etpTimeToMinutes ).c_str() );
}

void CUIZoneMap::SetHeading		(float angle)
{
	m_activeMap->SetHeading(angle);
	m_compass.SetHeading(angle);
};

void CUIZoneMap::UpdateRadar(Fvector pos)
{
    m_clipFrame.Update();
    m_background.Update();
    m_activeMap->SetActivePoint(pos);

    if (type_hud_token == 6 || type_hud_token == 5 || type_hud_token == 7 || type_hud_token == 4 || type_hud_token == 9)
    {
        if (IsGameTypeSingle())
        {
            if (m_activeMap->GetPointerDistance() > 0.5f)
            {
                string64 str;
                sprintf_s(str, "%.0f ì", m_activeMap->GetPointerDistance());
                if (UI().is_widescreen())
                {
                    m_pointerDistanceText_16_9->TextItemControl()->SetText(str);
                }
                else
                {
                    m_pointerDistanceText->TextItemControl()->SetText(str);
                }
            }
            else
            {
                if (UI().is_widescreen())
                {
                    m_pointerDistanceText_16_9->TextItemControl()->SetText("");
                }
                else
                {
                    m_pointerDistanceText->TextItemControl()->SetText("");
                }
            }
        }
    }

}

bool CUIZoneMap::ZoomIn()
{
	return true;
}

bool CUIZoneMap::ZoomOut()
{
	return true;
}

void CUIZoneMap::SetupCurrentMap()
{
	m_activeMap->Initialize			(Level().name(), "hud\\default");

	Frect r;
	m_clipFrame.GetAbsoluteRect		(r);	
	m_activeMap->WorkingArea().set	(r);
	
	Fvector2						wnd_size;
	float zoom_factor 				= float(m_clipFrame.GetWidth()) / 100.0f * minimap_zoom_factor;

	LPCSTR ln						= Level().name().c_str();
	if(	pGameIni->section_exist(ln) )
	{
		if(pGameIni->line_exist(ln, "minimap_zoom"))
			zoom_factor *= pGameIni->r_float(ln, "minimap_zoom");
	}else
	if(g_pGameLevel->pLevel->section_exist("minimap_zoom"))
	{
		zoom_factor *= g_pGameLevel->pLevel->r_float("minimap_zoom", "value");
	}
	wnd_size.x						= m_activeMap->BoundRect().width()*zoom_factor;
	wnd_size.y						= m_activeMap->BoundRect().height()*zoom_factor;
	m_activeMap->SetWndSize			(wnd_size);
}

void CUIZoneMap::OnSectorChanged(int sector)
{
	if(!g_pGameLevel->pLevel->section_exist("sub_level_map") )
		return;
	u8			map_idx = u8(-1);
	string64	s_sector;
	xr_sprintf	(s_sector, "%d", sector);
	
	if(!g_pGameLevel->pLevel->line_exist("sub_level_map", s_sector) )
		return;

	map_idx		= g_pGameLevel->pLevel->r_u8("sub_level_map", s_sector);
	if(m_current_map_idx == map_idx)
		return;

	m_current_map_idx = map_idx;

	string_path sub_texture;
	xr_sprintf(sub_texture,"%s#%d", m_activeMap->m_texture.c_str(), m_current_map_idx);
	
	if(map_idx==u8(-1))
		xr_sprintf(sub_texture,"%s", m_activeMap->m_texture.c_str());

	m_activeMap->InitTextureEx(sub_texture, m_activeMap->m_shader_name.c_str());
}

void CUIZoneMap::Counter_ResetClrAnimation()
{
	m_Counter_text.ResetColorAnimation();
}
