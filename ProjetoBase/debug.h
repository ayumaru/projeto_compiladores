#ifndef __DEBUG__
#define __DEBUG__

#include "funcoes.h"

#ifdef DEBUG
#define debug(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...) ((void)0)
#endif

#endif 