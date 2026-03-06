/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:01:29 by ikayiban          #+#    #+#             */
/*   Updated: 2026/03/06 09:18:58 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/* 
 * Channel
 * Default constructor that initializes a channel with all mode
 * flags disabled.
 */
Channel::Channel(void): _InviteOnlyFlag(false), _TopicFlag(false), 
_KeyFlag(false), _UserLimitFlag(false) {}

/* 
 * Channel
 * Constructor that creates a channel with a specified name
 * and default disabled modes.
 */
Channel::Channel(const std::string& name): _InviteOnlyFlag(false), 
_TopicFlag(false), _KeyFlag(false), _UserLimitFlag(false), _Name(name) {}

/* 
 * ~Channel
 * Destructor of the Channel class.
 */
Channel::~Channel() {}

/* 
 * getFlag
 * Returns the value of the requested channel mode flag.
 */
bool	Channel::getFlag(std::string whichFlag) {
	if (whichFlag == "_InviteOnlyFlag")
		return _InviteOnlyFlag;
	else if (whichFlag == "_TopicFlag")
		return _TopicFlag;
	else if (whichFlag == "_KeyFlag")
		return _KeyFlag;
	else if (whichFlag == "_UserLimitFlag")
		return _UserLimitFlag;
	else {
		displayLogs("FLAG ERROR", 0);
		return false;
	}
}

/* 
 * getName
 * Returns the name of the channel.
 */
std::string&    Channel::getName(void) {
    return _Name;
}

/* 
 * getTopic
 * Returns the current topic of the channel.
 */
std::string&	Channel::getTopic(void) {
	return _Topic;
}

/* 
 * getKey
 * Returns the key required to access the channel.
 */
std::string&	Channel::getKey(void) {
	return _Key;
}

/* 
 * getUserLimitNumber
 * Returns the user limit configured for the channel.
 */
std::string&	Channel::getUserLimitNumber(void) {
	return _UserLimitNumber;
}

/* 
 * getActiveModes
 * Builds and returns a string representing all active
 * channel modes and their parameters if needed.
 */
std::string	Channel::getActiveModes(void) {
	std::string	activeModes;

	if (_InviteOnlyFlag)
		activeModes += 'i';
	if (_TopicFlag)
		activeModes += 't';
	if (_KeyFlag)
		activeModes += 'k';
	if (_UserLimitFlag)
		activeModes += 'l';
	std::string::size_type found = activeModes.find('l');
	if (found!=std::string::npos) {
		activeModes += " ";
		activeModes += _UserLimitNumber;
	}
	return activeModes;
}

/* 
 * getMembersList
 * Returns a formatted list of channel members, marking
 * operators with '@'.
 */
std::string Channel::getMembersList() {
    std::string membersList = "";

    for (size_t i = 0; i < _ClientsList.size(); i++) {
        std::vector<Client* >::iterator it = std::find(_Operators.begin(), _Operators.end(), _ClientsList[i]);
        if (it != _Operators.end())
            membersList += "@";
        membersList += _ClientsList[i]->getNickname();

        if (i < _ClientsList.size() - 1)
            membersList += " ";
    }
    return membersList;
}

/* 
 * getClientsList
 * Returns the list of clients currently in the channel.
 */
std::vector<Client*>    Channel::getClientsList() {
    return _ClientsList;
}

/* 
 * getOperatorList
 * Returns the list of channel operators.
 */
std::vector<Client*>    Channel::getOperatorList() {
    return _Operators;
}

/* 
 * getClient
 * Searches for a client in the channel using 
 * its file descriptor.
 */
Client*	Channel::getClient(int clientFd) {
	std::vector<Client*>::iterator it = _ClientsList.begin();
	for (; it != _ClientsList.end(); ++it) {
		if ((*it)->getFd() == clientFd)
			return *it;
	}
	return NULL;
}

/* 
 * setTopic
 * Updates the topic of the channel.
 */
void	Channel::setTopic(std::string& topicName) {
	std::string newTopic;
	for (size_t i = 0; i < topicName.size(); i++)
		newTopic += topicName[i];
	_Topic = newTopic;
	return ;
}

/* 
 * setKey
 * Sets the key required to join the channel.
 */
void	Channel::setKey(std::string& key) {
	_Key = key;
	return ;
}

/* 
 * unsetKey
 * Removes the key protection from the channel.
 */
void	Channel::unsetKey(void) {
	_Key.erase();
	return ;
}

/* 
 * setUserLimitNumber
 * Sets the maximum number of users allowed in the channel.
 */
void	Channel::setUserLimitNumber(std::string& limit) {
	_UserLimitNumber = limit;
	return ;
}

/* 
 * setFlag
 * Updates the value of a specified channel mode flag.
 */
void	Channel::setFlag(std::string whichFlag, bool value) {
	if (whichFlag == "_InviteOnlyFlag")
		_InviteOnlyFlag = value;
	else if (whichFlag == "_TopicFlag")
		_TopicFlag = value;
	else if (whichFlag == "_KeyFlag")
		_KeyFlag = value;
	else if (whichFlag == "_UserLimitFlag")
		_UserLimitFlag = value;
	return ;
}

/* 
 * addClient
 * Adds a client to the list if it is not already present.
 */
void	Channel::addClient(Client *clt) {
    for (size_t i = 0; i < _ClientsList.size(); i++) {
        if (_ClientsList[i] == clt)
            return;
	}
    _ClientsList.push_back(clt);
	return ;
}

/* 
 * removeClient
 * Removes a client from the channel.
 */
void    Channel::removeClient(Client* clt) {
	std::vector<Client*>::iterator it = _ClientsList.begin();
    for (; it != _ClientsList.end(); ++it) {
        if (*it == clt) {
			_ClientsList.erase(it);
            return ;
		}
    }
}

/* 
 * addOperator
 * Adds a client who has been promoted to operator to the list
 * if it is not already present.
 */
void    Channel::addOperator(Client* clt) {
    for (size_t i = 0; i < _Operators.size(); i++) {
        if (_Operators[i] == clt)
            return ;
    }
    _Operators.push_back(clt);
}

/* 
 * removeOperator
 * Removes a client from the list of operators.
 */
void    Channel::removeOperator(Client* clt) {
	std::vector<Client*>::iterator it = _Operators.begin();
    for (; it != _Operators.end(); ++it) {
        if (*it == clt) {
			_Operators.erase(it);
            return ;
		}
    }
}

/* 
 * removeGuest
 * Removes a client from the list of guests (invite-only mode)
 * using its file descriptor.
 */
void	Channel::removeGuest(Client* client)
{
	std::vector<int>::iterator it = _GuestsList.begin();
    for (; it != _GuestsList.end(); ++it) {
        if (*it == client->getFd()) {
			_GuestsList.erase(it);
            return ;
		}
    }
}

/* 
 * addGuest
 * Adds a client to the guest list (invite-only mode).
 */
void    Channel::addGuest(int clientFd) {
    for (size_t i = 0; i < _GuestsList.size(); i++) {
        if (_GuestsList[i] == clientFd)
            return ;
    }
    _GuestsList.push_back(clientFd);
}

/* 
 * isClient
 * Checks if a client belongs to the channel.
 */
bool	Channel::isClient(int clientFd) {
	for (size_t i = 0; i < _ClientsList.size(); i++) {
		if (_ClientsList[i]->getFd() == clientFd)
			return true;
	}
	return false;
}

/* 
 * isOperator
 * Checks if a client has operator privileges in the channel.
 */
bool	Channel::isOperator(int clientFd) {
	for (size_t i = 0; i < _Operators.size(); i++) {
		if (_Operators[i]->getFd() == clientFd)
			return true;
	}
	return false;
}

/* 
 * isGuest
 * Checks if a client received an invitation 
 * to the channel (invite-only mode).
 */
bool	Channel::isGuest(int clientFd) {
	std::vector<int>::iterator	it = _GuestsList.begin();
	std::vector<int>::iterator	ite = _GuestsList.end();

	for (; it != ite; ++it) {
		if (*it == clientFd)
			return true;
	}
	return false;
}

/* 
 * hasKey
 * Indicates whether the channel is protected by a key.
 */
bool Channel::hasKey(void) const {
    return !_Key.empty();
}

/* 
 * checkKey
 * Validates whether a provided key matches the channel key.
 */
bool Channel::checkKey(const std::string& key) const {
    return _Key.empty() || _Key == key;
}

/* 
 * hasMode
 * Checks if a specific mode is currently active on the channel.
 */
bool Channel::hasMode(char mode) const {
    return _Modes.find(mode) != std::string::npos;
}

