/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MBED_HTTP_PARSED_URL_H_
#define _MBED_HTTP_PARSED_URL_H_

#include "../http_parser/http_parser.h"
#include <cstring>

class ParsedUrl {
public:
    explicit ParsedUrl(const char* url) {
        struct http_parser_url parsed_url{};
        http_parser_parse_url(url, strlen(url), false, &parsed_url);

        for (size_t ix = 0; ix < UF_MAX; ix++) {
            char* value;
            if (parsed_url.field_set & (1 << ix)) {
//                value = (char*)calloc(parsed_url.field_data[ix].len + 1, 1);
            	value = (char*)pvPortMalloc(parsed_url.field_data[ix].len + 1);
            	memset(value, 0, parsed_url.field_data[ix].len + 1);

                memcpy(value, url + parsed_url.field_data[ix].off,
                       parsed_url.field_data[ix].len);
            }
            else {
//                value = (char*)calloc(1, 1);
                value = (char*)pvPortMalloc(1);
				memset(value, 0, 1);
            }

            switch ((http_parser_url_fields)ix) {
                case UF_SCHEMA:   _schema   = value; break;
                case UF_HOST:     _host     = value; break;
                case UF_PATH:     _path     = value; break;
                case UF_QUERY:    _query    = value; break;
                case UF_USERINFO: _userinfo = value; break;
                default:
                    // PORT is already parsed, FRAGMENT is not relevant for HTTP requests
                	vPortFree(value);
                    break;
            }
        }

        _port = parsed_url.port;
        if (!_port) {
            if (strcmp(_schema, "https") == 0 || strcmp(_schema, "wss") == 0) {
                _port = 443;
            }
            else {
                _port = 80;
            }
        }

        if (strcmp(_path, "") == 0) {
        	vPortFree(_path);
//            _path = (char*)calloc(2, 1);
        	_path = (char*) pvPortMalloc(2);
        	memset(_path, 0, 2);
            _path[0] = '/';
        }
    }

    ~ParsedUrl() {
        if (_schema) vPortFree(_schema);
        if (_host) vPortFree(_host);
        if (_path) vPortFree(_path);
        if (_query) vPortFree(_query);
        if (_userinfo) vPortFree(_userinfo);
    }

    [[nodiscard]] uint16_t port() const { return _port; }
    [[nodiscard]] char* schema() const { return _schema; }
    [[nodiscard]] char* host() const { return _host; }
    [[nodiscard]] char* path() const { return _path; }
    [[nodiscard]] char* query() const { return _query; }
    [[nodiscard]] char* userinfo() const { return _userinfo; }

private:
    uint16_t _port;
    char* _schema;
    char* _host;
    char* _path;
    char* _query;
    char* _userinfo;
};

#endif // _MBED_HTTP_PARSED_URL_H_
