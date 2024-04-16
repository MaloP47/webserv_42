/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 08:50:25 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/15 16:53:42 by mpeulet          ###   ########.fr       */
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

typedef vector<string>::iterator	strVecIt;
typedef map<string, string>			mapStrStr;
typedef mapStrStr::iterator			mapStrStrIt;

class Client {
	private:
		Server			*_server;
		HttpRequest		*_request;
		HttpResponse	*_response;
		mapStrStr		_env;
		int				_fd;
		time_t			_creationDate;
		bool			_error;

	public:
		Client();
		Client(Server *server, int fd);
		Client(const Client &cpy);
		~Client();

		Client &operator=(const Client &rhs);

		int				getFd() const;
		bool			error() const;
		bool			olderThan(int sec) const;
		bool			appendRequest(const string str);
		void			setError();
		void			deleteEnv();
		void			sendResponse();
		void			deleteRequest();
		void			deleteResponse();
		void			addEnv(string name, string value);
		string			getRawRequest() const;
		Server			*getServer() const;
		mapStrStr		getEnv() const;
		HttpRequest		*getRequest() const;
		HttpResponse	*getResponse() const;
};

ostream &operator<<(ostream &o, const Client &client);

#endif
