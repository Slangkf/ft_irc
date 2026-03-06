/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 13:56:17 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:06:45 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* 
 * Client
 * Default constructor initializing the client authentication
 * state and connection data.
 */
Client::Client(void): _AuthenFlag(false), _PassFlag(false),
	_NickFlag(false), _UserFlag(false), _Fd(-1), _FloodingCount(0),
	_TimeWindow(0), _Buffer() {
	return ;
}

/* 
 * ~Client
 * Destructor of the Client class.
 */
Client::~Client() {
	return ;
}

/* 
 * setFlag
 * Updates one of the client authentication flags.
 */
void	Client::setFlag(std::string whichFlag, bool value) {
	if (whichFlag == "_AuthenFlag")
		_AuthenFlag = value;
	else if (whichFlag == "_PassFlag")
		_PassFlag = value;
	else if (whichFlag == "_NickFlag")
		_NickFlag = value;
	else if (whichFlag == "_UserFlag")
		_UserFlag = value;
	else
		displayLogs("FLAG ERROR", 0);
	return ;
}

/* 
 * setFd
 * Assigns the socket file descriptor to the client.
 */
void	Client::setFd(int fd) {
	_Fd = fd;
	return ;
}

/* 
 * setFloodingCount
 * Updates the flooding counter used for rate limiting.
 */
void	Client::setFloodingCount(unsigned int count) {
	_FloodingCount = count;
	return ;
}

/* 
 * setTimeWindow
 * Stores the timestamp used to track message flooding windows.
 */
void	Client::setTimeWindow(unsigned long long serverTime) {
	_TimeWindow = serverTime;
	return ;
}

/* 
 * setIPAddress
 * Stores the IP address of the client.
 */
void	Client::setIPAddress(std::string ipAddress) {
	_IPAddress = ipAddress;
	return ;
}

/* 
 * setPassword
 * Stores the password provided by the client.
 */
void	Client::setPassword(std::string password) {
	_Password = password;
	return ;
}

/* 
 * setNickname
 * Stores the nickname provided by the client.
 */
void	Client::setNickname(std::string nickname) {
	_Nickname = nickname;
	return ;
}

/* 
 * setUsername
 * Stores the username provided by the client.
 */
void	Client::setUsername(std::string username) {
	_Username = username;
	return ;
}

/* 
 * getFd
 * Returns the client's socket file descriptor.
 */
int		Client::getFd(void) {
	return _Fd;
}

/* 
 * getFloodingCount
 * Returns the current flooding counter value.
 */
unsigned int	Client::getFloodingCount(void) {
	return _FloodingCount;
}

/* 
 * getTimeWindow
 * Returns the timestamp of the current flooding window.
 */
unsigned long long	Client::getTimeWindow(void) {
	return _TimeWindow;
}

/* 
 * getFlag
 * Returns the value of a specified flag.
 */
bool	Client::getFlag(std::string whichFlag) {
	if (whichFlag == "_AuthenFlag")
		return _AuthenFlag;
	else if (whichFlag == "_PassFlag")
		return _PassFlag;
	else if (whichFlag == "_NickFlag")
		return _NickFlag;
	else if (whichFlag == "_UserFlag")
		return _UserFlag;
	else {
		displayLogs("FLAG ERROR", 0);
		return false;
	}
}

/* 
 * getIPAddress
 * Returns the client's IP address.
 */
std::string&	Client::getIPAddress(void) {
	return _IPAddress;
}

/* 
 * getPassword
 * Returns the stored client password.
 */
std::string&	Client::getPassword(void) {
	return _Password;
}

/* 
 * getNickname
 * Returns the client's nickname.
 */
std::string&	Client::getNickname(void) {
	return _Nickname;
}

/* 
 * getUsername
 * Returns the client's username.
 */
std::string&	Client::getUsername(void) {
	return _Username;	
}

/* 
 * getBuffer
 * Returns the buffer storing incoming client data.
 */
std::string&	Client::getBuffer(void) {
	return _Buffer;
}