/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 12:32:51 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 10:27:05 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

/*
 * ft_memset
 * Fills the first n bytes of the memory area pointed
 * to by s with the constant byte c.
 * Equivalent to the standard memset function.
 */
void	*ft_memset(void *s, int c, size_t n) {
	unsigned char	*ps;
	unsigned char	a;
	size_t			i;

	ps = (unsigned char *) s;
	a = (unsigned char) c;
	i = 0;
	while (i < n) {
		ps[i] = a;
		i++;
	}
	return (s);
}

/*
 * channelCount
 * Counts how many channel prefixes ('#' or '&')
 * appear in a string.
 * Useful for parsing multiple channels in one command.
 */
int	channelCount(std::string& channel) {
	int	count = 0;
	for (size_t i = 0; i < channel.size(); i++) {
		if (channel[i] == '#' || channel[i] == '&')
			count++;
	}
	return count;
}

/*
 * modesWithParams
 * Counts the number of channel modes in a mode string
 * that require parameters ('k', 'o', 'l').
 */
int	modesWithParams(std::string& modes) {
	int count = 0;
	for (size_t i = 1; i < modes.size(); i++) {
		if (modes[i] == 'k' || modes[i] == 'o' || modes[i] == 'l')
			count++;
	}
	return count;
}

/*
 * isOperCommand
 * Returns true if the command requires
 * operator privileges: KICK, INVITE, TOPIC, or MODE.
 */
bool	isOperCommand(std::string& command) {
	if (command == "KICK" || command == "INVITE" ||
			command == "TOPIC" || command == "MODE")
		return true;
	return false;
}

/*
 * validNickname
 * Checks if a nickname is valid according to IRC rules:
 * - Must start with a letter or allowed special character
 * - Remaining characters must be alphanumeric or
 *   allowed special characters
 * - Max length 9
 */
bool	validNickname(const std::string& nick) {
    if (nick.empty())
        return false;
    if (nick.size() > 9)
        return false;
    if (!isalpha(nick[0])
        && nick[0] != '['
        && nick[0] != ']'
        && nick[0] != '{'
        && nick[0] != '}'
        && nick[0] != '\\'
        && nick[0] != '_'
        && nick[0] != '^'
        && nick[0] != '`'
        && nick[0] != '|')
        return false;
    for (size_t i = 1; i < nick.size(); i++) {
        if (!isalnum(nick[i]) 
            && nick[i] != '['
            && nick[i] != ']'
            && nick[i] != '{'
            && nick[i] != '}'
            && nick[i] != '\\'
            && nick[i] != '_'
            && nick[i] != '^'
            && nick[i] != '`'
            && nick[i] != '|'
            && nick[i] != '-')
            return false;
    }
    return true;
}

/*
 * correctChannelFormat
 * Checks if a channel name is valid:
 * - Must start with '#', '&', '+', or '!'
 * - Max length 50
 * - Cannot contain ',' (comma), bell, or space
 */
bool	correctChannelFormat(const std::string& channelName) {
    if (channelName.empty())
        return false;
    if (channelName[0] != '#'
        && channelName[0] != '&'
        && channelName[0] != '+'
        && channelName[0] != '!')
        return false;
    if (channelName.size() > 50)
        return false;
    for (size_t i = 1; i < channelName.size(); i++) {
        if (channelName[i] == ',' 
            || channelName[i] == '\a' 
            || channelName[i] == ' ')
            return false;
    }
    return true;
}

/*
 * isValidMode
 * Validates a mode string:
 * - Starts with '+' or '-'
 * - Contains only allowed mode characters ('i','t','k','o','l')
 * - No repeated characters
 * - Length between 2 and 6
 */
bool	isValidMode(std::string& mode) {
	int	pCount = std::count(mode.begin(), mode.end(), '+');
	int	mCount = std::count(mode.begin(), mode.end(), '-');
	int	iCount = std::count(mode.begin(), mode.end(), 'i');
	int	tCount = std::count(mode.begin(), mode.end(), 't');
	int	kCount = std::count(mode.begin(), mode.end(), 'k');
	int	oCount = std::count(mode.begin(), mode.end(), 'o');
	int	lCount = std::count(mode.begin(), mode.end(), 'l');

	if (pCount > 1 || mCount > 1 ||iCount > 1 ||
			tCount > 1 || kCount > 1 || oCount > 1 || lCount > 1)
		return false;
	if (mode.size() == 1 || mode.size() > 6)
		return false;
	else if (mode[0] != '+' && mode[0] != '-')
		return false;
	for (size_t i = 1; i < mode.size(); i++) {
		if (mode[i] != 'i' && mode[i] != 't' && 
				mode[i] != 'k' && mode[i] != 'o' && mode[i] != 'l') 
		return false;
	}
	return true;
}

/*
 * isDigit
 * Checks if a string contains only numeric characters.
 */
bool	isDigit(std::string& value) {
	for (size_t i = 0; i < value.size(); i++) {
		if (value[i] < '0' || value[i] > '9')
			return false;
	}
	return true;
}

/*
 * isRequestToIgnore
 * Returns true for certain IRC messages that the server
 * can safely ignore:
 * - CAP LS (capability list request)
 * - MODE +i for the client's own nickname
 * - MODE with 'b' (ban list request)
 */
bool	isRequestToIgnore(Client *client, std::vector<std::string>& parsedData) {
	if (parsedData.size() == 2 && parsedData[0] == "CAP")
		return true;
	else if (parsedData.size() == 3 && parsedData[0] == "MODE" &&
				parsedData[1] == client->getNickname() &&
			parsedData[2] == "+i")
		return true;
	else if (parsedData.size() == 3 && parsedData[0] == "MODE" && parsedData[2] == "b")
		return true;
	return false;
}

/*
 * parseDataReceived
 * Splits an IRC input line into components for processing:
 * - Handles optional prefix starting with ':'
 * - Splits at the first ':' to separate trailing message
 * - Returns a vector of command and parameters
 */
std::vector<std::string> parseDataReceived(std::string& input) {
    std::vector<std::string> parsedCmd;

    size_t pos = input.find("\r\n");
    if (pos == std::string::npos)
        return parsedCmd;
    std::string line = input.substr(0, pos);
    input.erase(0, pos + 2);
	if (!line.empty() && line[0] == ':') {
		size_t pos = line.find(" ");
		if (pos == std::string::npos)
			return parsedCmd;
		line = line.substr(pos + 1);
	}
    std::stringstream str(line);
    std::string beforeWord;
    std::string word;
    if (std::getline(str, beforeWord, ':'))
        std::getline(str, word);
    std::stringstream s(beforeWord);
    std::string temp;
    while (s >> temp)
        parsedCmd.push_back(temp);
    if (!word.empty()) {
        parsedCmd.push_back(word);
	}
	displayDataReceived(parsedCmd);
    return parsedCmd;
}

/*
 * setModeMap
 * Constructs a map of mode characters to their parameters
 * from a parsed IRC MODE command.
 * - 'k', 'o', 'l' modes may have parameters
 * - Other modes get "NULL" as parameter
 */
std::map<char, std::string>	setModeMap(std::vector<std::string>& parsedData) {
	std::map<char,std::string>	map;
	size_t						i = 3;
	for (size_t j = 1; j < parsedData[2].size(); j++) {
		if (parsedData[2][j] == 'k' || parsedData[2][j] == 'o' || parsedData[2][j] == 'l') {
			if (i < parsedData.size())
				map.insert(std::pair<char,std::string>(parsedData[2][j], parsedData[i]));
			else
				map.insert(std::pair<char,std::string>(parsedData[2][j], "NULL"));
			i++;
		}
		else
			map.insert(std::pair<char,std::string>(parsedData[2][j], "NULL"));
	}
	return map;
}
