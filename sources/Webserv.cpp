/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maburnet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:49:31 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/06 18:37:10 by maburnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "Webserv.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

Webserv::Webserv() {}

Webserv::Webserv( vector<ConfigServer> const & conf ) :
	_logLevel(1) {
	for ( size_t i = 0; i < conf.size(); i++ ) {
		Server serv(this, conf, i) ;
		this->addServer(serv) ;
	}
	if (this->initEpoll() != SUCCESS)
		return ;
	this->start();
}

Webserv::Webserv(const Webserv &cpy) {
	*this = cpy;
}

Webserv::~Webserv() {	
	for (clientIt it = this->_clients.begin(); it != this->_clients.end(); it++)
		close(it->second.getFd());
	for (serverIt it = this->_servers.begin(); it != this->_servers.end(); it++)
		close(it->getFd());
	close(this->_epoll_fd);
}

Webserv	&Webserv::operator=(const Webserv &rhs) {
	this->_servers = rhs._servers;
	this->_clients = rhs._clients;
	this->_epoll_fd = rhs._epoll_fd;
	this->_command = rhs._command;
	this->_run = rhs._run;
	this->_logLevel = rhs._logLevel;
	return (*this);
}

void	Webserv::addServer(Server &serv) {
	this->_servers.push_back(serv);
}

int	Webserv::initEpoll() {
	struct epoll_event event;

	this->_epoll_fd = epoll_create(MAX_EVENTS);
	if (this->_epoll_fd < 0) {
		cerr << timeStamp() << CYAN << " Epoll_create " << THIN ITALIC;
		perror("");
		cerr << END_STYLE;
		return (ret(ERR_EPOLL_CREATE));
	}
	for(serverIt it = this->_servers.begin(); it != this->_servers.end(); it++) {
		memset(&event, 0, sizeof(struct epoll_event));
		if (it->getFd() != -1) {
			event.events = EPOLLIN;
			event.data.fd = it->getFd();
			if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, it->getFd(), &event) < 0) {
				cerr << timeStamp() << CYAN << " Epoll_ctl " << THIN ITALIC;
				perror("");
				cerr << END_STYLE;
				return (ret(ERR_EPOLL_CTL));
			}
			this->_serversFd[it->getFd()].push_back(&(*it));
		} else {
			for (serverIt sIt = this->_servers.begin(); sIt != it; sIt++) {
				if (sIt->getPort() == it->getPort()) {
					it->setFd(sIt->getFd());
					this->_serversFd[it->getFd()].push_back(&(*it));
					break;
				}
			}
		}
		cout << *it << endl;
	}
	cout << endl;
	memset(&event, 0, sizeof(struct epoll_event));
	event.events = EPOLLIN;
	event.data.fd = 0;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, 0, &event) < 0) {
		cerr << timeStamp() << CYAN << " Epoll_ctl " << THIN ITALIC;
		perror("");
		cerr << END_STYLE;
		return (ret(ERR_EPOLL_CTL));
	}
	return (SUCCESS);
}

void	Webserv::closeUnusedSockets() {
	int	fd;

	for (clientIt it = this->_clients.begin(); it != this->_clients.end();) {
		if (!it->second.olderThan(CLIENT_TIMEOUT))
			++it;
		else {
			fd = it->second.getFd();
			if (this->getLogLevel() > 0) {
				cout << timeStamp() << BLUE BOLD ITALIC " Closing " RED "Client (fd: " << fd;
				cout << ")" END_STYLE << endl;
			}
			++it;
			this->deleteClient(fd);
		}
	}
}

int	Webserv::start() {
	struct epoll_event	events[MAX_EVENTS];
	int					nfds;
	uint32_t			event;

	this->_run = true;
	while (this->_run && !env()->ctrl_c) {
		nfds = 0;
		this->closeUnusedSockets();
		nfds = epoll_wait(this->_epoll_fd, events, MAX_EVENTS, EPOLL_TIMEOUT);
		if (nfds < 0 && !env()->ctrl_c) {
			cerr << timeStamp() << CYAN << " Epoll_wait " << THIN ITALIC;
			perror("");
			cerr << END_STYLE;
			return (ret(ERR_EPOLL_WAIT));
		}
		for (int i = 0; i < nfds; i++) {
			event = events[i].events;
			if ((event & EPOLLERR) || (event & EPOLLHUP)) {
				cerr << timeStamp() << " " CYAN << "Epoll " << THIN ITALIC;
				cerr << "events error" << END_STYLE << endl;
				close(events[i].data.fd);
			}
			else if (events[i].data.fd == 0)
				this->processStdIn();
			else if (this->isServerFd(events[i].data.fd))
				this->acceptConnection(events[i].data.fd);
			else
				this->processRequest(events[i].data.fd);
		}
	}
	return (SUCCESS);
}

bool	Webserv::isServerFd(int fd) {
	for(serverIt it = this->_servers.begin(); it != this->_servers.end(); it++)
		if (fd == it->getFd())
			return (true);
	return (false);
}

Server	*Webserv::getServer(int fd) {
	for(serverIt it = this->_servers.begin(); it != this->_servers.end(); it++)
		if (fd == it->getFd())
			return (&(*it));
	return (NULL);
}

int	Webserv::acceptConnection(int fd) {
	struct sockaddr		in_addr;
	socklen_t			in_addr_len = sizeof(struct sockaddr);
	int					connected_fd;
	struct epoll_event	event;

	connected_fd = accept(fd, &in_addr, &in_addr_len);
	if (connected_fd < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			cerr << timeStamp() << CYAN << " Connect " << THIN ITALIC;
			cerr << "connection already processed" END_STYLE;
		} else {
			cerr << timeStamp() << CYAN << " Accept " << THIN ITALIC;
			perror("");
			cerr << END_STYLE;
		}
		return (ret(ERR_ACCEPT));
	}
	if (fcntl(connected_fd, F_SETFL, O_NONBLOCK) < 0) {
		cerr << timeStamp() << CYAN << " Fcntl " << THIN ITALIC;
		perror("");
		cerr << END_STYLE;
		return (ret(ERR_FCNTL));
	}
	memset(&event, 0, sizeof(struct epoll_event));
	event.events = EPOLLIN;
	event.data.fd = connected_fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, connected_fd, &event) < 0) {
		cerr << timeStamp() << CYAN << " Epoll_ctl " << THIN ITALIC;
		perror("");
		cerr << END_STYLE;
		return (ret(ERR_EPOLL_CTL));
	}
	this->_clients[connected_fd] = Client(this->_serversFd[fd], connected_fd);
	return (SUCCESS);
}

void	Webserv::deleteClient(int fd) {
	epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	if (close(fd) < 0) {
		cerr << timeStamp() << CYAN << " Close " << THIN ITALIC;
		perror("");
		cerr << END_STYLE;
		ret(ERR_CLOSE);
	}
	this->_clients.erase(fd);
}

void	Webserv::processRequest(int fd) {
	char	buf[BUFFER_SIZE + 1];
	int		received;
	Client	&client = this->_clients[fd];
	string	header;

	received = recv(fd, buf, BUFFER_SIZE, 0);
	if (received <= 0) {
		this->deleteClient(fd);
		return ;
	}
	buf[received] = 0;
	if (client.appendRequest(buf, received) && !client.error()) {
		client.sendResponse();
		cout << client;
		if (client.error()) {
			this->deleteClient(fd);
			return ;
		}
		if (client.getRequest()->keepAlive()) {
			client.deleteRequest();
			client.deleteResponse();
			client.deleteEnv();
		}
		else
			this->deleteClient(fd);
	}
	else if (client.error())
		this->deleteClient(fd);
}

void	Webserv::processStdIn() {
	char		buf[BUFFER_SIZE + 1];
	int			received;

	received = read(0, buf, BUFFER_SIZE);
	if (received < 0)
		return ;
	buf[received] = 0;
	this->_command += buf;
	if (isFullCommand()) {
		this->processCommand();
		this->_command = "";
	}
}

bool	Webserv::isFullCommand() const {
	return (this->_command.find("\n") != string::npos);
}

void	Webserv::processCommand() {
	if (this->_command == "\n")
		return ;
	else if (this->_command == "exit\n"){
		cout << CYAN "exit" END_STYLE << endl;
		this->_run = false;
	} else if (this->_command == "logs 0\n") {
		cout << CYAN "logs level: 0" END_STYLE << endl;
		this->_logLevel = 0;
	} else if (this->_command == "logs 1\n") {
		cout << CYAN "logs level: 1" END_STYLE << endl;
		this->_logLevel = 1;
	} else if (this->_command == "logs 2\n") {
		cout << CYAN "logs level: 2" END_STYLE << endl;
		this->_logLevel = 2;
	} else {
		this->_command[this->_command.length() - 1] = 0;
		cerr << RED << "command not found: " END_STYLE;
		cerr << RED << this->_command << END_STYLE <<endl;
	}
}

int	Webserv::getLogLevel() const {
	return (this->_logLevel);
}
