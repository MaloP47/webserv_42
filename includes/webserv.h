/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:15:28 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/13 11:09:27 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "style.h"
# include <set>
# include <map>
# include <ctime>
# include <cctype>
# include <cstdio>
# include <vector>
# include <limits>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <cstring>
# include <cstddef>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <csignal>
# include <unistd.h>
# include <iostream>
# include <dirent.h>
# include <exception>
# include <algorithm>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <sys/epoll.h>
# include <sys/socket.h>

using namespace std;

# include "Mime.hpp"
# include "StatusCode.hpp"
# include "DirectoryListing.hpp"

enum HttpMethod {
	GET,
	POST,
	DELETE,
	OTHER
};

# include "Webserv.hpp"
# include "Config.hpp"
# include "ConfigServer.hpp"

typedef struct s_env {
	Webserv	*webserv;
	bool	ctrl_c;
	int		return_val;
}	t_env;

# define MAX_EVENTS 1024
# define BUFFER_SIZE 4096

# define CLIENT_TIMEOUT 5
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
# define ERR_READ 105

# define MAXCLIENTBS 1024

int				ret(int return_val);
bool			isFile(const string & path);
bool			isAllDigits(string const & str);
bool			startsWithDot(const string & str);
bool			findLower(string str, string needle);
bool			childPath(string parent, string child);
bool			allStartWithDot(const vector<string> & strings);
bool			areAllPathsBinaries(const vector<string> & paths);
void			ltrim(string &str);
void			rtrim(string &str);
t_env			*env();
string			timeStamp();
string			rdmString(int nbChar);
string			getFullPath(string path);
string			decodeUri(const string uri);
string			decodeEnv(const string uri);
string			stringMethod(enum HttpMethod method);
string			strFromCharVec(size_t len, vector<char> &vec);
size_t			findInCharVec(string str, vector<char> &vec);
vector<string>	split_trim(string str, string needle);
vector<string>	split_trim_conf(string str, string needle);

#endif
