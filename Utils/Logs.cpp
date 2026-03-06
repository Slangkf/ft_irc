/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:35:33 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 09:36:36 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

/* 
 * displayDataReceived
 * Prints parsed command data to the console
 * for debugging purposes.
 */
void	displayDataReceived(std::vector<std::string>& parsedData) {
	if (parsedData.empty())
		return ;
	for (size_t i = 0; i < parsedData.size(); i++) {
		std::cout << BLUE << "Command [" << i << "]: ";
		std::cout << parsedData[i] << WHITE << std::endl;
	}
}

/* 
 * displayLogs
 * Displays server events, errors, and client actions
 * with color-coded messages for debugging purposes.
 */
void	displayLogs(std::string action, int clientFd) {
	if (action == "(SERVER STARTED)") {
		std::cout << GREEN << "(SERVER STARTED) " << YELLOW;
		std::cout << "Waiting for a connection..." << WHITE << std::endl;
		return ;
	}
	else if (action == "(SERVER CLOSED)") {
		std::cout << GREEN << "(SERVER CLOSED)" << WHITE << std::endl;
		return ;
	}
	else if (action == "is connected") {
		std::cout << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd << " authenticated.";
		std::cout <<  WHITE << std::endl;
		return ;
	}
	else if (action == "has quit") {
		std::cout << RED << "(CONNECTION CLOSED) " << YELLOW;
		std::cout << "Client number " << clientFd << " has quit.";
		std::cout << WHITE << std::endl;
		return ;
	}
	else if (action == "not registered") {
		std::cout << RED << "(ERROR) " << YELLOW;
		std::cout << "Client number " << clientFd << " is not registered.";
		std::cout <<  WHITE << std::endl;
		return ;
	}
	else if (action == "anti flooding activated") {
		std::cout << RED << "(ALERT) " << YELLOW;
		std::cout << "Client number " << clientFd << " is spamming.";
		std::cout <<  WHITE << std::endl;
		return ;
	}
	else if (action == "has been ejected") {
		std::cout << RED << "(CONNECTION CLOSED) " << YELLOW;
		std::cout << "Client number " << clientFd << " has been ejected from the server.";
		std::cout << WHITE << std::endl;
		return ;
	}
	else if (action == "channel created") {
		std::cout << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "A channel has been created." <<  WHITE << std::endl;
		return ;
	}
	else if (action == "PASS: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PASS: missing parameter." << WHITE << std::endl;
		return ;
	}
	else if (action == "PASS: already registered") {
		std::cerr << RED << "(ERROR 462) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PASS: already registered." << WHITE << std::endl;
	}
	else if (action == "PASS: incorrect password") {
		std::cerr << RED << "(ERROR 464) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PASS: incorrect password." << WHITE << std::endl;
	}
	else if (action == "NICK: has changed") {
		std::cout << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cout << " has changed his nickname." << WHITE << std::endl;
		return ;
	}
	else if (action == "NICK: no such channel") {
		std::cerr << RED << "(ERROR 403) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": NICK: no such channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "NICK: invalid format") {
		std::cerr << RED << "(ERROR 432) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": NICK: invalid format." << WHITE << std::endl;
		return ;
	}
	else if (action == "NICK: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": NICK: missing parameter." << WHITE << std::endl;
		return ;
	}
	else if (action == "NICK: missing password") {
		std::cerr << RED << "(ERROR 464) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": NICK: server pass. required first." << WHITE << std::endl;
		return ;
	}
	else if (action == "USER: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": USER: missing parameter." << WHITE << std::endl;
		return ;
	}
	else if (action == "USER: already registered") {
		std::cerr << RED << "(ERROR 462) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": USER: already registered." << WHITE << std::endl;
		return ;
	}
	else if (action == "USER: missing password") {
		std::cerr << RED << "(ERROR 464) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": USER: server pass. required first." << WHITE << std::endl;
		return ;
	}
	else if (action == "KICK: has been kicked") {
		std::cout << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cout << " has been kicked." << WHITE << std::endl;
		return ;
	}
	else if (action == "KICK: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": KICK: missing parameter." << WHITE << std::endl;
		return ;
	}
	else if (action == "KICK: no such channel") {
		std::cerr << RED << "(ERROR 403) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": KICK: no such channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "KICK: no user in the channel") {
		std::cerr << RED << "(ERROR 441) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": KICK: user is not on that chan." << WHITE << std::endl;
		return ;
	}
	else if (action == "KICK: you're not on that channel") {
		std::cerr << RED << "(ERROR 442) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": KICK: You're not on that chan." << WHITE << std::endl;
		return ;
	}
	else if (action == "KICK: is not channel operator") {
		std::cerr << RED << "(ERROR 482) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": KICK: You're not channel oper." << WHITE << std::endl;
		return ;
	}
	else if (action == "INVITE: No such nickname") {
		std::cerr << RED << "(ERROR 401) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": INVITE: No such nick/channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "INVITE: you're not on that channel") {
		std::cerr << RED << "(ERROR 442) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": INVITE: You're not on that chan." << WHITE << std::endl;
		return ;
	}
	else if (action == "INVITE: is already on channel") {
		std::cerr << RED << "(ERROR 443) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": INVITE: is already on chan." << WHITE << std::endl;
		return ;
	}
	else if (action == "INVITE: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": INVITE: missing param." << WHITE << std::endl;
		return ;
	}
	else if (action == "INVITE: is not channel operator") {
		std::cerr << RED << "(ERROR 482) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": INVITE: You're not channel oper." << WHITE << std::endl;
		return ;
	}
	else if (action == "INVITE: successful") {
		std::cerr << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": INVITE: has been invite to a chan." << WHITE << std::endl;
		return ;
	}
	else if (action == "TOPIC: no topic defined") {
		std::cerr << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": TOPIC: no topic defined." << WHITE << std::endl;
		return ;
	}
	else if (action == "TOPIC: new topic defined") {
		std::cerr << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": TOPIC: new topic defined." << WHITE << std::endl;
		return ;
	}
	else if (action == "TOPIC: topic returned") {
		std::cerr << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": TOPIC: topic returned." << WHITE << std::endl;
		return ;
	}
	else if (action == "TOPIC: unset") {
		std::cerr << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": TOPIC: has been deleted." << WHITE << std::endl;
		return ;
	}
	else if (action == "TOPIC: no such channel") {
		std::cerr << RED << "(ERROR 403) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": TOPIC: no such channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "TOPIC: you're not on that channel") {
		std::cerr << RED << "(ERROR 442) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": TOPIC: you're not on that chan." << WHITE << std::endl;
		return ;
	}
	else if (action == "TOPIC: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": TOPIC: missing parameter." << WHITE << std::endl;
		return ;
	}
	else if (action == "TOPIC: is not channel operator") {
		std::cerr << RED << "(ERROR 482) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": TOPIC: You're not channel oper." << WHITE << std::endl;
		return ;
	}
	else if (action == "MODE: channel modes asked") {
		std::cerr << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": MODE: channel modes asked." << WHITE << std::endl;
		return ;
	}
	else if (action == "MODE: no such channel") {
		std::cerr << RED << "(ERROR 403) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": MODE: no such channel" << WHITE << std::endl;
		return ;
	}
	else if (action == "MODE: no user in the channel") {
		std::cerr << RED << "(ERROR 441) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": MODE: user is not on that chan." << WHITE << std::endl;
		return ;
	}
	else if (action == "MODE: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": MODE: missing parameter." << WHITE << std::endl;
		return ;
	}
		else if (action == "MODE: Channel key already set") {
		std::cerr << RED << "(ERROR 467) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": MODE: Channel key already set." << WHITE << std::endl;
		return ;
	}
		else if (action == "MODE: unknown mode") {
		std::cerr << RED << "(ERROR 472) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": MODE: unknown mode." << WHITE << std::endl;
		return ;
	}
	else if (action == "MODE: is not channel operator") {
		std::cerr << RED << "(ERROR 482) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": MODE: You're not channel oper." << WHITE << std::endl;
		return ;
	}
	else if (action == "JOIN: client joined") {
		std::cerr << GREEN << "(SUCCESS) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << " successfully joined a channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "JOIN: no such channel") {
		std::cerr << RED << "(ERROR 403) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": JOIN: no such channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "JOIN: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": JOIN: missing parameter." << WHITE << std::endl;
		return ;
	}
	else if (action == "JOIN: channel is full") {
		std::cerr << RED << "(ERROR 471) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": JOIN: channel is full." << WHITE << std::endl;
		return ;
	}
	else if (action == "JOIN: channel is invite-only") {
		std::cerr << RED << "(ERROR 473) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": JOIN: channel is invite-only." << WHITE << std::endl;
		return ;
	}
	else if (action == "JOIN: bad channel key") {
		std::cerr << RED << "(ERROR 475) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": JOIN: bad channel key." << WHITE << std::endl;
		return ;
	}
	else if (action == "PRIVMSG: No such nickname") {
		std::cerr << RED << "(ERROR 401) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PRIVMSG: No such nick/channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "PRIVMSG: Cannot send to channel") {
		std::cerr << RED << "(ERROR 404) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PRIVMSG: Cannot send to channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "PRIVMSG: No recipient given") {
		std::cerr << RED << "(ERROR 411) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PRIVMSG: No recipient given." << WHITE << std::endl;
		return ;
	}
	else if (action == "PRIVMSG: No text") {
		std::cerr << RED << "(ERROR 412) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PRIVMSG: No text to send." << WHITE << std::endl;
		return ;
	}
	else if (action == "PART: no such channel") {
		std::cerr << RED << "(ERROR 403) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PART: no such channel." << WHITE << std::endl;
		return ;
	}
	else if (action == "PART: you're not on that channel") {
		std::cerr << RED << "(ERROR 442) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PART: you're not on that chan." << WHITE << std::endl;
		return ;
	}
	else if (action == "PART: missing parameter") {
		std::cerr << RED << "(ERROR 461) " << YELLOW;
		std::cout << "Client number " << clientFd;
		std::cerr << ": PART: missing parameter." << WHITE << std::endl;
		return ;
	}
	else if (action == "SIGNAL RECEIVED") {
		std::cout << RED << "\n(SIGNAL RECEIVED) " << YELLOW;
		std::cout << "Closing server..." << WHITE << std::endl;
		return ;
	}
	else if (action == "FLAG ERROR") {
		std::cerr << RED << "(SYNTAX ERROR) " << YELLOW;
		std::cerr << "Unknown flag in getFlag()." << WHITE << std::endl;
		return ;
	}
}
