/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 13:48:01 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 10:36:19 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Clients/Client.hpp"
#include "Channels/Channel.hpp"
#include "Utils/Utils.hpp"

#include <iostream>
#include <cstring>
#include <cctype>
#include <exception>
#include <vector>
#include <deque>
#include <map>

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>

#define GREEN	"\e[1;32m"
#define RED		"\e[1;31m"
#define YELLOW	"\e[1;33m"
#define WHITE	"\e[0;37m"

class Client;
class Channel;

class Server {
	public:
		Server(char *port, char *password);
		~Server();

		// SERVER HANDLER
		void	initialization(void);
		void	start(void);
		void	acceptNewClient(void);

		// DATA HANDLER
		void	receiveData(int clientFd);
		void	serverResponse(const std::string& code, Client *client, std::vector<std::string>& parsedData, Channel* channel);
		void	authenticationHandler(Client *client, std::vector<std::string>& parsedData);
		void	incomingDataHandler(Client* client, std::vector<std::string>& parsedData);
		bool	isFlooding(Client *client);

		// COMMANDS
		bool	passwordHandler(Client *client, std::vector<std::string>& parsedData);
		bool	nicknameHandler(Client *client, std::vector<std::string>& parsedData);
		bool	usernameHandler(Client *client, std::vector<std::string>& parsedData);
		void	userCmdHandler(Client *client, std::vector<std::string>& parsedData);
		void	joinHandler(Client *client, std::vector<std::string>& parsedData);
		void	operatorCmdHandler(Client *client, std::vector<std::string>& parsedData);
		void	kickCommand(Client *client, std::vector<std::string>& parsedData);
		void	inviteCommand(Client *client, std::vector<std::string>& parsedData);
		void	topicCommand(Client *client, std::vector<std::string>& parsedData);
		void	modeCmdHandler(Client *client, std::vector<std::string>& parsedData);
		void    privmsgHandler(Client* client, std::vector<std::string>& parsedData);
		void    partCommand(Client* client, std::vector<std::string>& parsedData);
		void    quitCommand(Client* client, std::vector<std::string>& parsedData);
		std::vector<std::string>	toList(const std::string& channelsName, Client* client, bool isKey);
		
		// DATA HANDLER UTILS
		bool	nickAlreadyExists(const std::string& nick);
		bool	channelExists(const std::string& name);
		bool	userIsOnChannel(int clientFd, std::string& channelName);
		void	RPLChannelModeIs(Client *client, std::string& parameter);
		void	setChannelModes(Client *client, Channel *channel, std::vector<std::string>& parsedData);
		void	setModesWithParams(Client *client, Channel *channel, std::pair<const char,std::string>& mode,
										std::vector<std::string>& parsedData);
		void	unsetChannelModes(Client *client, Channel *channel, std::vector<std::string>& parsedData);
		void	nickChange(Client *client, std::string& parameter);

		// PROGRAM HANDLER
		void		broadcast(Client* client, std::string msg, Channel* channel, std::string& cmd);
		void		removeClient(int fd);
		void		closeFd(void);
		static void	signalHandler(int signum);

		// GETTERS
		int						getClientFd(std::string& nickname);
		Client*					getClientByFd(int clientFd);
		Channel*				getChannelByName(std::string& channelName);
		std::vector<Channel*>	getChannelsByClient(Client *client);
	private:
		static bool						_Signal;
		int								_ServSockFd;
		unsigned int					_Port;
		unsigned long long				_ServerTimeCount;
		std::string						_Password;
		std::vector<struct pollfd>		_FdSet;
		std::deque<Client>				_ClientSet;
		std::map<std::string, Channel>	_ChannelList;
};

#endif
