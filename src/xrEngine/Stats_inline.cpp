#include "stdafx.h"
#include "../xrCore/cpuid.h"
#include "device.h"
#include "Stats.h"
#include "GameFont.h"
#include "NVIDIA.h"
#include "AMD.h"
#include "dxgi.h"
#include "XR_IOConsole.h"
#include <xstring>

using namespace std;
extern u32 renderer_value;
extern u32 ps_r3_msaa;
extern u32 ps_r3_msaa_atest;
extern u32 ps_r_sun_quality;
extern u32 render_video_size;
extern u32 ps_r_type_aa;
extern u32 ps_sunshafts_mode;
extern u32 ps_r_ssao_mode;
extern u32 ps_r_ssao;
extern u32 ps_r_sun_shafts;
extern u32 optTessQuality_;
extern Flags32 p_engine_flags32;
extern bool IsMainMenuActive();

#define CONST_HEIGHT_FONT pFontHW->SetHeightI(0.018f)
#define U32_NULL     u32(-1)

enum DebugTextColor : DWORD
{
    // Системные цвета
    DTC_RED = 0xFFFF0000,
    DTC_YELLOW = 0xFFF6D434,
    DTC_GREEN = 0xFF67F92E,
    DTC_GREEN_DX = 0xFF00FF7F,
    DTC_GREEN_NV = 0xFF00FF00,
    DTC_BLUE = 0xFF00FFFF,
    //Сглаживание MSAA на DX11
    DTC_MSAA = 0xFF7FFFD4,
    //Альфа-Тест прозрачных объектов, тоже MSAA
    DTC_MSAA_AT = 0xFF40E0D0,
    // Стата про лучи солнца
    DTC_SUN_OPT = 0xFFFFFF00,
    // Размер видеобуфера
    DTC_VIDEOSIZE = 0xFFE6E6FA,
    DTC_VIDEOSIZE_SCREEN = 0xFFFFFFFF,
    DTC_AA = 0xFF00BFFF,
    // SSAO
    DTC_SSAO = 0xFFFF0000,
};

//OldSerpskiStalker
void CStats::Show_HW_Stats()
{
    DISPLAY_DEVICEA dd;
    dd.cb = sizeof(DISPLAY_DEVICEA);
    EnumDisplayDevicesA(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
//  string str_GPU = string(dd.DeviceString);
    static const int GetMainInfoStats = 300;

    if (psDeviceFlags.test(rsRenderInfo))
    {
        static DWORD dwLastFrameTime = 0;
        DWORD dwCurrentTime = timeGetTime();
        if (dwCurrentTime - dwLastFrameTime > 500) //Апдейт раз в полсекунды
        {
            dwLastFrameTime = dwCurrentTime;

            // init all variables
            MEMORYSTATUSEX mem;
            PROCESS_MEMORY_COUNTERS_EX pmc;
            SYSTEM_INFO sysInfo;

            // Getting info about memory
            mem.dwLength = sizeof(MEMORYSTATUSEX);
            GlobalMemoryStatusEx((&mem));

            AvailableMem = (float)mem.ullAvailPhys;	// how much phys mem available
            AvailableMem /= (1024 * 1024);
            AvailablePageFileMem = (float)mem.ullAvailPageFile;	// how much pagefile mem available
            AvailablePageFileMem /= (1024 * 1024);

            // Getting info by request
            GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(PROCESS_MEMORY_COUNTERS_EX));
            GetSystemInfo(&sysInfo);

            PhysMemoryUsedPercent = (float)mem.dwMemoryLoad;
            PageFileMemUsedByApp = (float)pmc.PagefileUsage;
            PageFileMemUsedByApp /= (1024 * 1024);

            // Counting CPU load
            CPU::ID.getCPULoad(cpuLoad);
            cpuBefore = cpuLoad;

            CPU::ID.MTCPULoad();

            GPULoad = CAMDReader::bAMDSupportADL ? AMDData->GetPercentActive() : CNvReader::bSupport ? NvData->GetPercentActive() : U32_NULL;
            GPUTemperature = CAMDReader::bAMDSupportADL ? AMDData->GetTemperature() : CNvReader::bSupport ? NvData->GetTemperature() : U32_NULL;;
        }

        CONST_HEIGHT_FONT;

        //On every 25 frame, update last known fps
        if ((Core.dwFrame % 25) == 0)
            fLastDisplayedFPS = fFPS;

        //If game paused, engine not updating deltaTime variable, so FPS variable is freezed to last value
        const char*   FPSFormat = Device.Paused() ? "Last known FPS: %i" : "FPS: %i";
        // LoopScale всегда должен быть равен 2 или цикл съедет за экран
        constexpr int LoopScale = 2;
                  int InfoScale = 10;
        for (size_t i = 0; i < LoopScale; i++)
        {
            switch (i)
            {
                case 1:
                       pFontHW->SetColor(DebugTextColor::DTC_BLUE);
                       pFontHW->Out(GetMainInfoStats, InfoScale, "-- [Information about rendering the current scene] --");
                       InfoScale += 15;

                case 2:
                        if (fLastDisplayedFPS > 60)
                            pFontHW->SetColor(DebugTextColor::DTC_GREEN);
                        else if (fLastDisplayedFPS > 30)
                            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
                        else									
                            pFontHW->SetColor(DebugTextColor::DTC_RED);

                        pFontHW->Out(GetMainInfoStats, InfoScale, FPSFormat, (int)fLastDisplayedFPS);
                        InfoScale += 15;

                case 3: 
                        pFontHW->SetColor(DebugTextColor::DTC_GREEN_DX);
                        pFontHW->Out(GetMainInfoStats, InfoScale, renderer_value == 0 ? "Render: DirectX10" : renderer_value == 1 ? "Render: DirectX10.1" : renderer_value == 2 ? "Render: DirectX11" : "Render: ???");
                        InfoScale += 15;

                case 4:
                        pFontHW->SetColor(DebugTextColor::DTC_GREEN_DX);
                        pFontHW->Out(GetMainInfoStats, InfoScale, (renderer_value == 0 || renderer_value == 1) ? "Tessellation quality DX11: To activate this option, you must have DirectX11 enabled." : 
                            optTessQuality_ == 0 ? "Tessellation quality: Low" : optTessQuality_ == 1 ? "Tessellation quality: Medium" : optTessQuality_ == 2 ? "Tessellation quality: Optimum" : optTessQuality_ == 3 ? "Tessellation quality: Overneeded" : "Tessellation quality: ???");
                        InfoScale += 15;
                
                case 5:
                        pFontHW->SetColor(DebugTextColor::DTC_MSAA);
                        pFontHW->Out(GetMainInfoStats, InfoScale, ps_r3_msaa == 0 ? "MSAA Disabled" : ps_r3_msaa == 1 ? "MSAA x2" : ps_r3_msaa == 2 ? "MSAA x4" : ps_r3_msaa == 3 ? "MSAA x8" : "MSAA ???");
                        InfoScale += 15;

                case 6:
                        pFontHW->SetColor(DebugTextColor::DTC_MSAA_AT);
                        pFontHW->Out(GetMainInfoStats, InfoScale, ps_r3_msaa_atest == 0 ? "MSAA A-Test Disabled" : ps_r3_msaa_atest == 1 ? "MSAA A-Test quality DX10 (Standart)" : ps_r3_msaa_atest == 2 ? "MSAA A-Test quality DX10.1 (High)" : "MSAA A-Test quality ???");
                        InfoScale += 15;

                case 7:
                        pFontHW->SetColor(DebugTextColor::DTC_AA);
                        pFontHW->Out(GetMainInfoStats, InfoScale, ps_r_type_aa == 0 ? "Additional smoothing: Disabled" : ps_r_type_aa == 1 ? "Additional smoothing: FXAA" : ps_r_type_aa == 2 ? "Additional smoothing: DLAA" : ps_r_type_aa == 3 ? "Additional smoothing: SMAA" : "Additional smoothing: ???");
                        InfoScale += 15;

                case 8:
                        pFontHW->SetColor(DebugTextColor::DTC_SSAO);
                        pFontHW->Out(GetMainInfoStats, InfoScale, ps_r_ssao_mode == 0 ? "Mode AO: Disabled" : ps_r_ssao_mode == 1 ? "Mode AO: SSAO" : ps_r_ssao_mode == 2 ? "Mode AO: HDAO" : ps_r_ssao_mode == 3 ? "Mode AO: HBAO" : ps_r_ssao_mode == 4 ? "Mode AO: SSDO" : "Mode AO: ???");
                        InfoScale += 15;

                case 9:
                        pFontHW->SetColor(DebugTextColor::DTC_SSAO);
                        pFontHW->Out(GetMainInfoStats, InfoScale, ps_r_ssao == 0 ? "Quality AO: Standart" : ps_r_ssao == 1 ? "Quality AO: Low" : ps_r_ssao == 2 ? "Quality AO: Medium" : ps_r_ssao == 3 ? "Quality AO: High" : ps_r_ssao == 4 ? "Quality AO: Ultra" : "Quality AO: ???");
                        InfoScale += 15;

                case 10:
                        pFontHW->SetColor(DebugTextColor::DTC_SSAO);
                        static bool DEBUG_AO = p_engine_flags32.test(R2FLAGEXT_SSAO_DEBUG);
                        pFontHW->Out(GetMainInfoStats, InfoScale, DEBUG_AO == true ? "Debug AO: On" : "Debug AO: Off");
                        InfoScale += 15;

                case 11:
                        pFontHW->SetColor(DebugTextColor::DTC_SUN_OPT);
                        pFontHW->Out(GetMainInfoStats, InfoScale, ps_r_sun_quality == 0 ? "Sun quality: Low" : ps_r_sun_quality == 1 ? "Sun quality: Medium" : ps_r_sun_quality == 2 ? "Sun quality: High" : ps_r_sun_quality == 3 ? "Sun quality: Ultra" : ps_r_sun_quality == 4 ? "Sun quality: Extreme" : "Sun quality: ???");
                        InfoScale += 15;

                case 12:
                        pFontHW->SetColor(DebugTextColor::DTC_SUN_OPT);
                        pFontHW->Out(GetMainInfoStats, InfoScale, ps_r_sun_shafts == 0 ? "Quality of sun rays: Off" : ps_r_sun_shafts == 1 ? "Quality of sun rays: Low" : ps_r_sun_shafts == 2 ? "Quality of sun rays: Medium" : ps_r_sun_shafts == 3 ? "Quality of sun rays: High" : "Quality of sun rays: ???");
                        InfoScale += 15;

                case 13:
                        pFontHW->SetColor(DebugTextColor::DTC_SUN_OPT);
                        pFontHW->Out(GetMainInfoStats, InfoScale, ps_sunshafts_mode == 0 ? "Volumetric sunshafts: Enable" : "Volumetric sunshafts: Disabled");
                        InfoScale += 15;

                case 14:
                        pFontHW->SetColor(DebugTextColor::DTC_VIDEOSIZE);
                        pFontHW->Out(GetMainInfoStats, InfoScale, "Size video buffer: %i", render_video_size);
                        InfoScale += 15;

                case 15:
                        pFontHW->SetColor(DebugTextColor::DTC_VIDEOSIZE_SCREEN);
                        pFontHW->Out(GetMainInfoStats, InfoScale, "Video mode: %dx%d", psCurrentVidMode[0], psCurrentVidMode[1]);
                        InfoScale += 15;

                case 16:
                        pFontHW->SetColor(DebugTextColor::DTC_BLUE);
                        pFontHW->Out(GetMainInfoStats, InfoScale, "-- [Information about your computer configuration] --");
                        InfoScale += 15;

                case 17:
                        pFontHW->SetColor(CAMDReader::bAMDSupportADL ? DebugTextColor::DTC_RED : DebugTextColor::DTC_GREEN_NV);
                        pFontHW->Out(GetMainInfoStats, InfoScale, "Video card model: %s", dd.DeviceString);
                        InfoScale += 15;

                case 18: 
                        if (GPULoad != U32_NULL)
                        {
                            if (GPULoad > u32(80))
                                pFontHW->SetColor(DebugTextColor::DTC_RED);
                            else if (GPULoad > u32(60))
                                pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
                            else
                                pFontHW->SetColor(DebugTextColor::DTC_GREEN);

                            pFontHW->Out(GetMainInfoStats, InfoScale, "GPU load: %u%%", GPULoad);
                            InfoScale += 15;
                        }

                case 19:
                        if (GPUTemperature != U32_NULL)
                            pFontHW->Out(GetMainInfoStats, InfoScale, "GPU Temperature: %i°", CAMDReader::bAMDSupportADL ? GPUTemperature / 1000 : GPUTemperature);

                        InfoScale += 15;

                case 20:
                        pFontHW->SetColor(DebugTextColor::DTC_GREEN);
                        pFontHW->Out(GetMainInfoStats, InfoScale, "Processor model: CPU: %s [%s], F%d/M%d/S%d, %.2f mhz, %u-clk 'rdtsc'", CPU::ID.brand, CPU::ID.vendor, CPU::ID.family, CPU::ID.model, CPU::ID.stepping, float(CPU::clk_per_second / u64(1000000)), u32(CPU::clk_overhead));
                        InfoScale += 15;

                case 21:
                        pFontHW->Out(GetMainInfoStats, InfoScale, "CPU cores: %u, threads: %u", CPU::ID.coresCount, CPU::ID.threadCount);
                        InfoScale += 15;

                case 22:
                        if (AvailableMem < 512 || AvailablePageFileMem < 1596)
                            pFontHW->SetColor(DebugTextColor::DTC_RED);
                        else if (AvailableMem < 768 || AvailablePageFileMem < 2048)
                            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
                        else
                            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

                        pFontHW->Out(GetMainInfoStats, InfoScale, "Physical memory available: %0.0fMB", AvailableMem); // Physical memory available
                        InfoScale += 15;

                case 23:
                        pFontHW->Out(GetMainInfoStats, InfoScale, "Pagefile memory available: %0.0fMB", AvailablePageFileMem); // Pagefile memory available
                        InfoScale += 15;

                case 24:
                        pFontHW->Out(GetMainInfoStats, InfoScale, "Physical memory used by app: %0.0fMB", PageFileMemUsedByApp); // Physical memory used by app
                        InfoScale += 15;

                case 25:
                        if (PhysMemoryUsedPercent > 80.0)
                            pFontHW->SetColor(DebugTextColor::DTC_RED);
                        else if (PhysMemoryUsedPercent > 60.0)
                            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
                        else
                            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

                        pFontHW->Out(GetMainInfoStats, InfoScale, "Physical memory load: %0.0f%%", PhysMemoryUsedPercent); // Total Phys. memory load (%)
                        InfoScale += 15;

                case 26: 
                        if (cpuLoad > 80.0)
                            pFontHW->SetColor(DebugTextColor::DTC_RED);
                        else if (cpuLoad > 60.0)
                            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
                        else
                            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

                        pFontHW->Out(GetMainInfoStats, InfoScale, "CPU load: %0.0f%%", cpuLoad); // CPU load
                        InfoScale += 15;

                case 27: // Всегда должен быть последним параметром
                        int GetInfoScale = InfoScale;
                        for (size_t i = 0; i < CPU::ID.m_dwNumberOfProcessors; i++)
                        {
                            pFontHW->Out(GetMainInfoStats, GetInfoScale, "CPU %u: %0.0f%%", i, CPU::ID.fUsage[i]);
                            GetInfoScale += 15;
                        }
            }
            
        }
        pFontHW->OnRender();
    }
}

