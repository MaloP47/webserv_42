/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:10:50 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/15 16:57:27 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "webserv.h"
# include "Webserv.hpp"

# define EXAMPLE "Please run webserv with a config file :\n\033[0;32m./webserv [config file]\n\033[0m"

class	Config {

	public:

		Config( string const & path ) ;
		~Config( void ) ;

	private:

		int		_nbServer ;

		Config( void ) ;
		Config( Config const & cpy ) ;
		Config &	operator=( Config const & rhs ) ;
			
} ;

/* *** CONFIG.HPP *** */
