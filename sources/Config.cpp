/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:21:53 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/17 16:09:53 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Config.hpp"


/* *** constructors *** */

Config::Config( string const & path ) : _nbServer( 0 ) {
	if ( path.empty() )
		throw runtime_error( "Invalid config as parameter: NULL." ) ;
	if ( isDirectory( path ) )
		throw runtime_error( "Parameter is a directory." ) ;
	if ( !isConf( path ) )
		throw runtime_error( "Invalid config as parameter: must be .conf extension file." ) ;
	if ( access( path.c_str(), R_OK))
		throw runtime_error( "File is not readable." ) ;
	ifstream	config( path.c_str() ) ;
	if ( !config || !config.is_open() )
		throw runtime_error( "Invalid config as parameter: " + path ) ;
	cleanRawConfig( config ) ;
	if ( _rawConfig.find( '#' ) != string::npos )
		throw runtime_error( "Invalid character found in config file: #." ) ;
	if ( !parseRawConfig() )
		throw runtime_error( "Error in config file." ) ;
}

/* *** destructor *** */

Config::~Config( void ) {}

/* *** public functions *** */

int				Config::getNbServer( void ) const { return _nbServer ; }
string const &	Config::getRawConfig( void ) const { return _rawConfig ;}

/* *** private functions *** */

bool	Config::isDirectory( string const & path ) {
	struct stat	statbuf ;
	if ( !stat( path.c_str(), &statbuf ) )
		return S_ISDIR( statbuf.st_mode ) ;
	return false ;
}

bool	Config::isConf( string const & path ) {
	string::size_type pos = path.rfind(".conf");
	return pos != string::npos && pos == path.length() - 5;
}

void	Config::cleanRawConfig( ifstream & config ) {

	string	word ;

	while ( config >> word )
		_rawConfig += word ;
}

bool	Config::parseRawConfig( void ) {
	if ( _rawConfig.find("server{") == string::npos )
		return false ;
	return true ;
}

/* *** operator = *** */
/* *** copy constructor *** */
/* *** other *** */
