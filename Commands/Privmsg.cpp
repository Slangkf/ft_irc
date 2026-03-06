/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 13:52:54 by ikayiban          #+#    #+#             */
/*   Updated: 2026/03/06 09:26:36 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * privmsgHandler
 * Handles the PRIVMSG command by sending a private message
 * to a user or broadcasting it to a channel.
 */
void    Server::privmsgHandler(Client* client, std::vector<std::string>& parsedData) {
    if (parsedData.size() < 2) {
        serverResponse("411", client, parsedData, NULL);
        displayLogs("PRIVMSG: No recipient given", client->getFd());
        return;
    }
    if (parsedData.size() < 3 || parsedData[2].empty()) {
        serverResponse("412", client, parsedData, NULL);
        displayLogs("PRIVMSG: No text", client->getFd());
        return;
    }
    const std::string& target = parsedData[1];
    const std::string& msg = parsedData[2];
    int targetFd = getClientFd(parsedData[1]);
    std::string fullMsg = ":" + client->getNickname() + "!"
        + client->getUsername() + "@"
        + client->getIPAddress()
        + " PRIVMSG " + target + " :" + msg + "\r\n";
    for (size_t i = 0; i < _ClientSet.size(); i++) {
        if (_ClientSet[i].getFd() == targetFd) {
            ssize_t bytes_sent = send(_ClientSet[i].getFd(), fullMsg.c_str(), fullMsg.size(), 0);
            if (bytes_sent == -1)
                throw(std::runtime_error("\e[1;31m(ERROR) privmsg first send() failed"));
            return;
        }
    }
    std::map<std::string, Channel>::iterator it = _ChannelList.find(target);
    if (it != _ChannelList.end()) {
        Channel& channel = it->second;
        if (!channel.isClient(client->getFd())) {
            serverResponse("404", client, parsedData, &channel);
            displayLogs("PRIVMSG: Cannot send to channel", client->getFd());
            return;
        }
        std::vector<Client*> members = channel.getClientsList();
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i] != client) {
                ssize_t bytes_sent = send(members[i]->getFd(), fullMsg.c_str(), fullMsg.size(), 0);
                if (bytes_sent == -1)
					throw(std::runtime_error("\e[1;31m(ERROR) privmsg second send() failed"));
			}
        }
        return;
    }
    serverResponse("401", client, parsedData, NULL);
    displayLogs("PRIVMSG: No such nickname", client->getFd());
    return ;
}
