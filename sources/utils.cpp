/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:13:19 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/14 17:58:26 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

void	ltrim(string &str) {
	int	i = 0;

	if (str.length() == 0)
		return ;
	while (isspace(str[i]))
		i++;
	str = str.substr(i, str.length());
}

void	rtrim(string &str) {
	int	i = str.length() - 1;

	if (str.length() == 0)
		return ;
	while (isspace(str[i]))
		i--;
	str = str.substr(0, i + 1);
}

vector<string> split_trim(string str, string needle) {
	vector<string>	split;
	string			subs;
	size_t			end;

	end = str.find(needle);
	while (end != string::npos) {
		subs = str.substr(0, end);
		ltrim(subs);
		rtrim(subs);
		split.push_back(subs);
		str.erase(str.begin(), str.begin() + end + needle.length());
		end = str.find(needle);
	}
	ltrim(str);
	rtrim(str);
	split.push_back(str);
	return (split);
}

static bool	compareCharLower(char a, char b) {
	return (tolower(a) == tolower(b));
}

bool	findLower(string str, string needle) {
	string::iterator it = search(str.begin(), str.end(),
			needle.begin(), needle.end(), compareCharLower);
	return (it != str.end());
}

string	decodeUri(const string uri) {
	string	decoded;

	for (size_t i = 0; i < uri.length(); i++) {
		if (uri[i] == '%' && i + 2 < uri.length()) {
			int hex = strtol(uri.substr(i + 1, 2).c_str(), NULL, 16);
			decoded += static_cast<char>(hex);
			i += 2;
		} else if (uri[i] == '+'){
			decoded += ' ';
		} else {
			decoded += uri[i];
		}
	}
	return (decoded);
}

string	decodeEnv(const string uri) {
	string	decoded;

	for (size_t i = 0; i < uri.length(); i++) {
		if (uri[i] == '+'){
			decoded += ' ';
		} else {
			decoded += uri[i];
		}
	}
	return (decodeUri(decoded));
}

t_env *env() {
	static t_env e;

	return (&e);
}

int	ret(int return_val) {
	env()->return_val = return_val;
	env()->ctrl_c = true;
	return (return_val);
}
