/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:04:26 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:24:58 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * modeCmdHandler
 * Handles the MODE command by validating parameters and
 * applying or removing channel modes.
 */
void	Server::modeCmdHandler(Client *client,
								std::vector<std::string>& parsedData) {
	if (parsedData.size() < 2) {
		serverResponse("461", client, parsedData, NULL);
		return displayLogs("MODE: missing parameter", client->getFd());
	}
	if (!channelExists(parsedData[1])) {
		serverResponse("403", client, parsedData, NULL);
		return displayLogs("MODE: no such channel", client->getFd());
	}
	if (parsedData.size() == 3) {
		if (isRequestToIgnore(client, parsedData))
			return ;
		else if (!isValidMode(parsedData[2])) {
			serverResponse("472", client, parsedData, NULL);
			return displayLogs("MODE: unknown mode", client->getFd());
		}
	}
	Channel *channel = getChannelByName(parsedData[1]);
	if (parsedData.size() == 2) {
		serverResponse("324", client, parsedData, NULL);
		return displayLogs("MODE: channel modes asked", client->getFd());
	}
	if (!channel->isOperator(client->getFd())) {
		serverResponse("482", client, parsedData, channel);
		return displayLogs("MODE: is not channel operator", client->getFd());
	}
	if (parsedData[2][0] == '+')
		setChannelModes(client, channel, parsedData);
	else
		unsetChannelModes(client, channel, parsedData);
	if (parsedData.size() == 4 && parsedData[2][1] == 'o')
		return ;
	serverResponse("324", client, parsedData, channel);
	displayLogs("MODE: channel modes asked", client->getFd());
	return ;
}

/* 
 * setChannelModes
 * Enables channel modes that require parameters or flags based
 * on the parsed MODE command.
 */
void	Server::setChannelModes(Client *client, Channel *channel,
									std::vector<std::string>& parsedData) {
	std::map<char,std::string>				modesParams = setModeMap(parsedData);
	std::map<char,std::string>::iterator	it = modesParams.begin();
	std::map<char,std::string>::iterator	ite = modesParams.end();

	for (; it != ite; ++it) {
		if (it->first == 'k' || it->first == 'o' || it->first == 'l') {
			if (it->second == "NULL") {
				serverResponse("461", client, parsedData, NULL);
				displayLogs("MODE: missing parameter", client->getFd());
			}
			else
				setModesWithParams(client, channel, *it, parsedData);
		}
	}
	it = modesParams.find('i');
	if (it != ite)
		channel->setFlag("_InviteOnlyFlag", true);
	it = modesParams.find('t');
	if (it != ite)
		channel->setFlag("_TopicFlag", true);
	return ;
}

/* 
 * setModesWithParams
 * Applies channel modes that require additional parameters
 * such as key, operator, or user limit.
 */
void	Server::setModesWithParams(Client *client, Channel *channel, std::pair<const char,std::string>& mode,
								std::vector<std::string>& parsedData) {
	if (mode.first == 'k') {
		if (channel->getKey().empty()) {
			channel->setKey(mode.second);
			channel->setFlag("_KeyFlag", true);
		}
		else {
			serverResponse("467", client, parsedData, NULL);
			displayLogs("MODE: Channel key already set", client->getFd());;
		}
		return ;
	}
	else if (mode.first == 'o') {
		int		targetFd = getClientFd(mode.second);
		Client	*target = channel->getClient(targetFd);
		if (target) {
			if (!channel->isOperator(target->getFd())) {
				serverResponse("operator", client, parsedData, channel);
			 	channel->addOperator(target);
				displayLogs("MODE: channel modes asked", client->getFd());
			}
		}
		else {
			serverResponse("441", client, parsedData, NULL);
			displayLogs("MODE: no user in the channel", client->getFd());	
		}
		return ;
	}
	else {
		if (isDigit(mode.second)) {
			channel->setUserLimitNumber(mode.second);
			if (!channel->getFlag("_UserLimitFlag"))
				channel->setFlag("_UserLimitFlag", true);
		}
		else {
			serverResponse("461", client, parsedData, NULL);
			displayLogs("MODE: missing parameter", client->getFd());
		}
	}
	return ;
}

/* 
 * unsetChannelModes
 * Disables channel modes and removes associated parameters
 * when required.
 */
void	Server::unsetChannelModes(Client *client, Channel *channel,
									std::vector<std::string>& parsedData) {
	std::string modes = parsedData[2];

	std::string::size_type found = modes.find('i');
	if (found != std::string::npos)
		channel->setFlag("_InviteOnlyFlag", false);
	found = modes.find('t');
	if (found != std::string::npos)
		channel->setFlag("_TopicFlag", false);
	found = modes.find('k');
	if (found != std::string::npos) {
		if (!channel->getKey().empty())
			channel->getKey().erase();
		channel->setFlag("_KeyFlag", false);
	}
	found = modes.find('o');
	if (found != std::string::npos) {
		int		targetFd = getClientFd(parsedData[3]);
		Client	*target = channel->getClient(targetFd);
		if (!target) {
			serverResponse("441", client, parsedData, NULL);
			displayLogs("MODE: no user in the channel", client->getFd());
		}
		else {
			if (channel->isOperator(target->getFd())) {
				serverResponse("operator", client, parsedData, channel);
				channel->removeOperator(target);
				displayLogs("MODE: channel modes asked", client->getFd());
			}
		}
		return ;
	}
	found = modes.find('l');
	if (found != std::string::npos) {
		if (!channel->getUserLimitNumber().empty())
			channel->getUserLimitNumber().erase();
		if (channel->getFlag("_UserLimitFlag"))
			channel->setFlag("_UserLimitFlag", false);
	}
	return ;
}
