/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:10:50 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/12 11:38:15 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "webserv.h"
# include "Webserv.hpp"

class	Config {

	public:

		Config( void ) ;
		Config( string path ) ;
		~Config( void ) ;

	private:

		string	_path ;

		Config( Config const & cpy ) ;
		Config &	operator=( Config const & rhs ) ;
			
} ;

/* *** CONFIG.HPP *** */
