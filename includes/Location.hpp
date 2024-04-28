/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:41:57 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/28 11:58:16 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

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

		string	_locPath ;
		int		_locIndex ;

		void	extractLocPath( string & locBloc ) ;

		Location( void ) ;
		Location( Location const & cpy ) ;
		Location &	operator=( Location const & rhs ) ;
			
} ;

ostream &	operator<<( ostream & o, Location const & rhs ) ;

/* *** LOCATION.HPP *** */

#endif
