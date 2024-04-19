/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:47:47 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/19 15:16:23 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <map>
# include <vector>
# include "webserv.h"

using namespace std;

class	Location {

	public:

		Location( string const & locationBlock ) ;
		~Location( void ) ;

		void	setDirectoryListing( bool onOff ) ;
		void	setRoot( string const & root ) ;
		void	setIndexes( vector<string> const & indexes ) ;
		void	setMaxBodySize( int	MBS ) ;
		void	setAlloweMethod( vector<enum HttpMethod> const & AM ) ;
		void	setErrorPages( map<int,string> const & EP ) ;
		void	setReturnURI( string const & uri ) ;
		void	setUploadPath( string const & path ) ;
		void	setLocPath( string const & locpath ) ;

		bool const &					getDirectoryListing( void ) const ;
		string const &					getRoot( void ) const ;
		vector<string> const &			getIndexes( void ) const ;
		int const &						getMaxBodySize( void ) const ;
		vector<enum HttpMethod> const &	getAlloweMethod( void ) const ;
		map<int,string> const &			getErrorPages( void ) const ;
		string const &					getReturnURI( void ) const ;
		string const &					getUploadPath( void ) const ;
		string const &					getLocPath( void ) const ;

	private:

		bool					_directoryListing;
		string					_root;
		vector<string>			_indexes;
		int						_maxBodySize;
		vector<enum HttpMethod>	_allowedMethod;
		map<int,string>			_errorPages ;
		string					_returnURI ;
		string					_uploadPath ;
		string					_locPath ;

		Location( void ) ;
		Location( Location const & cpy ) ;
		Location &	operator=( Location const & rhs ) ;

} ;

/* *** LOCATION.HPP *** */
