/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 09:04:04 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/15 11:39:31 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "Client.hpp"

Client::Client(): _server(NULL), _request(NULL), _response(NULL), _error(false) {}

Client::Client(Server *server, int fd):
	_server(server), _request(NULL), _response(NULL), _fd(fd), _error(false) {
	this->_creationDate = time(0);
}

Client::Client(const Client &cpy) {
	*this = cpy;
}

Client::~Client() {
	if (this->_request)
		delete this->_request;
	if (this->_response)
		delete this->_response;
}

Client	&Client::operator=(const Client &rhs) {
	this->_server = rhs._server;
	this->_request = rhs._request;
	this->_response = rhs._response;
	this->_fd = rhs._fd;
	this->_creationDate = rhs._creationDate;
	this->_error = rhs._error;
	return (*this);
}

bool	Client::appendRequest(const string str) {
	if (!this->_request)
		this->_request = new HttpRequest(this);
	this->_creationDate = time(0);
	return (this->_request->appendRequest(str));
}

Server	*Client::getServer() const {
	return (this->_server);
}

HttpRequest	*Client::getRequest() const {
	return (this->_request);
}

HttpResponse	*Client::getResponse() const {
	return (this->_response);
}

string	Client::getRawRequest() const {
	if (this->_request)
		return (this->_request->getRawRequest());
	else
		return (NULL);
}

void	Client::sendResponse() {
	this->_response = new HttpResponse(this);
	this->_response->sendResponse();
}

void	Client::deleteRequest() {
	if (this->_request)
		delete this->_request;
	this->_request = NULL;
}

void	Client::deleteEnv() {
	this->_env.clear();
}

void	Client::deleteResponse() {
	if (this->_response)
		delete this->_response;
	this->_response = NULL;
}

int	Client::getFd() const {
	return (this->_fd);
}

bool	Client::olderThan(int sec) const {
	if (static_cast<int>(time(0) - this->_creationDate) > sec)
		return (true);
	return (false);
}

void	Client::setError() {
	this->_error = true;
}

bool	Client::error() const {
	return (this->_error);
}

void	Client::addEnv(string name, string value) {
	this->_env.insert(pair<string, string>(name, value));
}

mapStrStr	Client::getEnv() const {
	return (this->_env);
}

ostream	&operator<<(ostream &o, const Client &client) {
	if (client.getServer()->getLogLevel() == 0)
		return (o);
	o << RED BOLD "Client (fd: " << client.getFd() << ")" END_STYLE << endl;
	if (client.getServer())
		o << *client.getServer() << endl;
	if (client.getRequest())
		o << *client.getRequest();
	if (client.getResponse())
		o << *client.getResponse();
	o << endl;
	return(o);
}
