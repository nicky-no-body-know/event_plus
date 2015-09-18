#include "comm.h"
#include "log.h"
#include "event_util.h"
#define FILE_FULL_LEN 1024
NAMESPACE_BEGIN

char LogStr[][10] = {"info", "warning", "debug", "trace"};

void Log::log_open(const char *path, const char *prefix, const char *name)
{
	char filePath[ FILE_PATH_LEN ];
	char filePrefix[ FILE_PRE_NAME_LEN ];
	char fileName[ FILE_NAME_LEN ];
	char fullName[ FILE_FULL_LEN ];
	assert(name != NULL);
	if (NULL == path)
	{
		getcwd(filePath, FILE_PATH_LEN);
	} else
	{
		snprintf(filePath, FILE_PATH_LEN, "%s", path);
	}
	int pathLen = strlen(filePath);
	if ( filePath[pathLen - 1] == '/')
	{
		filePath[pathLen - 1] = 0;
	}

	filePrefix[0] = '\0';
	if (NULL != prefix)
	{
		snprintf(filePrefix, FILE_PRE_NAME_LEN, "%s", prefix);
	}

	snprintf(fileName, FILE_NAME_LEN, "%s", name);
	
	snprintf(fullName, FILE_FULL_LEN, "%s/%s-%s.log", filePath, filePrefix, fileName);
	_fd = EventUtil_open(fullName, O_RDWR | O_CREAT | O_APPEND);
}

void Log::log_p(int level, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("[%s][%s][%s][%d]:", LogStr[level], __FILE__, __FUNCTION__, __LINE__);
	printf(fmt, args);
	printf("\n");
	va_end(args);
}

void Log::log_close()
{
	::close(_fd);
}
NAMESPACE_END
