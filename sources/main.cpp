/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:20:01 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/28 12:52:22 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "Webserv.hpp"
#include "ConfigServer.hpp"

// void	handler(int signum) {
// 	static_cast<void>(signum);
// 	cout << DEL_LINE CYAN "exit" END_STYLE << endl;
// 	env()->ctrl_c = true;
// }

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
			for ( int i = 0; i < conf.getNbServer(); i++ )
				cout << conf.getServerBlocks()[i] << "\n" << endl ;
			ConfigServer	cs( conf.getServerBlocks()[0], 0 ) ;
			cout << cs ;
			cout << cs.getLocation()[0] << endl ;
			cout << cs.getLocation()[1] << endl ;
			cout << cs.getLocation()[2] << endl ;
			cout << cs.getLocation()[3] << endl ;		
			cout << cs.getLocation()[4] << endl ;
			// (void)av ;
			// Location	loc( "/test1{rootvar/www/test1;indextest1.html;allowedMethodsGETDELETE;}", 1 ) ;
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
