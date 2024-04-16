/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:20:01 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/16 11:11:37 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "Webserv.hpp"

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

	if ( ac == 2) {
		try {
			string	input = av[1] ;
			Config	conf(input) ; 
		}
		catch ( const runtime_error & e ) {
			std::cout << e.what() << std::endl ;
		}
	}
	else
		cout << EXAMPLE ;

	return 0;
}
