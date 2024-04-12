/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 16:45:50 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/08 17:13:41 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
# define MIME_HPP

# include "webserv.h"

class Mime {
	private:
		Mime();
		Mime(const Mime &cpy);
		~Mime();

		Mime &operator=(const Mime &rhs);

	public:
		static std::string	ext(const std::string str);
};

#endif
