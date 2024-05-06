/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 15:15:28 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/06 10:50:56 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "style.h"
# include <iostream>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/socket.h>
# include <sys/stat.h>
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
# include <dirent.h>
# include <exception>
# include <set>
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

// # include "Location.hpp"
# include "Webserv.hpp"
# include "Config.hpp"
# include "ConfigServer.hpp"

typedef struct s_env {
	Webserv	*webserv;
	bool	ctrl_c;
	int		return_val;
}	t_env;

# define MAX_EVENTS 1024
# define BUFFER_SIZE 8096

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
# define ERR_READ 105

# define MAXCLIENTBS 1024

int				ret(int return_val);
bool			findLower(string str, string needle);
bool			childPath(string parent, string child);
void			ltrim(string &str);
void			rtrim(string &str);
t_env			*env();
string			timeStamp();
string			getFullPath(string path);
string			decodeUri(const string uri);
string			decodeEnv(const string uri);
string			strFromCharVec(size_t len, vector<char> &vec);
size_t			findInCharVec(string str, vector<char> &vec);
vector<string>	split_trim(string str, string needle);
vector<string>	split_trim_conf(string str, string needle);
bool			isFile( const string & path );
bool			areAllPathsBinaries( const vector<string> & paths );
bool			isAllDigits( string const & str ) ;

#endif
