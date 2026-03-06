/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:23:23 by ikayiban          #+#    #+#             */
/*   Updated: 2026/03/06 09:29:08 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * quitCommand
 * Broadcasts client's quit to channels, removes client,
 * closes connection, and logs action.
 */
void    Server::quitCommand(Client* client, std::vector<std::string>& parsedData) {
    std::map<std::string, Channel>::iterator it = _ChannelList.begin();
    for (; it != _ChannelList.end(); it++) {
        Channel* channel = &(it->second);
        if (channel->isClient(client->getFd())) {
            std::string reason;
            if (parsedData.size() > 1)
                reason = parsedData[1];
            std::string cmd = "QUIT";
            broadcast(client, reason, channel, cmd);
        }
    }
    removeClient(client->getFd());
    close(client->getFd());
	displayLogs("has quit", client->getFd());
    return ;
}