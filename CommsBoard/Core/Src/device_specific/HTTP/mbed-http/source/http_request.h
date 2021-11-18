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

#ifndef _HTTP_REQUEST_
#define _HTTP_REQUEST_

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "../../../HTTP/mbed-http/source/http_parsed_url.h"
#include "../../../HTTP/mbed-http/source/http_request_base.h"

/**
 * @todo:
 *      - Userinfo parameter is not handled
 */

#ifndef HTTP_RECEIVE_BUFFER_SIZE
#define HTTP_RECEIVE_BUFFER_SIZE 8 * 1024
#endif


extern bool com_convert_IPString_to_sockaddr(uint16_t       ipaddr_port,
        com_char_t     *ipaddr_str,
        com_sockaddr_t *sockaddr);


/**
 * \brief HttpRequest implements the logic for interacting with HTTP servers.
 */
class HttpRequest: public HttpRequestBase
{
public:
	friend class HttpRequestBase;

	/**
	 * HttpRequest Constructor
	 *
	 * @param[in] network The network interface
	 * @param[in] method HTTP method to use
	 * @param[in] url URL to the resource
	 * @param[in] bodyCallback Callback on which to retrieve chunks of the response body.
	 If not set, the complete body will be allocated on the HttpResponse object,
	 which might use lots of memory.
	 */
	HttpRequest(http_method method, const char *url,
			const std::function<void(const char *at, uint32_t length)> &bodyCallback =
					nullptr) :
			HttpRequestBase(NULL, bodyCallback)
	{
		_error = 0;
		_response = NULL;

		_parsed_url = new ParsedUrl(url);
		_request_builder = new HttpRequestBuilder(method, _parsed_url);

//        _socket = new TCPSocket();
//        ((TCPSocket*)_socket)->open(network);

//        this->connect_socket(_parsed_url->host(), _parsed_url->port());
        _we_created_socket = true;
	}

	/**
	 * HttpRequest Constructor
	 *
	 * @param socket An open TCPSocket
	 * @param[in] method HTTP method to use
	 * @param[in] url URL to the resource
	 * @param[in] bodyCallback Callback on which to retrieve chunks of the response body.
	 If not set, the complete body will be allocated on the HttpResponse object,
	 which might use lots of memory.
	 */
	HttpRequest(int32_t socketfd, http_method method, const char *url,
			const std::function<void(const char *at, uint32_t length)> &bodyCallback =
					nullptr) :
			HttpRequestBase(socketfd, bodyCallback)
	{
		_error = 0;
		_response = nullptr;

		_parsed_url = new ParsedUrl(url);
		_request_builder = new HttpRequestBuilder(method, _parsed_url);

		_we_created_socket = false;
	}

	virtual ~HttpRequest()
	{
	}

protected:

	nsapi_error_t connect_socket(char *host, uint16_t port) override
	{

		_we_created_socket = false;
		_socket = com_socket(COM_AF_INET, COM_SOCK_STREAM, COM_IPPROTO_TCP);
		if (_socket < 0)
		{
			return NSAPI_ERROR_NO_SOCKET;
		}

		com_sockaddr_t addr;
		int32_t err = COM_SOCKETS_ERR_OK;
		if (!com_convert_IPString_to_sockaddr(port, (com_char_t *)host, &addr))
		{
			err = com_gethostbyname(reinterpret_cast<const com_char_t*>(host), &addr);

			if (err != COM_SOCKETS_ERR_OK)
			{
				com_closesocket(_socket);
				return NSAPI_ERROR_DNS_FAILURE;
			}

			com_sockaddr_in_t * address = (com_sockaddr_in_t *) &addr;
			address->sin_family = (uint8_t) COM_AF_INET;
			address->sin_port = COM_HTONS(port);
			address->sin_addr.s_addr = ((com_sockaddr_in_t*) &addr)->sin_addr.s_addr;
		}

		err = com_connect(_socket, (com_sockaddr_t*) &addr,
				sizeof(com_sockaddr_in_t));

		if (err != COM_SOCKETS_ERR_OK)
		{
			com_closesocket(_socket);
			return NSAPI_ERROR_CONNECTION_LOST;
		}
		_we_created_socket = true;
		return NSAPI_ERROR_OK;

//        return ((TCPSocket*)_socket)->connect(host, port);
	}
};

#endif // _HTTP_REQUEST_
