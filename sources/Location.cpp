/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:42:58 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/29 18:35:35 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( string const & locBlock, int index ) : ConfigServer(),
	_locIndex( index ) {

	string	loc = locBlock ;
	extractLocPath( loc ) ;
	if ( _locPath == "/cgi" ) {
		// manage CGI
		cout << "Execute CGI" << endl ;
		return ;
	}
	_root = extractStringVariable( loc, "root" ) ;
	if ( _root.empty() ) // Maybe not be necessary but might need to pass Server _root ;
		throw runtime_error( "Missing root, mandatory field for each location." ) ;
	checkMBS( loc ) ;
	checkAutoIndex( loc ) ;
	// Might need to pass Server _uploadPath if unspecified
	_uploadPath = extractStringVariable( loc, "upload_path" ) ;
	checkMethod( loc ) ;
	_index = extractStringVariable( loc, "index" ) ;
	extractMap( loc, "error_page", _errorPage ) ;
	extractMap( loc, "return", _returnURI ) ;
	if ( loc != "}" )
		throw runtime_error( "Location block in config file contains unknown instructions : " + loc ) ;
}

Location::~Location( void ) {}

void	Location::extractLocPath( string & locBloc ) {
	size_t	slash = locBloc.find( "/" ) ;
	size_t	end = locBloc.find( "{" ) ;
	if ( slash != 0 || slash >= end  )
		throw runtime_error( "Route for location block missing in block: " + locBloc ) ;
	_locPath = locBloc.substr( slash, end - slash ) ;
	locBloc.erase( slash, end - slash + 1 ) ;
}

void	Location::setLocPath( string const & lp ) { _locPath = lp ; }
void	Location::setLocIndex( int index ) { _locIndex = index ; }

string const &	Location::getLocPath( void ) const { return _locPath ; }
int				Location::getLocIndex( void ) const { return _locIndex ; }

ostream &	operator<<( ostream & o, Location const & rhs ) {
	o << "--------------------LOCATION--------------------" << endl ;
	o << "[ " << "Location block " << rhs.getLocIndex() << " ]" << endl ; 
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
	
	return o ;
}