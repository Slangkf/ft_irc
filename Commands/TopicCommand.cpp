/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:07:44 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:31:28 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * topicCommand
 * Gets or sets a channel's topic, applying permissions
 * and sending responses to the client and displaying server logs.
 */
void	Server::topicCommand(Client *client,
								std::vector<std::string>& parsedData) {
	if (parsedData.size() < 2) {
		serverResponse("461", client, parsedData, NULL);
		return displayLogs("TOPIC: missing parameter", client->getFd());
	}
	if (!channelExists(parsedData[1])) {
		serverResponse("403", client, parsedData, NULL);
		return displayLogs("TOPIC: no such channel", client->getFd());
	}
	if (!userIsOnChannel(client->getFd(), parsedData[1])) {
		serverResponse("442", client, parsedData, NULL);
		return displayLogs("TOPIC: you're not on that channel", client->getFd());
	}
	Channel *channel = getChannelByName(parsedData[1]);
	if (parsedData.size() == 2) {
		if (channel->getTopic().empty()) {
			serverResponse("331", client, parsedData, NULL);
			return displayLogs("TOPIC: no topic defined", client->getFd());
		}
		serverResponse("332", client, parsedData, channel);
		return displayLogs("TOPIC: topic returned", client->getFd());
	}
	if (channel->getFlag("_TopicFlag")) {
		if (!channel->isOperator(client->getFd())) {
			serverResponse("482", client, parsedData, channel);
			return displayLogs("TOPIC: is not channel operator", client->getFd());
		}
	}
	if (parsedData[2].size() == 1 && parsedData[2][0] == ':') {
		if (channel->getTopic().empty())
			return ;
		channel->getTopic().erase();
		serverResponse("TOPIC", client, parsedData, channel);
		return displayLogs("TOPIC: unset", client->getFd());
	}
	channel->setTopic(parsedData[2]);
	serverResponse("TOPIC", client, parsedData, channel);
	return displayLogs("TOPIC: new topic defined", client->getFd());
}
