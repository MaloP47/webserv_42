/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:01:52 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/15 17:43:13 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "webserv.h"

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(Client *client):
	_client(client), _headerLength(0), _requestLength(0), _goodRequest(true), _tooLarge(false), _method(OTHER), _contentLength(0){
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
	this->_tooLarge = rhs._tooLarge;
	this->_uri = rhs._uri;
	this->_acceptedMimes = rhs._acceptedMimes;
	this->_rawBytes = rhs._rawBytes;
	this->_contentLength = rhs._contentLength;
	return (*this);
}

bool	HttpRequest::isFullRequest() {
	size_t	pos;

	if (findLower(this->_rawRequest, "content-length")) {
		pos = this->_rawRequest.find("\r\n\r\n");
		if (pos != string::npos && pos + 4 < this->_rawRequest.length()) {
			this->parseContentLength();
			this->_headerLength = this->_rawRequest.find("\r\n\r\n") + 4;
			if (this->_requestLength - this->_headerLength == this->_contentLength) {
				this->_rawBytes.erase(this->_rawBytes.begin(), this->_rawBytes.begin() + this->_headerLength);
				return (true);
			}
		}
		return (false);
	} else
		return (this->_rawRequest.find("\r\n\r\n",
				this->_rawRequest.length() - 4) != string::npos);	
}

bool	HttpRequest::appendRequest(const char *data, int bytes) {
	for (int i = 0; i < bytes; i++) {
		this->_rawBytes.push_back(*data);
		this->_rawRequest += *data;
		data++;
	}
	this->_requestLength += bytes;
	if (this->_headerLength != 0) {
		if (static_cast<size_t>(this->getServer()->getMaxBodySize() * 1024) < this->_requestLength - this->_headerLength) {
			this->_tooLarge = true;
			vector<string> line = split_trim(this->_rawRequest.substr(0, this->_headerLength), "\r\n");
			if (line.size() < 1)
				return (true);
			this->_keepAliveConnection = false;
			this->parseRequestLine(line[0]);	
			return (true);
		}
	}
	if (isFullRequest()) {
		this->parse();
		return (true);
	}
	return (false);
}

string	HttpRequest::getRawRequest() const {
	return (this->_rawRequest);
}

void	HttpRequest::parse() {
	vector<string>	line;

	this->_goodRequest = true;
	if (this->_headerLength != 0)
		line = split_trim(this->_rawRequest.substr(0, this->_headerLength), "\r\n");
	else
		line = split_trim(this->_rawRequest, "\r\n");
	if (line.size() < 1)
		return ;
	this->parseRequestLine(line[0]);	
	for (strVecIt it = line.begin() + 1; it != line.end(); it++) {
		if (findLower(*it, "accept:"))
			this->parseAcceptedMimes(*it);
		else if (findLower(*it, "connection:"))
			this->parseConnection(*it);
		else if (findLower(*it, "content-type:"))
			this->parseContentType(*it);
	}
	if (this->_contentType == "multipart/form-data")
		this->decodeFormData();
	else if (this->_contentType == "application/x-www-form-urlencoded")
		this->decodeUrlEncoded();
	else if (this->_contentType == "text/plain") {
		this->_textPost = this->_rawRequest;
		this->_textPost.erase(0, this->_headerLength);
	}
}

void	HttpRequest::decodeUrlEncoded() {
	string			content;
	vector<string>	vars;
	vector<string>	tempEnv;

	content = this->_rawRequest;
	content.erase(0, this->_headerLength);
	vars = split_trim(content, "&");
	for (strVecIt it = vars.begin(); it != vars.end(); it++) {
		tempEnv = split_trim(*it, "=");
		if (tempEnv.size() != 2) {
			this->_goodRequest = false;
			return ;
		}
		this->_client->addEnv(decodeEnv(tempEnv[0]), decodeEnv(tempEnv[1]));
	}
}

void	HttpRequest::decodeFormData() {
	size_t			pos;
	string			header;
	string			filename;
	vector<char>	file;

	if ((pos = findInCharVec("\r\n\r\n", this->_rawBytes)) != string::npos) {
		header = strFromCharVec(pos + 4, this->_rawBytes);
		vector<string> split;
		split = split_trim(header, "filename=\"");
		if (split.size() == 2) {
			split = split_trim(split[1], "\"\r\n");
			if (split.size() == 2)
				filename = this->getServer()->getRoot() + this->getServer()->getUploadPath() + split[0];
		}
		this->_rawBytes.erase(this->_rawBytes.begin(), this->_rawBytes.begin() + pos + 4);
		if ((pos = findInCharVec("\r\n--" + this->_boundary, this->_rawBytes)) != string::npos) {
			file.insert(file.begin(), this->_rawBytes.begin(), this->_rawBytes.begin() + pos);
			this->_uploadedFiles.push_back(Upload(filename, file));
			this->_rawBytes.erase(this->_rawBytes.begin(), this->_rawBytes.begin() + pos);

		}
	}
	if (findInCharVec("--" + this->_boundary + "\r\n", this->_rawBytes) != string::npos)
		this->decodeFormData();
	else {
		if (findInCharVec("--" + this->_boundary + "--", this->_rawBytes) == string::npos)
			this->_goodRequest = false;
		else if (this->getServer()->methodeAllowed(this->_method))
			for (uploadIt it = this->_uploadedFiles.begin(); it != this->_uploadedFiles.end(); it++)
				it->createFile();
	}
}

void	HttpRequest::parseContentType(string line) {
	vector<string>	split;

	line.erase(0, 13);
	ltrim(line);
	rtrim(line);
	if (findLower(line, "application/x-www-form-urlencoded"))
		this->_contentType = "application/x-www-form-urlencoded";
	else if (findLower(line, "text/plain"))
		this->_contentType = "text/plain";
	else if (findLower(line, "multipart/form-data")) {
		this->_contentType = "multipart/form-data";
		split = split_trim(line, ";");
		if (split.size() == 2) {
			if (findLower(split[1], "boundary="))
				this->_boundary = split[1].erase(0, 9);
		} else
			this->_goodRequest = false;
	}
}

void	HttpRequest::parseContentLength() {
	vector<string>	line;

	line = split_trim(this->_rawRequest, "\r\n");
	for (strVecIt it = line.begin() + 1; it != line.end(); it++) {
		if (findLower(*it, static_cast<string>("content-length:"))) {
			(*it).erase(0, 15);
			this->_contentLength = atoi((*it).c_str());
		}	
	}
}

void	HttpRequest::parseConnection(string line) {
	line.erase(0, 11);
	ltrim(line);
	rtrim(line);
	this->_keepAliveConnection = findLower(line, static_cast<string>("keep-alive"));
}

void	HttpRequest::parseAcceptedMimes(string line) {
	line.erase(0, 7);
	this->_acceptedMimes = split_trim(line, ",");
}

void	HttpRequest::parseRequestLine(string line) {
	vector<string>	split;

	split = split_trim(line, " ");
	if (split.size() != 3) {
		this->_goodRequest = false;	
		return ;
	}
	this->setMethod(split[0]);
	this->getUriAndEnv(split[1]);
	split = split_trim(split[2], "/");
	if (split.size() != 2) {
		this->_goodRequest = false;	
		return ;
	}
	if (atof(split[1].c_str()) < 1.1 || split[0] != "HTTP") {
		this->_goodRequest = false;	
		return ;
	}
}

void	HttpRequest::getUriAndEnv(string str) {
	vector<string>	split;
	vector<string>	vars;
	vector<string>	tempEnv;

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

void	HttpRequest::setMethod(string str) {
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

bool	HttpRequest::tooLarge() const {
	return (this->_tooLarge);
}

enum HttpMethod	HttpRequest::getMethod() const {
	return (this->_method);
}

string	HttpRequest::getUri() const {
	return (this->_uri);
}

bool	HttpRequest::keepAlive() const {
	return (this->_keepAliveConnection);
}

static string	stringMethod(enum HttpMethod method) {
	if (method == GET)
		return ("GET");
	else if (method == POST)
		return ("POST");
	else if (method == DELETE)
		return ("DELETE");
	return ("OTHER");
}

ostream &operator<<(ostream &o, const HttpRequest &request) {
	if (request.getServer()->getLogLevel() == 0)
		return (o);
	o << timeStamp() << PURPLE BOLD << " Request " END_STYLE;
	o << " → " CYAN << stringMethod(request.getMethod()) << " " YELLOW;
	o << request.getUri() << END_STYLE << endl;
	if (request.getServer()->getLogLevel() == 2){
		o << THIN ITALIC << request.getRawRequest() << END_STYLE << endl;
	}
	return (o);
}
