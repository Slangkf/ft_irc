/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProgramParsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:36:25 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:39:52 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

/* 
 * checkPassword
 * Validates that a password is 4–12 printable ASCII characters.
 */
bool	checkPassword(char *password) {
	std::string	input = password;

	if (input.size() < 4 || input.size() > 12)
		return false;
	for (int i = 0; input[i]; i++) {
		if (input[i] < 33 || input[i] > 126)
			return false;
	}
	return true;
}

/* 
 * checkPort
 * Validates that a port is numeric and within
 * the range 1024–65535.
 */
bool	checkPort(char *port) {
	unsigned long	value = 0;

	for (int i = 0; port[i]; i++) {
		if (!isdigit(port[i]))
			return false;
		value *= 10;
		value += port[i] - '0';
	}
	if (value < 1024 || value > 65535)
		return false;
	return true;
}


/* 
 * checkArgs
 * Validates program arguments: correct count, valid port,
 * and valid password.
 */
bool	checkArgs(int argc, char *port, char *password) {
	if (argc != 3) {
		std::cerr << RED << "(ERROR) Program must be run as follows: ";
		std::cerr << YELLOW << "./ircserv <port> <password>" << WHITE << std::endl;
		return false;
	}
	if (!checkPort(port)) {
		std::cerr << RED << "(ERROR) Invalid port number: ";
		std::cerr << YELLOW << "must be between 1024 and 65535." << WHITE << std::endl;
		return false;
	}
	if (!checkPassword(password)) {
		std::cerr << RED << "(ERROR) Invalid password: ";
		std::cerr << YELLOW << "must be between 4 and 12 characters without spaces." << WHITE << std::endl;
		return false;
	}
	return true;
}
