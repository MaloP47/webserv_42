/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:41:57 by mpeulet           #+#    #+#             */
/*   Updated: 2024/05/06 11:18:32 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.h"

class	Location {

	public:

		Location(string const & locBloc, int index, long long servMbs);
		~Location(void);
		Location(Location const & cpy);
		Location &	operator=(Location const & rhs);

		void	setLocIndex(int index);
		void	setRoot(string const & root);
		void	setLocPath(string const & lp);
		void	setIndex(string const & index);
		void	setMaxBodySize(int maxBodySize);
		void	setUploadPath(string const & path);
		void	setReturnURI(map<int,string> const & uri);
		void	setCGIBin(vector<string> const & binPath);
		void	setDirectoryListing(bool directoryListing);
		void	setErrorPages(map<int,string> const & err);
		void	setCGIExtension(vector<string> const & cgiExtension);
		void	setAllowedMethod(vector<enum HttpMethod> const & allowedMethod);

		int								getLocIndex(void) const;
		int								getMaxBodySize(void) const;
		bool							getDirectoryListing(void) const;
		string const &					getRoot(void) const;
		string const &					getIndex(void) const;
		string const &					getLocPath(void) const;
		string const &					getUploadPath(void) const;
		map<int,string> const &			getReturnURI(void) const;
		map<int,string> const &			getErrorPages(void) const;
		vector<string> const &			getCGIBin(void) const;
		vector<string> const &			getCGIExtension(void) const;
		vector<enum HttpMethod> const &	getAllowedMethod(void) const;

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

		/* CGI */
		vector<string>			_binPath ;
		vector<string>			_cgiExtension ;

		void	checkMethod(string & loc);
		void	checkCGIBin(string & block);
		void	checkAutoIndex(string & loc);
		void	extractLocPath(string & locBloc);
		void	checkCGIExtension(string & block);
		void	checkMBS(string & loc, long long servMBS);
		void	extractMap(string & loc, string const & key, map<int,string> & map);
		string	extractStringVariable(string & tmp, string const & var);

		Location(void);
			
} ;

ostream &	operator<<( ostream & o, Location const & rhs ) ;
/* *** LOCATION.HPP *** */
#endif
