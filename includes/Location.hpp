/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:41:57 by mpeulet           #+#    #+#             */
/*   Updated: 2024/05/03 13:24:46 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.h"

class	Location {

	public:

		Location( string const & locBloc, int index, long long servMbs ) ;
		~Location( void ) ;
		Location( Location const & cpy ) ;
		Location &	operator=( Location const & rhs ) ;

		void	setLocPath( string const & lp ) ;
		void	setLocIndex( int index ) ;
		void	setRoot( string const & root ) ;
		void	setMaxBodySize( int maxBodySize ) ;
		void	setAllowedMethod( vector<enum HttpMethod> const & allowedMethod ) ;
		void	setDirectoryListing( bool directoryListing ) ;
		void	setIndex( string const & index ) ;
		void	setErrorPages( map<int,string> const & err ) ;
		void	setReturnURI( map<int,string> const & uri ) ;
		void	setUploadPath( string const & path ) ;

		string const &					getLocPath( void ) const ;
		int								getLocIndex( void ) const ;

		string const &					getRoot( void ) const ;
		int								getMaxBodySize( void ) const ;
		vector<enum HttpMethod> const &	getAllowedMethod( void ) const ;
		bool							getDirectoryListing( void ) const ;
		string const &					getIndex( void ) const ;
		map<int,string> const &			getErrorPages( void ) const ;
		map<int,string> const &			getReturnURI( void ) const ;
		string const &					getUploadPath( void ) const ;

	private:

		int						_locIndex ;
		string					_locPath ;

		string					_root ;
		long long				_maxBodySize ;
		vector<enum HttpMethod>	_allowedMethod ;
		bool					_directoryListing ;
		string					_index ;
		map<int,string>			_errorPage ;
		map<int,string>			_returnURI ;
		string					_uploadPath ;


		void	extractLocPath( string & locBloc ) ;
		string	extractStringVariable( string & tmp, string const & var ) ;
		void	checkMBS( string & loc, long long servMBS ) ;
		void	checkAutoIndex( string & loc ) ;
		void	checkMethod( string & loc ) ;
		void	extractMap( string & loc, string const & key, map<int,string> & map ) ;

		Location( void ) ;
			
} ;

ostream &	operator<<( ostream & o, Location const & rhs ) ;

/* *** LOCATION.HPP *** */

#endif
