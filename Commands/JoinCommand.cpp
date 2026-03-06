/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:47:05 by ikayiban          #+#    #+#             */
/*   Updated: 2026/03/06 09:23:04 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * toList
 * Splits a comma-separated string into a list of channel names
 * and validates channel format when required.
 */
std::vector<std::string> Server::toList(const std::string& channelsName, Client* client, bool isKey) {
    std::vector<std::string> list;
    size_t k = 0;
    
    for (size_t i = 0; i < channelsName.size(); i++) {
        if (channelsName[i] == ',') {
            std::string temp = channelsName.substr(k, i - k);
            if (correctChannelFormat(temp) || isKey)
                list.push_back(temp);
            else {
                std::cout << "Invalid channel format: " << temp << std::endl;
                std::vector<std::string> tempTab;
                tempTab.push_back(temp);
                serverResponse("403", client, tempTab, NULL);
				displayLogs("JOIN: no such channel", client->getFd());
            }
            k = i + 1;
        }
    }
    if (k < channelsName.size()) {
        std::string temp = channelsName.substr(k);
        if (correctChannelFormat(temp) || isKey)
        list.push_back(temp);
        else {
            std::vector<std::string> tempTab;
            tempTab.push_back(temp);
            serverResponse("403", client, tempTab, NULL);
            displayLogs("JOIN: no such channel", client->getFd());
        }
    }
    return list;
}

/* 
 * joinHandler
 * Handles the JOIN command by allowing a client
 * to join one or multiple channels.
 */
void Server::joinHandler(Client *client, std::vector<std::string>& parsedData) {
    if (parsedData.size() < 2) {
        serverResponse("461", client, parsedData, NULL);
		displayLogs("JOIN: missing parameter", client->getFd());
        return;
    }
    std::vector<std::string> channels = toList(parsedData[1], client, false);
    std::vector<std::string> keys;
    if (parsedData.size() >= 3) {
        keys = toList(parsedData[2], client, true);
	}
	for (size_t i = 0; i < channels.size(); i++) {
		std::string name = channels[i];
		std::string key = (i < keys.size()) ? keys[i] : "";
		if (!channelExists(name)) {
			std::pair<std::map<std::string, Channel>::iterator, bool> inserted
				= _ChannelList.insert(std::make_pair(name, Channel(name)));
			Channel &channel = inserted.first->second;
			channel.addClient(client);
			channel.addOperator(client);
			displayLogs("channel created", client->getFd());
            std::vector<std::string> nameTab;
            nameTab.push_back(name);
            serverResponse("JOIN", client, nameTab, &channel);
			displayLogs("JOIN: client joined", client->getFd());
			continue;
		}
		Channel *channelPtr = getChannelByName(name);
		if (!channelPtr || channelPtr->isClient(client->getFd()))
			continue;
		Channel &channel = *channelPtr;
        if (channel.getFlag("_InviteOnlyFlag") && !channel.isGuest(client->getFd())) {
            std::vector<std::string> errorData;
            errorData.push_back("JOIN");
            errorData.push_back(channel.getName());
            serverResponse("473", client, errorData, &channel);
            displayLogs("JOIN: channel is invite-only", client->getFd());
            continue;
        }
        if (channel.getFlag("_KeyFlag") && !channel.checkKey(key)) {
            std::vector<std::string> errorData;
            errorData.push_back("JOIN");
            errorData.push_back(channel.getName());
            serverResponse("475", client, errorData, &channel);
            displayLogs("JOIN: bad channel key", client->getFd());
            continue;
        }
        size_t userLimit = std::atoi(channel.getUserLimitNumber().c_str());
        if (channel.getFlag("_UserLimitFlag") && 
				channel.getClientsList().size() >= userLimit) {
            std::vector<std::string> errorData;
            errorData.push_back("JOIN");
            errorData.push_back(channel.getName());
            serverResponse("471", client, errorData, &channel);
            displayLogs("JOIN: channel is full", client->getFd());
            continue;
        }
        channel.addClient(client);
        std::vector<std::string> nameTab;
        nameTab.push_back(channel.getName());
        serverResponse("JOIN", client, nameTab, &channel);
		displayLogs("JOIN: client joined", client->getFd());
        if (channel.getFlag("_InviteOnlyFlag") && channel.isGuest(client->getFd()))
            channel.removeGuest(client);
    }
}
