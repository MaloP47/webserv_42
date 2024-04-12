/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:01:52 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/12 11:48:10 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(Client *client):
	_client(client), _goodRequest(true), _method(OTHER){
}

HttpRequest::HttpRequest(const HttpRequest &cpy) {
	*this = cpy;
}

HttpRequest::~HttpRequest() {}

HttpRequest	&HttpRequest::operator=(const HttpRequest &rhs) {
	this->_client = rhs._client;
	this->_rawRequest = rhs._rawRequest;
	this->_method = rhs._method;
	this->_goodRequest = rhs._goodRequest;
	this->_uri = rhs._uri;
	this->_acceptedMimes = rhs._acceptedMimes;
	return (*this);
}

bool	HttpRequest::isFullRequest() {
	return (this->_rawRequest.find("\r\n\r\n", this->_rawRequest.length() - 4) != std::string::npos);	
}

bool	HttpRequest::appendRequest(const std::string str) {
	this->_rawRequest += str;
	if (isFullRequest()) {
		this->parse();
		return (true);
	}
	return (false);
}

std::string	HttpRequest::getRawRequest() const {
	return (this->_rawRequest);
}

void	HttpRequest::parse() {
	std::vector<std::string>	line;

	this->_goodRequest = true;
	line = split_trim(this->_rawRequest, "\r\n");

	if (line.size() < 1)
		return ;
	this->parseRequestLine(line[0]);	
	for (std::vector<std::string>::iterator it = line.begin() + 1; it != line.end(); it++) {
//		std::cout << GREEN << *it << END_STYLE << std::endl; // Need to parse some header elem
		if (findLower(*it, static_cast<std::string>("accept:"))) {
			this->parseAcceptedMimes(*it);
		}
		if (findLower(*it, static_cast<std::string>("connection:"))) {
			this->parseConnection(*it);
		}
	}
}

void	HttpRequest::parseConnection(std::string line) {
	line.erase(0, 11);
	ltrim(line);
	rtrim(line);
	this->_keepAliveConnection = findLower(line, static_cast<std::string>("keep-alive"));
}

void	HttpRequest::parseAcceptedMimes(std::string line) {
	line.erase(0, 7);
	this->_acceptedMimes = split_trim(line, ",");
}

void	HttpRequest::parseRequestLine(std::string line) {
	std::vector<std::string>	split;

	split = split_trim(line, " ");
	if (split.size() != 3) {
		this->_goodRequest = false;	
		return ;
	}
	this->setMethod(split[0]);
	this->getUriAndEnv(split[1]);
	mapStrStr temp = this->_client->getEnv();
	for (mapStrStrIt it = temp.begin(); it != temp.end(); it++)
		std::cout << it->first << " - " << it->second << std::endl;
	split = split_trim(split[2], "/");
	if (split.size() != 2) {
		this->_goodRequest = false;	
		return ;
	}
	if (std::atof(split[1].c_str()) < 1.1 || split[0] != "HTTP") {
		this->_goodRequest = false;	
		return ;
	}
}

void	HttpRequest::getUriAndEnv(std::string str) {
	std::vector<std::string>	split;
	std::vector<std::string>	vars;
	std::vector<std::string>	tempEnv;

	split = split_trim(str, "?");
	if (split.size() == 2) {
		vars = split_trim(split[1], "&");
		for (strVecIt it = vars.begin(); it != vars.end(); it++) {
			tempEnv = split_trim(*it, "=");
			if (tempEnv.size() != 2) {
				this->_goodRequest = false;
				return ;
			}
			this->_client->addEnv(decodeEnv(tempEnv[0]), decodeEnv(tempEnv[1]));
		}
	}
	this->_uri = decodeUri(split[0]);
}

Client	*HttpRequest::getClient() const {
	return (this->_client);
}

Server	*HttpRequest::getServer() const {
	return (this->getClient()->getServer());
}

void	HttpRequest::setMethod(std::string str) {
	if (str == "GET")
		this->_method = GET;
	else if (str == "POST")
		this->_method = POST;
	else if (str == "DELETE")
		this->_method = DELETE;
}

bool	HttpRequest::isGood() const {
	return (this->_goodRequest);
}

enum HttpMethod	HttpRequest::getMethod() const {
	return (this->_method);
}

std::string	HttpRequest::getUri() const {
	return (this->_uri);
}

bool	HttpRequest::keepAlive() const {
	return (this->_keepAliveConnection);
}

static std::string	stringMethod(enum HttpMethod method) {
	if (method == GET)
		return ("GET");
	else if (method == POST)
		return ("POST");
	else if (method == DELETE)
		return ("DELETE");
	return ("OTHER");
}

std::ostream &operator<<(std::ostream &o, const HttpRequest &request) {
	if (request.getServer()->getLogLevel() == 0)
		return (o);
	o << PURPLE BOLD << "Request " END_STYLE;
	o << " → " CYAN << stringMethod(request.getMethod()) << " " YELLOW;
	o << request.getUri() << END_STYLE << std::endl;
	if (request.getServer()->getLogLevel() == 2){
		o << THIN ITALIC << request.getRawRequest() << END_STYLE << std::endl;
	}
	return (o);
}
