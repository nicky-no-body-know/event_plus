#ifndef EVENT_PLUS_COMM_H
#define EVENT_PLUS_COMM_H

#define NAMESPACE_BEGIN namespace event_plus {
#define NAMESPACE_END }

#define BUFLEN 1024

#include <unistd.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <map>
#include <vector>
#include <poll.h>
#include <pthread.h>
#include <semaphore.h>
#endif
