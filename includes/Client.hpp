/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 08:50:25 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/12 14:18:19 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class Server;
class HttpRequest;
class HttpResponse;

typedef std::vector<std::string>::iterator	strVecIt;
typedef std::map<std::string, std::string>	mapStrStr;
typedef mapStrStr::iterator					mapStrStrIt;

class Client {
	private:
		Server			*_server;
		HttpRequest		*_request;
		HttpResponse	*_response;
		mapStrStr		_env;
		int				_fd;
		std::time_t		_creationDate;
		bool			_error;

	public:
		Client();
		Client(Server *server, int fd);
		Client(const Client &cpy);
		~Client();

		Client &operator=(const Client &rhs);

		Server			*getServer() const;
		HttpRequest		*getRequest() const;
		HttpResponse	*getResponse() const;
		std::string		getRawRequest() const;
		bool			appendRequest(const std::string str);
		void			sendResponse();
		void			deleteRequest();
		void			deleteEnv();
		void			deleteResponse();
		int				getFd() const;
		bool			olderThan(int sec) const;
		void			setError();
		bool			error() const;
		void			addEnv(std::string name, std::string value);
		mapStrStr		getEnv() const;
};

std::ostream &operator<<(std::ostream &o, const Client &client);

#endif
