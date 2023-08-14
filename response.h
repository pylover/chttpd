// Copyright 2023 Vahid Mardani
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
#ifndef RESPONSE_H_
#define RESPONSE_H_


#include "request.h"


int
chttpd_response_start(struct chttpd_request *req, const char *format, ...);


int
chttpd_response_header(struct chttpd_request *req, const char *format, ...);


int
chttpd_response_flush(struct chttpd_request *req);


int
chttpd_response_close(struct chttpd_request *req);


int
chttpd_response_finalize(struct chttpd_request *req);


int
chttpd_response_body(struct chttpd_request *req, const char *format, ...);


#endif  // RESPONSE_H_
