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
#include "chttpd.h"
#include "request.h"
#include "helpers.h"


const char *
chttpd_request_header_get(struct chttpd_connection *req, const char *name) {
    int i;
    const char *header;

    for (i = 0; i < req->headerscount; i++) {
        header = req->headers[i];
        if (strcasestr(header, name) == header) {
            return trim((char *)(header + strlen(name) + 1));
        }
    }

    return NULL;
}


int
chttpd_request_parse(struct chttpd_connection *req) {
    char *saveptr;
    char *linesaveptr;
    char *line;
    char *token;

    /* Preserve header and it's len */
    req->headerscount = 0;

    /* Protocol's first line */
    line = strtok_r(req->header, "\r\n", &saveptr);
    if (line == NULL) {
        goto failed;
    }

    /* Verb */
    token = strtok_r(line, " ", &linesaveptr);
    if (token == NULL) {
        goto failed;
    }

    /* Initialize the request fields */
    req->verb = token;
    req->contentlength = -1;

    /* Path */
    token = strtok_r(NULL, " ", &linesaveptr);
    if (token == NULL) {
        goto failed;
    }
    req->path = token;

    /* HTTP version */
    token = strtok_r(NULL, "/", &linesaveptr);
    if (token) {
        req->version = token;
        token = strtok_r(NULL, "\r\n", &linesaveptr);
        if (token) {
            req->version = token;
        }
    }
    else {
        req->version = NULL;
    }

    /* Read headers */
    while ((line = strtok_r(NULL, "\r\n", &saveptr))) {
        if (strcasestr(line, "connection:") == line) {
            req->connection = trim(line + 11);
        }
        else if (strcasestr(line, "content-type:") == line) {
            req->contenttype = trim(line + 13);
        }
        else if (strcasestr(line, "content-length:") == line) {
            req->contentlength = atoi(trim(line + 15));
        }
        else if (req->headerscount < (CHTTPD_REQUESTHEADERS_MAX - 1)) {
            req->headers[req->headerscount++] = line;
        }
        else {
            goto failed;
        }
    }

    return 0;

failed:
    return -1;
}
