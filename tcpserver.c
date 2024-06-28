#include <stdio.h>

#include "tcpserver.h"


#undef CAIO_ARG1
#undef CAIO_ARG2
#undef CAIO_ENTITY
#define CAIO_ENTITY tcpserver
#include "caio/generic.c"
