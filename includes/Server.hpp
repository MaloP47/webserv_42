/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:13:35 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/14 17:52:20 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.h"
# include "Webserv.hpp"

class Webserv;

typedef vector<enum HttpMethod>::iterator	methodeIt;

class Server {
	private:
		Webserv					*_webserv;
		int						_port;
		string					_host;
		string					_name;
		int						_fd;
		struct sockaddr_in		_addr;
		bool					_directoryListing;
		string					_root;
		vector<string>			_indexes;
		int						_maxBodySize;
		vector<enum HttpMethod>	_allowedMethod;

		int		init();
		void	setAddr();

	public:
		Server();
		Server(Webserv *webserv, int port, string host, string name);
		Server(const Server &cpy);
		~Server();

		Server &operator=(const Server &rhs);

		int				getFd() const;
		int				getPort() const;
		int				getLogLevel() const;
		int				getMaxBodySize() const;
		bool			getDirectoryListing() const;
		string			getName() const;
		string			getHost() const;
		string			getRoot() const;
		vector<string>	getIndexes() const;
		bool			methodeAllowed(enum HttpMethod methode);
};

ostream &operator<<(ostream &o, const Server &server);

#endif
