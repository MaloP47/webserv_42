/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 11:21:53 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/19 15:26:46 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Config.hpp"
# include "webserv.h"

/* *** constructors *** */
Config::Config(string const & path): _nbServer(0){
	ifstream	config(path.c_str());

	if (path.empty())
		throw runtime_error("Invalid config as parameter: NULL.");
	if (isDirectory(path))
		throw runtime_error("Parameter is a directory.");
	if (!isConf(path))
		throw runtime_error("Invalid config as parameter: must be .conf extension file.");
	if (access(path.c_str(), R_OK))
		throw runtime_error("File is not readable.");
	if (!config || !config.is_open())
		throw runtime_error("Invalid config as parameter: " + path);
	cleanRawConfig(config);
	if (_rawConfig.find('#') != string::npos)
		throw runtime_error("Invalid character found in config file: #.");
	parseRawConfig();
}

/* *** destructor *** */
Config::~Config(void) {}

/* *** operator = *** */
Config	&Config::operator=(const Config &rhs){
	(void) rhs;
	return (*this);
}

/* *** copy constructor *** */
Config::Config(const Config &cpy){
	*this = cpy;
}

/* *** public functions *** */
int	Config::getNbServer(void) const{
	return _nbServer;
}

string const	&Config::getRawConfig(void) const{
	return _rawConfig;
}

vector<string> const	&Config::getServerBlocks(void) const{
	return _serverBlocks;
}

/* *** private functions *** */
bool	Config::isDirectory(string const & path){
	struct stat	statbuf;
	if (!stat(path.c_str(), &statbuf))
		return S_ISDIR(statbuf.st_mode);
	return (false);
}

bool	Config::isConf(string const & path){
	string::size_type pos = path.rfind(".conf");
	return pos != string::npos && pos == path.length() - 5;
}

void	Config::cleanRawConfig(ifstream & config){
	string	word;

	while (config >> word)
		_rawConfig += word;
}

void	Config::parseRawConfig(void){
	if (_rawConfig.find("server{") != 0)
		throw runtime_error("File is not starting with a server block.");
	_serverBlocks = split_trim_conf(_rawConfig, "server{");
	for (size_t i = 0; i < _serverBlocks.size(); i++){
		if (_serverBlocks[i][_serverBlocks[i].size() - 1] != '}'){
			std::ostringstream oss;
            oss << "Server block does not close properly.\nBlock: " << i;
			throw runtime_error(oss.str());
		}
	}
	_nbServer = _serverBlocks.size();
}
