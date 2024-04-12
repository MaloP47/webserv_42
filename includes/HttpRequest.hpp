/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:57:23 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/12 11:14:13 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "webserv.h"
# include "Server.hpp"
# include "Client.hpp"

class Server;
class Client;

class HttpRequest {
	private:
		Client						*_client;
		std::string					_rawRequest;
		bool						_goodRequest;
		enum HttpMethod				_method;
		std::string					_uri;
		std::vector<std::string>	_acceptedMimes;
		bool						_keepAliveConnection;

		bool	isFullRequest();
		void	parse();
		void	parseRequestLine(std::string line);
		void	setMethod(std::string str);
		void	parseAcceptedMimes(std::string line);
		void	parseConnection(std::string line);
		void	getUriAndEnv(std::string str);

	public:
		HttpRequest();
		HttpRequest(Client *client);
		HttpRequest(const HttpRequest &cpy);
		~HttpRequest();

		HttpRequest	&operator=(const HttpRequest &rhs);

		Server			*getServer() const;
		Client			*getClient() const;
		std::string		getRawRequest() const;
		bool			appendRequest(const std::string str);
		bool			isGood() const;
		enum HttpMethod	getMethod() const;
		std::string		getUri() const;
		bool			keepAlive() const;
};

std::ostream &operator<<(std::ostream &o, const HttpRequest &request);

#endif
