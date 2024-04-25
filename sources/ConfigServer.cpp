/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:37:58 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/23 18:36:31 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigServer.hpp"

// Constructeur qui initialise par defaut les 5 definis dans .hpp

ConfigServer::ConfigServer( string const & serverBlock, int indexOfServerBlock ) : 
	_host( "127.0.0.1" ),
	_maxBodySize( MAXCLIENTBS ),
	_directoryListing( false ) {

	std::stringstream ss ;
    ss << "Server" << indexOfServerBlock ;
    _name = ss.str();
	_allowedMethod.push_back( GET ) ;

	string 	block = serverBlock ;
	extractLocation( block ) ;
	// PROBABLY DO THE SAME FOR CGI
	// CREATE VECTOR FOR LOCATION
	_root = extractStringVariable( block, "root" ) ;
	if ( _root.empty() )
		 throw runtime_error( "Missing root, mandatory field." ) ;
	checkPort( block ) ;
	cout << block << endl ;
	cout << _port << endl ;
}

string	ConfigServer::extractStringVariable( string & tmp, string const & var ) {
	size_t	start = 0 ;
	size_t	end = 0 ;
	string	ret ;

	start = tmp.find( var ) ;
	if ( start == string::npos )
		return string() ;
	end = tmp.find( ";", start ) ;
	if ( end == string::npos ) 
		throw runtime_error( "Missing ;" ) ;
	ret = tmp.substr( start + var.length(), end - start - var.length() ) ;
	tmp.erase( start, end - start + 1 ) ;
	return ret ;
}

void	ConfigServer::checkPort( string & block ) {
	string	tmp = extractStringVariable( block, "listen" ) ;
	if ( tmp.empty() )
		throw runtime_error( "Missing port, mandatory field." ) ;
	if ( tmp.size() < 5 || tmp.size() > 5 || !isAllDigits( tmp ) )
		throw runtime_error( "Wrong port format." ) ;
	_port = atoi( tmp.c_str() ) ;
	if ( _port < 49152 || _port > 65535 )
		throw runtime_error( "By convention port must be between 49152 et 65535" ) ;
}

void	ConfigServer::extractLocation( string & tmp ) {
	size_t	pos = 0 ;
	size_t	start = 0 ;
	size_t	end = 0 ;

	while ( ( pos = tmp.find( "location", pos ) ) != string::npos ) {
		end = tmp.find( "}", pos ) ;
		if ( end == string::npos ) {
			stringstream	ss;
			ss << _location.size() + 1 ;
			throw runtime_error( "Missing location } in location block " + ss.str() ) ;
		}
		start =  tmp.find( "{", pos ) ;
		if ( start == string::npos || start > end ) {
			stringstream	ss;
			ss << _location.size() + 1 ;
			throw runtime_error( "Missing location { in location block " + ss.str() ) ;
		}
		_location.push_back( tmp.substr( pos + 8, end - pos + 1 ) ) ;
		tmp.erase( pos, end - pos + 1 ) ;
		pos = 0 ;
	}
}

ConfigServer::~ConfigServer( void ) {}

void	ConfigServer::setPort( int port ) { _port = port ; }
void	ConfigServer::setRoot( string const & root ) { _root = root ; }
void	ConfigServer::setHost( string const & host ) { _host = host ; }
void	ConfigServer::setMaxBodySize( int maxBodySize ) { _maxBodySize = maxBodySize ; }
void	ConfigServer::setAllowedMethod( vector<enum HttpMethod> const & allowedMethod ) { _allowedMethod = allowedMethod ; }
void	ConfigServer::setDirectoryListing( bool directoryListing ) { _directoryListing = directoryListing ; }
void	ConfigServer::setName( string const & name ) { _name = name ; }
void	ConfigServer::setIndexes( vector<string> const & indexes ) { _indexes = indexes ; }
void	ConfigServer::setErrorPages( map<int,string> const & err ) { _errorPages = err ; }
void	ConfigServer::setReturnURI( string const & url ) { _returnURI = url ; }
void	ConfigServer::setUploadPath( string const & path ) { _uploadPath = path ; }
// void	ConfigServer::setLocationBlock( vector<Location> const & locationBlock ) { _locationBlock = locationBlock ; }

int								ConfigServer::getPort( void ) const { return _port ; }
string const &					ConfigServer::getRoot( void ) const { return _root ; }
string const &					ConfigServer::getHost( void ) const { return _host ; }
int								ConfigServer::getMaxBodySize( void ) const { return _maxBodySize ; }
vector<enum HttpMethod> const &	ConfigServer::getAllowedMethod( void ) const { return _allowedMethod ; }
bool							ConfigServer::getDirectoryListing( void ) const { return _directoryListing ; }
string const &					ConfigServer::getName( void ) const { return _name ; }
vector<string> const &			ConfigServer::getIndexes( void ) const { return _indexes ; }
map<int,string> const &			ConfigServer::getErrorPages( void ) const { return _errorPages ; }
string const &					ConfigServer::getReturnURL( void ) const { return _returnURI ; }
string const &					ConfigServer::getUploadPath( void ) const { return _uploadPath ; }
// vector<Location> const &		ConfigServer::getLocationBlock( void ) const { return _locationBlock ; }

ostream &	operator<<( ostream & o, ConfigServer const & rhs ) {
	o << "Server: " << rhs.getName() << std::endl ;
	o << "Port: " << rhs.getPort() << std::endl ;
	o << "Root: " << rhs.getRoot() << std::endl ;
	o << "Host: " << rhs.getHost() << std::endl ;
	o << "MaxBodySize: " << rhs.getMaxBodySize() << std::endl ;
	o << "AllowedMethod: " ;
	for ( size_t i = 0 ; i < rhs.getAllowedMethod().size() ; i++ ) {
		o << rhs.getAllowedMethod()[i] ;
		if ( i + 1 < rhs.getAllowedMethod().size() )
			o << ", " ;
	}
	o << std::endl ;
	o << "DirectoryListing: " << rhs.getDirectoryListing() << std::endl ;
	o << "Indexes: " ;
	for ( size_t i = 0 ; i < rhs.getIndexes().size() ; i++ ) {
		o << rhs.getIndexes()[i] ;
		if ( i + 1 < rhs.getIndexes().size() )
			o << ", " ;
	}
	o << std::endl;
	o << "ErrorPages: " ;
	for ( map<int,string>::const_iterator it = rhs.getErrorPages().begin() ; it != rhs.getErrorPages().end() ; it++ ) {
		o << it->first << " -> " << it->second;
		if ( ++it != rhs.getErrorPages().end() )
			o << ", " ;
	}
	o << std::endl;
	o << "ReturnURI: " << rhs.getReturnURL() << std::endl ;
	o << "UploadPath: " << rhs.getUploadPath() << std::endl ;
	// o << "LocationBlock: " << rhs.getLocationBlock().size() << std::endl ;
	return o;
}
