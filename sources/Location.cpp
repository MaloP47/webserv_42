/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:42:58 by mpeulet           #+#    #+#             */
/*   Updated: 2024/05/06 14:50:14 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( string const & locBlock, int index, long long servMbs ) : _locIndex( index ) {

	string	loc = locBlock ;
	extractLocPath( loc ) ;
	_root = extractStringVariable( loc, "root" ) ;
	if ( _root.empty() )
		throw runtime_error( "Missing root, mandatory field for each location." ) ;
	checkMBS( loc, servMbs ) ;
	checkAutoIndex( loc ) ;
	_uploadPath = extractStringVariable( loc, "upload_path" ) ;
	checkMethod( loc ) ;
	_index = extractStringVariable( loc, "index" ) ;
	extractMap( loc, "error_page", _errorPage ) ;
	extractMap( loc, "return", _returnURI ) ;
	checkCGIBin( loc ) ;
	if ( _binPath.size() )
		checkCGIExtension( loc ) ;
	if ( loc != "}" )
		throw runtime_error( "Location block in config file contains unknown instructions : " + loc ) ;
}

Location::~Location( void ) {}

Location::Location( Location const & src ) {
	*this = src ;
}

Location &	Location::operator=( Location const & rhs ) {
	if ( this != &rhs ) {
		_locIndex = rhs._locIndex ;
		_locPath = rhs._locPath ;
		_root = rhs._root ;
		_maxBodySize = rhs._maxBodySize ;
		_allowedMethod = rhs._allowedMethod ;
		_directoryListing = rhs._directoryListing ;
		_index = rhs._index ;
		_errorPage = rhs._errorPage ;
		_returnURI = rhs._returnURI ;
		_uploadPath = rhs._uploadPath ;
		_binPath = rhs._binPath ;
		_cgiExtension = rhs._cgiExtension ;
	}
	return *this ;
}

void	Location::extractLocPath( string & locBloc ) {
	size_t	slash = locBloc.find( "/" ) ;
	size_t	end = locBloc.find( "{" ) ;
	if ( slash != 0 || slash >= end  )
		throw runtime_error( "Route for location block missing in block: " + locBloc ) ;
	_locPath = locBloc.substr( slash, end - slash ) ;
	locBloc.erase( slash, end - slash + 1 ) ;
}

string	Location::extractStringVariable( string & tmp, string const & var ) {
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

void	Location::checkMBS( string & block, long long servMBS ) {
	char	*endptr ;
	string	tmp = extractStringVariable( block, "client_max_body_size" ) ;
	if ( tmp.empty() ) {
		_maxBodySize = servMBS;
		return ;
	}
	_maxBodySize = strtoll( tmp.c_str(), &endptr, 10 ) ;
	if ( *endptr != 0 || !isAllDigits( tmp ) )
		throw runtime_error( "Wrong client BS format, only digits. In " + _locPath ) ;
}

void	Location::checkMethod( string & block ) {
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
		throw runtime_error( "Allowed methods are only GET, POST and DELETE. In " + _locPath ) ;
}

void	Location::extractMap( string & block, string const & var, map<int,string> & Map ) {
	char	*endptr ;
	int		key = 0 ;
	string	tmp = extractStringVariable( block, var ) ;
	if ( tmp.empty() )
		return ;
	if ( tmp.size() < 3 )
		throw runtime_error( "Invalid error page / return format. In " + _locPath ) ;
	key = strtol( tmp.substr( 0, 3 ).c_str(), &endptr, 10 ) ;
	if ( *endptr != 0 || ( key < 100 || key > 505 ) )
		throw runtime_error( "Invalid error page / return format. In " + _locPath) ;
	tmp.erase( 0, 3 ) ;
	Map.insert( pair<int,string>( key, tmp )) ;
}

void	Location::checkAutoIndex( string & block ) {
	string	tmp = extractStringVariable( block, "autoindex" ) ;
	if ( tmp.empty() ) {
		_directoryListing = false ;
		return ;
	}
	if ( tmp != "on" && tmp != "off" )
		throw runtime_error( "Autoindex setting is either on or off. In " + _locPath ) ;
	if ( tmp == "on" )
		_directoryListing = true ;
	else
		_directoryListing = false ;
}

void	Location::checkCGIBin( string & block ) {
	string	tmp = extractStringVariable( block, "cgi_bin" ) ;
	if ( tmp.empty() )
		return ;
	_binPath = split_trim_conf( tmp, "," ) ;
	if ( !areAllPathsBinaries( _binPath ) )
		throw runtime_error( "All paths in cgi_bin must be binaries. Block :" + _locPath ) ;
}

void	Location::checkCGIExtension( string & block ) {
	string	tmp = extractStringVariable( block, "cgi_extension" ) ;
	if ( tmp.empty() )
		return ;
	_cgiExtension = split_trim_conf( tmp, "," ) ;
	if ( !allStartWithDot( _cgiExtension ) )
		throw runtime_error( "All extensions in cgi_extension must start with a dot.Block :"  + _locPath ) ;
}


void	Location::setLocPath( string const & lp ) { _locPath = lp ; }
void	Location::setLocIndex( int index ) { _locIndex = index ; }
void	Location::setRoot( string const & root ) { _root = root ; }
void	Location::setMaxBodySize( int maxBodySize ) { _maxBodySize = maxBodySize ; }
void	Location::setAllowedMethod( vector<enum HttpMethod> const & allowedMethod ) { _allowedMethod = allowedMethod ; }
void	Location::setDirectoryListing( bool directoryListing ) { _directoryListing = directoryListing ; }
void	Location::setIndex( string const & index ) { _index = index ; }
void	Location::setErrorPages( map<int,string> const & err ) { _errorPage = err ; }
void	Location::setReturnURI( map<int,string> const & uri ) { _returnURI = uri ; }
void	Location::setUploadPath( string const & path ) { _uploadPath = path ; }
void	Location::setCGIBin( vector<string> const & binPath ) { _binPath = binPath ; }
void	Location::setCGIExtension( vector<string> const & cgiExtension ) { _cgiExtension = cgiExtension ; }

string const &					Location::getLocPath( void ) const { return _locPath ; }
int								Location::getLocIndex( void ) const { return _locIndex ; }
string const &					Location::getRoot( void ) const { return _root ; }
int								Location::getMaxBodySize( void ) const { return _maxBodySize ; }
vector<enum HttpMethod> const &	Location::getAllowedMethod( void ) const { return _allowedMethod ; }
bool							Location::getDirectoryListing( void ) const { return _directoryListing ; }
string const &					Location::getIndex( void ) const { return _index ; }
map<int,string> const &			Location::getErrorPages( void ) const { return _errorPage ; }
map<int,string> const &			Location::getReturnURI( void ) const { return _returnURI ; }
string const &					Location::getUploadPath( void ) const { return _uploadPath ; }
vector<string> const &			Location::getCGIBin( void ) const { return _binPath ; }
vector<string> const &			Location::getCGIExtension( void ) const { return _cgiExtension ; }

ostream &	operator<<( ostream & o, Location const & rhs ) {
	o << "--------------------LOCATION--------------------" << endl ;
	o << "[ " << "Location block " << rhs.getLocIndex() + 1 << " ]" << endl ; 
	o << "Location: " << rhs.getLocPath() << std::endl ;
	o << "Root: " << rhs.getRoot() << std::endl ;
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
	o << "BinPath: ";
    for (vector<string>::const_iterator it = rhs.getCGIBin().begin(); it != rhs.getCGIBin().end(); ++it) {
        o << *it;
        if (it + 1 != rhs.getCGIBin().end())
            o << ", ";
    }
    o << std::endl;
    o << "CgiExtension: ";
    for (vector<string>::const_iterator it = rhs.getCGIExtension().begin(); it != rhs.getCGIExtension().end(); ++it) {
        o << *it;
        if (it + 1 != rhs.getCGIExtension().end())
            o << ", ";
    }
	o << std::endl ;
	return o ;
}
