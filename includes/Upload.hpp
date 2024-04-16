/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 16:54:23 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/15 17:12:05 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HPP
# define UPLOAD_HPP

# include "webserv.h"

class Upload {
	private:
		string			_filename;
		vector<char>	_rawData;

		Upload();

	public:
		Upload(string filename, vector<char> &data);
		Upload(const Upload &cpy);
		~Upload();

		Upload	&operator=(const Upload &rhs);

		void	createFile();
};

#endif
