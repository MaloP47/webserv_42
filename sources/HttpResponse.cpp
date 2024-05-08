/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maburnet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:29:38 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/07 16:03:24 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "style.h"
#include "webserv.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include "HttpResponse.hpp"

HttpResponse::HttpResponse(): _client(NULL) {}

HttpResponse::HttpResponse(Client *client):
	_client(client), _statusCode(0), _contentLength(0), _cgiIndex(-1) {
}

HttpResponse::HttpResponse(const HttpResponse &cpy) {
	*this = cpy;
}

HttpResponse::~HttpResponse() {}

HttpResponse &HttpResponse::operator=(const HttpResponse &rhs) {
	this->_client = rhs._client;
	this->_statusCode = rhs._statusCode;
	this->_contentLength = rhs._contentLength;
	this->_statusLine = rhs._statusLine;
	this->_mime = rhs._mime;
	this->_header = rhs._header;
	this->_cgiIndex = rhs._cgiIndex;
	this->_pathInfo = rhs._pathInfo;
	this->_indexes = rhs._indexes;
	this->_locPath = rhs._locPath;
	this->_root = rhs._root;
	this->_maxBodySize = rhs._maxBodySize;
	this->_allowedMethod = rhs._allowedMethod;
	this->_directoryListing = rhs._directoryListing;
	this->_errorPage = rhs._errorPage;
	this->_returnURI = rhs._returnURI;
	this->_uploadPath = rhs._uploadPath;
	this->_cgiBin = rhs._cgiBin;
	this->_cgiExt = rhs._cgiExt;
	this->_isLocation = rhs._isLocation;
	this->_cgiTmpFile = rhs._cgiTmpFile;
	this->_contentType = rhs._contentType;
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
	if (this->_cgiTmpFile != "" && this->_contentType != "") {
		this->_header += this->_contentType + "\r\n";
	} else
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
	char	data[2048];
	size_t	sended = 0;
	int		bytes;

	if (this->_cgiTmpFile != "" && this->_contentType != "") {
		file.read(data, this->_contentType.size() + 4);
	}
	if (this->_contentLength > 0 && !this->getClientError()) {
		while (sended != this->_contentLength && !this->getClientError()) {
			if (!file.read(data, min(this->_contentLength - sended,
							static_cast<size_t>(2048)))) {
				cerr << timeStamp() << CYAN << " Read " << THIN ITALIC;
				perror("");
				cerr << END_STYLE;
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
		cerr << timeStamp() << CYAN << " Send " << THIN ITALIC;
		perror("");
		cerr << END_STYLE;
		this->setClientError();
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
	string	page = DirectoryListing::html(path, this->getServer()->getRoot(), this->getRequest()->getUri());

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

	isDir = false;
	if (this->_isLocation) {
		uri = this->getRequest()->getUri();
		uri.erase(0, this->_locPath.length());
		uri = this->_root + uri;
	} else
		uri = this->getServer()->getRoot() + this->getRequest()->getUri();
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
	}
	if (isDir) {
		for (vector<string>::iterator it = this->_indexes.begin(); it != this->_indexes.end(); it++) {
			if (access((uri + *it).c_str(), F_OK) != -1) {
				if (access((uri + *it).c_str(), R_OK) == -1) {
					this->error(403);
					return (false);
				}
				uri += *it;
				isDir = false;
				break;
			}
		}
	}
	return (true);
}

void	HttpResponse::tryDeleteFile(string uri) {
	string	root;

	if (this->_isLocation) {
		root = this->_root;
	} else
		root = this->getServer()->getRoot();
	if (!childPath(getFullPath(root), getFullPath(uri)))
		this->error(403);
	else {
		if (remove(uri.c_str()) == 0)
			this->error(200);
		else
			this->error(204);
	}
}

void	HttpResponse::setInfos() {
	vector<string>	index;
	vector<Location> locations = this->getServer()->getLocation();
	for (vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		if (this->getRequest()->getUri().compare(0, it->getLocPath().length(), it->getLocPath()) == 0) {
			this->_locPath = it->getLocPath();
			this->_root = it->getRoot();
			this->_maxBodySize = it->getMaxBodySize();
			this->_allowedMethod = it->getAllowedMethod();
			this->_directoryListing = it->getDirectoryListing();
			this->_errorPage = it->getErrorPages();
			this->_returnURI = it->getReturnURI();
			this->_uploadPath = it->getUploadPath();
			this->_cgiBin = it->getCGIBin();
			this->_cgiExt = it->getCGIExtension();
			this->_isLocation = true;
			index = split_trim(it->getIndex(), ",");
			if (index.size() == 1) {
				this->_indexes.push_back(it->getIndex());
			} else {
				for (strVecIt iti = this->_indexes.begin(); iti != this->_indexes.end(); iti++) {
					if (*iti != "")
						this->_indexes.push_back(*iti);
				}
			}
			return ;
		}
	}
	this->_root = this->getServer()->getRoot();
	this->_maxBodySize = this->getServer()->getMaxBodySize();
	this->_allowedMethod = this->getServer()->getAllowedMethod();
	this->_directoryListing = this->getServer()->getDirectoryListing();
	this->_errorPage = this->getServer()->getErrorPages();
	this->_returnURI = this->getServer()->getReturnURI();
	this->_uploadPath = this->getServer()->getUploadPath();
	this->_cgiBin = this->getServer()->getBinPath();
	this->_cgiExt = this->getServer()->getCgiExtension();
	this->_isLocation = false;
	this->_indexes = this->getServer()->getIndexes();
}

bool	HttpResponse::methodeAllowed(enum HttpMethod methode) {
	for (methodeIt it = this->_allowedMethod.begin(); it != this->_allowedMethod.end(); it++)
		if (*it == methode)
			return (true);
	return (false);
}

void	HttpResponse::sendResponse() {
	ifstream		file;
	string			uri;
	string			ext;
	bool			isDir;
	int				i = -1;

	if (this->getRequest()->tooLarge()) {
		this->error(413);
		return ;
	}
	this->setInfos();
	if (this->_returnURI.size() > 0) {
		int		err = this->_returnURI.begin()->first;
		string	uri = this->_returnURI.begin()->second;
		if (err == 301)
			this->movedPermanently(uri);
		else {
			this->error(this->_returnURI.begin()->first);
			return ;
		}
	}
	if (!this->getRequest()->isGood()) {
		this->error(400);
		return ;
	}
	if (!this->expandUri(uri, isDir))
		return ;
	if (!this->methodeAllowed(this->getRequest()->getMethod())) {
		this->error(405);
		return ;
	}
	if (this->getRequest()->getMethod() == DELETE) {
		tryDeleteFile(uri);
		return ;
	}
	if (isDir) {
		if (this->_directoryListing)
			this->directoryListing(uri);
		else
			this->error(404);
		return ;
	} else {
		if (access(uri.c_str(), F_OK) != -1) {
			if (access(uri.c_str(), R_OK) == -1)
				this->error(403);
		}
		ext = uri.substr(uri.find_last_of(".") + 1);
		if (ext.find("/") == string::npos)
			this->_mime = Mime::ext(ext);
		else {
			string tmp = ext;
			ext = ext.substr(0, ext.find_first_of("/"));
			this->_mime = Mime::ext(ext);
			this->_pathInfo = tmp.substr(tmp.find_first_of("/") + 1);
		}
		for (vector<string>::iterator it = this->_cgiExt.begin(); it != this->_cgiExt.end(); it++) {
			i++;
			if(("." + ext) == *it) {
				this->_cgiIndex = i;
				break;
			}
		}
		if (this->_cgiIndex >= 0) {
			if (access(uri.c_str(), F_OK == -1)) {
				this->error(404);
				return ;
			}
			if (access(uri.c_str(), R_OK == -1)) {
				this->error(403);
				return ;
			}
			if (this->executeCGI(uri)) {
				uri = this->_cgiTmpFile;
			} else {
				this->error(500);
				return ;
			}
		}
		file.open(uri.c_str(), ios::binary);
	}
	if (!file.is_open()) {
		this->error(404);
		return ;
	}
	file.seekg(0, ios::end);
	this->_contentLength = file.tellg();
	if (this->_cgiTmpFile != "" && this->_contentType != "")
		this->_contentLength -= (this->_contentType.size() + 4);
	file.seekg(0, ios::beg);
	if (file.fail()) {
		this->error(500);
		return ;
	}
	this->_statusCode = 200;
	this->createHeader();
	this->sendHeader();
	this->sendContent(file);
	if (this->_cgiTmpFile != "")
		remove(this->_cgiTmpFile.c_str());
}

void	HttpResponse::errorCGI(string str, int file_fd)
{
	cerr << timeStamp() << CYAN << str << THIN ITALIC;
	perror("");
	cerr << END_STYLE;
	this->setClientError();
	ret(ERR_READ);
    close(file_fd);
	return ;
}

char	**HttpResponse::createEnv(string uri) {
	mapStrStr		env;
	char			**cenv;
	int				i;
	stringstream	port;
	stringstream	host;

	this->_client->addEnv("REDIRECT_STATUS", "200");
	this->_client->addEnv("SERVER_SOFTWARE", "Webserv/1.0");
	this->_client->addEnv("SERVER_NAME", this->getServer()->getName());
	this->_client->addEnv("GATEWAY_INTERFACE", "CGI/1.1");
	this->_client->addEnv("SERVER_PROTOCOL", "HTTP/1.1");
	port << this->getServer()->getPort();
	this->_client->addEnv("SERVER_PORT", port.str());
	this->_client->addEnv("REQUEST_METHOD", stringMethod(this->getRequest()->getMethod()));
	this->_client->addEnv("PATH_INFO", this->_pathInfo);
	this->_client->addEnv("PATH_TRANSLATED", uri);
	string scriptName = this->getRequest()->getUri();
	scriptName = scriptName.substr(0, scriptName.find_last_of(".") + this->_cgiExt[this->_cgiIndex].length());
	this->_client->addEnv("SCRIPT_NAME", scriptName);
	this->_client->addEnv("QUERY_STRING", this->getRequest()->getQuery());
	this->_client->addEnv("REMOTE_HOST", this->getServer()->getName());
	host << this->getServer()->getHost();
	this->_client->addEnv("REMOTE_ADDR", host.str());
	this->_client->addEnv("CONTENT_TYPE", this->_client->getRequest()->getContentType());
	this->_client->addEnv("CONTENT_LENGTH", this->_client->getRequest()->getContentLength());
	this->_client->addEnv("HTTP_ACCEPT", this->_client->getRequest()->getAcceptedMime());
	this->_client->addEnv("HTTP_USER_AGENT", this->_client->getRequest()->getUserAgent());
	this->_client->addEnv("HTTP_COOKIE", this->_client->getRequest()->getCookie());
	env = this->_client->getEnv();
	cenv = new char*[env.size() + 1];
	i = 0;
	for (mapStrStr::iterator it = env.begin(); it != env.end(); it++) {
		string	var = it->first + "=" + it->second;
		cenv[i] = new char[var.size() + 1];
		strcpy(cenv[i], var.c_str());
		i++;
	}
	cenv[i] = NULL;
	return (cenv);
}

bool	HttpResponse::executeCGI(string uri)
{
	int			file_fd;
	int			pid;
	ifstream	file;
	string		fileName = "/tmp/" + rdmString(64);
	int			pipe_fd[2];
	string		content;

	file_fd = open(fileName.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
	this->_cgiTmpFile = fileName;
	if (file_fd == -1) {
		errorCGI(" Open ", file_fd);
		return (false);
	}
	if (pipe(pipe_fd) < 0) {
		errorCGI(" Pipe ", file_fd);
		return (false);
	}	
	pid = fork();
	if (pid == -1) {
		errorCGI(" Fork ", file_fd);
		return (false);
	} else if (pid == 0) {
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[1]);
		char **env = createEnv(uri);
		string script = uri.substr(0, uri.find_last_of(".") + this->_cgiExt[this->_cgiIndex].length());
		char **av;
		av = new char*[3];
		av[0] = new char[this->_cgiBin[this->_cgiIndex].size() + 1];
		strcpy(av[0], this->_cgiBin[this->_cgiIndex].c_str());
		av[1] = new char[script.size() + 1];
		strcpy(av[1], script.c_str());
		av[2] = NULL;
		if (dup2(file_fd, STDOUT_FILENO) == -1)
		{
			errorCGI(" Dup2 ", file_fd);
			return (false);
		}
		if (execve(this->_cgiBin[this->_cgiIndex].c_str(), av, env))
		{
			errorCGI(" Execve ", file_fd);
			exit(-1);
		}
		exit (0);
	} else {
		close(pipe_fd[0]);
		content = this->_client->getRequest()->getContent();
		if (write(pipe_fd[1], content.c_str(), content.size()) < 0) {
			errorCGI(" Write ", file_fd);
			close(pipe_fd[1]);
			return (false);
		}
		close(pipe_fd[1]);
		close(file_fd);
		waitpid(0, NULL, 0);
		this->parseCGI();
	}
	return (true);
}

void	HttpResponse::parseCGI() {
	ifstream	file;
	int			size;
	int			read = 0;
	char		data[2049];
	string		content;
	
	file.open(this->_cgiTmpFile.c_str(), ios::binary);
	if (!file.is_open()) {
		this->error(500);
		return ;
	}
	file.seekg(0, ios::end);
	size = file.tellg();
	file.seekg(0, ios::beg);
	if (file.fail()) {
		this->error(500);
		return ;
	}
	if (size > 0 && !this->getClientError()) {
		file.read(data, min(size - read, 2048));
		data[file.gcount()] = 0;
		content = string(data);
		int i = -1;
		while (content[++i] != 0)
		if (findLower(content, "content-type:")) {
			content = content.substr(0, content.find("\r\n\r\n"));
			this->_contentType = content;
		}
	}
	file.close();
}

Server	*HttpResponse::getServer() const {
	return (this->_client->getServer(this->_client->getRequest()->getServerIndex()));
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
	o << END_STYLE<< endl;
	if (response.getServer()->getLogLevel() == 2){
		o << THIN ITALIC << response.getHeader() << END_STYLE << endl;
	}
	return (o);
}
