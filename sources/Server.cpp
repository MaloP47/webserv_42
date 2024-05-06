/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:39:36 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/06 14:55:22 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include <vector>
#include "Server.hpp"

Server::Server()
	:_webserv(NULL), _fd(-1), _port(8080), _host("127.0.0.1"), _name("default") {}

Server::Server(Webserv *webserv, int port, string host, string name) :
	_webserv(webserv), _fd(-1), _port(port), _host(host), _name(name) {
	// TEMP : set manually for testing purpose
	this->_directoryListing = true;
	this->_root = "/home/gbrunet/Desktop/webserv_42/www";
	this->_indexes.push_back("index.html");
	this->_allowedMethod.push_back(GET);
	this->_allowedMethod.push_back(POST);
	this->init();
}

Server::Server( Webserv *webserv, vector<ConfigServer> const & conf, int i ) :
	_webserv( webserv ),
	_fd( -1 ),
	_port( conf[i].getPort() ),
	_host( conf[i].getHost() ),
	_name( conf[i].getName() ),
	_directoryListing( conf[i].getDirectoryListing() ),
	_root( conf[i].getRoot() ),
	_index( conf[i].getIndex() ),
	_maxBodySize( conf[i].getMaxBodySize() ),
	_allowedMethod( conf[i].getAllowedMethod() ),
	_locationBlock( conf[i].getLocationBlock() ),
	_errorPages( conf[i].getErrorPages() ),
	_returnURI( conf[i].getReturnURI() ),
	_uploadPath( conf[i].getUploadPath() ),
	_binPath( conf[i].getBinPath() ),
	_cgiExtension( conf[i].getCgiExtension() )
	{
		vector<string> indexes = split_trim(this->_index, ",");
		for (strVecIt it = indexes.begin(); it != indexes.end(); it++) {
			if (*it != "")
				this->_indexes.push_back(*it);
		}
		for (int j = 0; j < i; j++) {
			if (conf[i].getPort() == conf[j].getPort())
				return ;
		}
		this->init() ;
	}

Server::Server(Server const &cpy) {
	*this = cpy;
}

Server::~Server() {}

Server &Server::operator=(const Server &rhs) {
	this->_webserv = rhs._webserv;
	this->_fd = rhs._fd;
	this->_port = rhs._port;
	this->_host = rhs._host;
	this->_name = rhs._name;
	this->_directoryListing = rhs._directoryListing;
	this->_root = rhs._root;
	this->_indexes = rhs._indexes;
	this->_maxBodySize = rhs._maxBodySize;
	this->_allowedMethod = rhs._allowedMethod;
	this->_locationBlock = rhs._locationBlock;
	this->_errorPages = rhs._errorPages;
	this->_returnURI = rhs._returnURI;
	this->_uploadPath = rhs._uploadPath;
	this->_addr = rhs._addr;
	this->_binPath = rhs._binPath;
	this->_cgiExtension = rhs._cgiExtension;
	return (*this);
}

int	Server::init() {
	int	yes = 1;

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0) {
		perror("socket");
		return (ERR_SOCKET);
	}
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) {
		perror("setsockopt");
		return (ERR_SOCK_OPT);
	}
	this->setAddr();
	if (bind(this->_fd, (struct sockaddr *)&this->_addr, sizeof(struct sockaddr_in)) < 0) {
		perror("bind");
		return (ERR_BIND);
	}
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) < 0) {
		perror("fcntl");
		return (ERR_FCNTL);
	}
	if (listen(this->_fd, MAX_EVENTS) < 0) {
		perror("listen");
		return (ERR_LISTEN);
	}

	return (SUCCESS);
}

void	Server::setAddr() {
	memset(&this->_addr, 0, sizeof(struct sockaddr_in));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(this->_port);
	this->_addr.sin_addr.s_addr = inet_addr(this->_host.c_str());
}

string	Server::getName() const {
	return (this->_name);
}

string	Server::getHost() const {
	return (this->_host);
}

int	Server::getPort() const {
	return (this->_port);
}

int	Server::getFd() const {
	return (this->_fd);
}

void	Server::setFd(int fd) {
	this->_fd = fd;
}

int	Server::getLogLevel() const {
	return (this->_webserv->getLogLevel());
}

bool Server::getDirectoryListing() const {
	return (this->_directoryListing);
}

string	Server::getRoot() const {
	return (this->_root);
}

vector<string>	Server::getIndexes() const {
	return (this->_indexes);
}

int	Server::getMaxBodySize() const {
	return (this->_maxBodySize);
}

vector<Location> const &	Server::getLocation() const {
	return (this->_locationBlock);
}

vector<enum HttpMethod>	Server::getAllowedMethod() const {
	return (this->_allowedMethod);
}

vector<string> const &	Server::getBinPath( void ) const {
	return (this->_binPath);
}

vector<string> const &	Server::getCgiExtension( void ) const {
	return (this->_cgiExtension);
}

bool	Server::methodeAllowed(enum HttpMethod methode) {
	for (methodeIt it = this->_allowedMethod.begin(); it != this->_allowedMethod.end(); it++)
		if (*it == methode)
			return (true);
	return (false);
}

map<int,string> const &	Server::getErrorPages( void ) const { return _errorPages ;}
map<int,string> const &	Server::getReturnURI( void ) const { return _returnURI ; }
string const &			Server::getUploadPath( void ) const { return _uploadPath ; }


ostream	&operator<<(ostream &o, const Server &server) {
	o << CYAN BOLD<< server.getName() << END_STYLE " → " GREEN BOLD;
	o << server.getHost() << END_STYLE ":" YELLOW BOLD << server.getPort();
	o << END_STYLE " (fd: " << server.getFd() << ")";
	return(o);
}

void	Server::abstraction( void ) const {}
