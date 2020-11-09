#include "stdafx.h"
#include "igame_level.h"

//#include "xr_effgamma.h"
#include "x_ray.h"
#include "xr_ioconsole.h"
#include "xr_ioc_cmd.h"
//#include "fbasicvisual.h"
#include "cameramanager.h"
#include "environment.h"
#include "xr_input.h"
#include "CustomHUD.h"

#include "../Include/xrRender/RenderDeviceRender.h"

#include "xr_object.h"

xr_token* vid_quality_token = NULL;

xr_token vid_bpp_token[] =
{
    {"16", 16},
    {"32", 32},
    {0, 0}
};
//-----------------------------------------------------------------------

void IConsole_Command::add_to_LRU(shared_str const& arg)
{
    if (arg.size() == 0 || bEmptyArgsHandled)
    {
        return;
    }

    bool dup = (std::find(m_LRU.begin(), m_LRU.end(), arg) != m_LRU.end());
    if (!dup)
    {
        m_LRU.push_back(arg);
        if (m_LRU.size() > LRU_MAX_COUNT)
        {
            m_LRU.erase(m_LRU.begin());
        }
    }
}

void IConsole_Command::add_LRU_to_tips(vecTips& tips)
{
    vecLRU::reverse_iterator it_rb = m_LRU.rbegin();
    vecLRU::reverse_iterator it_re = m_LRU.rend();
    for (; it_rb != it_re; ++it_rb)
    {
        tips.push_back((*it_rb));
    }
}

// =======================================================

class CCC_Quit : public IConsole_Command
{
public:
    CCC_Quit(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        // TerminateProcess(GetCurrentProcess(),0);
        Console->Hide();
        Engine.Event.Defer("KERNEL:disconnect");
        Engine.Event.Defer("KERNEL:quit");
    }
};
//-----------------------------------------------------------------------
#ifdef DEBUG_MEMORY_MANAGER
class CCC_MemStat : public IConsole_Command
{
public:
    CCC_MemStat(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        string_path fn;
        if (args&&args[0]) xr_sprintf(fn, sizeof(fn), "%s.dump", args);
        else strcpy_s_s(fn, sizeof(fn), "x:\\$memory$.dump");
        Memory.mem_statistic(fn);
        // g_pStringContainer->dump ();
        // g_pSharedMemoryContainer->dump ();
    }
};
#endif // DEBUG_MEMORY_MANAGER

#ifdef DEBUG_MEMORY_MANAGER
class CCC_DbgMemCheck : public IConsole_Command
{
public:
    CCC_DbgMemCheck(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args) { if (Memory.debug_mode) { Memory.dbg_check(); } else { Msg("~ Run with -mem_debug options."); } }
};
#endif // DEBUG_MEMORY_MANAGER

class CCC_DbgStrCheck : public IConsole_Command
{
public:
    CCC_DbgStrCheck(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args) { g_pStringContainer->verify(); }
};

class CCC_DbgStrDump : public IConsole_Command
{
public:
    CCC_DbgStrDump(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args) { g_pStringContainer->dump(); }
};

//-----------------------------------------------------------------------
class CCC_MotionsStat : public IConsole_Command
{
public:
    CCC_MotionsStat(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        //g_pMotionsContainer->dump();
        // TODO: move this console commant into renderer
        VERIFY(0);
    }
};
class CCC_TexturesStat : public IConsole_Command
{
public:
    CCC_TexturesStat(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        Device.DumpResourcesMemoryUsage();
        //Device.Resources->_DumpMemoryUsage();
        // TODO: move this console commant into renderer
        //VERIFY(0);
    }
};
//-----------------------------------------------------------------------
class CCC_E_Dump : public IConsole_Command
{
public:
    CCC_E_Dump(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        Engine.Event.Dump();
    }
};
class CCC_E_Signal : public IConsole_Command
{
public:
    CCC_E_Signal(LPCSTR N) : IConsole_Command(N) { };
    virtual void Execute(LPCSTR args)
    {
        char Event[128], Param[128];
        Event[0] = 0;
        Param[0] = 0;
        sscanf(args, "%[^,],%s", Event, Param);
        Engine.Event.Signal(Event, (u64)Param);
    }
};



//-----------------------------------------------------------------------
class CCC_Help : public IConsole_Command
{
public:
    CCC_Help(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        Log("- --- Command listing: start ---");
        CConsole::vecCMD_IT it;
        for (it = Console->Commands.begin(); it != Console->Commands.end(); it++)
        {
            IConsole_Command& C = *(it->second);
            TStatus _S;
            C.Status(_S);
            TInfo _I;
            C.Info(_I);

            Msg("%-20s (%-10s) --- %s", C.Name(), _S, _I);
        }
        Log("Key: Ctrl + A         === Select all ");
        Log("Key: Ctrl + C         === Copy to clipboard ");
        Log("Key: Ctrl + V         === Paste from clipboard ");
        Log("Key: Ctrl + X         === Cut to clipboard ");
        Log("Key: Ctrl + Z         === Undo ");
        Log("Key: Ctrl + Insert    === Copy to clipboard ");
        Log("Key: Shift + Insert   === Paste from clipboard ");
        Log("Key: Shift + Delete   === Cut to clipboard ");
        Log("Key: Insert           === Toggle mode <Insert> ");
        Log("Key: Back / Delete          === Delete symbol left / right ");

        Log("Key: Up   / Down            === Prev / Next command in tips list ");
        Log("Key: Ctrl + Up / Ctrl + Down === Prev / Next executing command ");
        Log("Key: Left, Right, Home, End {+Shift/+Ctrl}       === Navigation in text ");
        Log("Key: PageUp / PageDown      === Scrolling history ");
        Log("Key: Tab  / Shift + Tab     === Next / Prev possible command from list");
        Log("Key: Enter  / NumEnter      === Execute current command ");

        Log("- --- Command listing: end ----");
    }
};


XRCORE_API void _dump_open_files(int mode);
class CCC_DumpOpenFiles : public IConsole_Command
{
public:
    CCC_DumpOpenFiles(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = FALSE; };
    virtual void Execute(LPCSTR args)
    {
        int _mode = atoi(args);
        _dump_open_files(_mode);
    }
};

//-----------------------------------------------------------------------
class CCC_SaveCFG : public IConsole_Command
{
public:
    CCC_SaveCFG(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        string_path cfg_full_name;
        xr_strcpy(cfg_full_name, (xr_strlen(args) > 0) ? args : Console->ConfigFile);

        bool b_abs_name = xr_strlen(cfg_full_name) > 2 && cfg_full_name[1] == ':';

        if (!b_abs_name)
            FS.update_path(cfg_full_name, "$app_data_root$", cfg_full_name);

        if (strext(cfg_full_name))
            *strext(cfg_full_name) = 0;
        xr_strcat(cfg_full_name, ".ltx");

        BOOL b_allow = TRUE;
        if (FS.exist(cfg_full_name))
            b_allow = SetFileAttributes(cfg_full_name, FILE_ATTRIBUTE_NORMAL);

        if (b_allow)
        {
            IWriter* F = FS.w_open(cfg_full_name);
            CConsole::vecCMD_IT it;
            for (it = Console->Commands.begin(); it != Console->Commands.end(); it++)
                it->second->Save(F);
            FS.w_close(F);
            Msg("~ file [%s] saved successfully", cfg_full_name);
        }
        else
            Msg("!Cannot store config file [%s]", cfg_full_name);
    }
};
CCC_LoadCFG::CCC_LoadCFG(LPCSTR N) : IConsole_Command(N)
{};

void CCC_LoadCFG::Execute(LPCSTR args)
{
    Msg("Executing config-script \"%s\"...", args);
    string_path cfg_name;

    xr_strcpy(cfg_name, args);
    if (strext(cfg_name)) *strext(cfg_name) = 0;
    xr_strcat(cfg_name, ".ltx");

    string_path cfg_full_name;

    FS.update_path(cfg_full_name, "$app_data_root$", cfg_name);

    if (NULL == FS.exist(cfg_full_name))
        FS.update_path(cfg_full_name, "$fs_root$", cfg_name);

    if (NULL == FS.exist(cfg_full_name))
        xr_strcpy(cfg_full_name, cfg_name);

    IReader* F = FS.r_open(cfg_full_name);

    string1024 str;
    if (F != NULL)
    {
        while (!F->eof())
        {
            F->r_string(str, sizeof(str));
            if (allow(str))
                Console->Execute(str);
        }
        FS.r_close(F);
        Msg("[%s] successfully loaded.", cfg_full_name);
    }
    else
    {
        Msg("! Cannot open script file [%s]", cfg_full_name);
    }
}

CCC_LoadCFG_custom::CCC_LoadCFG_custom(LPCSTR cmd)
    :CCC_LoadCFG(cmd)
{
    xr_strcpy(m_cmd, cmd);
};
bool CCC_LoadCFG_custom::allow(LPCSTR cmd)
{
    return (cmd == strstr(cmd, m_cmd));
};

//-----------------------------------------------------------------------
class CCC_Start : public IConsole_Command
{
    void parse(LPSTR dest, LPCSTR args, LPCSTR name)
    {
        dest[0] = 0;
        if (strstr(args, name))
        {
            std::string str = strstr(args, name) + xr_strlen(name);
            std::regex Reg("\\(([^)]+)\\)");
            std::smatch results;
            if (std::regex_search(str, results, Reg))
                strcpy(dest, results[1].str().c_str());
        }
    }

    void protect_Name_strlwr(LPSTR str)
    {
        string4096 out;
        xr_strcpy(out, sizeof(out), str);
        strlwr(str);

        LPCSTR name_str = "name=";
        LPCSTR name1 = strstr(str, name_str);
        if (!name1 || !xr_strlen(name1))
        {
            return;
        }
        int begin_p = xr_strlen(str) - xr_strlen(name1) + xr_strlen(name_str);
        if (begin_p < 1)
        {
            return;
        }

        LPCSTR name2 = strchr(name1, '/');
        int end_p = xr_strlen(str) - ((name2) ? xr_strlen(name2) : 0);
        if (begin_p >= end_p)
        {
            return;
        }
        for (int i = begin_p; i < end_p; ++i)
        {
            str[i] = out[i];
        }
    }
public:
    CCC_Start(LPCSTR N) : IConsole_Command(N) { bLowerCaseArgs = false; };
    virtual void Execute(LPCSTR args)
    {
        /* if (g_pGameLevel) {
         Log ("! Please disconnect/unload first");
         return;
         }
         */
        string4096 op_server, op_client, op_demo;
        op_server[0] = 0;
        op_client[0] = 0;

        parse(op_server, args, "server"); // 1. server
        parse(op_client, args, "client"); // 2. client
        parse(op_demo, args, "demo"); // 3. demo

        strlwr(op_server);
        protect_Name_strlwr(op_client);

        if (!op_client[0] && strstr(op_server, "single"))
            xr_strcpy(op_client, "localhost");

        if ((0 == xr_strlen(op_client)) && (0 == xr_strlen(op_demo)))
        {
            Log("! Can't start game without client. Arguments: '%s'.", args);
            return;
        }
        if (g_pGameLevel)
            Engine.Event.Defer("KERNEL:disconnect");

        if (xr_strlen(op_demo))
        {
            Engine.Event.Defer("KERNEL:start_mp_demo", u64(xr_strdup(op_demo)), 0);
        }
        else
        {
            Engine.Event.Defer("KERNEL:start", u64(xr_strlen(op_server) ? xr_strdup(op_server) : 0), u64(xr_strdup(op_client)));
        }
    }
};

class CCC_Disconnect : public IConsole_Command
{
public:
    CCC_Disconnect(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        Engine.Event.Defer("KERNEL:disconnect");
    }
};
//-----------------------------------------------------------------------
class CCC_VID_Reset : public IConsole_Command
{
public:
    CCC_VID_Reset(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        if (Device.b_is_Ready)
        {
            Device.Reset();
        }
    }
};


class CCC_VidMode : public CCC_Token
{
    u32 _dummy;
public:
    CCC_VidMode(LPCSTR N) : CCC_Token(N, &_dummy, NULL) { bEmptyArgsHandled = FALSE; };
    virtual void Execute(LPCSTR args)
    {
        u32 _w, _h;
        int cnt = sscanf(args, "%dx%d", &_w, &_h);
        if (cnt == 2)
        {
            psCurrentVidMode[0] = _w;
            psCurrentVidMode[1] = _h;
        }
        else
        {
            Msg("! Wrong video mode [%s]", args);
            return;
        }
    }
    virtual void Status(TStatus& S)
    {
        xr_sprintf(S, sizeof(S), "%dx%d", psCurrentVidMode[0], psCurrentVidMode[1]);
    }
    virtual xr_token* GetToken() { return vid_mode_token; }
    virtual void Info(TInfo& I)
    {
        xr_strcpy(I, sizeof(I), "change screen resolution WxH");
    }

    virtual void fill_tips(vecTips& tips, u32 mode)
    {
        TStatus str, cur;
        Status(cur);

        bool res = false;
        xr_token* tok = GetToken();
        while (tok->name && !res)
        {
            if (!xr_strcmp(tok->name, cur))
            {
                xr_sprintf(str, sizeof(str), "%s (current)", tok->name);
                tips.push_back(str);
                res = true;
            }
            tok++;
        }
        if (!res)
        {
            tips.push_back("--- (current)");
        }
        tok = GetToken();
        while (tok->name)
        {
            tips.push_back(tok->name);
            tok++;
        }
    }

};
//-----------------------------------------------------------------------
class CCC_SND_Restart : public IConsole_Command
{
public:
    CCC_SND_Restart(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
    virtual void Execute(LPCSTR args)
    {
        Sound->_restart();
    }
};

//-----------------------------------------------------------------------
float ps_gamma = 0.f;
float ps_brightness = 0.f;
float ps_contrast = 0.f;

class CCC_Gamma : public CCC_Float
{
public:
    CCC_Gamma(LPCSTR N, float* V) : CCC_Float(N, V, 0.0f, 1.5f) {}

    virtual void Execute(LPCSTR args)
    {
        CCC_Float::Execute(args);
        Device.m_pRender->setGamma(1.f + ps_gamma);
        Device.m_pRender->setBrightness(1.f + ps_brightness);
        Device.m_pRender->setContrast(1.f + ps_contrast);
        Device.m_pRender->updateGamma();
    }
};

ENGINE_API BOOL r2_sun_static = FALSE;
ENGINE_API BOOL r2_advanced_pp = TRUE; // advanced post process and effects
ENGINE_API BOOL render_dx10_1 = FALSE;
ENGINE_API u32 renderer_value = 0;

class CCC_r2 : public CCC_Token
{
    typedef CCC_Token inherited;
public:
    CCC_r2(LPCSTR N) :inherited(N, &renderer_value, NULL) { renderer_value = 3; };
    virtual ~CCC_r2()
    {
        //free_render_mode_list();
    }
    virtual void Execute(LPCSTR args)
    {
        //fill_render_mode_list ();
        // vid_quality_token must be already created!
        tokens = vid_quality_token;

        inherited::Execute(args);

        //DX10
        {
            psDeviceFlags.set(rsR3, ((renderer_value == 0) || (renderer_value == 1)));
            render_dx10_1 = (renderer_value == 1);
        }
        //DX11
        psDeviceFlags.set(rsR4, (renderer_value >= 2));
    }

    virtual void Save(IWriter* F)
    {
        //fill_render_mode_list ();
        tokens = vid_quality_token;
        if (!strstr(Core.Params, "-r2"))
        {
            inherited::Save(F);
        }
    }
    virtual xr_token* GetToken()
    {
        tokens = vid_quality_token;
        return inherited::GetToken();
    }

};
#ifndef DEDICATED_SERVER
class CCC_soundDevice : public CCC_Token
{
    typedef CCC_Token inherited;
public:
    CCC_soundDevice(LPCSTR N) :inherited(N, &snd_device_id, NULL) {};
    virtual ~CCC_soundDevice()
    {}

    virtual void Execute(LPCSTR args)
    {
        GetToken();
        if (!tokens) return;
        inherited::Execute(args);
    }

    virtual void Status(TStatus& S)
    {
        GetToken();
        if (!tokens) return;
        inherited::Status(S);
    }

    virtual xr_token* GetToken()
    {
        tokens = snd_devices_token;
        return inherited::GetToken();
    }

    virtual void Save(IWriter* F)
    {
        GetToken();
        if (!tokens) return;
        inherited::Save(F);
    }
};
#endif
//-----------------------------------------------------------------------
class CCC_ExclusiveMode : public IConsole_Command
{
private:
    typedef IConsole_Command inherited;

public:
    CCC_ExclusiveMode(LPCSTR N) :
        inherited(N)
    {
    }

    virtual void Execute(LPCSTR args)
    {
        bool value = false;
        if (!xr_strcmp(args, "on"))
            value = true;
        else if (!xr_strcmp(args, "off"))
            value = false;
        else if (!xr_strcmp(args, "true"))
            value = true;
        else if (!xr_strcmp(args, "false"))
            value = false;
        else if (!xr_strcmp(args, "1"))
            value = true;
        else if (!xr_strcmp(args, "0"))
            value = false;
        else InvalidSyntax();

        pInput->exclusive_mode(value);
    }

    virtual void Save(IWriter* F)
    {
    }
};

class ENGINE_API CCC_HideConsole : public IConsole_Command
{
public:
    CCC_HideConsole(LPCSTR N) : IConsole_Command(N)
    {
        bEmptyArgsHandled = true;
    }

    virtual void Execute(LPCSTR args)
    {
        Console->Hide();
    }
    virtual void Status(TStatus& S)
    {
        S[0] = 0;
    }
    virtual void Info(TInfo& I)
    {
        xr_sprintf(I, sizeof(I), "hide console");
    }
};

extern int psNET_ClientUpdate;
extern int psNET_ClientPending;
extern int psNET_ServerUpdate;
extern int psNET_ServerPending;
extern int psNET_DedicatedSleep;
extern char psNET_Name[32];
extern Flags32 psEnvFlags;

////////////////////////////////////////////////////////////////////////////////////////////////////
ENGINE_API float psHUD_FOV_def = 0.35f; //--#SM+#--
ENGINE_API float psHUD_FOV = psHUD_FOV_def; //--#SM+#--
ENGINE_API BOOL game_value_discord_status = 1;

BOOL xrengint_noprefetch = 0;
BOOL ps_rs_loading_stages = 0;

extern int g_ErrorLineCount;
extern int	show_FPS_only = 0;

ENGINE_API float ps_r2_sun_shafts_min = 0.f;
ENGINE_API float ps_r2_sun_shafts_value = 1.f;

ENGINE_API u32 g_screenmode = 1;
xr_token screen_mode_tokens[] =
{
    {"Monitor_2k",      2},
    {"Monitor_4k",      1},
    {"Window_mode_1k",  0},
    {nullptr, 0}
};

extern void GetMonitorResolution(u32& horizontal, u32& vertical);

class CCC_Screenmode : public CCC_Token
{
public:
    CCC_Screenmode(LPCSTR N) : CCC_Token(N, &g_screenmode, screen_mode_tokens) {};

    virtual void Execute(LPCSTR args)
    {
        u32 prev_mode = g_screenmode;
        CCC_Token::Execute(args);

        if ((prev_mode != g_screenmode))
        {
            if (Device.b_is_Ready && (prev_mode == 2 || g_screenmode == 2))
                Device.Reset();

            if (g_screenmode == 0 || g_screenmode == 1)
            {
                u32 w, h;
                GetMonitorResolution(w, h);
                SetWindowLongPtr(Device.m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
                SetWindowPos(Device.m_hWnd, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);

                if (g_screenmode == 0)
                    SetWindowLongPtr(Device.m_hWnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
            }
        }

        RECT winRect;
        GetWindowRect(Device.m_hWnd, &winRect);
        ClipCursor(&winRect);
    }
};

xr_token FpsLockToken[] = {
  { "50 fps",  50 },
  { "59 fps",  59 },
  { "60 fps",  60 },
  { "74 fps",  74 },
  { "75 fps",  75 },
  { "80 fps",  80 },
  { "90 fps",  90 },
  { "100 fps", 100 },
  { "120 fps", 120 },
  { "121 fps", 121 },
  { "144 fps", 144 },
  { "165 fps", 165 },
  { "180 fps", 180 },
  { "240 fps", 240 },
  { "244 fps", 244 },
  { nullptr, 0 }
};

ENGINE_API u32 ps_r3_msaa = 0;
xr_token qmsaa_token[] = {
    { "mode_off",	0},
    { "mode_2x",	1},
    { "mode_4x",	2},
    { "mode_8x",	3},
    { nullptr, 0}
};

ENGINE_API u32	ps_r3_msaa_atest = 0;
xr_token qmsaa_atest_token[] = {
    { "mode_msaa_atest_off", 0},
    { "mode_msaa_atest_dx10_0", 1},
    { "mode_msaa_atest_dx10_1", 2},
    { nullptr, 0}
};

ENGINE_API u32 ps_r_sun_quality = 1;
xr_token qsun_quality_token[] = {
    { "st_opt_low",	0},
    { "st_opt_medium", 1},
    { "st_opt_high", 2},
    { "st_opt_ultra", 3},
    { "st_opt_extreme",	4},
    { nullptr, 0}
};

ENGINE_API u32 render_video_size = 512 + 1024;
xr_token render_video_size_token[] = {
{ "video_standart",			512 + 1024},
{ "video_average",			1024 + 1024},
{ "video_above_average",	2048 + 1024},
{ "video_high",				4096 + 2048},
{ "video_maximum",			8192},
{ nullptr,					0}
};

ENGINE_API u32	ps_r_type_aa = 0;
xr_token type_aa_token[] = {
{ "disable_aa",	  0},
{ "FXAA",		  1},
{ "DLAA",		  2},
{ "SMAA",		  3},
{ nullptr,		  0}
};

ENGINE_API u32 ps_sunshafts_mode = 0;
xr_token sunshafts_mode_token[] = {
    { "sunshafts_enable", 0 },
    { "sunshafts_disable", 1 },
    { nullptr, 0 }
};

ENGINE_API Flags32 p_engine_flags32 = { /*ITS_CLEAR_1_4_22*/ };

#include "device.h"
void CCC_Register()
{
#pragma TODO("OldSerpskiStalker. Новый команды тут")
    CMD1(CCC_Screenmode, "monitor_mode");
    //OldSerpskiStalker, переключение версий между BttR и 1.4.22
    {
        if (BttR_mode)
            p_engine_flags32.set(ITS_CLEAR_1_4_22, false);
        else
            p_engine_flags32.set(ITS_CLEAR_1_4_22, true);

        CMD3(CCC_Mask, "_game_preset_clear_version_call_of_chernobyl", &p_engine_flags32, ITS_CLEAR_1_4_22);
    }
    //Опции которые относятся к рендеру
    const static bool xrRender_options = true;
    if (xrRender_options)
    {
        CMD3(CCC_Token, "xrRenderDX_video_size", &render_video_size, render_video_size_token);
        CMD3(CCC_Token, "r2_sun_quality", &ps_r_sun_quality, qsun_quality_token);
        CMD3(CCC_Token, "r3_msaa", &ps_r3_msaa, qmsaa_token);
        CMD3(CCC_Token, "r3_msaa_alphatest", &ps_r3_msaa_atest, qmsaa_atest_token);

        CMD3(CCC_Token, "xrRenderDX10_type_aa", &ps_r_type_aa, type_aa_token);
        CMD3(CCC_Token, "xrRenderDX10_sunshafts_mode", &ps_sunshafts_mode, sunshafts_mode_token);
    }

    CMD3(CCC_Token,     "xrEngine_fps_lock",    &g_dwFPSlimit, FpsLockToken);
    CMD3(CCC_Mask,      "xrEngine_xrRender_stats", &psDeviceFlags, rsRenderInfo);
    CMD4(CCC_Integer,   "xrEngine_noprefetch",  &xrengint_noprefetch, 0, 1);
    CMD4(CCC_Integer,   "xrEngine_discord",     &game_value_discord_status, 0, 1);

#if 0
    CMD4(CCC_Integer,   "xrEngine_rs_fps_test", &show_FPS_only, 0, 1);
#endif

    CMD4(CCC_Float,     "xrSound_snd_speed",    &psSpeedOfSound, 0.2f, 2.0f);

    if (BttR_mode)
        CMD4(CCC_Integer, "xrEngine_loadingstages", &ps_rs_loading_stages, 0, 1);
////////////////////////////////////////////////////////////////////////////////////////////////////

    // General
    CMD1(CCC_Help, "help");
    CMD1(CCC_Quit, "quit");
    CMD1(CCC_Start, "start");
    CMD1(CCC_Disconnect, "disconnect");
    CMD1(CCC_SaveCFG, "cfg_save");
    CMD1(CCC_LoadCFG, "cfg_load");

#ifdef DEBUG
    CMD1(CCC_MotionsStat, "stat_motions");
    CMD1(CCC_TexturesStat, "stat_textures");
#endif // DEBUG

#ifdef DEBUG_MEMORY_MANAGER
    CMD1(CCC_MemStat, "dbg_mem_dump");
    CMD1(CCC_DbgMemCheck, "dbg_mem_check");
#endif // DEBUG_MEMORY_MANAGER

#ifdef DEBUG
    CMD3(CCC_Mask, "mt_particles", &psDeviceFlags, mtParticles);

    CMD1(CCC_DbgStrCheck, "dbg_str_check");
    CMD1(CCC_DbgStrDump, "dbg_str_dump");

    CMD3(CCC_Mask, "mt_sound", &psDeviceFlags, mtSound);
    CMD3(CCC_Mask, "mt_physics", &psDeviceFlags, mtPhysics);
    CMD3(CCC_Mask, "mt_network", &psDeviceFlags, mtNetwork);

    // Events
    CMD1(CCC_E_Dump, "e_list");
    CMD1(CCC_E_Signal, "e_signal");

    CMD3(CCC_Mask, "rs_wireframe", &psDeviceFlags, rsWireframe);
    CMD3(CCC_Mask, "rs_clear_bb", &psDeviceFlags, rsClearBB);
    CMD3(CCC_Mask, "rs_occlusion", &psDeviceFlags, rsOcclusion);

    CMD3(CCC_Mask, "rs_detail", &psDeviceFlags, rsDetails);
    //CMD4(CCC_Float, "r__dtex_range", &r__dtex_range, 5, 175 );

    // CMD3(CCC_Mask, "rs_constant_fps", &psDeviceFlags, rsConstantFPS );
    CMD3(CCC_Mask, "rs_render_statics", &psDeviceFlags, rsDrawStatic);
    CMD3(CCC_Mask, "rs_render_dynamics", &psDeviceFlags, rsDrawDynamic);
#endif

    CMD4(CCC_Float, "r2_sun_shafts_min", &ps_r2_sun_shafts_min, 0.0, 0.5);
    CMD4(CCC_Float, "r2_sun_shafts_value", &ps_r2_sun_shafts_value, 0.5, 2.0);

    CMD3(CCC_Mask, "rs_v_sync", &psDeviceFlags, rsVSync);
    // CMD3(CCC_Mask, "rs_disable_objects_as_crows",&psDeviceFlags, rsDisableObjectsAsCrows );
    CMD3(CCC_Mask, "rs_refresh_60hz", &psDeviceFlags, rsRefresh60hz);
    CMD3(CCC_Mask, "rs_stats", &psDeviceFlags, rsStatistic);
    CMD4(CCC_Float, "rs_vis_distance", &psVisDistance, 0.4f, 1.5f);

    CMD3(CCC_Mask, "rs_cam_pos", &psDeviceFlags, rsCameraPos);
#ifdef DEBUG
    CMD3(CCC_Mask, "rs_occ_draw", &psDeviceFlags, rsOcclusionDraw);
    CMD3(CCC_Mask, "rs_occ_stats", &psDeviceFlags, rsOcclusionStats);
    //CMD4(CCC_Integer, "rs_skeleton_update", &psSkeletonUpdate, 2, 128 );
#endif // DEBUG

    CMD2(CCC_Gamma, "rs_c_gamma", &ps_gamma);
    CMD2(CCC_Gamma, "rs_c_brightness", &ps_brightness);
    CMD2(CCC_Gamma, "rs_c_contrast", &ps_contrast);
    // CMD4(CCC_Integer, "rs_vb_size", &rsDVB_Size, 32, 4096);
    // CMD4(CCC_Integer, "rs_ib_size", &rsDIB_Size, 32, 4096);

    // Texture manager
    CMD4(CCC_Integer, "texture_lod", &psTextureLOD, 0, 4);
    CMD4(CCC_Integer, "net_dedicated_sleep", &psNET_DedicatedSleep, 0, 64);

    // General video control
    CMD1(CCC_VidMode, "vid_mode");

#ifdef DEBUG
    CMD3(CCC_Token, "vid_bpp", &psCurrentBPP, vid_bpp_token);
#endif // DEBUG

    CMD1(CCC_VID_Reset, "vid_restart");

    // Sound
    CMD2(CCC_Float, "snd_volume_eff", &psSoundVEffects);
    CMD2(CCC_Float, "snd_volume_music", &psSoundVMusic);
    CMD1(CCC_SND_Restart, "snd_restart");
    CMD3(CCC_Mask, "snd_acceleration", &psSoundFlags, ss_Hardware);
    CMD3(CCC_Mask, "snd_efx", &psSoundFlags, ss_EAX);
    CMD4(CCC_Integer, "snd_targets", &psSoundTargets, 16, 256);
    CMD4(CCC_Integer, "snd_cache_size", &psSoundCacheSizeMB, 8, 256);

#ifdef DEBUG
    CMD3(CCC_Mask, "snd_stats", &g_stats_flags, st_sound);
    CMD3(CCC_Mask, "snd_stats_min_dist", &g_stats_flags, st_sound_min_dist);
    CMD3(CCC_Mask, "snd_stats_max_dist", &g_stats_flags, st_sound_max_dist);
    CMD3(CCC_Mask, "snd_stats_ai_dist", &g_stats_flags, st_sound_ai_dist);
    CMD3(CCC_Mask, "snd_stats_info_name", &g_stats_flags, st_sound_info_name);
    CMD3(CCC_Mask, "snd_stats_info_object", &g_stats_flags, st_sound_info_object);

    CMD4(CCC_Integer, "error_line_count", &g_ErrorLineCount, 6, 1024);
#endif // DEBUG

    // Mouse
    CMD3(CCC_Mask, "mouse_invert", &psMouseInvert, 1);
    psMouseSens = 0.12f;
    CMD4(CCC_Float, "mouse_sens", &psMouseSens, 0.001f, 0.6f);

    // Camera
    CMD2(CCC_Float, "cam_inert", &psCamInert);
    CMD2(CCC_Float, "cam_slide_inert", &psCamSlideInert);

    CMD1(CCC_r2, "renderer");

#ifndef DEDICATED_SERVER
    CMD1(CCC_soundDevice, "snd_device");
#endif
    //psSoundRolloff = pSettings->r_float ("sound","rolloff"); clamp(psSoundRolloff, EPS_S, 2.f);
    psSoundOcclusionScale = pSettings->r_float("sound", "occlusion_scale");
    clamp(psSoundOcclusionScale, 0.1f, .5f);

    extern int g_Dump_Export_Obj;
    extern int g_Dump_Import_Obj;
    CMD4(CCC_Integer, "net_dbg_dump_export_obj", &g_Dump_Export_Obj, 0, 1);
    CMD4(CCC_Integer, "net_dbg_dump_import_obj", &g_Dump_Import_Obj, 0, 1);

#ifdef DEBUG
    CMD1(CCC_DumpOpenFiles, "dump_open_files");
#endif

    CMD1(CCC_ExclusiveMode, "input_exclusive_mode");

    extern int g_svTextConsoleUpdateRate;
    CMD4(CCC_Integer, "sv_console_update_rate", &g_svTextConsoleUpdateRate, 1, 100);

    extern int g_svDedicateServerUpdateReate;
    CMD4(CCC_Integer, "sv_dedicated_server_update_rate", &g_svDedicateServerUpdateReate, 1, 1000);

    CMD1(CCC_HideConsole, "hide");

#ifdef DEBUG
    extern BOOL debug_destroy;
    CMD4(CCC_Integer, "debug_destroy", &debug_destroy, FALSE, TRUE);
#endif
};

