/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:41:57 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/29 18:19:28 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.h"
# include "ConfigServer.hpp"

class	Location : public ConfigServer {

	public:

		Location( string const & locBloc, int index ) ;
		~Location( void ) ;

		void	setLocPath( string const & lp ) ;
		void	setLocIndex( int index ) ;

		string const &	getLocPath( void ) const ;
		int				getLocIndex( void ) const ;

	private:

		int		_locIndex ;

		// MANDATORY FOR LOCATION
		string	_locPath ;
		/* In ConfigServer 
		string	_root;
		*/

		void	extractLocPath( string & locBloc ) ;

		Location( void ) ;
		// Location( Location const & cpy ) ;
		// Location &	operator=( Location const & rhs ) ;
			
} ;

ostream &	operator<<( ostream & o, Location const & rhs ) ;

/* *** LOCATION.HPP *** */

#endif
