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
#include <clog.h>
#include <carrow.h>

#include "chttpd.h"
#include "request.h"
#include "addr.h"


#undef CARROW_ENTITY
#define CARROW_ENTITY chttpd_request
#include <carrow_generic.c>  // NOLINT


#define REQ_SIZE 1024 * 10


int
headtok(char *headers, char **saveptr, char **key, char **value) {
    if ((headers != NULL) && (*saveptr != NULL)) {
        /* **saveptr must be null in the first try */
        return -1;
    }

    if ((*saveptr == NULL) && (headers == NULL)) {
        /* headers is not provided to headtok. */
        return -1;
    }

    if ((key == NULL) || (value == NULL)) {
        /* key/value provided to headtok cannot be NULL. */
        return -1;
    }

    *key = strtok_r(headers, ":", saveptr);
    *value = strtok_r(NULL, "\r\n", saveptr);

    if ((*key == NULL) || (*value == NULL)) {
        return 1;
    }

    while (((**key) == ' ') || ((**key) == '\r') || ((**key) == '\n')) {
        (*key)++;
    }

    while ((**value) == ' ' || ((**value) == '\r') || ((**value) == '\n')) {
        (*value)++;
    }

    return 0;
}


int
reqtok(char *request, char **saveptr, char **verb, char **path,
        char **version) {
    char *token;

    if (*saveptr != NULL) {
        /* **saveptr must be null. */
        return -1;
    }

    if (request == NULL) {
        /* request is not provided to reqtok. */
        return -1;
    }

    /* Parse the HTTP verb. */
    token = strtok_r(request, " ", saveptr);
    if (token != NULL) {
        *verb = token;
    }
    else {
        return -1;
    }

    /* Parse the URL. */
    token = strtok_r(NULL, " ", saveptr);
    if (token != NULL) {
        *path = token;
    }
    else {
        return -1;
    }

    /* Parse request body. */
    token = strtok_r(NULL, "/", saveptr);
    if (token != NULL) {
        token = strtok_r(NULL, "\r\n", saveptr);
        if (token != NULL) {
            *version = token;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }


    return 0;
}


void
get_first_line(const char* header, char* first_line, size_t max_length) {
    const char* newline_pos = strchr(header, '\n'); // Find the position of the first newline character
    if (newline_pos != NULL) {
        size_t chars_to_copy = newline_pos - header;
        if (chars_to_copy > max_length - 1) {
            chars_to_copy = max_length - 1; // Limit the number of characters copied to fit the destination string
        }
        strncpy(first_line, header, chars_to_copy);
        first_line[chars_to_copy] = '\0'; // Null-terminate the copied string
    }
}


void
get_headers(const char* header, char* content, size_t max_length) {
     // Find the position after the first "\r\n"
    const char* start_pos = strstr(header, "\r\n") + 2;

    // Find the position of the first "\r\n\r\n"
    const char* end_pos = strstr(header, "\r\n\r\n"); 

    if (start_pos != NULL && end_pos != NULL) {
        size_t chars_to_copy = end_pos - start_pos;
        if (chars_to_copy > max_length - 1) {
            // Limit the number of characters copied to fit the destination
            chars_to_copy = max_length - 1;
        }
        strncpy(content, start_pos, chars_to_copy);
        // NULL-terminate the copied string
        content[chars_to_copy] = '\0';
    }
}


void
requestA(struct chttpd_request_coro *self, struct chttpd_request *conn) {
    /*
    TODO:
    - Wait for headers, then Parse them
    - Find handler, otherwise 404
    */
    ssize_t bytes;
    struct mrb *buff = conn->reqbuff;
    CORO_START;
    static int e = 0;
    INFO("New conn: %s", sockaddr_dump(&conn->remoteaddr));

    while (true) {
        e = CET;

        /* tcp write */
        /* Write as mush as possible until EAGAIN */
        while (!mrb_isempty(buff)) {
            bytes = mrb_writeout(buff, conn->fd, mrb_used(buff));
            if ((bytes == -1) && CMUSTWAIT()) {
                e |= COUT;
                break;
            }
            if (bytes == -1) {
                CORO_REJECT("write(%d)", conn->fd);
            }
            if (bytes == 0) {
                CORO_REJECT("write(%d) EOF", conn->fd);
            }
        }

        /* tcp read */
        /* Read as mush as possible until EAGAIN */
        while (!mrb_isfull(buff)) {
            bytes = mrb_readin(buff, conn->fd, mrb_available(buff));
            if ((bytes == -1) && CMUSTWAIT()) {
                e |= CIN;
                break;
            }
            if (bytes == -1) {
                CORO_REJECT("read(%d)", conn->fd);
            }
            if (bytes == 0) {
                CORO_REJECT("read(%d) EOF", conn->fd);
            }
        }

        char request[REQ_SIZE];
        while (!mrb_isempty(buff)) {
            mrb_get(buff, request, REQ_SIZE);

            char *headerend = strstr(request, "\r\n\r\n");
            if (headerend == NULL) {
                continue;
            }

            /* request parser */

            /* Tokenize verb, path, and version. */
            char *saveptr = NULL;
            char *verb;
            char *path;
            char *version; 

            int max_length = 1024;
            char firstline[1024];
            get_first_line(request, firstline, max_length);

            if (reqtok(firstline, &saveptr, &verb, &path, &version) == -1) {
                CORO_REJECT("Request tokenization failed.");
            }

            
            char headers[1024];
            get_headers(request, headers, 1024);
            int headtok_resp;
            char *key;
            char *value;
            saveptr = NULL;
            
            headtok_resp = headtok(headers, &saveptr, &key, &value);
            if (headtok_resp == -1) {
                CORO_REJECT("h");
            }


            int i;
            do {
                headtok_resp = headtok(NULL, &saveptr, &key, &value);
                
                if (headtok_resp == -1) {
                    CORO_REJECT("Headers tokenization failed.");
                }

                if (headtok_resp == 1) {
                    break;
                }
                
                INFO("%d, %s, %s", __LINE__, key, value);
                
                i++;
            } while (headtok_resp == 0);
        }

        

        /* reset errno and rewait events if neccessary */
        errno = 0;
        if (mrb_isempty(buff) || (e & COUT)) {
            CORO_WAIT(conn->fd, e);
        }
    }

    CORO_FINALLY;
    if (conn->fd != -1) {
        chttpd_request_evloop_unregister(conn->fd);
        close(conn->fd);
    }
    if (mrb_destroy(conn->reqbuff)) {
        ERROR("Cannot dispose buffers.");
    }
    if (mrb_destroy(conn->respbuff)) {
        ERROR("Cannot dispose buffers.");
    }
    free(conn);
}
