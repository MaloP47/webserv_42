/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:37:40 by mpeulet           #+#    #+#             */
/*   Updated: 2024/05/06 13:32:29 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include "webserv.h"
# include "Location.hpp"

class Location;

class	ConfigServer {

	public:

		ConfigServer(string const & serverBlock, int indexOfServerBlock);
		~ConfigServer(void);
		ConfigServer(ConfigServer const & cpy);
		ConfigServer &	operator=(ConfigServer const & rhs);

		void	setPort(int port);
		void	setRoot(string const & root);
		void	setHost(string const & host);
		void	setName(string const & name);
		void	setMaxBodySize(int maxBodySize);
		void	setDirectoryListing(bool directoryListing);
		void	setAllowedMethod(vector<enum HttpMethod> const & allowedMethod);
		void	setIndex(string const & index);
		void	setErrorPages(map<int,string> const & err);
		void	setReturnURI(map<int,string> const & uri);
		void	setUploadPath(string const & path);
		void	setLocation(vector<string> loc);
		void	setLocationBlock(vector<Location> const & locationBlock);
		void	setBinPath(vector<string> const & binPath);
		void	setCgiExtension(vector<string> const & cgiExtension);

		int								getPort(void) const;
		int								getMaxBodySize(void) const;
		int								getServerIndex(void) const;
		bool							getDirectoryListing(void) const;
		string const &					getRoot(void) const;
		string const &					getHost(void) const;
		string const &					getName(void) const;
		string const &					getIndex(void) const;
		string const &					getUploadPath(void) const;
		string const &					getServerBlock(void) const;
		vector<string> const &			getBinPath(void) const;
		vector<string> const &			getLocation(void) const;
		vector<string> const &			getCgiExtension(void) const;
		map<int,string> const &			getReturnURI(void) const;
		map<int,string> const &			getErrorPages(void) const;
		vector<Location> const &		getLocationBlock(void) const;
		vector<enum HttpMethod> const &	getAllowedMethod(void) const;

	private:

		string					_serverBlock ;
		int						_indexServer ;
		vector<string>			_location ;

		/* MANDATORY */
		int						_port ;
		string					_root ;

		/* SET TO DEFAULT IF NOT SPECIFIED */
		string					_host;
		long long				_maxBodySize ;
		vector<enum HttpMethod>	_allowedMethod ;
		bool					_directoryListing ;
		string					_name ;

		/* OPTIONAL */
		string					_index ;
		map<int,string>			_errorPage ;
		map<int,string>			_returnURI ;
		string					_uploadPath ;
		vector<Location>		_locationBlock ;

		/* CGI */
		vector<string>			_binPath ;
		vector<string>			_cgiExtension ;

		void		checkMBS(string & block);
		void		checkPort(string & block);
		void		checkCGIBin(string & block);
		void		checkMethod(string & block);
		void		checkAutoIndex(string & block);
		void		initLocation(long long servMBS);
		void		extractLocation(string & block);
		void		checkCGIExtension(string & block);
		void		checkName(string & block, int index);
		void		extractMap(string & block, string const & var, map<int,string> & Map);
		string		extractStringVariable(string & block, string const & var);

		ConfigServer( void ) ;
} ;

ostream &	operator<<( ostream & o, ConfigServer const & rhs ) ;
/* *** CONFIGSERVER.HPP *** */
#endif
