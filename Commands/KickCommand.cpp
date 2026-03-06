/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:06:49 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:23:35 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * kickCommand
 * Handles the KICK command by allowing a channel operator
 * to remove a user from a channel.
 */
void	Server::kickCommand(Client *client,
								std::vector<std::string>& parsedData) {
	if (parsedData.size() < 3) {
		serverResponse("461", client, parsedData, NULL);
		return displayLogs("KICK: missing parameter", client->getFd());
	}
	if (!channelExists(parsedData[1])) {
		serverResponse("403", client, parsedData, NULL);
		return displayLogs("KICK: no such channel", client->getFd());
	}
	int	targetFd = getClientFd(parsedData[2]);
	if (!userIsOnChannel(targetFd, parsedData[1])) {
		serverResponse("441", client, parsedData, NULL);
		return displayLogs("KICK: no user in the channel", client->getFd());
	}
	if (!userIsOnChannel(client->getFd(), parsedData[1])) {
		serverResponse("442", client, parsedData, NULL);
		return displayLogs("KICK: you're not on that channel", client->getFd());
	}
	Channel *channel = getChannelByName(parsedData[1]);
	if (!channel->isOperator(client->getFd())) {
		serverResponse("482", client, parsedData, channel);
		return displayLogs("KICK: is not channel operator", client->getFd());
	}
	serverResponse("KICK", client, parsedData, channel);
	Client	*clt = channel->getClient(targetFd);
	if (channel->isOperator(clt->getFd()))
		channel->removeOperator(clt);
	channel->removeClient(clt);
	displayLogs("KICK: has been kicked", client->getFd());
}
