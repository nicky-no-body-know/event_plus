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
		char cwd[10];
		getcwd(cwd, FILE_PATH_LEN);
		snprintf(filePath, FILE_PATH_LEN, "%s/../lib", cwd);
	} else
	{
		snprintf(filePath, FILE_PATH_LEN, "%s", path);
	}
	int pathLen = strlen(filePath);
	if ( filePath[pathLen - 1] == '/')
	{
		filePath[pathLen - 1] = 0;
	}

	if (NULL != prefix)
	{
		snprintf(filePrefix, FILE_PRE_NAME_LEN, "%s", prefix);
	} else
	{
		snprintf(filePrefix, FILE_PRE_NAME_LEN, "%s", "all");
	}

	snprintf(fileName, FILE_NAME_LEN, "%s", name);
	
	snprintf(fullName, FILE_FULL_LEN, "%s/%s-%s.log", filePath, filePrefix, fileName);
	printf("log file name = %s\n", fullName);
	if (-1 == (_fd = open(fullName, O_RDWR | O_CREAT | O_APPEND, 0666)))
	{
		perror("open log file failed!");
		exit(1);
	}
	printf("fd = %d\n", _fd);
}

void Log::log_p(int level, const char *file_name, const char *func, const int line, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("[%s][%s][%s][%d]:", LogStr[level], file_name, func, line);
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

void Log::log_close()
{
	::close(_fd);
}
NAMESPACE_END

