/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:43:55 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/11 12:12:29 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "webserv.h"
# include "Server.hpp"
# include "Client.hpp"

class Server;
class Client;

typedef std::map<int, Client>::iterator	clientIt;
typedef std::vector<Server>::iterator	serverIt;

class Webserv{
	private:
		std::vector<Server>		_servers;
		std::map<int, Client>	_clients;
		int						_epoll_fd;
		std::string				_command;
		bool					_run;
		int						_logLevel;

		void	addServer(Server &serv);
		int		initEpoll();
		int		start();
		int		acceptConnection(int fd);
		void	processRequest(int fd);
		void	processStdIn();
		bool	isServerFd(int fd);
		Server	*getServer(int fd);
		bool	isFullCommand() const;
		void	processCommand();
		void	closeUnusedSockets();
		void	deleteClient(int fd);

		Webserv();
		Webserv(const Webserv &cpy);

		Webserv &operator=(const Webserv &rhs);

	public:
		Webserv(std::string config);
		~Webserv();


		int		getLogLevel() const;
};

#endif
