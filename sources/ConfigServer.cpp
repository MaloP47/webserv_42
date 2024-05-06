/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:37:58 by mpeulet           #+#    #+#             */
/*   Updated: 2024/05/06 10:51:38 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigServer.hpp"

ConfigServer::ConfigServer( string const & serverBlock, int indexOfServerBlock ) :
		_serverBlock( serverBlock ), 
 		_indexServer( indexOfServerBlock ) {

	string 	block = serverBlock ;
	extractLocation( block ) ;
	checkMBS( block ) ;
	initLocation( getMaxBodySize() ) ;
	_root = extractStringVariable( block, "root" ) ;
	if ( _root.empty() )
		throw runtime_error( "Missing root, mandatory field." ) ;
	checkPort( block ) ;
	_host = extractStringVariable( block, "host" ) ;
	if ( _host.empty() )
		_host = "127.0.0.1" ;
	checkAutoIndex( block ) ;
	checkName( block, indexOfServerBlock ) ;
	_uploadPath = extractStringVariable( block, "upload_path" ) ;
	checkMethod( block ) ;
	_index = extractStringVariable( block, "index" ) ;
	extractMap( block, "error_page", _errorPage ) ;
	extractMap( block, "return", _returnURI ) ;
	if ( block != "}" )
		throw runtime_error( "Config file contains unknown instructions :" + block ) ;
}

ConfigServer::ConfigServer( ConfigServer const & cpy ) {
	*this = cpy ;
}

ConfigServer &	ConfigServer::operator=( ConfigServer const & rhs ) {
	if ( this != &rhs ) {
		_indexServer = rhs._indexServer ;
		_port = rhs._port ;
		_root = rhs._root ;
		_host = rhs._host ;
		_maxBodySize = rhs._maxBodySize ;
		_allowedMethod = rhs._allowedMethod ;
		_directoryListing = rhs._directoryListing ;
		_name = rhs._name ;
		_index = rhs._index ;
		_errorPage = rhs._errorPage ;
		_returnURI = rhs._returnURI ;
		_uploadPath = rhs._uploadPath ;
		_location = rhs._location ;
		_locationBlock = rhs._locationBlock ;
	}
	return *this ;
}

ConfigServer::~ConfigServer( void ) {}

string 	ConfigServer::extractStringVariable( string & tmp, string const & var ) {
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
	if ( tmp.size() < 4 || tmp.size() > 5 || !isAllDigits( tmp ) )
		throw runtime_error( "Wrong port format." ) ;
	_port = atoi( tmp.c_str() ) ;
	if ( _port < 8079 || _port > 65535 )
		throw runtime_error( "By convention port must be between 49152 et 65535" ) ;
}

void ConfigServer::checkMBS( string & block ) {
	char	*endptr ;
	string	tmp = extractStringVariable( block, "client_max_body_size" ) ;
	if ( tmp.empty() ) {
		_maxBodySize = MAXCLIENTBS ;
		return ;
	}
	_maxBodySize = strtoll( tmp.c_str(), &endptr, 10 ) ;
	if ( *endptr != 0 || !isAllDigits( tmp ) )
		throw runtime_error( "Wrong client BS format, only digits." ) ;
}

void	ConfigServer::checkAutoIndex( string & block ) {
	string	tmp = extractStringVariable( block, "autoindex" ) ;
	if ( tmp.empty() ) {
		_directoryListing = false ;
		return ;
	}
	if ( tmp != "on" && tmp != "off" )
		throw runtime_error( "Autoindex setting is either on or off." ) ;
	if ( tmp == "on" )
		_directoryListing = true ;
	else
		_directoryListing = false ;
}

void	ConfigServer::checkName( string & block, int index ) {
	std::stringstream ss ;

	_name = extractStringVariable( block, "server_name" ) ;
	if ( _name.empty() ) {
		std::stringstream ss ;
    ss << "(default)Server" << index ;
    	_name = ss.str();
	}
}

void	ConfigServer::checkMethod( string & block ) {
	size_t	pos = 0 ;

	string	tmp = extractStringVariable( block, "allowedMethods" ) ;
	if ( tmp.empty() ) {
		_allowedMethod.push_back( GET ) ;
		return ;
	}
	pos = tmp.find( "GET" ) ;
	if ( pos != string::npos ) {
		_allowedMethod.push_back( GET ) ;
		tmp.erase( pos, 3 ) ;
	}
	pos = tmp.find( "POST" ) ;
	if ( pos != string::npos ) {
		_allowedMethod.push_back( POST ) ;
		tmp.erase( pos, 4 ) ;
		if ( _uploadPath.empty() )
			throw runtime_error( "Upload path is mandatory if POST method is allowed." ) ;
	}
	pos = tmp.find( "DELETE" ) ;
	if ( pos != string::npos ) {
		_allowedMethod.push_back( DELETE ) ;
		tmp.erase( pos, 6 ) ;
	}
	if ( !tmp.empty() )
		throw runtime_error( "Allowed methods are only GET, POST and DELETE" ) ;
}

void	ConfigServer::extractMap( string & block, string const & var, map<int,string> & Map ) {
	char	*endptr ;
	int		key = 0 ;
	string	tmp = extractStringVariable( block, var ) ;
	if ( tmp.empty() )
		return ;
	if ( tmp.size() < 3 )
		throw runtime_error( "Invalid error page / return format." ) ;
	key = strtol( tmp.substr( 0, 3 ).c_str(), &endptr, 10 ) ;
	if ( *endptr != 0 || ( key < 100 || key > 505 ) )
		throw runtime_error( "Invalid error page / return format." ) ;
	tmp.erase( 0, 3 ) ;
	Map.insert( pair<int,string>( key, tmp )) ;
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
		start = tmp.find( "{", pos ) ;
		if ( start == string::npos || start > end ) {
			stringstream	ss;
			ss << _location.size() + 1 ;
			throw runtime_error( "Missing location { in location block " + ss.str() ) ;
		}
		_location.push_back( tmp.substr( pos + 8, ( end - pos + 1 ) - 8 ) ) ;
		tmp.erase( pos, end - pos + 1 ) ;
		pos = 0 ;
	}
}

void		ConfigServer::initLocation( long long servMBS ) {
	size_t	locSize = _location.size() ;
	if ( locSize ) {
		for ( size_t i = 0; i < locSize; ++i ) {
			Location lc( _location[i], i, servMBS ) ;
			_locationBlock.push_back( lc ) ;
		}
	}	
}

void	ConfigServer::checkCGIBin( string & block ) {
	string	tmp = extractStringVariable( block, "cgi_bin" ) ;
	if ( tmp.empty() )
		return ;
	_binPath = split_trim_conf( tmp, "," ) ;
	
}

void	ConfigServer::setPort( int port ) { _port = port ; }
void	ConfigServer::setRoot( string const & root ) { _root = root ; }
void	ConfigServer::setHost( string const & host ) { _host = host ; }
void	ConfigServer::setMaxBodySize( int maxBodySize ) { _maxBodySize = maxBodySize ; }
void	ConfigServer::setAllowedMethod( vector<enum HttpMethod> const & allowedMethod ) { _allowedMethod = allowedMethod ; }
void	ConfigServer::setDirectoryListing( bool directoryListing ) { _directoryListing = directoryListing ; }
void	ConfigServer::setName( string const & name ) { _name = name ; }
void	ConfigServer::setIndex( string const & index ) { _index = index ; }
void	ConfigServer::setErrorPages( map<int,string> const & err ) { _errorPage = err ; }
void	ConfigServer::setReturnURI( map<int,string> const & uri ) { _returnURI = uri ; }
void	ConfigServer::setUploadPath( string const & path ) { _uploadPath = path ; }
void	ConfigServer::setLocation( vector<string> loc ) { _location = loc ; }
void	ConfigServer::setLocationBlock( vector<Location> const & locationBlock ) { _locationBlock = locationBlock ; }
void	ConfigServer::setBinPath( vector<string> const & binPath ) { _binPath = binPath ; }
void	ConfigServer::setCgiExtension( vector<string> const & cgiExtension ) { _cgiExtension = cgiExtension ; }

int								ConfigServer::getServerIndex( void ) const { return _indexServer ; }
string const &					ConfigServer::getServerBlock( void ) const { return _serverBlock ; }
int								ConfigServer::getPort( void ) const { return _port ; }
string const &					ConfigServer::getRoot( void ) const { return _root ; }
string const &					ConfigServer::getHost( void ) const { return _host ; }
int								ConfigServer::getMaxBodySize( void ) const { return _maxBodySize ; }
vector<enum HttpMethod> const &	ConfigServer::getAllowedMethod( void ) const { return _allowedMethod ; }
bool							ConfigServer::getDirectoryListing( void ) const { return _directoryListing ; }
string const &					ConfigServer::getName( void ) const { return _name ; }
string const &					ConfigServer::getIndex( void ) const { return _index ; }
map<int,string> const &			ConfigServer::getErrorPages( void ) const { return _errorPage ; }
map<int,string> const &			ConfigServer::getReturnURI( void ) const { return _returnURI ; }
string const &					ConfigServer::getUploadPath( void ) const { return _uploadPath ; }
vector<string> const &			ConfigServer::getLocation( void ) const { return _location ; } ;
vector<Location> const &		ConfigServer::getLocationBlock( void ) const { return _locationBlock ; }
vector<string> const &			ConfigServer::getBinPath( void ) const { return _binPath ; }
vector<string> const &			ConfigServer::getCgiExtension( void ) const { return _cgiExtension ; }

ostream &	operator<<( ostream & o, ConfigServer const & rhs ) {
	o << "//////////////////////SERVER////////////////////////" << endl ;
	o << "[ " << "Server block " << rhs.getServerIndex() << " ]" << endl ; 
	o << "Server: " << rhs.getName() << std::endl ;
	o << "Port: " << rhs.getPort() << std::endl ;
	o << "Root: " << rhs.getRoot() << std::endl ;
	o << "Host: " << rhs.getHost() << std::endl ;
	o << "MaxBodySize: " << rhs.getMaxBodySize() << std::endl ;
	o << "AllowedMethod: " ;
	for ( size_t i = 0 ; i < rhs.getAllowedMethod().size() ; i++ ) {
		string	ret ;
		if ( rhs.getAllowedMethod()[i] == 0 )
			ret = "GET" ;
		else if ( rhs.getAllowedMethod()[i] == 1 )
			ret = "POST" ;
		else if ( rhs.getAllowedMethod()[i] == 2 )
			ret = "DELETE" ;
		o << ret ;
		if ( i + 1 < rhs.getAllowedMethod().size() )
			o << ", " ;
	}
	o << std::endl ;
	o << "Autoindex: " ;
	if ( rhs.getDirectoryListing() == false )
		o << "off" << endl ;
	else if ( rhs.getDirectoryListing() == true )
		o << "on" << endl ;
	o << "Index: " << rhs.getIndex() << std::endl; 
	o << "ErrorPages: " ;
	for ( map<int,string>::const_iterator it = rhs.getErrorPages().begin() ; it != rhs.getErrorPages().end() ; it++ ) {
		o << it->first << " -> " << it->second;
		if ( it != --rhs.getErrorPages().end() )
			o << ", " ;
	}
	o << std::endl ;
	o << "ReturnURI: " ;
	for ( map<int,string>::const_iterator it = rhs.getReturnURI().begin() ; it != rhs.getReturnURI().end() ; it++ ) {
		o << it->first << " -> " << it->second;
		if ( it != --rhs.getReturnURI().end() )
			o << ", " ;
	}
	o << std::endl ;
	o << "UploadPath: " << rhs.getUploadPath() << std::endl ;
	o << "LocationBlock: " << rhs.getLocationBlock().size() << std::endl ;
	for ( size_t i = 0 ; i < rhs.getLocationBlock().size() ; i++ ) {
		o << rhs.getLocationBlock()[i] ;
		if ( i + 1 < rhs.getLocationBlock().size() )
			o << std::endl ;
	}
	o << "BinPath: " ;
	for ( size_t i = 0 ; i < rhs.getBinPath().size() ; i++ ) {
		o << rhs.getBinPath()[i] ;
		if ( i + 1 < rhs.getBinPath().size() )
			o << ", " ;
	}
	o << std::endl ;
	o << "CgiExtension: " ;
	for ( size_t i = 0 ; i < rhs.getCgiExtension().size() ; i++ ) {
		o << rhs.getCgiExtension()[i] ;
		if ( i + 1 < rhs.getCgiExtension().size() )
			o << ", " ;
	}
	o << std::endl ;
	o << "///////////////////////////////////////////////////" << endl ;
	return o;
}
