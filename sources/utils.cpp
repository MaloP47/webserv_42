/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:13:19 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/15 16:17:03 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "style.h"
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

size_t	findInCharVec(string str, vector<char> &vec) {
	bool	finded = false;
	size_t	pos = 0;

	if (str.length() > vec.size())
		return (string::npos);
	for (charVecIt it = vec.begin(); it != vec.end() - str.length(); it++) {
		finded = true;
		for (size_t i = 0; i < str.length(); i++) {
			if (str[i] != *(it + i)) {
				finded = false;
				break;
			}
		}
		if (finded)
			return (pos);
		pos++;
	}
	return (string::npos);
}

string	strFromCharVec(size_t len, vector<char> &vec) {
	string	str = "";
	for (size_t i = 0; i < vec.size() && i < len; i++)
		str += vec[i];
	return (str);
}

string	timeStamp() {
	char				date[128];
	time_t				rawtime;
	struct tm			*info;
	size_t				length;

	time(&rawtime);
	info = gmtime(&rawtime);
	length = strftime(date, 128, "[%Y-%m-%d  %H:%M:%S]", info);
	date[length] = 0;
	return (THIN + string(date) + END_STYLE);
}
