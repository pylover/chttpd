#include <stdio.h>

#include "server.h"


#undef CAIO_ARG1
#undef CAIO_ARG2
#undef CAIO_ENTITY
#define CAIO_ENTITY chttpd_server
#include "caio/generic.c"
