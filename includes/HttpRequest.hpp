/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:57:23 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/15 11:30:28 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "webserv.h"
# include "Server.hpp"
# include "Client.hpp"

class Server;
class Client;

typedef vector<string>::iterator	strVecIt;

class HttpRequest {
	private:
		Client			*_client;
		string			_rawRequest;
		bool			_goodRequest;
		enum HttpMethod	_method;
		string			_uri;
		vector<string>	_acceptedMimes;
		bool			_keepAliveConnection;
		size_t			_contentLength;
		string			_content;

		bool	isFullRequest();
		void	parse();
		void	setMethod(string str);
		void	getUriAndEnv(string str);
		void	parseConnection(string line);
		void	parseRequestLine(string line);
		void	parseAcceptedMimes(string line);
		void	parseContentLength();

	public:
		HttpRequest();
		HttpRequest(Client *client);
		HttpRequest(const HttpRequest &cpy);
		~HttpRequest();

		HttpRequest	&operator=(const HttpRequest &rhs);

		bool			isGood() const;
		bool			keepAlive() const;
		bool			appendRequest(const string str);
		string			getUri() const;
		string			getRawRequest() const;
		Server			*getServer() const;
		Client			*getClient() const;
		enum HttpMethod	getMethod() const;
};

ostream &operator<<(ostream &o, const HttpRequest &request);

#endif
