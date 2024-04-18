/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:43:55 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/18 18:40:23 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "webserv.h"
# include "AServerAttributes.hpp"
# include "Server.hpp"
# include "Client.hpp"

class Server;
class Client;

typedef map<int, Client>::iterator	clientIt;
typedef vector<Server>::iterator	serverIt;

class Webserv{
	private:
		int					_logLevel;
		int					_epoll_fd;
		bool				_run;
		string				_command;
		vector<Server>		_servers;
		map<int, Client>	_clients;

		int		start();
		int		initEpoll();
		int		acceptConnection(int fd);
		bool	isServerFd(int fd);
		bool	isFullCommand() const;
		void	processStdIn();
		void	processCommand();
		void	deleteClient(int fd);
		void	closeUnusedSockets();
		void	processRequest(int fd);
		void	addServer(Server &serv);
		Server	*getServer(int fd);

		Webserv();
		Webserv(const Webserv &cpy);

		Webserv &operator=(const Webserv &rhs);

	public:
		Webserv(string config);
		~Webserv();

		int		getLogLevel() const;
};

#endif
