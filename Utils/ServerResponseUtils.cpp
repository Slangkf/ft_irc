// /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponseUtils.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@42paris.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:36:30 by tclouet           #+#    #+#             */
/*   Updated: 2026/02/20 09:26:32 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

/* 
 * authenticationSuccessful
 * Sends the standard 001–004 numeric replies
 * to a client upon successful authentication.
 */
void	authenticationSuccessful(Client *client) {
	std::vector<std::string>	responses;
	std::string	r001 = RPLWelcome(client);
	responses.push_back(r001);
	std::string	r002 = RPLYourHost(client);
	responses.push_back(r002);
	std::string	r003 = RPLCreated(client);
	responses.push_back(r003);
	std::string	r004 = RPLMyInfo(client);
	responses.push_back(r004);
	for (int i = 0; i < 4; i++) {
		ssize_t bytes_sent = send(client->getFd(), responses[i].c_str(), responses[i].size(), 0);
		if (bytes_sent == -1)
			throw(std::runtime_error("\e[1;31m(ERROR) authenticationSuccessful: send() failed"));
	}
	return ;
}

/* 
 * RPLChannelModeIs
 * Sends numeric 324 showing the current modes
 * of a specified channel.
 */
void	Server::RPLChannelModeIs(Client *client, std::string& parameter) {
	Channel *channel = getChannelByName(parameter);
	std::string r324 = ":";
	r324 += SERVER_NAME;
	r324 += " 324 ";
	r324 += client->getNickname();
	r324 += " ";
	r324 += parameter;
	r324 += " +";
	if (channel->getActiveModes().empty())
		r324 += CRLF;
	else {	
		r324 += channel->getActiveModes();
		r324 += CRLF;
	}
	ssize_t bytes_sent = send(client->getFd(), r324.c_str(), r324.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) RPLChannelModeIs: send() failed"));
	return ;
}

/* 
 * joinSuccessful
 * Sends topic and member list information (353 and 366)
 * when a client joins a channel.
 * Calls RPLNoTopic or RPLTopic depending on whether
 * a topic exists.
 */
void	joinSuccessful(Client* client, Channel* channel) {
	std::vector<std::string> responses;
	if (channel->getTopic().empty())
		RPLNoTopic(client, channel->getName());
	else {
		std::vector<std::string> parameters;
		parameters.push_back("TOPIC");
		parameters.push_back(channel->getName());
		parameters.push_back(channel->getTopic());
		RPLTopic(client, parameters, channel);
	}
	std::string r353 = RPLNameReply(client, channel);
	responses.push_back(r353);
	std::string r366 = RPLEndOfNames(client, channel->getName());
	responses.push_back(r366);
	for (size_t i = 0; i < responses.size(); i++) {
		ssize_t bytes_sent = send(client->getFd(), responses[i].c_str(), responses[i].size(), 0);
		if (bytes_sent == -1)
			throw(std::runtime_error("\e[1;31m(ERROR) joinSuccessful: send() failed"));
	}
	return ;
}

/* 
 * RPLNoTopic / RPLTopic
 * Sends numeric 331 or 332 for channel topic information.
 */
void	RPLNoTopic(Client *client, std::string& parameter) {
	std::string r331 = ":";
	r331 += SERVER_NAME;
	r331 += " 331 ";
	r331 += client->getNickname();
	r331 += " ";
	r331 += parameter;
	r331 += " :No topic is set";
	r331 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r331.c_str(), r331.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) RPLNoTopic: send() failed"));
	return ;
}

void	RPLTopic(Client *client, std::vector<std::string>& parameters, Channel *channel) {
	std::string r332 = ":";
	r332 += SERVER_NAME;
	r332 += " 332 ";
	r332 += client->getNickname();
	r332 += " ";
	r332 += parameters[1];
	r332 += " :";
	if (!channel->getTopic().empty())
		r332 += channel->getTopic();
	r332 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r332.c_str(), r332.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) RPLTopic: send() failed"));
	return ;
}

/* 
 * RPLInviting
 * Sends numeric 341 to both the sender and the target client
 * for an INVITE command.
 * Uses helper functions senderResponse and targetResponse
 * to format messages.
 */
static std::string	senderResponse(Client *client, std::vector<std::string>& parameters) {
	std::string sender = ":";
	sender += SERVER_NAME;
	sender += " 341 ";
	if (client->getNickname().empty())
		sender += "* ";
	else {
		sender += client->getNickname();
		sender += " ";
	}
	sender += parameters[2];
	sender += " ";
	sender += parameters[1];
	sender += CRLF;
	return sender;
}

static std::string	targetResponse(Client *client, std::vector<std::string>& parameters) {
	std::string target = ":";
	target += client->getNickname();
	target += "!";
	target += client->getUsername();
	target += "@";
	target += client->getIPAddress();
	target += " INVITE ";
	target += parameters[1];
	target += " ";
	target += parameters[2];
	target += CRLF;
	return target;
}

void	RPLInviting(Client *client, std::vector<std::string>& parameters,
						int targetFd) {
	std::string r341 = senderResponse(client, parameters);
	ssize_t bytes_sent = send(client->getFd(), r341.c_str(), r341.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) RPLInviting: first send() failed"));
	std::string r341_target = targetResponse(client, parameters);
	bytes_sent = send(targetFd, r341_target.c_str(), r341_target.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) RPLInviting: second send() failed"));
	return ;
}

/* 
 * noSuchNickname / noSuchChannel
 * / cannotSendToChan / noRecipient / noTextToSend
 * Sends numeric 401, 403, 404, 411, 412 respectively
 * for various error conditions.
 */
void	noSuchNickname(Client *client, std::string& parameter) {
	std::string r401 = ":";
	r401 += SERVER_NAME;
	r401 += " 401 ";
	if (client->getNickname().empty())
		r401 += "* ";
	else {
		r401 += client->getNickname();
		r401 += " ";
	}
	r401 += parameter;
	r401 += " :No such nick/channel";
	r401 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r401.c_str(), r401.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) noSuchNickname: send() failed"));
	return ;
}

void	noSuchChannel(Client *client, std::string& parameter) {
	std::string r403 = ":";
	r403 += SERVER_NAME;
	r403 += " 403 ";
	if (client->getNickname().empty())
		r403 += "* ";
	else {
		r403 += client->getNickname();
		r403 += " ";
	}
	r403 += parameter;
	r403 += " :No such channel";
	r403 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r403.c_str(), r403.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) noSuchChannel: send() failed"));
	return ;
}

void    cannotSendToChan(Client *client, std::string channelName) {
    std::string r404 = ":";
    r404 += SERVER_NAME;
    r404 += " 404 ";
    r404 += client->getNickname();
    r404 += " ";
    r404 += channelName;
    r404 += " :Cannot send to channel\r\n";

    ssize_t bytes_sent = send(client->getFd(), r404.c_str(), r404.size(), 0);
    if (bytes_sent == -1)
        throw(std::runtime_error("\e[1;31m(ERROR) cannotSendToChan: send() failed"));
}

void	noRecipient(Client *client) {
	std::string r411 = ":";
	r411 += SERVER_NAME;
	r411 += " 411 ";
	if (client->getNickname().empty())
		r411 += "* ";
	else {
		r411 += client->getNickname();
		r411 += " ";
	}
	r411 += " :No recipient given";
	r411 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r411.c_str(), r411.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) noRecipient: send() failed"));
	return ;
}

void	noTextToSend(Client *client) {
	std::string r412 = ":";
	r412 += SERVER_NAME;
	r412 += " 412 ";
	if (client->getNickname().empty())
		r412 += "* ";
	else {
		r412 += client->getNickname();
		r412 += " ";
	}
	r412 += " :No text to send";
	r412 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r412.c_str(), r412.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) noTextToSend: send() failed"));
	return ;
}


/* 
 * invalidNickFormat / nickInUse 
 * / noUserInChannel / notOnThatChannel / alreadyOnChannel
 * Sends numeric 432, 433, 441, 442, 443 for nickname
 * and channel errors.
 */
void	invalidNickFormat(Client *client, std::string& parameter) {
	std::string r432 = ":";
	r432 += SERVER_NAME;
	r432 += " 432 ";
	if (client->getNickname().empty())
		r432 += "* ";
	else {
		r432 += client->getNickname();
		r432 += " ";
	}
	r432 += parameter;
	r432 += " :Erroneous nickname";
	r432 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r432.c_str(), r432.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) invalidNickFormat: send() failed"));
	return ;
}

void	nickInUse(Client *client, std::string& parameter) {
	std::string r433 = ":";
	r433 += SERVER_NAME;
	r433 += " 433 ";
	if (client->getNickname().empty())
		r433 += "* ";
	else {
		r433 += client->getNickname();
		r433 += " ";
	}
	r433 += parameter;
	r433 += " :Nickname is already in use";
	r433 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r433.c_str(), r433.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) nickInUse: send() failed"));
	return ;
}

void	noUserInChannel(Client *client, std::vector<std::string>& parameters) {
	std::string r441 = ":";
	r441 += SERVER_NAME;
	r441 += " 441 ";
	if (client->getNickname().empty())
		r441 += "* ";
	else {
		r441 += client->getNickname();
		r441 += " ";
	}
	r441 += parameters[2];
	r441 += " ";
	r441 += parameters[1];
	r441 += " :They aren't on that channel";
	r441 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r441.c_str(), r441.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) noUserInChannel: send() failed"));
	return ;
}

void	notOnThatChannel(Client *client, std::string& parameter) {
	std::string r442 = ":";
	r442 += SERVER_NAME;
	r442 += " 442 ";
	if (client->getNickname().empty())
		r442 += "* ";
	else {
		r442 += client->getNickname();
		r442 += " ";
	}
	r442 += parameter;
	r442 += " :You're not on that channel";
	r442 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r442.c_str(), r442.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) notOnThatChannel: send() failed"));
	return ;
}

void	alreadyOnChannel(Client *client, std::string& parameter) {
	std::string r443 = ":";
	r443 += SERVER_NAME;
	r443 += " 443 ";
	if (client->getNickname().empty())
		r443 += "* ";
	else {
		r443 += client->getNickname();
		r443 += " ";
	}
	r443 += client->getUsername();
	r443 += " ";
	r443 += parameter;
	r443 += " :is already on channel";
	r443 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r443.c_str(), r443.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) alreadyOnChannel: send() failed"));
	return ;
}

/* 
 * notRegistered / needMoreParams 
 * / alreadyRegistered / passwdMismatch
 * Sends numeric 451, 461, 462, 464 for registration
 * and command validation errors.
 */
void	notRegistered(Client *client) {
	std::string r451 = ":";
	r451 += SERVER_NAME;
	r451 += " 451 ";
	if (client->getNickname().empty())
		r451 += "* ";
	else {
		r451 += client->getNickname();
		r451 += " ";
	}
	r451 += ":You have not registered";
	r451 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r451.c_str(), r451.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) notRegistered: send() failed"));
	return ;
}

void	needMoreParams(Client *client, std::string& parameter) {
	std::string r461 = ":";
	r461 += SERVER_NAME;
	r461 += " 461 ";
	if (client->getNickname().empty())
		r461 += "* ";
	else {
		r461 += client->getNickname();
		r461 += " ";
	}
	r461 += parameter;
	r461 += " :Not enough parameters";
	r461 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r461.c_str(), r461.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) needMoreParams: send() failed"));
	return ;
}

void	alreadyRegistered(Client *client) {
	std::string r462 = ":";
	r462 += SERVER_NAME;
	r462 += " 462 ";
	if (client->getNickname().empty())
		r462 += "* ";
	else {
		r462 += client->getNickname();
		r462 += " ";
	}
	r462 += ":Unauthorized command (already registered)";
	r462 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r462.c_str(), r462.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) alreadyRegistered: send() failed"));
	return ;
}

void	passwdMismatch(Client *client) {
	std::string r464 = ":";
	r464 += SERVER_NAME;
	r464 += " 464 ";
	if (client->getNickname().empty())
		r464 += "* ";
	else {
		r464 += client->getNickname();
		r464 += " ";
	}
	r464 += ":Password incorrect";
	r464 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r464.c_str(), r464.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) passwdMissMatch: send() failed"));
	return ;
}

/* 
 * keyAlreadySet / channelFull / unknownMode 
 * / inviteOnly / badChannelKey / notChannelOperator
 * Sends numeric 467, 471, 472, 473, 475, 482 for channel mode
 * and privilege errors.
 * Includes helper badChar() for unknown mode character.
 */
void	keyAlreadySet(Client *client, std::string& channelName) {
	std::string r467 = ":";
	r467 += SERVER_NAME;
	r467 += " 467 ";
	if (client->getNickname().empty())
		r467 += "* ";
	else {
		r467 += client->getNickname();
		r467 += " ";
	}
	r467 += channelName;
	r467 += " :Channel key already set";
	r467 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r467.c_str(), r467.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) keyAlreadySet: send() failed"));
	return ;
}

void	channelFull(Client *client, std::string& channelName) {
	std::string r471 = ":";
	r471 += SERVER_NAME;
	r471 += " 471 ";
	r471 += client->getNickname();
	r471 += " ";
	if (!channelName.empty())
		r471 += channelName;
	r471 += " :Cannot join channel (+l)";
	r471 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r471.c_str(), r471.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) channelFull: send() failed"));
	return ;
}

static char badChar(std::string& parameters){
	size_t i = 0;
	for (; i < parameters.size(); i++) {
		if (parameters[i] != '+' && parameters[i] != '-' &&
			parameters[i] != 'i' && parameters[i] != 't' &&
			parameters[i] != 'k' && parameters[i] != 'o' && parameters[i] != 'l')
			break ;
	}
	return parameters[i];
}

void	unknownMode(Client *client, std::vector<std::string>& parameters) {
	std::string r472 = ":";
	r472 += SERVER_NAME;
	r472 += " 472 ";
	r472 += client->getNickname();
	r472 += " ";
	r472 += badChar(parameters[2]);
	r472 += " :is unknown mode char to me for ";
	r472 += parameters[1];
	r472 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r472.c_str(), r472.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) unknownMode: send() failed"));
	return ;
}

void	inviteOnly(Client* client, Channel* channel) {
    std::string r473 = ":";
    r473 += SERVER_NAME;
    r473 += " 473 ";
    r473 += client->getNickname();
    r473 += " ";
	if (!channel->getName().empty())
    	r473 += channel->getName();
    r473 += " :Cannot join channel (+i)";
	r473 += CRLF;
    ssize_t bytes_sent = send(client->getFd(), r473.c_str(), r473.size(), 0);
    if (bytes_sent == -1)
        throw(std::runtime_error("\e[1;31m(ERROR) inviteOnly: send() failed"));
}

void badChannelKey(Client *client, Channel* channel) {
    std::string r475 = ":";
    r475 += SERVER_NAME;
    r475 += " 475 ";
    r475 += client->getNickname();
    r475 += " ";
	if (!channel->getName().empty())
    	r475 += channel->getName();
    r475 += " :Cannot join channel (+k)";
    r475 += CRLF;
    ssize_t bytes_sent = send(client->getFd(), r475.c_str(), r475.size(), 0);
    if (bytes_sent == -1)
        throw(std::runtime_error("\e[1;31m(ERROR) badChannelKey: send() failed"));
}

void	notChannelOperator(Client *client, std::string& parameter) {
	std::string r482 = ":";
	r482 += SERVER_NAME;
	r482 += " 482 ";
	if (client->getNickname().empty())
		r482 += "* ";
	else {
		r482 += client->getNickname();
		r482 += " ";
	}
	if (!parameter.empty())
		r482 += parameter;
	r482 += " :You're not channel operator";
	r482 += CRLF;
	ssize_t bytes_sent = send(client->getFd(), r482.c_str(), r482.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) notChannelOperator: send() failed"));
	return ;
}

/* 
 * sendToEveryone / nickChange / joinForAll 
 * / operatorRights / kickUser / changeTopic
 * Broadcasts user or server actions (nickname change, join, kick,
 * topic change, operator changes) to all relevant clients
 * on affected channels.
 */
static void	sendToEveryone(Client *client, std::string&	response, std::vector<Channel*>&	channels) {
	std::vector<int>		fds;
	for (size_t i = 0; i < channels.size(); i++) {
		std::vector<Client*> list = channels[i]->getClientsList();
		for (size_t j = 0; j < list.size(); j++) {
			std::vector<int>::iterator it = std::find(fds.begin(), fds.end(), list[j]->getFd());
			if (it == fds.end() && list[j]->getFd() != client->getFd())
				fds.push_back(list[j]->getFd());
		}
	}
	for (size_t i = 0; i < fds.size(); i++) {
		if (send(fds[i], response.c_str(), response.size(), 0) == -1)
			throw(std::runtime_error("\e[1;31m(ERROR) nickChange: send() failed"));
	}
	if (send(client->getFd(), response.c_str(), response.size(), 0) == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) nickChange: second send() failed"));
}

void	Server::nickChange(Client *client, std::string& parameter) {
	std::vector<Channel*>	channels = getChannelsByClient(client);
	std::string	response = ":";
	response += client->getNickname();
	response += "!";
	response += client->getUsername();
	response += "@";
	response += client->getIPAddress();
	response += " NICK ";
	response += parameter;
	response += CRLF;
	sendToEveryone(client, response, channels);
	return ;
}

void	joinForAll(Client* client, Channel* channel) {
    std::string msg = ":" + client->getNickname() + "!" + client->getUsername() +
		"@" + client->getIPAddress() + " JOIN :" + channel->getName() + "\r\n";
    
    const std::vector<Client*>& clients = channel->getClientsList();
    for (size_t i = 0; i < clients.size(); i++) {
        Client* targetClient = clients[i];
        if (targetClient && targetClient->getFd() != -1) {
            if (send(targetClient->getFd(), msg.c_str(), msg.size(), 0) == -1)
                std::cerr << "Failed to send JOIN to " << targetClient->getNickname() << std::endl;
        }
    }
}

void	operatorRights(Client *client, std::vector<std::string>& parsedData,
						Channel *channel) {
	if (!channel)
		return ;
	std::vector<Client*> clients = channel->getClientsList();
	std::vector<Client*>::iterator it = clients.begin();
	std::vector<Client*>::iterator ite = clients.end();

	std::string	response = ":";
	response += client->getNickname();
	response += "!";
	response += client->getUsername();
	response += "@";
	response += client->getIPAddress();
	response += " MODE ";
	response += channel->getName();
	if (parsedData[2][0] == '+')
		response += " +o ";
	else
		response += " -o ";
	response += parsedData[3];
	response += CRLF;
	for (; it != ite; ++it) {
		ssize_t bytes_sent = send((*it)->getFd(), response.c_str(), response.size(), 0);
		if (bytes_sent == -1)
			throw(std::runtime_error("\e[1;31m(ERROR) operatorRights: send() failed"));
	}
}

void	kickUser(Client *client, std::vector<std::string>& parsedData,
					Channel *channel) {
	std::vector<Client*> clients = channel->getClientsList();
	std::vector<Client*>::iterator it = clients.begin();
	std::vector<Client*>::iterator ite = clients.end();

	std::string response = ":";
	response += client->getNickname();
	response += "!";
	response += client->getUsername();
	response += "@";
	response += client->getIPAddress();
	response += " KICK ";
	if (!channel->getName().empty())
		response += channel->getName();
	response += " ";
	response += parsedData[2];
	if (parsedData.size() == 4) {
		response += " ";
		response += parsedData[3];
	}
	else {
		response += " :";
		response += parsedData[2];
	}
	response += CRLF;
	for (; it != ite; ++it) {
		ssize_t bytes_sent = send((*it)->getFd(), response.c_str(), response.size(), 0);
		if (bytes_sent == -1)
			throw(std::runtime_error("\e[1;31m(ERROR) kickUser: send() failed"));
	}
	return ;
}

void	changeTopic(Client *client, Channel *channel) {
	std::vector<Client*> clients = channel->getClientsList();
	std::vector<Client*>::iterator it = clients.begin();
	std::vector<Client*>::iterator ite = clients.end();

	std::string response = ":";
	response += client->getNickname();
	response += "!";
	response += client->getUsername();
	response += "@";
	response += client->getIPAddress();
	response += " TOPIC ";
	if (!channel->getName().empty())
		response += channel->getName();
	response += " :";
	if (!channel->getTopic().empty())
		response += channel->getTopic();
	response += CRLF;
	for (; it != ite; ++it) {
		ssize_t bytes_sent = send((*it)->getFd(), response.c_str(), response.size(), 0);
		if (bytes_sent == -1)
			throw(std::runtime_error("\e[1;31m(ERROR) changeTopic: send() failed"));
	}
	return ;
}

/* 
 * pongResponse
 * Replies to a PING client request with a PONG message.
 */
void	pongResponse(Client *client, std::string& parameter) {
	std::string response = "PONG ";
	response += parameter;
	response += CRLF;
	ssize_t bytes_sent = send(client->getFd(), response.c_str(), response.size(), 0);
	if (bytes_sent == -1)
		throw(std::runtime_error("\e[1;31m(ERROR) pongResponse: send() failed"));
	return ;
}


/* 
 * RPLWelcome / RPLYourHost / RPLCreated
 * / RPLMyInfo / RPLNameReply / RPLEndOfNames
 * Construct numeric reply strings 001–004, 353, 366 for standard
 * IRC client-server protocol messages.
 */
std::string	RPLWelcome(Client *client) {
	std::string r001 = ":";
	r001 += SERVER_NAME;
	r001 += " 001 ";
	r001 += client->getNickname();
	r001 += " :Welcome to the Internet Relay Network ";
	r001 += client->getNickname();
	r001 += "!";
	r001 += client->getUsername();
	r001 += "@";
	r001 += client->getIPAddress();
	r001 += "\r\n";
	return r001;
}

std::string	RPLYourHost(Client *client) {
	std::string r002 = ":";
	r002 += SERVER_NAME;
	r002 += " 002 ";
	r002 += client->getNickname();
	r002 += " :Your host is ";
	r002 += SERVER_NAME;
	r002 += ", running version ";
	r002 += VERSION;
	r002 += CRLF;
	return r002;
}

std::string	RPLCreated(Client *client) {
	std::string r003 = ":";
	r003 += SERVER_NAME;
	r003 += " 003 ";
	r003 += client->getNickname();
	r003 += " :This server was created in ";
	r003 += DATE;
	r003 += CRLF;
	return r003;
}

std::string	RPLMyInfo(Client *client) {
	std::string r004 = ":";
	r004 += SERVER_NAME;
	r004 += " 004 ";
	r004 += client->getNickname();
	r004 += " ";
	r004 += SERVER_NAME;
	r004 += " version ";
	r004 += VERSION;
	r004 += " user modes <none> channel modes <itkol>";
	r004 += CRLF;
	return r004;
}

std::string RPLNameReply(Client* client, Channel* channel) {
	std::string r353 = ":";
	r353 += SERVER_NAME;
	r353 += " 353 ";
	r353 += client->getNickname();
	r353 += " = ";
	r353 += channel->getName();
	r353 += " :";
	r353 += channel->getMembersList();
	r353 += "\r\n";
	return r353;
}

std::string RPLEndOfNames(Client *client, std::string channelName) {
	std::string r366 = ":";
	r366 += SERVER_NAME;
	r366 += " 366 ";
	r366 += client->getNickname();
	r366 += " ";
	r366 += channelName;
	r366 += " :End of /NAMES list\r\n";
	return r366;
}