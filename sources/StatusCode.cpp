/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:44:41 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/11 15:21:24 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

StatusCode::StatusCode(){}

StatusCode::StatusCode(const StatusCode &cpy) {
	*this = cpy;
}

StatusCode::~StatusCode() {}

StatusCode	&StatusCode::operator=(const StatusCode &rhs) {
	(void)rhs;
	return (*this);
}

std::string	StatusCode::name(int err) {
	if (err == 100)
		return ("100 Continue");
	else if (err == 101)
		return ("101 Switching Protocols");
	else if (err == 200)
		return ("200 OK");
	else if (err == 201)
		return ("201 Created");
	else if (err == 202)
		return ("202 Accepted");
	else if (err == 203)
		return ("203 Non-Authoritative Information");
	else if (err == 204)
		return ("204 No Content");
	else if (err == 205)
		return ("205 Reset Content");
	else if (err == 206)
		return ("206 Partial Content");
	else if (err == 300)
		return ("300 Multiple Choices");
	else if (err == 301)
		return ("301 Moved Permanently");
	else if (err == 302)
		return ("302 Found");
	else if (err == 303)
		return ("303 See Other");
	else if (err == 304)
		return ("304 Not Modified");
	else if (err == 305)
		return ("305 Use Proxy");
	else if (err == 306)
		return ("306 (Unused)");
	else if (err == 307)
		return ("307 Temporary Redirect");
	else if (err == 400)
		return ("400 Bad Request");
	else if (err == 401)
		return ("401 Unauthorized");
	else if (err == 402)
		return ("402 Payment Required");
	else if (err == 403)
		return ("403 Forbidden");
	else if (err == 404)
		return ("404 Not Found");
	else if (err == 405)
		return ("405 Method Not Allowed");
	else if (err == 406)
		return ("406 Not Acceptable");
	else if (err == 407)
		return ("407 Proxy Authentification Required");
	else if (err == 408)
		return ("408 Request Timeout");
	else if (err == 409)
		return ("409 Conflict");
	else if (err == 410)
		return ("410 Gone");
	else if (err == 411)
		return ("411 Length Required");
	else if (err == 412)
		return ("412 Precondition Failed");
	else if (err == 413)
		return ("413 Request Entity Too Large");
	else if (err == 414)
		return ("414 Request-URI Too Long");
	else if (err == 415)
		return ("415 Unsupported Media Type");
	else if (err == 416)
		return ("416 Requested Range Not Satisfiable");
	else if (err == 417)
		return ("417 Expectation Failed");
	else if (err == 500)
		return ("500 Internal Server Error");
	else if (err == 501)
		return ("501 Not Implemented");
	else if (err == 502)
		return ("502 Bad Gateway");
	else if (err == 503)
		return ("503 Service Unavailable");
	else if (err == 504)
		return ("504 Gateway Timeout");
	else if (err == 505)
		return ("505 HTTP Version Not Supported");
	return ("");
}

std::string	StatusCode::page(int num) {
	std::string	html;

	html = "<!doctype html><html><head><title>" + StatusCode::name(num).erase(0, 4) + "</title>\
			<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/\
			bootstrap.min.css\" rel=\"stylesheet\"></head><body class=\"m-3\">\
			<div class=\"container p-4\"><div class=\"d-flex\
			align-items-center justify-content-center\"><div><div class=\"text-center\
			p-4 bg-dark text-body rounded-top-4\" style=\"--bs-bg-opacity: .2;\">\
			<h1 class=\"fw-bold\">" + StatusCode::name(num) + "</h1></div><div class=\"text-center\
			pt-3 pb-1 bg-dark text-body rounded-bottom-4\" style=\"--bs-bg-opacity: .1\">\
			<p><i>webserv</i></p></div></div></div></div></body></html>";
	return (html);
}
