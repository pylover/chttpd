// Copyright 2024 Vahid Mardani
/*
 * This file is part of chttpd.
 *  chttpd is free software: you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  chttpd is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with chttpd. If not, see <https://www.gnu.org/licenses/>.
 *
 *  Author: Vahid Mardani <vahid.mardani@gmail.com>
 */
#ifndef CONNECTION_H_
#define CONNECTION_H_


/* TCP connection state types */
typedef struct chttpd_connection {
    int fd;
    struct sockaddr_in localaddr;
    struct sockaddr_in remoteaddr;
    char buff[BUFFSIZE];
    size_t bufflen;
    struct chttpd_server *server;
} chttpd_connection_t;


#undef CAIO_ARG1
#undef CAIO_ARG2
#undef CAIO_ENTITY
#define CAIO_ENTITY chttpd_connection
#include "caio/generic.h"  // NOLINT


#endif  // CONNECTION_H_
