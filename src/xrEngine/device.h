#ifndef xr_device
#define xr_device
#pragma once

// Note:
// ZNear - always 0.0f
// ZFar - always 1.0f

//class ENGINE_API CResourceManager;
//class ENGINE_API CGammaControl;

#include "pure.h"
//#include "hw.h"
#include "../xrcore/ftimer.h"
#include "../xrCore/Event.hpp"
#include "stats.h"
//#include "shader.h"
//#include "R_Backend.h"

#include <psapi.h>
#include <mmsystem.h>

#define VIEWPORT_NEAR 0.2f		//Main viewport near
#define R_VIEWPORT_NEAR 0.05f	//Second viewport near (to avoid z-fighting)

#define DEVICE_RESET_PRECACHE_FRAME_COUNT 10

#include "../Include/xrRender/FactoryPtr.h"
#include "../Include/xrRender/RenderDeviceRender.h"

#ifdef INGAME_EDITOR
# include "../Include/editor/interfaces.hpp"
#endif // #ifdef INGAME_EDITOR

extern u32 g_dwFPSlimit;

class engine_impl;

#pragma pack(push,4)

class IRenderDevice
{
public:
    virtual CStatsPhysics* 	 StatPhysics() = 0;
    virtual void 	 AddSeqFrame(pureFrame* f, bool mt) = 0;
    virtual void 	 RemoveSeqFrame(pureFrame* f) = 0;
};

class ENGINE_API CRenderDeviceData
{

public:
    u32 dwWidth;
    u32 dwHeight;

    u32 dwPrecacheFrame;
    BOOL b_is_Ready;
    BOOL b_is_Active;
public:

    // Engine flow-control
    u32 dwFrame;

    float fTimeDelta;
    float fTimeGlobal;
    u32 dwTimeDelta;
    u32 dwTimeGlobal;
    u32 dwTimeContinual;

    Fvector vCameraPosition;
    Fvector vCameraDirection;
    Fvector vCameraTop;
    Fvector vCameraRight;

    Fmatrix mView;
    Fmatrix mProject;
    Fmatrix mFullTransform;

    // Copies of corresponding members. Used for synchronization.
    Fvector vCameraPosition_saved;

    Fmatrix mView_saved;
    Fmatrix mProject_saved;
    Fmatrix mFullTransform_saved;

    float fFOV;
    float fASPECT;
protected:

    u32 Timer_MM_Delta;
    CTimer_paused Timer;
    CTimer_paused TimerGlobal;

    //AVO: 
    CTimer frame_timer;   //TODO: проверить, не дублируется-ли схожий таймер (alpet)
    //-AVO

public:

    // Registrators
    CRegistrator <pureRender > seqRender;
    CRegistrator <pureAppActivate > seqAppActivate;
    CRegistrator <pureAppDeactivate > seqAppDeactivate;
    CRegistrator <pureAppStart > seqAppStart;
    CRegistrator <pureAppEnd > seqAppEnd;
    CRegistrator <pureFrame > seqFrame;
    CRegistrator <pureScreenResolutionChanged> seqResolutionChanged;

    HWND m_hWnd;
    // CStats* Statistic;

};

class ENGINE_API CRenderDeviceBase :
    public IRenderDevice,
    public CRenderDeviceData
{
public:
};

#pragma pack(pop)
// refs
class ENGINE_API CRenderDevice : public CRenderDeviceBase
{
public:
	class ENGINE_API CSecondVPParams //--#SM+#-- +SecondVP+
	{
		bool isActive;
		u8 frameDelay; 

	public:
		bool isCamReady;

		IC bool IsSVPActive() { return isActive; }
		IC void SetSVPActive(bool bState);
		bool    IsSVPFrame();

		IC u8 GetSVPFrameDelay() { return frameDelay; }
		void  SetSVPFrameDelay(u8 iDelay)
		{
			frameDelay = iDelay;
			clamp<u8>(frameDelay, 2, u8(-1));
		}
	};
	
private:
    // Main objects used for creating and rendering the 3D scene
    u32 m_dwWindowStyle;
    RECT m_rcWindowBounds;
    RECT m_rcWindowClient;
    BOOL b_hide_cursor;
    //u32 Timer_MM_Delta;
    //CTimer_paused Timer;
    //CTimer_paused TimerGlobal;
    CTimer TimerMM;

    void _Create(LPCSTR shName);
    void _Destroy(BOOL bKeepTextures);
    void _SetupStates();
public:
    // HWND m_hWnd;
    LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

    // u32 dwFrame;
    // u32 dwPrecacheFrame;
    u32 dwPrecacheTotal;

    // u32 dwWidth, dwHeight;
    float fWidth_2, fHeight_2;
    // BOOL b_is_Ready;
    // BOOL b_is_Active;
    void OnWM_Activate(WPARAM wParam, LPARAM lParam);
public:
    //ref_shader m_WireShader;
    //ref_shader m_SelectionShader;

    IRenderDeviceRender* m_pRender;

    BOOL m_bNearer;
    void SetNearer(BOOL enabled)
    {
        if (enabled&&!m_bNearer)
        {
            m_bNearer = TRUE;
            mProject._43 -= EPS_L;
        }
        else if (!enabled&&m_bNearer)
        {
            m_bNearer = FALSE;
            mProject._43 += EPS_L;
        }
        m_pRender->SetCacheXform(mView, mProject);
        //R_ASSERT(0);
        // TODO: re-implement set projection
        //RCache.set_xform_project (mProject);
    }

    void DumpResourcesMemoryUsage() { m_pRender->ResourcesDumpMemoryUsage(); }
public:
    // Registrators
    CRegistrator <pureFrame > seqFrameMT;
    CRegistrator <pureDeviceReset > seqDeviceReset;
    xr_vector <fastdelegate::FastDelegate0<> > seqParallel;
	CSecondVPParams m_SecondViewport;	//--#SM+#-- +SecondVP+
	
    CStats* Statistic;

    Fmatrix mInvFullTransform;

    CRenderDevice();
    ~CRenderDevice();

    void Pause(BOOL bOn, BOOL bTimer, BOOL bSound, LPCSTR reason);
    BOOL Paused();

    // Scene control
    void PreCache(u32 amount, bool b_draw_loadscreen, bool b_wait_user_input);
    BOOL Begin();
    void Clear();
    void End();
    void FrameMove();

    void overdrawBegin();
    void overdrawEnd();

    // Mode control
    void DumpFlags();
    IC CTimer_paused* GetTimerGlobal() { return &TimerGlobal; }
    u32 TimerAsync() { return TimerGlobal.GetElapsed_ms(); }
    u32 TimerAsync_MMT() { return TimerMM.GetElapsed_ms() + Timer_MM_Delta; }

    // Creation & Destroying
    void ConnectToRender();
    void Create(void);
    void Run(void);
    void Destroy(void);
    void Reset(bool precache = true);

    void Initialize(void);

public:
    void time_factor(const float& time_factor)
    {
        Timer.time_factor(time_factor);
        TimerGlobal.time_factor(time_factor);
    }

    IC const float& time_factor() const
    {
        VERIFY(Timer.time_factor() == TimerGlobal.time_factor());
        return (Timer.time_factor());
    }

    // Multi-threading
    xrCriticalSection mt_csEnter;
    xrCriticalSection mt_csLeave;
    volatile BOOL mt_bMustExit;

    ICF void remove_from_seq_parallel(const fastdelegate::FastDelegate0<>& delegate)
    {
        xr_vector<fastdelegate::FastDelegate0<> >::iterator I = std::find(
                    seqParallel.begin(),
                    seqParallel.end(),
                    delegate
                );
        if (I != seqParallel.end())
            seqParallel.erase(I);
    }

    //AVO: elapsed famed counter (by alpet)
    IC u32 frame_elapsed()
    {
        return frame_timer.GetElapsed_ms();
    }

public:
    void xr_stdcall on_idle();
    bool xr_stdcall on_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);

private:
    void message_loop();
    virtual void 	 AddSeqFrame(pureFrame* f, bool mt);
    virtual void 	 RemoveSeqFrame(pureFrame* f);
    virtual CStatsPhysics* 	 StatPhysics() { return Statistic; }
#ifdef INGAME_EDITOR
public:
    IC editor::ide* editor() const { return m_editor; }

private:
    void initialize_editor();
    void message_loop_editor();

private:
    typedef editor::initialize_function_ptr initialize_function_ptr;
    typedef editor::finalize_function_ptr finalize_function_ptr;

private:
    HMODULE m_editor_module;
    initialize_function_ptr m_editor_initialize;
    finalize_function_ptr m_editor_finalize;
    editor::ide* m_editor;
    engine_impl* m_engine;
#endif // #ifdef INGAME_EDITOR
};

extern ENGINE_API CRenderDevice Device;

#define RDEVICE Device

extern ENGINE_API bool g_bBenchmark;

typedef fastdelegate::FastDelegate0<bool> LOADING_EVENT;
extern ENGINE_API xr_list<LOADING_EVENT> g_loading_events;

class ENGINE_API CLoadScreenRenderer :public pureRender
{
public:
    CLoadScreenRenderer();
    void start(bool b_user_input);
    void stop();
    virtual void OnRender();

    bool b_registered;
    bool b_need_user_input;
};
extern ENGINE_API CLoadScreenRenderer load_screen_renderer;
extern ENGINE_API Flags32             p_engine_flags32;

//-' Ключи относящиеся к переключению версий ЗЧ и BttR
#define ITS_CLEAR_1_4_22            (1<<0)
//-' Команды для включения AO
#define R2FLAGEXT_SSAO_BLUR         (1<<1)
#define R2FLAGEXT_SSAO_OPT_DATA     (1<<2)
#define R2FLAGEXT_SSAO_HALF_DATA    (1<<3)
#define R2FLAGEXT_SSAO_HBAO         (1<<4)
#define R2FLAGEXT_SSAO_HDAO         (1<<5)
#define R2FLAGEXT_SSAO_SSDO         (1<<6)
#define R2FLAGEXT_SSAO_DEBUG        (1<<7)
//-' Прицелы
#define AF_CROSSHAIR_COLLIDE        (1<<8)
#define AF_CROSSHAIR_INERT          (1<<9)
#define AF_CROSSHAIR_STANDART       (1<<10)
//-' Флаг больше реализма
#define FLAG_MORE_REALISM           (1<<11)
//-' Открытие инветаря
#define AF_HAND_HIDE_INVENTORY      (1<<12)
#define AF_HAND_HIDE_WITH_RUCK      (1<<13)
//-' Переключение выбора группировки ГГ
#define E_STALKER                   (1<<14)
#define E_BANDIT                    (1<<15)
#define E_CSKY                      (1<<16)
#define E_DOLG                      (1<<17)
#define E_FREEDOM                   (1<<18)
#define E_KILLER                    (1<<19)
#define E_ARMY                      (1<<20)
#define E_ECOLOG                    (1<<21)
#define E_MONOLITH                  (1<<22)
//-' Доф оружия
#define AF_RELOAD_DOF               (1<<23)
#define AF_ZOOM_DOF                 (1<<24)
//-' Намокание поверхносте DX10
#define R3FLAG_DYN_WET_SURF         (1<<25)
#endif