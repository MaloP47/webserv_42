/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:10:50 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/23 11:02:17 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "webserv.h"
# include "Webserv.hpp"

# define EXAMPLE "Please run webserv with a config file :\n\033[0;32m./webserv [config file]\n\033[0m"

class	Config {

	public:

		Config(string const & path);
		~Config(void);

		int						getNbServer(void) const;
		string const &			getRawConfig(void) const;
		vector<string> const & 	getServerBlocks(void) const;

	private:

		int						_nbServer;
		string					_rawConfig;
		vector<string>			_serverBlocks;

		bool	isConf( string const & path );
		bool	isDirectory( string const & path );
		void	parseRawConfig( void );
		void	cleanRawConfig( ifstream & config );

		Config(void);
		Config(Config const & cpy);
		Config	&operator=(Config const & rhs);
			
};
