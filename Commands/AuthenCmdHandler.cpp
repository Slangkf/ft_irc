/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AuthenCmdHandler.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:56:09 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:20:15 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * passwordHandler
 * Handles the PASS command and validates the password sent
 * by the client during authentication.
 */
bool	Server::passwordHandler(Client *client,
									std::vector<std::string>& parsedData) {
	if (client->getFlag("_PassFlag") == true) {
		serverResponse("462", client, parsedData, NULL);
		displayLogs("PASS: already registered", client->getFd());
		return false;
	}
	if (parsedData.size() < 2) {
		serverResponse("461", client, parsedData, NULL);
		displayLogs("PASS: missing parameter", client->getFd());
		return false;
	}
	if (parsedData[1] != _Password) {
		serverResponse("464", client, parsedData, NULL);
		displayLogs("PASS: incorrect password", client->getFd());
		return false;
	}
	client->setPassword(_Password);
	if (!client->getFlag("_PassFlag"))
		client->setFlag("_PassFlag", true);
	return true;
}

/* 
 * nicknameHandler
 * Handles the NICK command by validating and assigning
 * a nickname to the client.
 */
bool	Server::nicknameHandler(Client *client,
									std::vector<std::string>& parsedData) {
	if (!client->getFlag("_PassFlag")) {
		serverResponse("464", client, parsedData, NULL);
		displayLogs("PASS: incorrect password", client->getFd());
        return false;
    }
	if (parsedData.size() < 2) {
		serverResponse("461", client, parsedData, NULL);
		displayLogs("NICK: missing parameter", client->getFd());
        return false;
    }
	if (!validNickname(parsedData[1])) {
		serverResponse("432", client, parsedData, NULL);
		displayLogs("NICK: invalid format", client->getFd());
		return false;
	}
	if (nickAlreadyExists(parsedData[1])) {
		serverResponse("433", client, parsedData, NULL);
		displayLogs("NICK: already exists", client->getFd());
        return false;
    }
	if (client->getFlag("_NickFlag")) {
		std::vector<Channel*> channels = getChannelsByClient(client);
		serverResponse("nickChange", client, parsedData, NULL);
		client->setNickname(parsedData[1]);
		displayLogs("NICK: has changed", client->getFd());
		return false;
	}
	client->setNickname(parsedData[1]);
	if (!client->getFlag("_NickFlag"))
    	client->setFlag("_NickFlag", true);
	return true;
}

/* 
 * usernameHandler
 * Handles the USER command and stores the username
 * required to complete client registration.
 */
bool	Server::usernameHandler(Client *client,
									std::vector<std::string>& parsedData) {
	if (!client->getFlag("_PassFlag")) {
		serverResponse("464", client, parsedData, NULL);
		displayLogs("PASS: incorrect password", client->getFd());
        return false;
    }
	if (parsedData.size() < 2) {
		serverResponse("461", client, parsedData, NULL);
		displayLogs("USER: missing parameter", client->getFd());
        return false;
    }
	if (client->getFlag("_UserFlag") && client->getFlag("_AuthenFlag")) {
		serverResponse("462", client, parsedData, NULL);
		displayLogs("USER: already registered", client->getFd());
		return false;
	}
    client->setUsername(parsedData[1]);
	if (!client->getFlag("_UserFlag"))
    	client->setFlag("_UserFlag", true);
	return true;
}
