/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:13:35 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/06 14:55:02 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.h"
# include "Webserv.hpp"
# include "ConfigServer.hpp"
# include "Location.hpp"

class Webserv;
class ConfigServer;
class Location;

typedef vector<enum HttpMethod>::iterator	methodeIt;

class Server {
	private:
		Webserv					*_webserv;
		int						_fd;
		struct sockaddr_in		_addr;
		
		int						_port;
		string					_host;
		string					_name;
		bool					_directoryListing;
		string					_root;
		vector<string>			_indexes;
		string					_index;	
		long long				_maxBodySize;
		vector<enum HttpMethod>	_allowedMethod;
		vector<Location>		_locationBlock;

		map<int,string>			_errorPages ;
		map<int,string>			_returnURI ;
		string					_uploadPath ;
		vector<string>			_binPath ;
		vector<string>			_cgiExtension ;
			
		int		init();
		void	setAddr();
		void	abstraction( void ) const;

	public:
		Server();
		Server(Webserv *webserv, int port, string host, string name);
		Server( Webserv *webserv, vector<ConfigServer> const & conf, int i ) ;
		Server(const Server &cpy);
		~Server();

		Server &operator=(const Server &rhs);

		int							getFd() const;
		int							getPort() const;
		int							getLogLevel() const;
		int							getMaxBodySize() const;
		bool						getDirectoryListing() const;
		void						setFd(int fd);
		string						getName() const;
		string						getHost() const;
		string						getRoot() const;
		vector<string>				getIndexes() const;
		vector<Location> const &	getLocation() const;
		map<int,string> const &		getErrorPages( void ) const ;
		map<int,string> const &		getReturnURI( void ) const ;
		string const &				getUploadPath( void ) const ;
		bool						methodeAllowed(enum HttpMethod methode);
		vector<enum HttpMethod>		getAllowedMethod() const;
		vector<string> const &		getBinPath( void ) const ;
		vector<string> const &		getCgiExtension( void ) const ;

};

ostream &operator<<(ostream &o, const Server &server);

#endif
