#include "stdafx.h"
#include "../xrCore/cpuid.h"
#include "device.h"
#include "Stats.h"
#include "GameFont.h"
#include "NVIDIA.h"
#include "AMD.h"

enum DebugTextColor : DWORD
{
    DTC_RED = 0xFFF0672B,
    DTC_YELLOW = 0xFFF6D434,
    DTC_GREEN = 0xFF67F92E,
};

static const int GetMainInfoStats = 10;

void CStats::Show_HW_Stats()
{
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
        }

        pFontHW->SetHeightI(0.018f);

        if (AvailableMem < 512 || AvailablePageFileMem < 1596)
            pFontHW->SetColor(DebugTextColor::DTC_RED);
        else if (AvailableMem < 768 || AvailablePageFileMem < 2048)
            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
        else
            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

        // Draw all your stuff
        pFontHW->Out(GetMainInfoStats, 25, "MEM AVAILABLE: %0.0fMB", AvailableMem); // Physical memory available
        pFontHW->Out(GetMainInfoStats, 40, "PAGE AVAILABLE: %0.0fMB", AvailablePageFileMem); // Pagefile memory available
        pFontHW->Out(GetMainInfoStats, 55, "PAGE APPUSED: %0.0fMB", PageFileMemUsedByApp); // Physical memory used by app

        if (PhysMemoryUsedPercent > 80.0)
            pFontHW->SetColor(DebugTextColor::DTC_RED);
        else if (PhysMemoryUsedPercent > 60.0)
            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
        else
            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

        pFontHW->Out(GetMainInfoStats, 70, "MEM USED: %0.0f%%", PhysMemoryUsedPercent); // Total Phys. memory load (%)

        if (cpuLoad > 80.0)
            pFontHW->SetColor(DebugTextColor::DTC_RED);
        else if (cpuLoad > 60.0)
            pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
        else
            pFontHW->SetColor(DebugTextColor::DTC_GREEN);

        pFontHW->Out(GetMainInfoStats, 85, "CPU LOAD: %0.0f%%", cpuLoad); // CPU load

        // get MT Load
        float dwScale = 100;
        for (size_t i = 0; i < CPU::ID.m_dwNumberOfProcessors; i++)
        {
            pFontHW->Out(GetMainInfoStats, dwScale, "CPU %u: %0.0f%%", i, CPU::ID.fUsage[i]);
            dwScale += 15;
        }

        if (GpuLoad != u32(-1))
        {
            if (GpuLoad > 80)
                pFontHW->SetColor(DebugTextColor::DTC_RED);
            else if (GpuLoad > 60)
                pFontHW->SetColor(DebugTextColor::DTC_YELLOW);
            else
                pFontHW->SetColor(DebugTextColor::DTC_GREEN);

            pFontHW->Out(GetMainInfoStats, dwScale, "GPU LOAD: %u%%", GpuLoad);
        }

        pFontHW->OnRender();
    }
}