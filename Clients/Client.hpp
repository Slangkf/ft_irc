/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@42paris.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 13:54:51 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/02 17:09:59 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../Utils/Utils.hpp"
#include <iostream>
#include <exception>
#include <vector>

#define GREEN	"\e[1;32m"
#define RED		"\e[1;31m"
#define YELLOW	"\e[1;33m"
#define WHITE	"\e[0;37m"

class Client {
	public:
		Client();
		~Client();
		
		// SETTERS
		void	setFlag(std::string whichFlag, bool value);
		void	setFd(int fd);
		void	setFloodingCount(unsigned int count);
		void	setTimeWindow(unsigned long long serverTime);
		void	setIPAddress(std::string ipAddress);
		void	setPassword(std::string password);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);

		// GETTERS
		bool				getFlag(std::string whichFlag);
		int					getFd(void);
		unsigned int		getFloodingCount(void);
		unsigned long long	getTimeWindow(void);
		std::string&		getIPAddress(void);
		std::string&		getPassword(void);
		std::string&		getNickname(void);
		std::string&		getUsername(void);
		std::string&		getBuffer(void);
	private:
		bool				_AuthenFlag;
		bool				_PassFlag;
		bool				_NickFlag;
		bool				_UserFlag;
		int					_Fd;
		unsigned int		_FloodingCount;
		unsigned long long	_TimeWindow;
		std::string			_Buffer;
		std::string			_IPAddress;
		std::string			_Password;
		std::string			_Nickname;
		std::string			_Username;
};

#endif