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
#include <stdlib.h>

#include "server.h"


int
main() {
    int exitstatus = EXIT_SUCCESS;
    struct chttpd_server server;
//     struct sockaddr_in bindaddr = {
//         .sin_addr = {htons(0)},
//         .sin_port = htons(3030),
//     };
//
//     if (_handlesignals()) {
//         return EXIT_FAILURE;
//     }
//
//     _caio = caio_create(MAXCONN + 1);
//     if (_caio == NULL) {
//         exitstatus = EXIT_FAILURE;
//         goto terminate;
//     }
//
// #if (CHTTPD_IOMONITORING == "epoll")
//     struct caio_epoll *epoll;
//     epoll = caio_epoll_create(_caio, MAXCONN + 1, 1);
//     if (epoll == NULL) {
//         exitstatus = EXIT_FAILURE;
//         goto terminate;
//     }
//     state.iomodule = (struct caio_iomodule*)epoll;
//     printf("Using epoll(7) as IO monitoring.\n");
//
// #elif (CHTTPD_IOMONITORING == "select")
//     struct caio_select *select;
//     select = caio_select_create(_caio, MAXCONN + 1, 1);
//     if (select == NULL) {
//         exitstatus = EXIT_FAILURE;
//         goto terminate;
//     }
//     state.iomodule = (struct caio_iomodule*)select;
//     printf("Using select(2) as IO monitoring.\n");
//
// #endif
//
//     // tcpserver_spawn(_caio, listenA, &state, bindaddr, MAXCONN);
//
//     if (caio_loop(_caio)) {
//         exitstatus = EXIT_FAILURE;
//     }
//
// terminate:
// #ifdef CAIO_EPOLL
//
//     if (caio_epoll_destroy(_caio, epoll)) {
//         exitstatus = EXIT_FAILURE;
//     }
//
// #elifdef CAIO_SELECT
//
//     if (caio_select_destroy(_caio, select)) {
//         exitstatus = EXIT_FAILURE;
//     }
//
// #endif
//
//     if (caio_destroy(_caio)) {
//         exitstatus = EXIT_FAILURE;
//     }
//
    return exitstatus;
}
