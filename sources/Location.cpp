/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:42:58 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/27 12:52:24 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( string const & locBlock, int index ) : ConfigServer(),
	_locIndex( index ) {

	string	loc = locBlock ;
	extractLocPath( loc ) ;
	cout << _locPath << endl ;
	cout << loc << endl ;
}

Location::~Location( void ) {}

void	Location::extractLocPath( string & locBloc ) {
	size_t	slash = locBloc.find( "/" ) ;
	size_t	end = 0 ;
	if ( slash == string::npos )
		throw runtime_error( "Route for location block missing in block: " + locBloc ) ;
	end = locBloc.find( "{" ) ;
	_locPath = locBloc.substr( slash, end - slash + 1 ) ;
	locBloc.erase( slash, end - slash + 1 ) ;
}

void	Location::setLocPath( string const & lp ) { _locPath = lp ; }
void	Location::setLocIndex( int index ) { _locIndex = index ; }

string const &	Location::getLocPath( void ) const { return _locPath ; }
int				Location::getLocIndex( void ) const { return _locIndex ; }

ostream &	operator<<( ostream & o, Location const & rhs ) {
	(void)rhs ;
	return o ;
}