/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:15:28 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/11 15:21:50 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "style.h"
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <sys/epoll.h>
# include <vector>
# include <algorithm>
# include <cstring>
# include <unistd.h>
# include <map>
# include <limits>
# include <fstream>
# include <sstream>
# include <cctype>
# include <ctime>
# include <cstdlib>
# include <csignal>
# include "Mime.hpp"
# include "StatusCode.hpp"

enum HttpMethod {
	GET,
	POST,
	DELETE,
	OTHER
};

# include "Webserv.hpp"

typedef struct s_env {
	Webserv	*webserv;
	bool	ctrl_c;
	int		return_val;
}	t_env;

# define MAX_EVENTS 1024
# define BUFFER_SIZE 128

# define CLIENT_TIMEOUT 10
# define EPOLL_TIMEOUT 5000

# define SUCCESS 0

# define ERR_SOCKET 1
# define ERR_SOCK_OPT 2
# define ERR_BIND 3
# define ERR_FCNTL 4
# define ERR_LISTEN 5
# define ERR_ACCEPT 6
# define ERR_CLOSE 7

# define ERR_EPOLL_CREATE 101
# define ERR_EPOLL_CTL 102
# define ERR_EPOLL_WAIT 103
# define ERR_SEND 104

std::vector<std::string>	split_trim(std::string str, std::string needle);

std::string					decodeUri(const std::string uri);

t_env						*env();

bool						findLower(std::string str, std::string needle);

void						ltrim(std::string &str);
void						rtrim(std::string &str);

int							ret(int return_val);

#endif
