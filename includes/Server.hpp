/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:13:35 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/30 14:10:59 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.h"
# include "AServerAttributes.hpp"
# include "Webserv.hpp"
# include "ConfigServer.hpp"

class Webserv;
class ConfigServer;

typedef vector<enum HttpMethod>::iterator	methodeIt;

class Server : public AServerAttributes {
	private:
		Webserv					*_webserv;
		int						_port;
		string					_host;
		string					_name;
		int						_fd;
		struct sockaddr_in		_addr;
		bool					_directoryListing;
		string					_root;
		vector<string>			_indexes;
		long long				_maxBodySize;
		vector<enum HttpMethod>	_allowedMethod;
		// vector<Location>		_locationBlock;
			
		int		init();
		void	setAddr();
		void	abstraction( void ) const;

	public:
		Server();
		Server(Webserv *webserv, int port, string host, string name);
		Server( ConfigServer const & conf ) ;
		Server(const Server &cpy);
		~Server();

		Server &operator=(const Server &rhs);

		int							getFd() const;
		int							getPort() const;
		int							getLogLevel() const;
		int							getMaxBodySize() const;
		bool						getDirectoryListing() const;
		string						getName() const;
		string						getHost() const;
		string						getRoot() const;
		vector<string>				getIndexes() const;
		// vector<Location> const &	getLocation() const;
		bool						methodeAllowed(enum HttpMethod methode);

};

ostream &operator<<(ostream &o, const Server &server);

#endif
