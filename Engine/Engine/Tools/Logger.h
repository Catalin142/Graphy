#pragma once
#include "Core/Types.h"

#define INFO 0x2
#define WARNING 0xE
#define ERROR 0x4
#define FATAL 0x4F

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

template<typename ...args>
static void INTERNAL_LOG(int level, const args& ...argv)
{
#if DEBUG == 1
	std::cout << "[Graphy Engine]:";

	switch (level)
	{
		case INFO:
		SetConsoleTextAttribute(hConsole, INFO); break;

		case WARNING:
		SetConsoleTextAttribute(hConsole, WARNING); break;

		case ERROR:
		SetConsoleTextAttribute(hConsole, ERROR); break;

		case FATAL:
		SetConsoleTextAttribute(hConsole, FATAL); break;
	}

	((std::cout << " " << argv), ...) << std::endl;
	SetConsoleTextAttribute(hConsole, 0x7);
#endif
}

#if DEBUG == 1

#define GR_INFO(...)    INTERNAL_LOG(INFO,    "INFO:",				 	 __VA_ARGS__);
#define GR_WARN(...)    INTERNAL_LOG(WARNING, "AVERTISMENT:",			 __VA_ARGS__);
#define GR_ERROR(...) { INTERNAL_LOG(ERROR,   "EROARE:",					 __VA_ARGS__); DebugBreak(); }
#define GR_FATAL(...) { INTERNAL_LOG(FATAL,   "EROARE FATALA:",			 __VA_ARGS__); abort(); }

#define GR_ASSERT(x, ...) {if (x) {GR_ERROR(__VA_ARGS__); assert(0);} }
#define GR_WARNING(x, ...) {if (x) GR_WARN(__VA_ARGS__); }


#elif RELEASE == 1

#define GR_INFO(...) 
#define GR_WARN(...) 
#define GR_ERROR(...)
#define GR_FATAL(...)

#define GR_ASSERT(x, ...) {if (x) {GR_ERROR(__VA_ARGS__); assert(0);} }
#define GR_WARNING(x, ...) {if (x) GR_WARN(__VA_ARGS__); }

#endif
