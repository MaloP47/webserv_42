/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:37:18 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/14 18:22:30 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTORYLISTING_HPP
# define DIRECTORYLISTING_HPP

# include "webserv.h"

typedef map<string, struct dirent*>		dirMap;
typedef dirMap::iterator				dirMapIt;
typedef pair<string, struct dirent*>	dirPair;

class DirectoryListing {
	private:
		DirectoryListing();
		DirectoryListing(const DirectoryListing &cpy);
		~DirectoryListing();

		DirectoryListing	&operator=(const DirectoryListing &rhs);
	public:
		static string	html(string path, string root, string requestPath);
};

#endif
