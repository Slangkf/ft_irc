/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:07:12 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:20:46 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * inviteCommand
 * Handles the INVITE command by allowing a channel
 * operator to invite a user to join a channel.
 */
void	Server::inviteCommand(Client *client, std::vector<std::string>& parsedData) {
	if (parsedData.size() < 3) {
		serverResponse("461", client, parsedData, NULL);
		return displayLogs("INVITE: missing parameter", client->getFd());
	}
	if (!nickAlreadyExists(parsedData[1])) {
		serverResponse("401", client, parsedData, NULL);
		return displayLogs("INVITE: No such nickname", client->getFd());
	}
	if (!userIsOnChannel(client->getFd(), parsedData[2])) {
		serverResponse("442", client, parsedData, NULL);
		return displayLogs("INVITE: you're not on that channel", client->getFd());
	}
	int	clientFd = getClientFd(parsedData[1]);
	if (userIsOnChannel(clientFd, parsedData[2])) {
		serverResponse("443", client, parsedData, NULL);
		return displayLogs("INVITE: is already on channel", client->getFd());
	}
	Channel *channel = getChannelByName(parsedData[2]);
	if (!channel->isOperator(client->getFd())) {
		serverResponse("482", client, parsedData, channel);
		return displayLogs("INVITE: is not channel operator", client->getFd());
	}
	serverResponse("341", client, parsedData, channel);
	channel->addGuest(getClientFd(parsedData[1]));
	displayLogs("INVITE: successful", client->getFd());
}
