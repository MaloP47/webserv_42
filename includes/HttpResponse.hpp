/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:27:41 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/14 17:46:41 by gbrunet          ###   ########.fr       */
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
		Client	*_client;
		string	_statusLine;
		int		_statusCode;
		size_t	_contentLength;
		string	_mime;
		string	_header;

		int				getClientFd() const;
		int				sendData(const void *data, int len);
		bool			keepAlive() const;
		bool			getClientError() const;
		bool			expandUri(string &uri, bool &isDir);
		void			sendHeader();
		void			sendChunkEnd();
		void			error(int num);
		void			createHeader();
		void			setDateHeader();
		void			setStatusLine();
		void			tryDeleteFile();
		void			sendFinalChunk();
		void			setClientError();
		void			setServerHeader();
		void			checkSend(int bytes);
		void			sendChunkSize(int len);
		void			setContentTypeHeader();
		void			sendErrorPage(int num);
		void			setContentLengthHeader();
		void			sendContent(ifstream &file);
		void			movedPermanently(string uri);
		void			directoryListing(string path);
		void			setKeepAliveConnectionHeader();
		void			sendDirectoryPage(string path);
		void			errorCGI(string, int tmpfd);
		void			executeCGI(char **env);
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
