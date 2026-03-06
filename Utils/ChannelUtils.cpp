/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 15:41:43 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:35:41 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * myToUpper
 * Converts a string to uppercase and replaces
 * certain special characters.
 */
std::string myToUpper(std::string str) {
    for (size_t i = 0; i < str.size(); i++) {
        str[i] = static_cast<char>(toupper(static_cast<unsigned char>(str[i])));
        if (str[i] == '{') str[i] = '[';
        else if (str[i] == '}')
			str[i] = ']';
        else if (str[i] == '|')
			str[i] = '\\';
        else if (str[i] == '~')
			str[i] = '^';
    }
    return str;
}

/* 
 * getChannelByName
 * Returns a pointer to the channel matching
 * the given name (case-insensitive), or NULL.
 */
Channel*	Server::getChannelByName(std::string& channelName) {
	std::string inputUpper = myToUpper(channelName);
	std::map<std::string, Channel >::iterator it = _ChannelList.begin();

	for (; it != _ChannelList.end(); ++it) {
		if (myToUpper(it->first) == inputUpper)
			return &(it->second);
	}
	return NULL;
}

/* 
 * getChannelsByClient
 * Returns a list of channels that the given client is part of.
 */
std::vector<Channel*>	Server::getChannelsByClient(Client *client) {
	std::vector<Channel*>						channels;	
	std::map<std::string, Channel>::iterator 	it = _ChannelList.begin();
	std::map<std::string, Channel>::iterator 	ite = _ChannelList.end();

	for (; it != ite; ++it) {
		if (it->second.isClient(client->getFd()))
			channels.push_back(&(it->second));
	}
	return channels;
}

/* 
 * channelExists
 * Checks if a channel with the given name exists
 * (case-insensitive).
 */
bool Server::channelExists(const std::string& name) {
    std::string inputUpper = myToUpper(name);
    
    std::map<std::string, Channel>::iterator it = _ChannelList.begin();
    for (; it != _ChannelList.end(); ++it) {
        if (myToUpper(it->first) == inputUpper)
            return true;
    }
    return false;
}

/* 
 * userIsOnChannel
 * Checks if a user is a member of the specified channel.
 */
bool	Server::userIsOnChannel(int clientFd, std::string& channelName) {
	Channel	*channel = getChannelByName(channelName);
	if (!channel)
		return false;
	return channel->isClient(clientFd);
}
