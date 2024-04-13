/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:29:38 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/13 11:24:38 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "HttpResponse.hpp"

HttpResponse::HttpResponse(): _client(NULL) {}

HttpResponse::HttpResponse(Client *client):
	_client(client), _statusCode(0), _contentLength(0) {
}

HttpResponse::HttpResponse(const HttpResponse &cpy) {
	*this = cpy;
}

HttpResponse::~HttpResponse() {}

HttpResponse &HttpResponse::operator=(const HttpResponse &rhs) {
	this->_client = rhs._client;
	this->_statusCode = rhs._statusCode;
	this->_contentLength = rhs._contentLength;
	return (*this);
}

void	HttpResponse::setStatusLine() {
	this->_header = "HTTP/1.1 " + StatusCode::name(this->_statusCode) + "\r\n";
}

void	HttpResponse::setServerHeader() {
	if (this->_client->getServer()->getName() != "")
		this->_header += "Server: " + this->_client->getServer()->getName() + "\r\n";
}

void	HttpResponse::setDateHeader() {
	char				date[128];
	time_t				rawtime;
	struct tm			*info;
	size_t				length;

	std::time(&rawtime);
	info = std::gmtime(&rawtime);
	length = strftime(date, 128, "%a, %d %h %Y %T %Z", info);
	date[length] = 0;
	this->_header += "Date: " + std::string(date) + "\r\n";
}

void	HttpResponse::setContentTypeHeader() {
	this->_header += "Content-type: " + this->_mime + "\r\n";
}

void	HttpResponse::setKeepAliveConnectionHeader() {
	this->_header += "Transfer-Encoding: chunked\r\n";
	this->_header += "Connection: keep-alive\r\n";
}

void	HttpResponse::setContentLengthHeader() {
	std::stringstream	str;

	if (this->_statusCode == 200)
		str << this->_contentLength;
	else
		str << StatusCode::page(this->_statusCode).length();
	this->_header += "Content-Length: " + str.str() + "\r\n";
}

void	HttpResponse::createHeader() {

	this->setStatusLine();
	this->setServerHeader();
	this->setDateHeader();
	this->setContentTypeHeader();
	if (this->getRequest()->keepAlive())
		this->setKeepAliveConnectionHeader();
	else
		this->setContentLengthHeader();
	this->_header += "\r\n";
}

void	HttpResponse::sendHeader() {
	int			bytes;
	std::string	header;

	this->createHeader();
	bytes = send(this->_client->getFd(), this->_header.c_str(), this->_header.length(), 0);
	if (bytes < 0) {
		perror("send");
		this->_client->setError();
		return ;
	}
}

int	HttpResponse::sendData(const void *data, int len) {
	const char			*ptr = static_cast<const char *>(data);
	std::stringstream	str;
	int					bytes;

	if (this->getRequest()->keepAlive()) {
		str << std::hex << std::uppercase << len << "\r\n";
		bytes = send(this->_client->getFd(), str.str().c_str(), str.str().length(), 0);
		if (bytes <= 0) {
			perror("send");
			this->_client->setError();
			return (ret(ERR_SEND));		
		}
	}
	while (len > 0) {
		bytes = send(this->_client->getFd(), ptr, len, 0);
		if (bytes <= 0) {
			perror("send");
			this->_client->setError();
			return (ret(ERR_SEND));		
		}
		ptr += bytes;
		len -= bytes;
	}
	if (this->getRequest()->keepAlive()) {
		bytes = send(this->_client->getFd(), "\r\n", 2, 0);
		if (bytes <= 0) {
			perror("send");
			this->_client->setError();
			return (ret(ERR_SEND));	
		}
	}
	return (0);
}

void	HttpResponse::sendContent(std::ifstream &file) {
	char		data[1024];
	std::size_t	sended = 0;
	int			bytes;

	if (this->_contentLength > 0 && !this->_client->error()) {
		while (sended != this->_contentLength && !this->_client->error()) {
			if (!file.read(data, std::min(this->_contentLength - sended, static_cast<std::size_t>(1024)))) {
				perror("read");
				this->_client->setError();
				return ;
			}
			bytes = file.gcount();
			this->sendData(data, bytes);
			sended += bytes;
		}
		bytes = send(this->_client->getFd(), "0\r\n\r\n", 5, 0);
		if (bytes <= 0) {
			perror("send");
			this->_client->setError();
			return ;
		}
	}	
}

void	HttpResponse::sendErrorPage(int num) {
	int					bytes;
	std::stringstream	str;
	std::string			page = StatusCode::page(num);

	if (this->getRequest()->keepAlive() && !this->_client->error()) {
		str << std::hex << std::uppercase << page.length() << "\r\n";
		bytes = send(this->_client->getFd(), str.str().c_str(), str.str().length(), 0);
		if (bytes <= 0) {
			perror("send");
			this->_client->setError();
			return ;
		}
	}
	bytes = send(this->_client->getFd(), page.c_str(), page.length(), 0);
	if (bytes <= 0) {
		perror("send");
		this->_client->setError();
		return ;
	}
	if (this->getRequest()->keepAlive()) {
		bytes = send(this->_client->getFd(), "\r\n0\r\n\r\n", 7, 0);
		if (bytes <= 0) {
			perror("send");
			this->_client->setError();
			return ;
		}
	}
}

void	HttpResponse::error(int num) {
	this->_statusCode = num;
	this->_mime = Mime::ext("html");
	this->sendHeader();
	this->sendErrorPage(num);
}

void	HttpResponse::sendDirectoryPage(std::string path) {
	int					bytes;
	std::stringstream	str;
	std::string			page = DirectoryListing::html(path);

	if (this->getRequest()->keepAlive() && !this->_client->error()) {
		str << std::hex << std::uppercase << page.length() << "\r\n";
		bytes = send(this->_client->getFd(), str.str().c_str(), str.str().length(), 0);
		if (bytes <= 0) {
			perror("send");
			this->_client->setError();
			return ;
		}
	}
	bytes = send(this->_client->getFd(), page.c_str(), page.length(), 0);
	if (bytes <= 0) {
		perror("send");
		this->_client->setError();
		return ;
	}
	if (this->getRequest()->keepAlive()) {
		bytes = send(this->_client->getFd(), "\r\n0\r\n\r\n", 7, 0);
		if (bytes <= 0) {
			perror("send");
			this->_client->setError();
			return ;
		}
	}
}

void	HttpResponse::directoryListing(std::string path) {
	this->_statusCode = 200;
	this->_mime = Mime::ext("html");
	this->sendHeader();
	this->sendDirectoryPage(path);	
}

void	HttpResponse::sendResponse() {
	std::ifstream		file;
	std::string			ext;

	if (!this->getRequest()->isGood()) {
		this->error(400);
		return ;
	}
	//temp : need to add infos form config file
	if (this->getRequest()->getUri() == "/")
		ext = this->getRequest()->getUri() + "index.html";
	else
		ext = this->getRequest()->getUri();
	// end temp
	ext = ext.substr(ext.find_last_of(".") + 1);
	if (ext.find("/") == std::string::npos)
		this->_mime = Mime::ext(ext);
	//temp : need to add infos form config file
	if (this->getRequest()->getUri() == "/") {
		//file.open(("." + this->getRequest()->getUri() + "index.html").c_str(), std::ios::binary);
		// TEMP for testing Directory Listing
		this->directoryListing("./");
	}
	else
		file.open(("." + this->getRequest()->getUri()).c_str(), std::ios::binary);
	// end temp
	if (!file.is_open()) {
		this->error(404);
		return ;
	}
	file.seekg(0, std::ios::end);
	this->_contentLength = file.tellg();
	file.seekg(0, std::ios::beg);
	if (file.fail()) {
		this->_statusCode = 500;
		return ;
	}
	this->_statusCode = 200;

	this->sendHeader();
	this->sendContent(file);
}

Server	*HttpResponse::getServer() const {
	return (this->_client->getServer());
}

HttpRequest	*HttpResponse::getRequest() const {
	return (this->_client->getRequest());
}

int	HttpResponse::getStatusCode() const {
	return (this->_statusCode);
}

std::string	HttpResponse::getMime() const {
	return (this->_mime);
}

std::string	HttpResponse::getHeader() const {
	return (this->_header);
}

std::ostream &operator<<(std::ostream &o, const HttpResponse &response) {
	if (response.getServer()->getLogLevel() == 0)
		return (o);
	o << GREEN BOLD << "Response" END_STYLE " → " CYAN << response.getStatusCode();
	o << YELLOW " " << response.getMime() << END_STYLE << std::endl;
	if (response.getServer()->getLogLevel() == 2){
		o << THIN ITALIC << response.getHeader() << END_STYLE << std::endl;
	}
	return (o);
}
