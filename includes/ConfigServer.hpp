/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:37:40 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/23 17:57:59 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "webserv.h"

// class Location;

class	ConfigServer {

	public:

		ConfigServer( string const & serverBlock, int indexOfServerBlock ) ;
		~ConfigServer( void ) ;

		void					setPort( int port ) ;
		void					setRoot( string const & root ) ;
		void					setHost( string const & host ) ;
		void					setMaxBodySize( int maxBodySize ) ;
		void					setAllowedMethod( vector<enum HttpMethod> const & allowedMethod ) ;
		void					setDirectoryListing( bool directoryListing ) ;
		void					setName( string const & name ) ;
		void					setIndexes( vector<string> const & indexes ) ;
		void					setErrorPages( map<int,string> const & err ) ;
		void					setReturnURI( string const & uri ) ;
		void					setUploadPath( string const & path ) ;
		// void					setLocationBlock( vector<Location> const & locationBlock ) ;

		int								getPort( void ) const ;
		string const &					getRoot( void ) const ;
		string const &					getHost( void ) const ;
		int								getMaxBodySize( void ) const ;
		vector<enum HttpMethod> const &	getAllowedMethod( void ) const ;
		bool							getDirectoryListing( void ) const ;
		string const &					getName( void ) const ;
		vector<string> const &			getIndexes( void ) const ;
		map<int,string> const &			getErrorPages( void ) const ;
		string const &					getReturnURL( void ) const ;
		string const &					getUploadPath( void ) const ;
		// vector<Location> const &		getLocationBlock( void ) const ;

	private:

		vector<string>			_location ;

		/* MANDATORY */
		int						_port ;
		string					_root ;

		/* SET TO DEFAULT IF NOT SPECIFIED */
		string					_host;
		int						_maxBodySize ;
		vector<enum HttpMethod>	_allowedMethod ;
		bool					_directoryListing ;
		string					_name ;

		/* OPTIONAL */
		vector<string>			_indexes ;
		map<int,string>			_errorPages ;
		string					_returnURI ;
		string					_uploadPath ;
		// vector<Location>		_locationBlock ;

		void	extractLocation( string & block ) ;
		string 	extractStringVariable( string & block, string const & var ) ;
		void	checkPort( string & block ) ;

		ConfigServer( void ) ;
		ConfigServer( ConfigServer const & cpy ) ;
		ConfigServer &	operator=( ConfigServer const & rhs ) ;

} ;

ostream &	operator<<( ostream & o, ConfigServer const & rhs ) ;

/* *** CONFIGSERVER.HPP *** */
