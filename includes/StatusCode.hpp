/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:44:58 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/11 15:21:32 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUSCODE_HPP
# define STATUSCODE_HPP

class StatusCode {
	private:
		StatusCode();
		StatusCode(const StatusCode &cpy);
		~StatusCode();

		StatusCode	&operator=(const StatusCode &rhs);
	
	public:
		static std::string	page(int num);
		static std::string	name(int num);
};

#endif
