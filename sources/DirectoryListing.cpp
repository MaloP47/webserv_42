/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbrunet <gbrunet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:40:06 by gbrunet           #+#    #+#             */
/*   Updated: 2024/04/12 16:52:37 by gbrunet          ###   ########.fr       */
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

static std::string	htmlDir(struct dirent *dp) { // need to edit the href...
	std::string	html = "<tr><th><svg xmlns=\"http://www.w3.org/2000/svg\"\
						width=\"30\" height=\"16\" fill=\"currentColor\" class=\
						\"bi bi-folder-fill\" viewBox=\"0 0 16 16\"><path d=\
						\"M9.828 3h3.982a2 2 0 0 1 1.992 2.181l-.637 7A2 2 0 0 \
						1 13.174 14H2.825a2 2 0 0 1-1.991-1.819l-.637-7a2 2 0 0 \
						1 .342-1.31L.5 3a2 2 0 0 1 2-2h3.672a2 2 0 0 1 1.414.586\
						l.828.828A2 2 0 0 0 9.828 3m-8.322.12q.322-.119.684-.12\
						h5.396l-.707-.707A1 1 0 0 0 6.172 2H2.5a1 1 0 0 0-1 .981\
						z\"/></svg><a style=\"text-decoration:none; color:#8585f2\" \
						href=\"..\"> ";
	return (html + dp->d_name + "</a></th><td>time modif</td><td>size</td></tr>");
}

static std::string	htmlFile(struct dirent *dp) {	
	std::string	html = "<tr><th><svg xmlns=\"http://www.w3.org/2000/svg\" \
						width=\"30\" height=\"14\" fill=\"currentColor\" class=\
						\"bi bi-file-earmark-code-fill\" viewBox=\"0 0 16 16\">\
						<path d=\"M9.293 0H4a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h8a2 \
						2 0 0 0 2-2V4.707A1 1 0 0 0 13.707 4L10 .293A1 1 0 0 0 \
						9.293 0M9.5 3.5v-2l3 3h-2a1 1 0 0 1-1-1M6.646 7.646a.5.5 \
						0 1 1 .708.708L5.707 10l1.647 1.646a.5.5 0 0 1-.708.708\
						l-2-2a.5.5 0 0 1 0-.708zm2.708 0 2 2a.5.5 0 0 1 0 .708l-2 \
						2a.5.5 0 0 1-.708-.708L10.293 10 8.646 8.354a.5.5 0 1 1 \
						.708-.708\"/></svg><a style=\"text-decoration:none; color:#8585f2\" \
						href=\"..\"> ";
	return (html + dp->d_name + "</a></th><td>time modif</td><td>size</td></tr>");
}

std::string	DirectoryListing::html(std::string path) {
	DIR				*dir;
	struct dirent	*dp;
	std::string		html;

	html = "<!doctype html><html><head><title>Index of " + path + "=</title>\
			<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/\
			bootstrap@5.3.3/dist/css/bootstrap.min.css\"></head><body \
			class=\"m-3 mt-5\" style=\"background-color:#353535\"><div \
			class=\"container text-white p-3\"><div class=\"rounded-3 p-5\" \
			style=\"background-color:#2b2a33; border:solid 1px #1b1b1b\"><h4>\
			Index of " + path + "</h4><hr /><div class=\"p-3\"><table \
			style=\"width:100%\"><thead><tr style=\"font-size:1.2em\"><th>Name\
			</th><th>Last modified</th><th>Size</th></tr></thead><tbody>";

	if ((dir = opendir(path.c_str())) == NULL) {
		perror("Cannot open directory");
		return (NULL);
	}
	while ((dp = readdir(dir)) != NULL) {
		if (dp->d_type == DT_DIR)
			html += htmlDir(dp);
		else
			html += htmlFile(dp);
	}

	html += "</tbody></table></div></div></div></body></html>";
	std::cout << html <<std::endl;
	return (html);
}
