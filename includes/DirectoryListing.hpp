/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:37:18 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/12 14:51:39 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTORYLISTING_HPP
# define DIRECTORYLISTING_HPP

# include "webserv.h"

class DirectoryListing {
	private:
		DirectoryListing();
		DirectoryListing(const DirectoryListing &cpy);
		~DirectoryListing();

		DirectoryListing	&operator=(const DirectoryListing &rhs);
	public:
		static std::string	html(std::string path);
};

#endif
