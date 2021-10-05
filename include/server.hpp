/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:37:43 by root              #+#    #+#             */
/*   Updated: 2021/10/04 10:50:12 by mboivin          ###   ########.fr       */
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
# include <vector>
# include <map>
# include <list>
# include <poll.h>
# include <algorithm>
# include "client.hpp"
# include "Parser.hpp"
# include "server_operations.hpp"

namespace ft_irc
{
	class Message;
	class Channel;
	class IRCClient;

	class IRCServer
	{
	public:
		// aliases
		typedef void (IRCServer::*cmd_fun)(Message& msg); // pointer to command function
		typedef std::map<std::string, cmd_fun>	cmds_map; // commands functions

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
		Parser					parser;
		cmds_map				commands;
		std::list<IRCClient>	clients;

	public:
		// constructor
					IRCServer(std::string bind_address="0.0.0.0",
							  std::string port="6697",
							  std::string password="",
							  std::string hostname="irc.42.fr",
							  int backlog_max=5);
		//copy constructor
					IRCServer(const IRCServer &other);
		//assignment operator
		IRCServer&	operator=(const IRCServer &other);
		//destructor
					~IRCServer();
		//IRCServer getters
		std::string	getBindAddress() const;
		std::string	getPort() const;
		std::string	getPassword() const;
		cmds_map	getCommands() const;

		//IRCServer setters
		void		setBindAddress(std::string bind_address);
		void		setPort(std::string port);
		void		setPassword(std::string password);
		int			run();

		// map of commands helpers
		void		init_commands_map(cmds_map& m);
		void		exec_cmd(const cmds_map& m, Message& msg);

		// execution helpers
		Message		parse(const std::string& packet, IRCClient& sender);

		// commands functions
		void		exec_pass_cmd(Message& msg);
		void		exec_nick_cmd(Message& msg);
		void		exec_quit_cmd(Message& msg);
		void		exec_notice_cmd(Message& msg);
		void		exec_privmsg_cmd(Message& msg);

	private:
		//Function to create a socket.
		//create a new listening tcp s	ocket and bind it to the given address and port
		//https://www.geeksforgeeks.org/socket-programming-cc/
		bool		createSocket();
		int			sockGetLine(int sockfd, std::string &line);
		int			sockGetLine(int sockfd, std::string &line, std::size_t max_bytes);
		//awaitConnection
		bool		awaitNewConnection();
		bool		processClients();
		bool		hasPendingConnections();
		int			executeCommand(Message& msg, IRCClient &client);
		int			sendList(IRCClient &client);
		int			sendError(IRCClient &client, const std::string &error);
		int			disconnectClient(IRCClient &client);
	};
}

#endif // !SERVER_HPP
