#include "stdafx.h"
#include "../xrCore/cpuid.h"
#include "device.h"
#include "Stats.h"
#include "GameFont.h"
#include "NVIDIA.h"
#include "AMD.h"
#include "dxgi.h"

#define CONST_HEIGHT_FONT pFontHW->SetHeightI(0.018f)

static const int stat_1 = 10;
static const int stat_2 = 25;
static const int stat_3 = 40;
static const int stat_4 = 55;
static const int stat_5 = 70;
static const int stat_6 = 85;
static const int stat_7 = 100;
static const int stat_8 = 115;
static const int stat_9 = 130;
static const int stat_10 = 145;
static const int GetMainInfoStats = 10;
extern u32 renderer_value;

#include <xstring>
using namespace std;

/*static*/ enum DebugTextColor : DWORD
{
    DTC_RED = 0xFFF0672B,
    DTC_YELLOW = 0xFFF6D434,
    DTC_GREEN = 0xFF67F92E,
};

void CStats::Show_HW_Stats()
{
    DISPLAY_DEVICEA dd;
    dd.cb = sizeof(DISPLAY_DEVICEA);
    EnumDisplayDevicesA(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
    string str = string(dd.DeviceString);

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

            GpuLoad = CAMDReader::bAMDSupportADL ? AMDData->GetPercentActive() : CNvReader::bSupport ? NvData->GetPercentActive() : u32(-1);
            GPUType = CAMDReader::bAMDSupportADL ? _CARD = 1 : CNvReader::bSupport ? _CARD = 2 : u32(-1);
        }

        CONST_HEIGHT_FONT;

        pFontHW->SetColor(DebugTextColor::DTC_GREEN);

        pFontHW->Out(GetMainInfoStats, stat_1, "-- [Information about rendering the current scene] --");
        pFontHW->Out(GetMainInfoStats, stat_2, "FPS: %f", fDeviceMeasuredFPS);
        pFontHW->Out(GetMainInfoStats, stat_3, renderer_value == 0 ? "Render: DirectX10" : renderer_value == 1 ? "Render: DirectX10.1" : renderer_value == 2 ? "Render: DirectX11" : "Render: ???");

        pFontHW->Out(GetMainInfoStats, stat_4, "-- [Information about your computer configuration] --");

        pFontHW->SetColor(DebugTextColor::DTC_GREEN);

        pFontHW->Out(GetMainInfoStats, stat_5, "Video card model: %s", dd.DeviceString);

        if (AvailableMem < 512 || AvailablePageFileMem < 1596)
            pFontHW->SetColor(DebugTextColor::DTC_RED);
        else if (AvailableMem < 768 || AvailablePageFileMem < 2048)
            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
        else
            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

        // Draw all your stuff
        pFontHW->Out(GetMainInfoStats, stat_6, "Physical memory available: %0.0fMB", AvailableMem); // Physical memory available
        pFontHW->Out(GetMainInfoStats, stat_7, "Pagefile memory available: %0.0fMB", AvailablePageFileMem); // Pagefile memory available
        pFontHW->Out(GetMainInfoStats, stat_8, "Physical memory used by app: %0.0fMB", PageFileMemUsedByApp); // Physical memory used by app

        if (PhysMemoryUsedPercent > 80.0)
            pFontHW->SetColor(DebugTextColor::DTC_RED);
        else if (PhysMemoryUsedPercent > 60.0)
            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
        else
            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

        pFontHW->Out(GetMainInfoStats, stat_9, "Physical memory load: %0.0f%%", PhysMemoryUsedPercent); // Total Phys. memory load (%)

        if (cpuLoad > 80.0)
            pFontHW->SetColor(DebugTextColor::DTC_RED);
        else if (cpuLoad > 60.0)
            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
        else
            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

        pFontHW->Out(GetMainInfoStats, stat_10, "CPU load: %0.0f%%", cpuLoad); // CPU load

        // get MT Load
        static const int stat_11 = 160;
        int GetInfoScale = stat_11;
        for (size_t i = 0; i < CPU::ID.m_dwNumberOfProcessors; i++)
        {
            pFontHW->Out(GetMainInfoStats, GetInfoScale, "CPU %u: %0.0f%%", i, CPU::ID.fUsage[i]);
            GetInfoScale += 15;
        }

        if (GpuLoad != u32(-1))
        {
            if (GpuLoad > 80)
                pFontHW->SetColor(DebugTextColor::DTC_RED);
            else if (GpuLoad > 60)
                pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
            else
                pFontHW->SetColor(DebugTextColor::DTC_GREEN);

            pFontHW->Out(GetMainInfoStats, GetInfoScale, "GPU load: %u%%", GpuLoad);
        }

        pFontHW->OnRender();
    }
}