/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 10:25:21 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 10:21:53 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/*
 * isFlooding
 * Checks whether a client is sending messages too quickly.
 * Resets the flood count if the time window has passed 2 seconds.
 * Increments flood count otherwise.
 * If count reaches 8, the client is removed, their socket closed,
 * and logs generated.
 * Returns true if client is flooding, false otherwise.
 */
bool Server::isFlooding(Client *client) {
	if (_ServerTimeCount - client->getTimeWindow() > 20) {
		client->setTimeWindow(_ServerTimeCount);
		client->setFloodingCount(0);
	}
	else
		client->setFloodingCount(client->getFloodingCount() + 1);
	if (client->getFloodingCount() >= 8) {
		removeClient(client->getFd());
		close(client->getFd());
		displayLogs("anti flooding activated", client->getFd());
		displayLogs("has been ejected", client->getFd());
		return true;
	}
	return false;
}


/*
 * nickAlreadyExists
 * Checks whether the given nickname is already in use
 * by another connected client.
 * Iterates through _ClientSet comparing nicknames.
 */
bool    Server::nickAlreadyExists(const std::string& nick) {
    for (size_t i = 0; i < _ClientSet.size(); i++) {
        if (_ClientSet[i].getNickname() == nick)
            return true;
    }
    return false;
}

/*
 * getClientByFd
 * Returns a pointer to the Client object matching
 * the given file descriptor (fd).
 * Returns NULL if no match is found.
 */
Client*	Server::getClientByFd(int clientFd) {
    for (size_t i = 0; i < _ClientSet.size(); i++) {
        if (_ClientSet[i].getFd() == clientFd)
            return &_ClientSet[i];
    }
    return NULL;
}

/*
 * getClientFd
 * Returns the file descriptor (fd) of the client
 * with the given nickname.
 * Returns -1 if no client with that nickname exists.
 */
int	Server::getClientFd(std::string& nickname) {
	size_t i = 0;
	for (; i < _ClientSet.size(); i++) {
		if (_ClientSet[i].getNickname() == nickname) {
			return _ClientSet[i].getFd();
		}
	}
	return -1;
}

