/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:29:38 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/14 18:16:44 by gbrunet          ###   ########.fr       */
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
	if (this->getServer()->getName() != "")
		this->_header += "Server: " + this->getServer()->getName() + "\r\n";
}

void	HttpResponse::setDateHeader() {
	char				date[128];
	time_t				rawtime;
	struct tm			*info;
	size_t				length;

	time(&rawtime);
	info = gmtime(&rawtime);
	length = strftime(date, 128, "%a, %d %h %Y %T %Z", info);
	date[length] = 0;
	this->_header += "Date: " + string(date) + "\r\n";
}

void	HttpResponse::setContentTypeHeader() {
	this->_header += "Content-type: " + this->_mime + "\r\n";
}

void	HttpResponse::setKeepAliveConnectionHeader() {
	this->_header += "Transfer-Encoding: chunked\r\n";
	this->_header += "Connection: keep-alive\r\n";
}

void	HttpResponse::setContentLengthHeader() {
	stringstream	str;

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
	if (this->keepAlive())
		this->setKeepAliveConnectionHeader();
	else
		this->setContentLengthHeader();
	this->_header += "\r\n";
}

void	HttpResponse::sendHeader() {
	int	bytes;

	bytes = send(this->getClientFd(), this->_header.c_str(), this->_header.length(), 0);
	this->checkSend(bytes);
}

void	HttpResponse::sendChunkSize(int len) {
	stringstream	str;
	int				bytes;
	
	str << hex << uppercase << len << "\r\n";
	bytes = send(this->getClientFd(), str.str().c_str(), str.str().length(), 0);
	this->checkSend(bytes);
}

void	HttpResponse::sendChunkEnd() {
	int	bytes;

	bytes = send(this->getClientFd(), "\r\n", 2, 0);
	this->checkSend(bytes);
}

void	HttpResponse::sendFinalChunk() {
	int	bytes;

	bytes = send(this->getClientFd(), "0\r\n\r\n", 5, 0);
	this->checkSend(bytes);
}

int	HttpResponse::sendData(const void *data, int len) {
	const char	*ptr = static_cast<const char *>(data);
	int			bytes;

	if (this->keepAlive() && !this->getClientError())
		this->sendChunkSize(len);
	while (len > 0 && !this->getClientError()) {
		bytes = send(this->getClientFd(), ptr, len, 0);
		this->checkSend(bytes);
		ptr += bytes;
		len -= bytes;
	}
	if (this->keepAlive() && !this->getClientError())
		this->sendChunkEnd();
	return (0);
}

void	HttpResponse::sendContent(ifstream &file) {
	char	data[1024];
	size_t	sended = 0;
	int		bytes;
	
	if (this->_contentLength > 0 && !this->getClientError()) {
		while (sended != this->_contentLength && !this->getClientError()) {
			if (!file.read(data, min(this->_contentLength - sended,
							static_cast<size_t>(1024)))) {
				perror("read");
				this->setClientError();
				ret(ERR_READ);
				return ;
			}
			bytes = file.gcount();
			this->sendData(data, bytes);
			sended += bytes;
		}
		if (this->keepAlive())
			this->sendFinalChunk();
	}	
}

void	HttpResponse::checkSend(int bytes) {
	if (bytes <= 0) {
		perror("send");
		this->setClientError();
		ret(ERR_SEND);
	}
}

void	HttpResponse::sendErrorPage(int num) {
	int		bytes;
	string	page = StatusCode::page(num);

	if (this->keepAlive() && !this->getClientError())
		this->sendChunkSize(page.length());
	bytes = send(this->getClientFd(), page.c_str(), page.length(), 0);
	this->checkSend(bytes);
	if (this->keepAlive() && !this->getClientError()) {
		this->sendChunkEnd();
		this->sendFinalChunk();
	}
}

void	HttpResponse::movedPermanently(string uri) {
	this->_statusCode = 301;
	this->_mime = Mime::ext("html");
	this->_header = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + uri + "\r\n\r\n";
	this->sendHeader();
	this->setClientError();
}

void	HttpResponse::error(int num) {
	this->_statusCode = num;
	this->_mime = Mime::ext("html");
	this->createHeader();
	this->sendHeader();
	map<int, string> errorPages = this->getServer()->getErrorPages();
	if (errorPages[num] != "") {
		string uri = this->getServer()->getRoot() + errorPages[num];
		ifstream file;
		file.open(uri.c_str(), ios::binary);
		if (file.is_open()) {
			file.seekg(0, ios::end);
			this->_contentLength = file.tellg();
			file.seekg(0, ios::beg);
			if (file.fail()) {
				this->error(500);
				return ;
			}
			this->sendContent(file);
		} else
			this->sendErrorPage(num);
	} else
		this->sendErrorPage(num);
}

void	HttpResponse::sendDirectoryPage(string path) {
	int		bytes;
	string	page = DirectoryListing::html(path, this->getServer()->getRoot());

	if (this->keepAlive() && !this->getClientError())
		this->sendChunkSize(page.length());
	bytes = send(this->getClientFd(), page.c_str(), page.length(), 0);
	this->checkSend(bytes);
	if (this->keepAlive() && !this->getClientError()) {
		this->sendChunkEnd();
		this->sendFinalChunk();
	}
}

void	HttpResponse::directoryListing(string path) {
	this->_statusCode = 200;
	this->_mime = Mime::ext("html");
	this->createHeader();
	this->sendHeader();
	this->sendDirectoryPage(path);	
}

vector<string>	HttpResponse::getIndexes() const {
	return (this->getServer()->getIndexes());
}

bool	HttpResponse::expandUri(string &uri, bool &isDir) {
	struct stat		s;
	vector<string>	indexes;
	string			index;

	vector<Location> locations = this->getServer()->getLocation();
	for (vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		if (this->getRequest()->getUri().compare(0, it->getLocPath().length(), it->getLocPath()) == 0) {
			uri = this->getRequest()->getUri();
			uri.erase(0, it->getLocPath().length());
			uri = it->getRoot() + uri;
			index = it->getIndex();
			vector<string> indexes = split_trim(index, ",");
			for (strVecIt it = indexes.begin(); it != indexes.end(); it++) {
				if (*it != "")
					indexes.push_back(*it);
			}
			if (stat(uri.c_str(), &s) == 0) {
				if (s.st_mode & S_IFDIR) {
					isDir = true;
					if (uri[uri.length() - 1] != '/') {
						this->movedPermanently(this->getRequest()->getUri() + "/");
						return (false);
					}
				}
				else
					isDir = false;
			} else {
				this->error(404);
				return (false);
			}
			if (isDir) {
				for (vector<string>::iterator index_it = indexes.begin(); index_it != indexes.end(); index_it++) {
					if (*index_it != "" && access((uri + *index_it).c_str(), F_OK) != -1) {
						uri += *index_it;
						isDir = false;
						break;
					}
				}
			}
			return (true);
		}
	}
	uri = this->getServer()->getRoot() + this->getRequest()->getUri();
	indexes = this->getIndexes();
	if (stat(uri.c_str(), &s) == 0) {
		if (s.st_mode & S_IFDIR) {
			isDir = true;
			if (uri[uri.length() - 1] != '/') {
				this->movedPermanently(this->getRequest()->getUri() + "/");
				return (false);
			}
		}
		else
			isDir = false;
	} else {
		this->error(404);
		return (false);
	}
	if (isDir) {
		for (vector<string>::iterator it = indexes.begin(); it != indexes.end(); it++) {
			if (access((uri + *it).c_str(), F_OK) != -1) {
				uri += *it;
				isDir = false;
				break;
			}
		}
	}
	return (true);
}

void	HttpResponse::tryDeleteFile() {
	string	uri;

	if (this->getRequest()->getUri()[0] == '/')
		uri = this->getServer()->getRoot() + this->getRequest()->getUri();
	else
		uri = this->getServer()->getRoot() + "/" + this->getRequest()->getUri();
	if (!childPath(getFullPath(this->getServer()->getRoot()), getFullPath(uri)))
		this->error(403);
	else {
		if (remove(uri.c_str()) == 0)
			this->error(200);
		else
			this->error(204);
	}
}

void	HttpResponse::sendResponse() {
	ifstream		file;
	string			ext;
	string			uri;
	bool			isDir;

	if (this->getRequest()->tooLarge()) {
		this->error(413);
		return ;
	}
	if (!this->getRequest()->isGood()) {
		this->error(400);
		return ;
	}
	// not sure if this is at the right place...
	if (this->getRequest()->getMethod() == DELETE) {
		tryDeleteFile();
		return ;
	}
	if (!this->expandUri(uri, isDir))
		return ;
	if (!this->getServer()->methodeAllowed(this->getRequest()->getMethod())) {
		this->error(405);
		return ;
	}
	ext = ext.substr(ext.find_last_of(".") + 1);
	if (ext.find("/") == string::npos)
		this->_mime = Mime::ext(ext);
	cout << isDir << endl;
	if (isDir) {
		if (this->getServer()->getDirectoryListing())
			this->directoryListing(uri);
		else
			this->error(404);
		return ;
	} else {
		file.open(uri.c_str(), ios::binary);
	}
	if (!file.is_open()) {
		this->error(404);
		return ;
	}
	file.seekg(0, ios::end);
	this->_contentLength = file.tellg();
	file.seekg(0, ios::beg);
	if (file.fail()) {
		this->error(500);
		return ;
	}
	this->_statusCode = 200;
	this->createHeader();
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

string	HttpResponse::getMime() const {
	return (this->_mime);
}

string	HttpResponse::getHeader() const {
	return (this->_header);
}

int	HttpResponse::getClientFd() const {
	return (this->_client->getFd());
}

bool	HttpResponse::getClientError() const {
	return (this->_client->error());
}

void	HttpResponse::setClientError() {
	this->_client->setError();
}

bool	HttpResponse::keepAlive() const {
	return (this->getRequest()->keepAlive());
}

ostream &operator<<(ostream &o, const HttpResponse &response) {
	if (response.getServer()->getLogLevel() == 0)
		return (o);
	o << timeStamp() << GREEN BOLD << " Response" END_STYLE " → " CYAN << response.getStatusCode();
	o << YELLOW " " << response.getMime() << END_STYLE << endl;
	if (response.getServer()->getLogLevel() == 2){
		o << THIN ITALIC << response.getHeader() << END_STYLE << endl;
	}
	return (o);
}
