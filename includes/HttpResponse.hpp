/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maburnet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:27:41 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/07 15:57:23 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "webserv.h"
# include "HttpRequest.hpp"
# include "Server.hpp"

class Client;
class Server;
class HttpRequest;

class HttpResponse {
	private:
		Client					*_client;
		string					_statusLine;
		int						_statusCode;
		size_t					_contentLength;
		string					_mime;
		string					_header;
		int						_cgiIndex;
		string					_pathInfo;
		string					_cgiTmpFile;
		string					_contentType;

		vector<string>			_indexes;
		string					_locPath;
		string					_root;
		long long				_maxBodySize;
		vector<enum HttpMethod>	_allowedMethod;
		bool					_directoryListing;
		map<int,string>			_errorPage;
		map<int,string>			_returnURI;
		string					_uploadPath;
		vector<string>			_cgiBin;
		vector<string>			_cgiExt;
		bool					_isLocation;

		int				getClientFd() const;
		int				sendData(const void *data, int len);
		char			**createEnv(string uri);
		bool			keepAlive() const;
		bool			getClientError() const;
		bool			executeCGI(string uri);
		bool			expandUri(string &uri, bool &isDir);
		bool			methodeAllowed(enum HttpMethod methode);
		void			setInfos();
		void			sendHeader();
		void			sendChunkEnd();
		void			error(int num);
		void			createHeader();
		void			setDateHeader();
		void			setStatusLine();
		void			sendFinalChunk();
		void			setClientError();
		void			setServerHeader();
		void			checkSend(int bytes);
		void			sendChunkSize(int len);
		void			setContentTypeHeader();
		void			sendErrorPage(int num);
		void			setContentLengthHeader();
		void			tryDeleteFile(string uri);
		void			sendContent(ifstream &file);
		void			movedPermanently(string uri);
		void			directoryListing(string path);
		void			setKeepAliveConnectionHeader();
		void			sendDirectoryPage(string path);
		void			errorCGI(string str, int tmpfd);
		void			parseCGI();
		HttpRequest		*getRequest() const;
		vector<string>	getIndexes() const;

	public:
		HttpResponse();
		HttpResponse(Client *client);
		HttpResponse(const HttpResponse &cpy);
		~HttpResponse();

		HttpResponse &operator=(const HttpResponse &rhs);

		int				getStatusCode() const;
		void			sendResponse();
		string			getMime() const;
		string			getHeader() const;
		Server			*getServer() const;
};

ostream &operator<<(ostream &o, const HttpResponse &response);

#endif
