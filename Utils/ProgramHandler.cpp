/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProgramHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:42:41 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:38:58 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * signalHandler
 * Sets the server's signal flag and logs
 * that a termination signal was received.
 */
void	Server::signalHandler(int signum){
	(void)signum;
	_Signal = true;
	displayLogs("SIGNAL RECEIVED", 0);
	return ;
}

/* 
 * removeClient
 * Removes a client from the server, including all channels
 * and operator lists.
 */
void	Server::removeClient(int fd){
	Client *target = NULL;

	for (size_t i = 0; i < _FdSet.size(); i++) {
		if (_FdSet[i].fd == fd) {
			_FdSet.erase(_FdSet.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < _ClientSet.size(); i++) {
		if (_ClientSet[i].getFd() == fd) {
			target = &_ClientSet[i];
			break ;
		}
	}
	if (!target)
		return ;
	for (std::map<std::string, Channel>::iterator it = _ChannelList.begin();
		it != _ChannelList.end(); ) {
		it->second.removeOperator(target);
		it->second.removeClient(target);
		if (it->second.getClientsList().empty())
			_ChannelList.erase(it++);
		else
			++it;
	}
	for (size_t i = 0; i < _ClientSet.size(); i++) {
		if (&_ClientSet[i] == target) {
			_ClientSet.erase(_ClientSet.begin() + i);
			break ;
		}
	}
}

/* 
 * closeFd
 * Closes all file descriptors tracked by the server.
 */
void	Server::closeFd(void) {
	for (size_t i = 0; i < _FdSet.size(); i++) {
		close(_FdSet[i].fd);
	}
}


/* 
 * broadcast
 * Sends a message from a client to all members of a channel,
 * formatting it with the command.
 */
void  Server::broadcast(Client* client, std::string msg, Channel* channel, std::string& cmd) {
	std::string finalMsg = ":";
	finalMsg += client->getNickname();
	finalMsg += "!";
	finalMsg += client->getUsername();
	finalMsg += "@";
	finalMsg += client->getIPAddress();
	finalMsg += " ";
	finalMsg += cmd;
	finalMsg += " ";
	if (cmd != "QUIT")
		finalMsg += channel->getName();
	if (!msg.empty()) {
		finalMsg += " :";
    	finalMsg += msg;
  	}
  	finalMsg += CRLF;
  	for (size_t i = 0; i < channel->getClientsList().size(); i++) {
		ssize_t bytes_sent = send(channel->getClientsList()[i]->getFd(), finalMsg.c_str(), finalMsg.size(), 0);
    	if (bytes_sent == -1)
			throw(std::runtime_error("\e[1;31m(ERROR) partBroadcast: send() failed"));
	}
	return ;
}
