/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 13:51:35 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 10:35:12 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::_Signal = false;

/*
 * Server::Server / ~Server
 * Constructor and destructor for the Server class.
 *
 * Constructor:
 * - Stores the provided port and password.
 * - Initializes server file descriptor to -1 and server time
 *   counter to 0.
 *
 * Destructor:
 * - Currently empty (no dynamic resources to free manually).
 */
Server::Server(char *port, char *password): _ServSockFd(-1), _ServerTimeCount(0) {
	if (!port || !password)
		return;
	_Port = atoi(port);
	_Password = password;
	return ;
}

Server::~Server(){
	return ;
}


/*
 * Server::initialization
 * Sets up the server to accept incoming connections.
 *
 * Steps:
 * 1. Configure a sockaddr_in structure with AF_INET,
 *    port (network byte order), and any local IP.
 * 2. Create a TCP socket (SOCK_STREAM).
 * 3. Enable SO_REUSEADDR to allow immediate reuse of the port.
 * 4. Set the socket to non-blocking mode using fcntl().
 * 5. Bind the socket to the address and port.
 * 6. Listen for incoming connections (passive mode) with maximum
 *    backlog (SOMAXCONN).
 * 7. Add the listening socket to the _FdSet vector for poll
 *    monitoring.
 */
void	Server::initialization(void) {
	struct sockaddr_in	servSockAdd;
	struct pollfd		newPollFd;
	socklen_t			servSockAddSize;
	int					optval = 1;	

	servSockAddSize = sizeof(servSockAdd);
	servSockAdd.sin_family = AF_INET;
	servSockAdd.sin_port = htons(this->_Port);
	servSockAdd.sin_addr.s_addr = INADDR_ANY;

	this->_ServSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_ServSockFd == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) socket() failed"));
	if (setsockopt(_ServSockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) setsockopt() failed"));
	if (fcntl(_ServSockFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) server fcntl() failed"));
	if (bind(_ServSockFd, (sockaddr *)&servSockAdd, servSockAddSize) == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) bind() failed"));
	if (listen(_ServSockFd, SOMAXCONN) == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) listen() failed"));

	newPollFd.fd = _ServSockFd;
	newPollFd.events = POLLIN;
	newPollFd.revents = 0;
	_FdSet.push_back(newPollFd);
	return ;
}

/*
 * Server::start
 * Main server loop.
 *
 * Steps:
 * 1. Log that the server has started.
 * 2. Enter a loop that continues until a termination signal
 *    (_Signal) is received.
 * 3. Use poll() to monitor all file descriptors in _FdSet
 *    for readable events.
 * 4. If a listening socket is ready, accept a new client
 *    connection.
 * 5. If a client socket is ready, receive and process incoming
 *    data.
 * 6. Increment server time counter for flood control.
 * 7. On exit (signal received), close all open file descriptors.
 */
void	Server::start(void) {
	displayLogs("(SERVER STARTED)", 0);
	while (!_Signal) {
		if ((poll(&_FdSet[0], _FdSet.size(), 100) == -1) && !_Signal)
			throw(std::runtime_error("\e[1;31m(ERROR) poll() failed"));
		_ServerTimeCount++;
		for (size_t i = 0; i < _FdSet.size(); i++) {
			if (_FdSet[i].revents & POLLIN) { // &: bitwise AND.
				if (_FdSet[i].fd == _ServSockFd)
					acceptNewClient();
				else
					receiveData(_FdSet[i].fd);
			}
		}
	}
	closeFd();
	return ;
}

/*
 * Server::acceptNewClient
 * Handles new client connections.
 *
 * Steps:
 * 1. Accept a new connection using accept() and obtain
 *    the client socket descriptor.
 * 2. Set the new client socket to non-blocking mode.
 * 3. Prepare a pollfd structure for the client and add it
 *    to _FdSet for monitoring.
 * 4. Initialize a Client object with the file descriptor and
 *    IP address.
 * 5. Add the Client object to the _ClientSet vector for server
 *    tracking.
 */
void	Server::acceptNewClient(void) {
	Client				newClient;
	struct sockaddr_in	cliSockAdd;
	struct pollfd		newPollFd;
	socklen_t			cliSockAddSize;
	int					newCliFd;

	cliSockAddSize = sizeof(cliSockAdd);
	newCliFd = accept(_ServSockFd, (sockaddr *)&cliSockAdd, &cliSockAddSize);
	if (newCliFd == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) accept() failed"));
	if (fcntl(newCliFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) client fcntl() failed"));
	
	newPollFd.fd = newCliFd;
	newPollFd.events = POLLIN; 		// incoming events defined on: data is waiting to be read.
	newPollFd.revents = 0;			// detected events defined on NULL. 
	_FdSet.push_back(newPollFd);	// save the new pollfd.
	newClient.setFd(newCliFd);
	newClient.setIPAddress(inet_ntoa((cliSockAdd.sin_addr))); // inet_ntoa: converts the Internet host address, given in network byte order, to a string in IPv4 dotted-decimal notation.
	_ClientSet.push_back(newClient); // save the new client.
	return ;
}
