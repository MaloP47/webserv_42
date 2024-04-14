/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:40:06 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/14 18:37:01 by gbrunet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

DirectoryListing::DirectoryListing() {}

DirectoryListing::DirectoryListing(const DirectoryListing &cpy) {
	*this = cpy;
}

DirectoryListing::~DirectoryListing() {}

DirectoryListing	&DirectoryListing::operator=(const DirectoryListing &rhs) {
	static_cast<void>(rhs);
	return (*this);
}

static string	fileSize(off_t size) {
	stringstream	str;
	int				roundedSize;

	if (size > 1073741824) {
		roundedSize = size / 1073741824;
		str << roundedSize << " Gb";
	} else if (size > 1048576) {
		roundedSize = size / 1048576;
		str << roundedSize << " Mb";
	} else if (size > 1024) {
		roundedSize = size / 1024;
		str << roundedSize << " kb";
	} else {
		str << size << " b&nbsp;";
	}
	return (str.str())	;
}

static string	htmlParentDir() {
	string	html = "<svg xmlns=\"http://www.w3.org/2000/svg\"\
					width=\"30\" height=\"16\" fill=\"#EBAD3A\" class=\
					\"bi bi-folder-fill\" viewBox=\"0 0 16 16\"><path d=\
					\"M9.828 3h3.982a2 2 0 0 1 1.992 2.181l-.637 7A2 2 0 0 \
					1 13.174 14H2.825a2 2 0 0 1-1.991-1.819l-.637-7a2 2 0 0 \
					1 .342-1.31L.5 3a2 2 0 0 1 2-2h3.672a2 2 0 0 1 1.414.586\
					l.828.828A2 2 0 0 0 9.828 3m-8.322.12q.322-.119.684-.12\
					h5.396l-.707-.707A1 1 0 0 0 6.172 2H2.5a1 1 0 0 0-1 .981\
					z\"/></svg><a style=\"text-decoration:none; color:#8585f2\" \
					href=\"..\"> Parent directory</a>";
	return (html);
}

static string	htmlDir(struct dirent *dp, string path, string relativePath) {
	char		date[128];
	struct tm	*info;
	size_t		length;
	struct stat	result;

	
	if (stat((path + dp->d_name).c_str(), &result) == 0) {
		info = localtime(&result.st_ctime);
		length = strftime(date, 128, "%F %r", info);
		date[length] = 0;
	}
	string	html = "<tr><th><svg xmlns=\"http://www.w3.org/2000/svg\"\
					width=\"30\" height=\"16\" fill=\"#EBAD3A\" class=\
					\"bi bi-folder-fill\" viewBox=\"0 0 16 16\"><path d=\
					\"M9.828 3h3.982a2 2 0 0 1 1.992 2.181l-.637 7A2 2 0 0 \
					1 13.174 14H2.825a2 2 0 0 1-1.991-1.819l-.637-7a2 2 0 0 \
					1 .342-1.31L.5 3a2 2 0 0 1 2-2h3.672a2 2 0 0 1 1.414.586\
					l.828.828A2 2 0 0 0 9.828 3m-8.322.12q.322-.119.684-.12\
					h5.396l-.707-.707A1 1 0 0 0 6.172 2H2.5a1 1 0 0 0-1 .981\
					z\"/></svg><a style=\"text-decoration:none; color:#8585f2\" \
					href=\"" + relativePath + dp->d_name + "\"> ";
	return (html + dp->d_name + "</a></th><td></td><td style=\"text-align:right\">"
			+ date + "</td></tr>");
}

static string	htmlFile(struct dirent *dp, string path, string relativePath) {	
	char		date[128];
	struct tm	*info;
	size_t		length;
	struct stat	result;
	string		size;

	
	if (stat((path + dp->d_name).c_str(), &result) == 0) {
		info = localtime(&result.st_ctime);
		length = strftime(date, 128, "%F %r", info);
		date[length] = 0;
		size = fileSize(result.st_size);
	}
	string	html = "<tr><th><svg xmlns=\"http://www.w3.org/2000/svg\" \
					width=\"30\" height=\"14\" fill=\"currentColor\" class=\
					\"bi bi-file-earmark-code-fill\" viewBox=\"0 0 16 16\">\
					<path d=\"M9.293 0H4a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h8a2 \
					2 0 0 0 2-2V4.707A1 1 0 0 0 13.707 4L10 .293A1 1 0 0 0 \
					9.293 0M9.5 3.5v-2l3 3h-2a1 1 0 0 1-1-1M6.646 7.646a.5.5 \
					0 1 1 .708.708L5.707 10l1.647 1.646a.5.5 0 0 1-.708.708\
					l-2-2a.5.5 0 0 1 0-.708zm2.708 0 2 2a.5.5 0 0 1 0 .708l-2 \
					2a.5.5 0 0 1-.708-.708L10.293 10 8.646 8.354a.5.5 0 1 1 \
					.708-.708\"/></svg><a style=\"text-decoration:none;\
					color:#8585f2\" href=\"" + relativePath + dp->d_name + "\"> ";
	return (html + dp->d_name
			+ "</a></th><td style=\"text-align:right;padding-right:10px;"
			+ "font-family: \'Roboto Mono\', monospace;\">"
			+ size + "</td><td style=\"text-align:right\">" + date
			+ "</td></tr>");
}

string	DirectoryListing::html(string path, string root) {
	DIR				*dir;
	struct dirent	*dp;
	string			html;
	dirMap			folder;
	dirMap			file;
	string			relativePath;

	relativePath = path;
	relativePath.erase(0, root.length());

	html = "<!doctype html><html><head><title>Index of " + relativePath + "=</title>\
			<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/\
			bootstrap@5.3.3/dist/css/bootstrap.min.css\"><link rel=\"preconnect\"\
			href=\"https://fonts.googleapis.com\"><link rel=\"preconnect\"\
			href=\"https://fonts.gstatic.com\" crossorigin><link href=\
			\"https://fonts.googleapis.com/css2?family=Roboto+Mono:ital,\
			wght@0,100..700;1,100..700&display=swap\" rel=\"stylesheet\"></head>\
			<body class=\"m-3 mt-5\" style=\"background-color:#353535\"><div \
			class=\"container text-white p-3\"><div class=\"rounded-3 p-5\" \
			style=\"background-color:#2b2a33; border:solid 1px #1b1b1b\"><h4>\
			Index of " + relativePath + "</h4><hr />";
	if (relativePath != "/")
		html += htmlParentDir();
	html += "<div class=\"p-3\"><table \
			style=\"width:100%\"><thead><tr style=\"font-size:1.2em\"><th>Name\
			</th><th style=\"text-align:right;padding-right:10px\">Size</th>\
			<th style=\"text-align:right\">Last modified</th></tr></thead><tbody\
			style=\"font-size:0.9em;\">";
	if ((dir = opendir(path.c_str())) == NULL) {
		perror("Cannot open directory");
		return (NULL);
	}

	while ((dp = readdir(dir)) != NULL) {
		if (string(dp->d_name) == "." || string(dp->d_name) == "..")
			continue ;
		if (dp->d_type == DT_DIR)
			folder.insert(dirPair(dp->d_name, dp));
		else
			file.insert(dirPair(dp->d_name, dp));
	}
	for (dirMapIt it = folder.begin(); it != folder.end(); it++)
		html += htmlDir(it->second, path, relativePath);
	for (dirMapIt it = file.begin(); it != file.end(); it++)
		html += htmlFile(it->second, path, relativePath);

	html += "</tbody></table></div></div></div></body></html>";
	return (html);
}
