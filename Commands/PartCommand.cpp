/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:54:47 by ikayiban          #+#    #+#             */
/*   Updated: 2026/03/06 09:26:08 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * partCommand
 * Handles the PART command by allowing a client
 * to leave a channel.
 */
void    Server::partCommand(Client* client, std::vector<std::string>& parsedData) {
    if (parsedData.size() < 2) {
      serverResponse("461", client, parsedData, NULL);
      return displayLogs("PART: missing parameter", client->getFd());
    }
    std::vector<std::string> channelsName = toList(parsedData[1], client, false);
    for (size_t i = 0; i < channelsName.size(); i++) {
      if (!channelExists(channelsName[i])) {
        std::vector<std::string> error;
        error.push_back(channelsName[i]);
        serverResponse("403", client, error, NULL);
        displayLogs("PART: no such channel", client->getFd());
      }
      else {
        Channel* channel = getChannelByName(channelsName[i]);
        if (!channel->isClient(client->getFd())) {
          std::vector<std::string> error;
          error.push_back("PART");
          error.push_back(channelsName[i]);
          serverResponse("442", client, error, NULL);
          displayLogs("PART: you're not on that channel", client->getFd());
        }
        else {
          std::string reason;
          if (parsedData.size() > 2)
            reason = parsedData[2];
          std::string cmd = "PART";
          broadcast(client, reason, channel, cmd);
          channel->removeOperator(client);
          channel->removeClient(client);
        }
      }
    }
}
