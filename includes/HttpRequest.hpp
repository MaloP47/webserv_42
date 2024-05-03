/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 10:57:23 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/03 13:53:14 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "webserv.h"
# include "Server.hpp"
# include "Client.hpp"
# include "Upload.hpp"

// enum HttpMethod {
// 	GET,
// 	POST,
// 	DELETE,
// 	OTHER
// };


class Server;
class Client;
class Upload;

typedef vector<string>::iterator	strVecIt;
typedef vector<char>::iterator		charVecIt;
typedef vector<Upload>::iterator	uploadIt;

class HttpRequest {
	private:
		Client			*_client;
		string			_rawRequest;
		vector<char>	_rawBytes;
		vector<Upload>	_uploadedFiles;
		size_t			_headerLength;
		size_t			_requestLength;
		bool			_goodRequest;
		bool			_tooLarge;
		enum HttpMethod	_method;
		string			_uri;
		string			_host;
		int				_serverIndex;
		vector<string>	_acceptedMimes;
		bool			_keepAliveConnection;
		size_t			_contentLength;
		string			_content;
		string			_contentType;
		string			_boundary;
		string			_textPost;

		bool	isFullRequest();
		void	parse();
		void	setMethod(string str);
		void	parseHost(string line);
		void	getUriAndEnv(string str);
		void	parseConnection(string line);
		void	parseRequestLine(string line);
		void	parseAcceptedMimes(string line);
		void	parseContentLength();
		void	parseContentType(string line);
		void	decodeFormData();
		void	decodeUrlEncoded();

	public:
		HttpRequest();
		HttpRequest(Client *client);
		HttpRequest(const HttpRequest &cpy);
		~HttpRequest();

		HttpRequest	&operator=(const HttpRequest &rhs);

		int				getServerIndex() const;
		bool			isGood() const;
		bool			tooLarge() const;
		bool			keepAlive() const;
		bool			appendRequest(const char *data, int bytes);
		string			getUri() const;
		string			getHost() const;
		string			getRawRequest() const;
		Server			*getServer() const;
		Client			*getClient() const;
		enum HttpMethod	getMethod() const;
};

ostream &operator<<(ostream &o, const HttpRequest &request);

#endif
