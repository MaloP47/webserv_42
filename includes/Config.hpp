/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:10:50 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/18 11:08:29 by mpeulet          ###   ########.fr       */
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

		int						getNbServer( void ) const ;
		string const &			getRawConfig( void ) const ;
		vector<string> const & 	getServerBlocks( void ) const ;

	private:

		int				_nbServer ;
		string			_rawConfig ;
		vector<string>	_serverBlocks ;

		bool	isDirectory( string const & path ) ;
		bool	isConf( string const & path ) ;
		void	cleanRawConfig( ifstream & config ) ;
		void	parseRawConfig( void ) ;

		Config( void ) ;
		Config( Config const & cpy ) ;
		Config &	operator=( Config const & rhs ) ;
			
} ;

/* *** CONFIG.HPP *** */
