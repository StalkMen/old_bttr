#include "stdafx.h"
#pragma hdrstop

#include "resource.h"
#include "log.h"

#include <mutex> //KRodin: ???? ??????? ? xrCore.h
#include <sstream> //??? std::stringstream

static string_path			logFName = "engine.log";
static BOOL 				no_log = TRUE;
static BOOL					no_log_owerflow = TRUE;
static std::recursive_mutex logCS;
static LogCallback			LogCB = nullptr;
xr_vector<std::string>* LogFile = nullptr;
IWriter* LogWriter;

void FlushLog() //???? ???????
{}

void AddOne(std::string& split, bool first_line)
{
	if (!LogFile)
		return;

	std::lock_guard<decltype(logCS)> lock(logCS);

#ifdef DEBUG
	OutputDebugString(split.c_str()); //????? ? ???????? ???????
	OutputDebugString("\n");
#endif

	if (LogCB)
		LogCB(split.c_str()); //?????? ? ?????? ???????????? ?????.

	LogFile->push_back(split); //????? ? ???????

	if (!LogWriter) return;

	if (first_line)
	{
		SYSTEMTIME lt;
		GetLocalTime(&lt);
		char buf[64];
		std::snprintf(buf, 64, "\n[%02d:%02d:%02d]: ", lt.wHour, lt.wMinute, lt.wSecond);
		split = buf + split;
	}
	else
	{
		split = "\n" + split;
	}

	//????? ? ???-????
	LogWriter->w(split.c_str(), split.length());
	LogWriter->flush();
}

void Log(const char* s)
{
	std::string str(s);

	if (str.empty()) return; //?????? ????? - ???????

	bool not_first_line = false;
	bool have_color = false;
	auto color_s = str.front();
	if ( //???? ? ?????? ?????? ???????? ???
		color_s == '-' //???????
		|| color_s == '~' //??????
		|| color_s == '!' //???????
		|| color_s == '*' //?????
		|| color_s == '#' //?????????
		) have_color = true;

	std::stringstream ss(str);
	for (std::string item; std::getline(ss, item);) //????????? ????? ?? "\n"
	{
		if (not_first_line && have_color)
		{
			item = ' ' + item;
			item = color_s + item; //???? ????, ????? ?????? ??????? ????????? ????-?????? ?????, ????? ? ??????? ???????? ???? ??? ?????? ??????, ? ?? ?????? ??????.
		}
		AddOne(item, !not_first_line);
		not_first_line = true;
	}
}

void __cdecl Msg(const char* format, ...) //KRodin: ????? ??????????? ?? ?????? ??????
{
	va_list args;
	va_start(args, format);
	int buf_len = std::vsnprintf(nullptr, 0, format, args);
	auto strBuf = std::make_unique<char[]>(buf_len + 1);
	std::vsnprintf(strBuf.get(), buf_len + 1, format, args);
	Log(strBuf.get());
}

void Log(const char* msg, const char* dop) { //???? ??????
	char buf[1024];
	if (dop)
		std::snprintf(buf, sizeof(buf), "%s %s", msg, dop);
	else
		std::snprintf(buf, sizeof(buf), "%s", msg);
	Log(buf);
}

void Log(const char* msg, u32 dop) { //???? ??????
	char buf[1024];
	std::snprintf(buf, sizeof(buf), "%s %d", msg, dop);
	Log(buf);
}

void Log(const char* msg, int dop) {
	char buf[1024];
	std::snprintf(buf, sizeof(buf), "%s %d", msg, dop);
	Log(buf);
}

void Log(const char* msg, float dop) {
	char buf[1024];
	std::snprintf(buf, sizeof(buf), "%s %f", msg, dop);
	Log(buf);
}

void Log(const char* msg, const Fvector& dop) {
	char buf[1024];
	std::snprintf(buf, sizeof(buf), "%s (%f,%f,%f)", msg, dop.x, dop.y, dop.z);
	Log(buf);
}

void Log(const char* msg, const Fmatrix& dop) {
	char buf[1024];
	std::snprintf(buf, sizeof(buf), "%s:\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n", msg, dop.i.x, dop.i.y, dop.i.z, dop._14_
		, dop.j.x, dop.j.y, dop.j.z, dop._24_
		, dop.k.x, dop.k.y, dop.k.z, dop._34_
		, dop.c.x, dop.c.y, dop.c.z, dop._44_);
	Log(buf);
}

void SetLogCB(LogCallback cb)
{
	LogCB = cb;
}

const char* log_name()
{
	return logFName;
}

void InitLog()
{
	R_ASSERT(!LogFile);
	LogFile = xr_new< xr_vector<std::string> >();
}

void CreateLog(BOOL nl, BOOL nlo)
{
	no_log = nl;
	no_log_owerflow = nlo;

	strconcat(sizeof(logFName), logFName, Core.ApplicationName, "_", Core.UserName, ".log");

	if (FS.path_exist("$logs$"))
		FS.update_path(logFName, "$logs$", logFName);

	if (!no_log)
	{
		//Alun: Backup existing log
		xr_string backup_logFName = EFS.ChangeFileExt(logFName, ".bkp");
		FS.file_rename(logFName, backup_logFName.c_str(), true);
		//-Alun
		LogWriter = FS.w_open(logFName);
		if (!LogWriter)
		{
			MessageBox(nullptr, "Can't create log file.", "Error", MB_ICONERROR);
			abort();
		}

		for (u32 it = 0; it < LogFile->size(); it++)
		{
			auto str = (*LogFile)[it];
			str = "\n" + str;
			LogWriter->w(str.c_str(), str.length());
		}

		LogWriter->flush();
	}
	LogFile->reserve(128);
}

void CloseLog()
{
	if (LogWriter)
		FS.w_close(LogWriter);

	FlushLog();
	LogFile->clear();
	xr_delete(LogFile);
}