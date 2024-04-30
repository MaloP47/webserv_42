/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerAttributes.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeulet <mpeulet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 17:53:30 by mpeulet           #+#    #+#             */
/*   Updated: 2024/04/30 14:08:20 by mpeulet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AServerAttributes.hpp"

AServerAttributes::AServerAttributes( void ) {}
AServerAttributes::~AServerAttributes( void ) {}

void	AServerAttributes::setErrorPages( map<int,string> const & err ) { _errorPages = err ; }
void	AServerAttributes::setReturnURI( map<int,string> const & url ) { _returnURI = url ; }
void	AServerAttributes::setUploadPath( string const & path ) { _uploadPath = path ; }

map<int,string> const &	AServerAttributes::getReturnURI( void ) const { return _returnURI ; }
string const &			AServerAttributes::getUploadPath( void ) const { return _uploadPath ; }
