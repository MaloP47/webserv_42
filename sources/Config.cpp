/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:21:53 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/15 16:39:00 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Config.hpp"


/* *** constructors *** */

Config::Config( string const & path ) {
	ifstream	config( path.c_str() ) ;
	if ( !config || !config.is_open() )
		throw runtime_error( "Invalid config as parameter: " + path ) ;
}

/* *** copy constructor *** */



/* *** destructor *** */

Config::~Config( void ) {}

/* *** operator = *** */



/* *** public functions *** */



/* *** private functions *** */



/* *** other *** */
