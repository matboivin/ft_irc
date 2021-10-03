/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:37:43 by root              #+#    #+#             */
/*   Updated: 2021/10/04 01:25:25 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <cstring>
# include <cstdlib>
# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include "client.hpp"
# include <vector>
# include <list>
# include <poll.h>
# include <algorithm>


namespace ft_irc
{
	class Parser;
	class Message;

	class IRCServer
	{
	private:
		std::string				hostname;
		std::string				bind_address;
		std::string				port;
		std::string				password;
		//Structure describing an Internet socket address.
		struct sockaddr_in		address;
		//Socket descriptor.
		int						sockfd;
		int						backlog_max;
		std::list<IRCClient>	clients;
	public:
							IRCServer(std::string bind_address="0.0.0.0",
									std::string port="6697",
									std::string password="",
									std::string hostname="irc.42.fr",
									int backlog_max=5);
		//copy constructor
							IRCServer(const IRCServer &other);
		//assignment operator
		IRCServer 			&operator=(const IRCServer &other);
		//destructor
							~IRCServer();
		//IRCServer getters
		std::string			getBindAddress() const;
		std::string			getPort() const;
		std::string			getPassword() const;

		//IRCServer setters
		void				setBindAddress(std::string bind_address);
		void				setPort(std::string port);
		void				setPassword(std::string password);
		int					run();

	private:
		//Function to create a socket.
		//create a new listening tcp s	ocket and bind it to the given address and port
		//https://www.geeksforgeeks.org/socket-programming-cc/
		bool				createSocket();
		int					sockGetLine(int sockfd, std::string &line);
		int					sockGetLine(int sockfd, std::string &line, std::size_t max_bytes);
		//awaitConnection
		bool				awaitNewConnection();
		bool				processClients(Parser& parser);
		bool				hasPendingConnections();
		int					executeCommand(Message& msg, IRCClient &client);
		int					sendList(IRCClient &client);
		int					sendError(IRCClient &client, const std::string &error);
		int					disconnectClient(IRCClient &client);
	};
}

#endif // !SERVER_HPP
