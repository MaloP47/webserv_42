/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:37:40 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/28 12:09:44 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>

# include "enum.h"
# include "utils.h"

using namespace std;

class	ConfigServer {

	public:

		ConfigServer( void ) ;
		ConfigServer( string const & serverBlock, int indexOfServerBlock ) ;
		virtual ~ConfigServer( void ) ;

		void	setPort( int port ) ;
		void	setRoot( string const & root ) ;
		void	setHost( string const & host ) ;
		void	setMaxBodySize( int maxBodySize ) ;
		void	setAllowedMethod( vector<enum HttpMethod> const & allowedMethod ) ;
		void	setDirectoryListing( bool directoryListing ) ;
		void	setName( string const & name ) ;
		void	setIndex( string const & index ) ;
		void	setErrorPages( map<int,string> const & err ) ;
		void	setReturnURI( map<int,string> const & uri ) ;
		void	setUploadPath( string const & path ) ;
		void	setLocation( vector<string> loc ) ;
		// void					setLocationBlock( vector<Location> const & locationBlock ) ;

		int								getServerIndex( void ) const ;
		int								getPort( void ) const ;
		string const &					getRoot( void ) const ;
		string const &					getHost( void ) const ;
		int								getMaxBodySize( void ) const ;
		vector<enum HttpMethod> const &	getAllowedMethod( void ) const ;
		bool							getDirectoryListing( void ) const ;
		string const &					getName( void ) const ;
		string const &					getIndex( void ) const ;
		map<int,string> const &			getErrorPages( void ) const ;
		map<int,string> const &			getReturnURI( void ) const ;
		string const &					getUploadPath( void ) const ;
		vector<string> const &			getLocation( void ) const ;
		// vector<Location> const &		getLocationBlock( void ) const ;

	protected:

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
		string				_index ;
		map<int,string>		_errorPage ;
		map<int,string>		_returnURI ;
		string				_uploadPath ;
		// vector<Location>	_locationBlock ;

		void		extractLocation( string & block ) ;
		string 		extractStringVariable( string & block, string const & var ) ;
		void		checkPort( string & block ) ;
		void		checkMBS( string & block ) ;
		void		checkAutoIndex( string & block ) ;
		void		checkName( string & block, int index ) ;
		void		checkMethod( string & block ) ;
		void		extractMap( string & block, string const & var, map<int,string> & Map ) ;

	private:

		ConfigServer( ConfigServer const & cpy ) ;
		ConfigServer &	operator=( ConfigServer const & rhs ) ;

} ;

ostream &	operator<<( ostream & o, ConfigServer const & rhs ) ;

/* *** CONFIGSERVER.HPP *** */

#endif
