#ifndef EVENT_PLUS_LOG_H
#define EVENT_PLUS_LOG_H

#include "comm.h"
#define FILE_PATH_LEN 20
#define FILE_PRE_NAME_LEN 20
#define FILE_NAME_LEN 20
NAMESPACE_BEGIN
enum
{
	LOG_INFO = 0,
	LOG_WARNING = 1,
	LOG_DEBUG = 2,
	LOG_TRACE = 3
};
class Log
{
public:
	Log() : _fd(-1) 
	{
		memset(_filePath, 0, sizeof(_filePath));
		memset(_filePrefix, 0, sizeof(_filePrefix));
		memset(_fileName, 0, sizeof(_fileName));
	}
	void log_open(const char *filePath, const char *filePrefix, const char *fileName);
	void log_p(int level, const char *fmt, ...);
	void log_close();
private:
	Log(const Log&);
	Log& operator=(const Log&);

	int _fd;
	char _filePath[ FILE_PATH_LEN ];
	char _filePrefix[ FILE_PRE_NAME_LEN ];
	char _fileName[ FILE_NAME_LEN ];
};

#define LOG_OPEN(filePath,filePrefix,fileName) Log::log_open(filePath, filePrefix, fileName)
#define LOG_P(level, fmt, args...) Log::log_p(level, fmt, ##args)
#define LOG_CLOSE() Log::log_close()
NAMESPACE_END
#endif
