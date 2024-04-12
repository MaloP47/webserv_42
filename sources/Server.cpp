/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:39:36 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/10 09:18:26 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "Server.hpp"

Server::Server():
	_webserv(NULL), _port(8080), _host("127.0.0.1"), _name("default"), _fd(-1) {}

Server::Server(Webserv *webserv, int port, std::string host, std::string name):
	_webserv(webserv), _port(port), _host(host), _name(name), _fd(-1) {
	this->init();
}

Server::Server(Server const &cpy) {
	*this = cpy;
}

Server::~Server() {}

Server &Server::operator=(const Server &rhs) {
	this->_webserv = rhs._webserv;
	this->_port = rhs._port;
	this->_host = rhs._host;
	this->_name = rhs._name;
	this->_fd = rhs._fd;
	this->_addr = rhs._addr;
	return (*this);
}

int	Server::init() {
	int	yes = 1;

	// socket creation
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0) {
		perror("socket");
		return (ERR_SOCKET);
	}

	// reusable socket
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) {
		perror("setsockopt");
		return (ERR_SOCK_OPT);
	}

	// set the address
	this->setAddr();

	// bind socket
	if (bind(this->_fd, (struct sockaddr *)&this->_addr, sizeof(struct sockaddr_in)) < 0) {
		perror("bind");
		return (ERR_BIND);
	}

	// non blocking fd
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) < 0) {
		perror("fcntl");
		return (ERR_FCNTL);
	}

	// listen socket
	if (listen(this->_fd, MAX_EVENTS) < 0) {
		perror("listen");
		return (ERR_LISTEN);
	}

	return (SUCCESS);
}

void	Server::setAddr() {
	std::memset(&this->_addr, 0, sizeof(struct sockaddr_in));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(this->_port);
	this->_addr.sin_addr.s_addr = inet_addr(this->_host.c_str());
}

std::string	Server::getName() const {
	return (this->_name);
}

std::string	Server::getHost() const {
	return (this->_host);
}

int	Server::getPort() const {
	return (this->_port);
}

int	Server::getFd() const {
	return (this->_fd);
}

int	Server::getLogLevel() const {
	return (this->_webserv->getLogLevel());
}

std::ostream	&operator<<(std::ostream &o, const Server &server) {
	o << CYAN BOLD<< server.getName() << END_STYLE " → " GREEN BOLD;
	o << server.getHost() << END_STYLE ":" YELLOW BOLD << server.getPort();
	o << END_STYLE " (fd: " << server.getFd() << ")";
	return(o);
}
