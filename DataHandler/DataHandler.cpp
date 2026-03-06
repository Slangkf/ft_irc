/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:41:15 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:34:21 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* 
 * receiveData
 * Receives data from client, appends to buffer, parses
 * and handles commands.
 */
void	Server::receiveData(int clientFd) {
	Client	*client = getClientByFd(clientFd);
	char	buf[512];

	if (!client)
		throw(std::runtime_error("\e[1;31m(ERROR) getClientByFd() failed"));
	ft_memset(buf, 0, sizeof(buf));
	ssize_t recv_bytes = recv(clientFd, buf, sizeof(buf), 0);
	if (recv_bytes == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) recv() failed"));
	else if (recv_bytes == 0) {
		displayLogs("has quit", clientFd);
		removeClient(clientFd);
		close(clientFd);
		return ;
	}
	else if (!isFlooding(client)) {
		client->getBuffer().append(buf, recv_bytes);
		while (!client->getBuffer().empty() && !_Signal) {
			std::vector<std::string> parsedData = parseDataReceived(client->getBuffer());
			if (parsedData.empty())
				break;
			else if (parsedData[0] == "PASS" || parsedData[0] == "NICK" || parsedData[0] == "USER")
				authenticationHandler(client, parsedData);
			else
				incomingDataHandler(client, parsedData);
		}
	}
	return ;
}


/* 
 * serverResponse
 * Sends the appropriate response to the client's request.
 */
void	Server::serverResponse(const std::string& code, Client *client,
									std::vector<std::string>& parsedData, Channel* channel) {
	if (code == "001")
		authenticationSuccessful(client);
	else if (code == "324")
		RPLChannelModeIs(client, parsedData[1]);
	else if (code == "331")
		RPLNoTopic(client, parsedData[1]);
	else if (code == "332")
		RPLTopic(client, parsedData, channel);
	else if (code == "341")
		RPLInviting(client, parsedData, getClientFd(parsedData[1]));
	else if (code == "401")
		noSuchNickname(client, parsedData[1]);
	else if (code == "403")
		noSuchChannel(client, parsedData[0]);
	else if (code == "404")
		cannotSendToChan(client, channel->getName());
	else if (code == "411")
		noRecipient(client);
	else if (code == "412")
		noTextToSend(client);
	else if (code == "432")
		invalidNickFormat(client, parsedData[1]);
	else if (code == "433")
		nickInUse(client, parsedData[1]);
	else if (code == "441")
		noUserInChannel(client, parsedData);
	else if (code == "442")
		notOnThatChannel(client, parsedData[1]);
	else if (code == "443")
		alreadyOnChannel(client, parsedData[2]);
	else if (code == "451")
		notRegistered(client);
	else if (code == "461")
		needMoreParams(client, parsedData[0]);
	else if (code == "462")
		alreadyRegistered(client);
	else if (code == "464")
		passwdMismatch(client);
	else if (code == "467")
		keyAlreadySet(client, parsedData[1]);
	else if (code == "471")
		channelFull(client, channel->getName());
	else if (code == "472")
		unknownMode(client, parsedData);
	else if (code == "473")
		inviteOnly(client, channel);
	else if (code == "475")
		badChannelKey(client, channel);
	else if (code == "482")
		notChannelOperator(client, channel->getName());
	else if (code == "nickChange")
		nickChange(client, parsedData[1]);
	else if (code == "JOIN") {
		joinSuccessful(client, channel);
		joinForAll(client, channel);
	}
	else if (code == "operator")
		operatorRights(client, parsedData, channel);
	else if (code == "KICK")
		kickUser(client, parsedData, channel);
	else if (code == "TOPIC")
		changeTopic(client, channel);
	else if (code == "PONG")
		pongResponse(client, parsedData[1]);
	return ;
}

/* 
 * authenticationHandler
 * Handles PASS, NICK, USER commands and authenticates the client.
 */
void	Server::authenticationHandler(Client *client,
										std::vector<std::string>& parsedData) {
    std::string input = parsedData[0];
	if (input == "PASS") {
		if (!passwordHandler(client, parsedData))
			return ;
	}
    else if (input == "NICK") {
		if (!nicknameHandler(client, parsedData))
			return ;
	}
    else if (input == "USER") {
		if (!usernameHandler(client, parsedData))
			return ;
	}
    if (client->getFlag("_PassFlag") && client->getFlag("_NickFlag")
			&& client->getFlag("_UserFlag") && !client->getFlag("_AuthenFlag")) {
		client->setFlag("_AuthenFlag", true);
		serverResponse("001", client, parsedData, NULL);
		displayLogs("is connected", client->getFd());
    }
	return ;
}

/* 
 * incomingDataHandler
 * Routes incoming commands: PING, ignored requests,
 * or authenticated user/operator commands.
 */
void	Server::incomingDataHandler(Client* client,
										std::vector<std::string>& parsedData) {
	if (parsedData[0] == "PING")
		return serverResponse("PONG", client, parsedData, NULL);
	else if (isRequestToIgnore(client, parsedData))
		return ;
	else if (client->getFlag("_AuthenFlag")) {
		if (!isOperCommand(parsedData[0]))
			return userCmdHandler(client, parsedData);
		else
			return operatorCmdHandler(client, parsedData);
	}
	else {
		serverResponse("451", client, parsedData, NULL);
		displayLogs("not registered", client->getFd());
		return ;
	}
}


/* 
 * userCmdHandler
 * Processes standard user commands: JOIN, PRIVMSG, PART, QUIT.
 */
void	Server::userCmdHandler(Client *client,
										std::vector<std::string>& parsedData) {
	if (parsedData[0] == "JOIN")
		joinHandler(client, parsedData);
	else if (parsedData[0] == "PRIVMSG")
		privmsgHandler(client, parsedData);
	else if(parsedData[0] == "PART")
		partCommand(client, parsedData);
	else if (parsedData[0] == "QUIT")
		quitCommand(client, parsedData);
}

/* 
 * operatorCmdHandler
 * Processes operator commands: KICK, INVITE, TOPIC, MODE.
 */
void	Server::operatorCmdHandler(Client *client,
										std::vector<std::string>& parsedData) {
	if (parsedData[0] == "KICK")
		kickCommand(client, parsedData);
	else if (parsedData[0] == "INVITE")
		inviteCommand(client, parsedData);
	else if (parsedData[0] == "TOPIC")
		topicCommand(client, parsedData);
	else if (parsedData[0] == "MODE")
		modeCmdHandler(client, parsedData);
	return ;
}
