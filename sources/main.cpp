/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:20:01 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/06 14:50:54 by mpeulet          ###   ########.fr       */
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

int exec_webserv(string input) {
	try {
		Config	conf(input) ;
		size_t	nbServer = conf.getNbServer() ;
		vector<ConfigServer>	serv ;
		for ( size_t i = 0; i < nbServer; i++ ) {
			ConfigServer	servConf( conf.getServerBlocks()[i], i ) ;
			serv.push_back( servConf ) ;
		}
		signal(SIGINT, handler);
		env()->ctrl_c = false;
		env()->return_val = 0;
		env()->webserv = new Webserv( serv );
		delete (env()->webserv);
		return (env()->return_val);
	}
	catch ( const runtime_error & e ) {
		std::cout << e.what() << std::endl ;
		return (-1);
	}
	return 0;
}

int	main( int ac, char **av ) {
	signal(SIGPIPE, SIG_IGN);
	if ( ac == 2 ) {
		exec_webserv(av[1]);
	}
	else if (ac == 1) {
		exec_webserv("./config/_default.conf");
	}
	else {
		cout << EXAMPLE ;
	}
	return (env()->return_val);
}
