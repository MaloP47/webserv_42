/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 16:47:57 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/08 17:22:27 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mime.hpp"

Mime::Mime() {}

Mime::Mime(const Mime &cpy) {
	*this = cpy;
}

Mime::~Mime() {}

Mime &Mime::operator=(const Mime &rhs) {
	(void)rhs;
	return (*this);
}

std::string	Mime::ext(const std::string str) {
	if (str == "aac")
		return ("audio/aac");
	else if (str == "abw")
		return ("application/x-abiword");
	else if (str == "apng")
		return ("image/apng");
	else if (str == "arc")
		return ("application/x-freearc");
	else if (str == "avif")
		return ("image/avif");
	else if (str == "avi")
		return ("video/x-msvideo");
	else if (str == "azw")
		return ("application/vnd.amazon.ebook");
	else if (str == "bin")
		return ("application/octet-stream");
	else if (str == "bmp")
		return ("image/bmp");
	else if (str == "bz")
		return ("application/x-bzip");
	else if (str == "bz2")
		return ("application/x-bzip2");
	else if (str == "cda")
		return ("application/x-cdf");
	else if (str == "csh")
		return ("application/x-csh");
	else if (str == "css")
		return ("text/css");
	else if (str == "csv")
		return ("text/csv");
	else if (str == "doc")
		return ("application/msword");
	else if (str == "docx")
		return ("application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	else if (str == "eot")
		return ("application/vnd.ms-fontobject");
	else if (str == "epub")
		return ("application/epub+zip");
	else if (str == "gz")
		return ("application/gzip");
	else if (str == "gif")
		return ("image/gif");
	else if (str == "htm")
		return ("text/html");
	else if (str == "html")
		return ("text/html");
	else if (str == "ico")
		return ("image/vnd.microsoft.icon");
	else if (str == "ics")
		return ("text/calendar");
	else if (str == "jar")
		return ("application/java-archive");
	else if (str == "jpeg")
		return ("image/jpeg");
	else if (str == "jpg")
		return ("image/jpeg");
	else if (str == "js")
		return ("text/javascript");
	else if (str == "json")
		return ("application/json");
	else if (str == "jsonld")
		return ("application/ld+json");
	else if (str == "mid")
		return ("audio/midi,audi/x-midi");
	else if (str == "midi")
		return ("audio/midi,audi/x-midi");
	else if (str == "mjs")
		return ("text/javascript");
	else if (str == "mp3")
		return ("audio/mpeg");
	else if (str == "mp4")
		return ("video/mp4");
	else if (str == "mpeg")
		return ("video/mpeg");
	else if (str == "mpkg")
		return ("application/vnd.apple.installer+xml");
	else if (str == "odp")
		return ("application/vnd.oasis.opendocument.presentation");
	else if (str == "ods")
		return ("application/vnd.oasis.opendocument.spreadsheet");
	else if (str == "odt")
		return ("application/vnd.oasis.opendocument.text");
	else if (str == "oga")
		return ("audio/ogg");
	else if (str == "ogv")
		return ("video/ogg");
	else if (str == "ogx")
		return ("application/ogg");
	else if (str == "opus")
		return ("audio/opus");
	else if (str == "otf")
		return ("font/otf");
	else if (str == "png")
		return ("image/png");
	else if (str == "pdf")
		return ("application/pdf");
	else if (str == "php")
		return ("application/x-httpd-php");
	else if (str == "ppt")
		return ("application/vnd.ms-powerpoint");
	else if (str == "pptx")
		return ("application/vnd.openxmlformats-officedocument.presentationml.presentation");
	else if (str == "rar")
		return ("application/vnd.rar");
	else if (str == "rtf")
		return ("application/rtf");
	else if (str == "sh")
		return ("application/x-sh");
	else if (str == "svg")
		return ("image/svg+xml");
	else if (str == "tar")
		return ("application/x-tar");
	else if (str == "tif")
		return ("image/tiff");
	else if (str == "tiff")
		return ("image/tiff");
	else if (str == "ts")
		return ("video/mp2t");
	else if (str == "ttf")
		return ("font/ttf");
	else if (str == "txt")
		return ("text/plain");
	else if (str == "vsd")
		return ("application/vnd.visio");
	else if (str == "wav")
		return ("audio/wav");
	else if (str == "weba")
		return ("audio/webm");
	else if (str == "webm")
		return ("audio/webm");
	else if (str == "webp")
		return ("image/webp");
	else if (str == "woff")
		return ("font/woff");
	else if (str == "xhtml")
		return ("application/xhtml+xml");
	else if (str == "xls")
		return ("application/vnd.ms-excel");
	else if (str == "xlsx")
		return ("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
	else if (str == "xml")
		return ("application/xml");
	else if (str == "xul")
		return ("application/vnd.mozilla.xul+xml");
	else if (str == "zip")
		return ("application/zip");
	else if (str == "3gp")
		return ("video/3gpp");
	else if (str == "3g2")
		return ("video/3gpp2");
	else if (str == "7z")
		return ("application/x-7z-compressed");
	return ("");
}
