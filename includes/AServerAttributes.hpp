/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerAttributes.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:25:14 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/18 18:37:10 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <map>
# include <vector>
using namespace std;

class	AServerAttributes {

	public:

		AServerAttributes( void ) ;
		virtual ~AServerAttributes( void ) ;

		virtual void			abstraction( void ) const = 0 ;

		void					setErrorPages( map<int,string> const & err ) ;
		void					setReturnURL( string const & url ) ;
		void					setUploadPath( string const & path ) ;

		map<int,string> const &	getErrorPages( void ) const ;
		string const &			getReturnURL( void ) const ;
		string const &			getUploadPath( void ) const ;

	protected:

		map<int,string>			_errorPages ;
		string					_returnURL ;
		string					_uploadPath ;

} ;

// #endif

/* *** ASERVERATTRIBUTES.HPP *** */
