#pragma once
#include <ctime>
#include <stdarg.h>
#include <string>

#pragma warning( disable : 4996 )



class CLog {
	CLog() {};
	CLog(std::string filename) { 
		fptr = fopen(filename.c_str(), "w+");
	};

	~CLog() { if(fptr) fclose(fptr); };
	FILE* fptr = NULL;
	//static int logLevel; // 0 - none ; 1 - all ; 
public:
#define LOG_BUFF_SIZE 4096
	//static void setLevel(int l) { logLevel = l; }
	static void trace(int logLevel , const char* format, ...) {
		static CLog _log;
		static char buffer[LOG_BUFF_SIZE];
		if (_log.fptr == NULL)
		{
			//if (logLevel < 0)
			//{
			//	_log.fptr = fopen("ed_fm_trace.Debug.log", "w+");
			//}
			//else  if(logLevel > 0)
			{
				_log.fptr = fopen("ed_fm_trace.log", "w+");
			}
		}
		if (logLevel != 0)
		{
			if (_log.fptr == NULL)
				return;
			va_list args;
			va_start(args, format);
			vsprintf_s(buffer, LOG_BUFF_SIZE, format, args);
			perror(buffer);
			va_end(args);
			fprintf(_log.fptr, "%s", buffer);
			//fflush(_log.fptr);
		}
	};
	static void cotrace(const char* format, ...) {
		static CLog _log("ed_fm_co.txt");
		static char buffer[LOG_BUFF_SIZE];
		if (_log.fptr == NULL)
			return;
		//if (logLevel != 0)
		{
			va_list args;
			va_start(args, format);
			vsprintf_s(buffer, LOG_BUFF_SIZE, format, args);
			perror(buffer);
			va_end(args);

			fprintf(_log.fptr, "%s", buffer);
			//fflush(_log.fptr);
		}
	};
};

#pragma warning( default : 4996 )