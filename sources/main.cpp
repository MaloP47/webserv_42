/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:20:01 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/13 09:53:56 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"
#include "Webserv.hpp"
#include <map>

void	handler(int signum) {
	static_cast<void>(signum);
	std::cout << DEL_LINE CYAN "exit" END_STYLE << std::endl;
	env()->ctrl_c = true;
}

int	main(void) {
	std::signal(SIGINT, handler);
	env()->ctrl_c = false;
	env()->return_val = 0;
	env()->webserv = new Webserv("path of the config file");
	delete (env()->webserv);
	return (env()->return_val);
}
