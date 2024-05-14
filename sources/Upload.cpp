/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 16:57:08 by gbrunet           #+#    #+#             */
/*   Updated: 2024/05/06 15:55:11 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Upload.hpp"
#include <sstream>

Upload::Upload() {}

Upload::Upload(string filename, vector<char> &data): _filename(filename), _rawData(data) {}

Upload::Upload(const Upload &cpy) {
	*this = cpy;
}

Upload::~Upload() {}

Upload	&Upload::operator=(const Upload &rhs) {
	this->_rawData = rhs._rawData;
	this->_filename = rhs._filename;
	return (*this);
}

void	Upload::createFile() {
	ofstream		file;
	ifstream		fileExist;
	int				i = 1;
	stringstream	fullName;
	string			name;
	string			ext;
	
	if (this->_filename == "")
		return ;
	if (this->_filename.find_last_of(".") != string::npos) {
		name = this->_filename.substr(0, this->_filename.find_last_of("."));
		ext = this->_filename.substr(this->_filename.find_last_of("."));
	}
	else {
		name = this->_filename;
		ext = "";	
	}
	fullName << name << "_" << i << ext;
	fileExist.open(fullName.str().c_str());
	while (fileExist.good()) {
		fileExist.close();
		fullName.str("");
		i++;
		fullName << name << "_" << i << ext;
		fileExist.open(fullName.str().c_str());
	}
	fileExist.close();
	file.open(fullName.str().c_str(), ios::out | ios::binary);
	if (!file) {
		cerr << timeStamp() << CYAN << " Open " << THIN ITALIC;
		perror("");
		cerr << END_STYLE;
		return ;
	}
	for (charVecIt it = this->_rawData.begin(); it != this->_rawData.end(); it++)
		file.write(&(*it), sizeof(char));
	file.close();
	if (!file.good()) {
		cerr << timeStamp() << CYAN << " Write " << THIN ITALIC;
		perror("");
		cerr << END_STYLE;
		return ;
	}
}
