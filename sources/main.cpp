/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:20:01 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/30 16:46:49 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "Webserv.hpp"
#include "ConfigServer.hpp"
#include "Location.hpp"

void	handler(int signum) {
	static_cast<void>(signum);
	cout << DEL_LINE CYAN "exit" END_STYLE << endl;
	env()->ctrl_c = true;
}

// int	main(void) {
// 	signal(SIGINT, handler);
// 	env()->ctrl_c = false;
// 	env()->return_val = 0;
// 	env()->webserv = new Webserv("path of the config file");
// 	delete (env()->webserv);
// 	return (env()->return_val);
// }

int	main( int ac, char **av ) {

	if ( ac == 2 ) {
		try {
			string	input = av[1] ;
			Config	conf(input) ;
			size_t	nbServer = conf.getNbServer() ;
			vector<ConfigServer>	serv ;
			for ( size_t i = 0; i < nbServer; i++ ) {
				ConfigServer	servConf( conf.getServerBlocks()[i], i ) ;
				serv.push_back( servConf ) ;
			}
			checkDuplicatePorts( serv ) ;
			// for ( size_t i = 0; i < nbServer; i++ ) {
			// 	cout << serv[i] ;
			// }
			signal(SIGINT, handler);
			env()->ctrl_c = false;
			env()->return_val = 0;
			env()->webserv = new Webserv( serv );
			delete (env()->webserv);
			return (env()->return_val);
		}
		catch ( const runtime_error & e ) {
			std::cout << e.what() << std::endl ;
		}
	}
	else
		cout << EXAMPLE ;

	return 0;
}

// int	main( void ) {
// 	try {
// 		ConfigServer	cs("rotvar/www;", 1) ;
// 		// cout << cs ;
// 	}
// 	catch ( const runtime_error & e ) {
// 		std::cout << e.what() << std::endl ;
// 	}
// }
