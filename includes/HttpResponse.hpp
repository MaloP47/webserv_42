/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:27:41 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/11 15:39:17 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "webserv.h"
# include "HttpRequest.hpp"
# include "Server.hpp"

class Client;
class Server;
class HttpRequest;

class HttpResponse {
	private:
		Client		*_client;
		std::string	_statusLine;
		int			_statusCode;
		std::size_t	_contentLength;
		std::string	_mime;
		std::string	_header;

		int		sendData(const void *data, int len);
		void	setStatusLine();
		void	setServerHeader();
		void	setDateHeader();
		void	setContentTypeHeader();
		void	setKeepAliveConnectionHeader();
		void	setContentLengthHeader();
	public:
		HttpResponse();
		HttpResponse(Client *client);
		HttpResponse(const HttpResponse &cpy);
		~HttpResponse();

		HttpResponse &operator=(const HttpResponse &rhs);
		
		void		createHeader();
		void		sendHeader();
		void		sendResponse();
		void		sendContent(std::ifstream &file);
		void		sendErrorPage(int num);
		void		error(int num);
		Server		*getServer() const;
		HttpRequest	*getRequest() const;
		int			getStatusCode() const;
		std::string	getMime() const;
		std::string	getHeader() const;
};

std::ostream &operator<<(std::ostream &o, const HttpResponse &response);

#endif
