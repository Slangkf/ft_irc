/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:01:36 by ikayiban          #+#    #+#             */
/*   Updated: 2026/03/05 11:01:50 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Clients/Client.hpp"
#include <map>
#include <iostream>
#include <vector>

class Client;

class Channel {
	public:
        Channel();
        Channel(const std::string& name);
        ~Channel();

		// SETTERS
		void	setTopic(std::string& topicName);
		void	setKey(std::string& key);
		void	unsetKey(void);
		void	setUserLimitNumber(std::string& limit);
		void	setFlag(std::string whichFlag, bool value);
	
		// GETTERS
		bool					getFlag(std::string whichFlag);
        std::string&			getName(void);
		std::string&			getTopic(void);
		std::string&			getKey(void);
		std::string&			getUserLimitNumber(void);
		std::string				getActiveModes(void);
		std::string    			getMembersList(void);
		Client*					getClient(int clientFd);
        std::vector<Client*>    getClientsList(void);
        std::vector<Client*>    getOperatorList(void);

		// UTILS
        void	addClient(Client* clt);
		void    removeClient(Client* clt);
		void    addOperator(Client* clt);
		void    removeGuest(Client* clt);
		void    removeOperator(Client* clt);
		void	addGuest(int clientFd);
		bool	isClient(int clientFd);
		bool	isOperator(int clientFd);
		bool	isGuest(int clientFd);
		bool	hasKey(void) const;
		bool 	checkKey(const std::string& key) const;
		bool	hasMode(char mode) const;
    private:
		bool						_InviteOnlyFlag;
		bool						_TopicFlag;
		bool						_KeyFlag;
		bool						_UserLimitFlag;
		std::string					_UserLimitNumber;
        std::string					_Name;
		std::string					_Topic;
		std::string					_Key;
		std::string					_Modes;
        std::vector<Client*>		_ClientsList;
        std::vector<Client*>		_Operators;
		std::vector<int>			_GuestsList;

};

#endif