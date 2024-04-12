/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:13:35 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/10 09:13:32 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.h"
# include "Webserv.hpp"

class Webserv;

class Server {
	private:
		Webserv				*_webserv;
		int					_port;
		std::string			_host;
		std::string 		_name;
		int					_fd;
		struct sockaddr_in	_addr;

		int		init();
		void	setAddr();

	public:
		Server();
		Server(Webserv *webserv, int port, std::string host, std::string name);
		Server(const Server &cpy);
		~Server();

		Server &operator=(const Server &rhs);

		std::string	getName() const;
		std::string	getHost() const;
		int			getPort() const;
		int			getFd() const;
		int			getLogLevel() const;
};

std::ostream &operator<<(std::ostream &o, const Server &server);

#endif
