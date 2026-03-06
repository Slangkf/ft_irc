/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 15:41:15 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/05 09:29:43 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "../Server.hpp"
#include "../Clients/Client.hpp"

#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

#define RED		"\e[1;31m"
#define YELLOW	"\e[1;33m"
#define BLUE	"\e[1;34m"
#define WHITE	"\e[0;37m"

#define SERVER_NAME	"irc.IgorTomServer.com"
#define VERSION		"1.0.0"
#define DATE		"February 2026"
#define	CRLF		"\r\n"

class Server;
class Client;
class Channel;

// PARSING
bool	checkArgs(int argc, char *port, char *password);
bool	checkPort(char *port);
bool	checkPassword(char *password);

// SERVER RESPONSES
void	authenticationSuccessful(Client *client);
void	RPLNoTopic(Client *client, std::string& parameter);
void	RPLTopic(Client *client, std::vector<std::string>& parameters, Channel *channel);
void	RPLInviting(Client *client, std::vector<std::string>& parameter, int targetFd);

void	noSuchNickname(Client *client, std::string& parameter);
void	noSuchChannel(Client *client, std::string& parameter);
void    cannotSendToChan(Client *client, std::string channelName);
void	noRecipient(Client *client);
void	noTextToSend(Client *client);
void	invalidNickFormat(Client *client, std::string& command);
void	nickInUse(Client *client, std::string& command);
void	noUserInChannel(Client *client, std::vector<std::string>& parameters);
void	notOnThatChannel(Client *client, std::string& parameter);
void	alreadyOnChannel(Client *client, std::string& parameter);
void	notRegistered(Client *client);
void	needMoreParams(Client *client, std::string& command);
void	alreadyRegistered(Client *client);
void	passwdMismatch(Client *client);
void	keyAlreadySet(Client *client, std::string& channelName);
void	channelFull(Client *client, std::string& channelName);
void	unknownMode(Client *client, std::vector<std::string>& parameters);
void	inviteOnly(Client* client, Channel* channel);
void    badChannelKey(Client *client, Channel* channel);
void	notChannelOperator(Client *client, std::string& parameter);
void	joinForAll(Client* client, Channel* channel);
void	joinSuccessful(Client* client, Channel* channel);
void	operatorRights(Client *client, std::vector<std::string>& parsedData,
						Channel *channel);
void	kickUser(Client *client, std::vector<std::string>& parsedData,
    				Channel *channel);
void	changeTopic(Client *client, Channel *channel);
void	pongResponse(Client *client, std::string& parameter);

std::string		RPLWelcome(Client *client);
std::string		RPLYourHost(Client *client);
std::string		RPLCreated(Client *client);
std::string		RPLMyInfo(Client *client);
std::string     RPLNameReply(Client* client, Channel* channel);
std::string     RPLEndOfNames(Client *client, std::string channelName);

// LOGS
void	displayDataReceived(std::vector<std::string>& parsedData);
void	displayLogs(std::string action, int clientFd);

// UTILS
void	*ft_memset(void *s, int c, size_t n);
int		channelCount(std::string& channel);
int		modesWithParams(std::string& modes);
bool	isOperCommand(std::string& command);
bool	validNickname(const std::string& nick);
bool    correctChannelFormat(const std::string& channelName);
bool	isValidMode(std::string& mode);
bool	isDigit(std::string& value);
bool	isRequestToIgnore(Client *client, std::vector<std::string>& parsedData);
std::vector<std::string>	parseDataReceived(std::string& input);
std::map<char, std::string>	setModeMap(std::vector<std::string>& input);

#endif