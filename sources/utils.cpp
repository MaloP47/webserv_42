/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:13:19 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/12 11:49:12 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

void	ltrim(std::string &str) {
	int	i = 0;

	if (str.length() == 0)
		return ;
	while (std::isspace(str[i]))
		i++;
	str = str.substr(i, str.length());
}

void	rtrim(std::string &str) {
	int	i = str.length() - 1;

	if (str.length() == 0)
		return ;
	while (std::isspace(str[i]))
		i--;
	str = str.substr(0, i + 1);
}

std::vector<std::string> split_trim(std::string str, std::string needle) {
	std::vector<std::string>	split;
	std::string					subs;
	size_t						end;

	end = str.find(needle);
	while (end != std::string::npos) {
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
	return (std::tolower(a) == std::tolower(b));
}

bool	findLower(std::string str, std::string needle) {
	std::string::iterator it = std::search(str.begin(), str.end(),
			needle.begin(), needle.end(), compareCharLower);
	return (it != str.end());
}

std::string	decodeUri(const std::string uri) {
	std::string	decoded;

	for (std::size_t i = 0; i < uri.length(); i++) {
		if (uri[i] == '%' && i + 2 < uri.length()) {
			int hex = std::strtol(uri.substr(i + 1, 2).c_str(), NULL, 16);
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

t_env *env() {
	static t_env e;

	return (&e);
}

int	ret(int return_val) {
	env()->return_val = return_val;
	env()->ctrl_c = true;
	return (return_val);
}
