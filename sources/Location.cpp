/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 13:27:17 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/23 11:00:29 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( string const & locationBlock ) {
	if ( locationBlock.find( "location/" ))
		throw runtime_error( "Location block is not set properly" ) ;
}

Location::~Location( void ) {}

void	Location::setDirectoryListing( bool onOff ) { _directoryListing = onOff ; }
void	Location::setRoot( string const & root ) { _root = root ; }
void	Location::setIndexes( vector<string> const & indexes ) { _indexes = indexes ; }
void	Location::setMaxBodySize( int MBS ) { _maxBodySize = MBS ; }
void	Location::setAlloweMethod( vector<enum HttpMethod> const & AM ) { _allowedMethod = AM ; }
void	Location::setErrorPages( map<int,string> const & EP ) { _errorPages = EP ; }
void	Location::setReturnURI( string const & uri ) { _returnURI = uri ; }
void	Location::setUploadPath( string const & path ) { _uploadPath = path ; }
void	Location::setLocPath( string const & locpath ) { _locPath = locpath ; }

bool const &					Location::getDirectoryListing( void ) const { return _directoryListing ; }
string const &					Location::getRoot( void ) const { return _root ; }
vector<string> const &			Location::getIndexes( void ) const { return _indexes ; }
int const &						Location::getMaxBodySize( void ) const { return _maxBodySize ; }
vector<enum HttpMethod> const &	Location::getAlloweMethod( void ) const { return _allowedMethod ; }
map<int,string> const &			Location::getErrorPages( void ) const { return _errorPages ; }
string const &					Location::getReturnURI( void ) const { return _returnURI ;}
string const &					Location::getUploadPath( void ) const { return _uploadPath ; }
string const &					Location::getLocPath( void ) const { return _locPath ; }
