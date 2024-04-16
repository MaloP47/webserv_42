/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 16:57:08 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/15 17:43:43 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Upload.hpp"

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
	ofstream	file;

	file.open(this->_filename.c_str(), ios::out | ios::binary);
	if (!file) {
		perror("open");
		return ;
	}
	for (charVecIt it = this->_rawData.begin(); it != this->_rawData.end(); it++)
		file.write(&(*it), sizeof(char));
	file.close();
	if (!file.good()) {
		perror("write");
		return ;
	}
}
