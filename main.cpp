/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclouet <tclouet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 15:03:06 by tclouet           #+#    #+#             */
/*   Updated: 2026/03/06 10:30:12 by tclouet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils/Utils.hpp"

/*
 * main - Entry point of the IRC server.
 * 
 * 1. Create an instance of the IRC server with the port
 *    and password from arguments.
 * 2. Validate command-line arguments (count, port range,
 *    password format).
 * 3. Set up signal handlers for SIGINT and SIGQUIT to allow
 *    clean shutdown.
 * 4. Initialize the server (socket creation, options, bind,
 *    listen, etc.).
 * 5. Start the main server loop with ircServer.start().
 * 6. Handle exceptions: print error messages and close
 *    all file descriptors.
 * 7. Log server closure.
 * 
 * Return:
 * - 0 if everything runs successfully
 * - 1 if arguments are invalid
 */
int	main (int argc, char **argv) {
	Server	ircServer(argv[1], argv[2]);
	if (!checkArgs(argc, argv[1], argv[2]))
		return 1;
	try {
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		ircServer.initialization();
		ircServer.start();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		ircServer.closeFd();
	}
	displayLogs("(SERVER CLOSED)", 0);
	return 0;
}
