/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:39:18 by root              #+#    #+#             */
/*   Updated: 2021/10/04 10:39:42 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "server.hpp"
#include "client.hpp"
#include "Parser.hpp"
#include "Message.hpp"
#include "server_operations.hpp"

int	setNonblocking(int fd);

namespace ft_irc
{
	IRCServer::IRCServer(std::string bind_address,
						std::string port,
						std::string password,
						std::string hostname,
						int backlog_max)
	: parser()
	{
		this->hostname = hostname;
		this->bind_address = bind_address;
		this->port = port;
		this->password = password;
		this->sockfd = -1;
		this->backlog_max = backlog_max;
		//create a new address struct
		address.sin_family = AF_INET;
		address.sin_port = htons(atoi(this->port.c_str()));
		address.sin_addr.s_addr = inet_addr(this->bind_address.c_str());
		// init map of commands
		this->init_commands_map(commands);
	}
	
	//IRCServer getters
	std::string IRCServer::getBindAddress() const
	{
		return (this->bind_address);
	}

	std::string IRCServer::getPort() const
	{
		return (this->port);
	}

	std::string IRCServer::getPassword() const
	{
		return (this->password);
	}

	IRCServer::cmds_map	IRCServer::getCommands() const
	{
		return (this->commands);
	}

	//IRCServer setters
	void IRCServer::setBindAddress(std::string bind_address)
	{
		this->bind_address = bind_address;
	}

	void IRCServer::setPort(std::string port)
	{
		this->port = port;
	}

	void IRCServer::setPassword(std::string password)
	{
		this->password = password;
	}

	//copy constructor
	IRCServer::IRCServer(const IRCServer &other)
	{
		this->bind_address = other.bind_address;
		this->port = other.port;
		this->password = other.password;
		this->sockfd = other.sockfd;
		this->backlog_max = other.backlog_max;
		this->address = other.address;
		this->parser = other.parser;
		this->commands = other.commands;
	}
	//assignment operator
	IRCServer &IRCServer::operator=(const IRCServer &other)
	{
		if (this != &other)
		{
			this->bind_address = other.bind_address;
			this->port = other.port;
			this->password = other.password;
			this->sockfd = other.sockfd;
			this->backlog_max = other.backlog_max;
			this->parser = other.parser;
			this->commands = other.commands;
		}
		return (*this);
	}
	//destructor
	IRCServer::~IRCServer()
	{
	}
	//Function to create a socket.
	//create a new listening tcp s	ocket and bind it to the given address and port
	//https://www.geeksforgeeks.org/socket-programming-cc/

	//create a new socket
	//AF_INET: IPv4
	//SOCK_STREAM: TCP
	//IPPROTO_TCP: TCP protocol
	bool IRCServer::createSocket()
	{
		//Create a socket.
		this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (this->sockfd < 0)
		{
			std::cerr << "Error: Could not create socket." << std::endl;
			return (false);
		}
		setNonblocking(this->sockfd);
		//Set socket options.
		int optval = 1;
		if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		{
			std::cerr << "Error: Could not set socket options." << std::endl;
			return (false);
		}
		//Bind the socket to the address.
		if (bind(this->sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			std::cerr << "Error: Could not bind socket." << std::endl;
			return (false);
		}
		//Listen for connections.
		if (listen(this->sockfd, this->backlog_max) < 0)
		{
			std::cerr << "Error: Could not listen on socket." << std::endl;
			return (false);
		}
		return (true);
	}
	bool				IRCServer::hasPendingConnections()
	{
		//Check if there are pending connections. (poll)
		struct pollfd		poll_fd = {this->sockfd, POLLIN, 0};
		int					poll_result = poll(&poll_fd, 1, 0);
		if (poll_result < 0)
		{
			std::cerr << "Error: Could not poll socket." << std::endl;
			return (false);
		}
		if (poll_result == 0)
			return (false);
		return (true);
	}

	int IRCServer::run()
	{
		if (createSocket() == false)
		{
			return (-1);
		}

		//accept incoming connections
		while (true)
		{
			if (hasPendingConnections() == true)
				awaitNewConnection();
			processClients();
		}
	}
	int IRCServer::sockGetLine(int sockfd, std::string &line)
	{
		//read char by char
		char c;
		line = "";
		while (true)
		{
			if (recv(sockfd, &c, 1, 0) < 0)
			{
				return (false);
			}
			if (c == '\n')
			{
				break;
			}
			line += c;
		}
		return (true);
	}

	int IRCServer::sockGetLine(int sockfd, std::string &line, std::size_t max_bytes)
	{
		//read char by char
		char c;
		line = "";
		while (true)
		{
			if (recv(sockfd, &c, 1, 0) < 0)
			{
				return (false);
			}
			if (c == '\n')
			{
				break;
			}
			line += c;
			if (line.size() > max_bytes)
			{
				throw std::runtime_error("line too long");
			}
		}
		return (true);
	}
	//awaitNewConnection
	//accepts a new connection
	bool				IRCServer::awaitNewConnection()
	{
		IRCClient new_client;

		//accept a new connection
		
		new_client.awaitConnection(this->sockfd);
		if (new_client.getSocketFd() < 0)
		{
			throw std::runtime_error("accept() failed");
		}
		//log the clients IP address
		std::cout << "Client " << new_client.getIpAddressStr()
		<< " connected" << std::endl;
		this->clients.push_back(new_client);
		return (true);
	}

	bool				IRCServer::processClients()
	{
		//process all clients
		for (std::list<IRCClient>::iterator it = this->clients.begin();
		 it != this->clients.end(); ++it)
		{
			if (it->updateOutBuffer())
			{
				continue;
			}
			if (it->hasUnprocessedCommands() == true)
			{
				// parse the message
				Message	msg = this->parse(it->popUnprocessedCommand(), *it);
				// execute the command
				this->executeCommand(msg, *it);
				// send response to recipient(s)
				if (!msg.getRecipients().empty())
				{
					// there can be many recipients (ex: broadcast to channel)
					std::vector<IRCClient>	recipients = msg.getRecipients();

					for (std::vector<IRCClient>::const_iterator	it = recipients.begin();
						it != recipients.end();
						++it)
					{
						//std::cout << "Sending: '" << msg.getResponse() << "' to " << it->getIpAddressStr() << std::endl;
						if (send(it->getSocketFd(), msg.getResponse().c_str(), msg.getResponse().size(), 0) < 0)
						{
							throw std::runtime_error("send() failed");
						}
					}
				}
			}
			if (it->getSocketFd() > 0)
			{
				it->updateInBuffer();
			}
		}
		return (true);
	}

	int	IRCServer::executeCommand(Message& msg, IRCClient &client)
	{
		// msg.displayMessage();
		if (msg.getCommand() == "QUIT")
		{
			this->disconnectClient(client);
		}
		exec_cmd(commands, msg);
		return (0);
	}

	int	IRCServer::disconnectClient(IRCClient &client)
	{
		std::list<IRCClient>::iterator it = std::find(this->clients.begin(), this->clients.end(), client);
		//log the closing of the connection
		std::cout << "Closing connection to " << it->getIpAddressStr() << std::endl
		<< "---------------------------------------------------------" << std::endl;
		close(it->getSocketFd());
		it = this->clients.erase(it);
		return (0);
	}

	int	IRCServer::sendList(IRCClient &client)
	{
		for (std::list<IRCClient>::iterator it = this->clients.begin();
		 it != this->clients.end(); ++it)
		{
			std::string response = ":";
			response += it->getNick();
			response += " 322 ";
			response += client.getNick();
			response += " ";
			response += it->getNick();
			response += " 0 :";
			response += it->getNick();
			response += " :";
			response += it->getNick();
			response += " 0\r\n";
			//log the response
			std::cout << "Sending: " << response << "to " << it->getIpAddressStr() << std::endl;
			client.sendCommand(response);
		}
		return (0);
	}

	int					IRCServer::sendError(IRCClient &client, const std::string &error)
	{
		std::string response = ":" + hostname + " 451 ";
		response += client.getNick();
		response += " ";
		response += error;
		response += "\r\n";
		//log the response
		std::cout << "Sending: " << response << "to " << client.getIpAddressStr() << std::endl;
		if (send(client.getSocketFd(), response.c_str(), response.size(), 0) < 0)
		{
			throw std::runtime_error("send() failed");
		}
		return (0);
	}

	// Map of commands helpers
	// Init the map containing the commands
	void	IRCServer::init_commands_map(cmds_map& m)
	{
		m["PASS"]    = &IRCServer::exec_pass_cmd;
		m["NICK"]    = &IRCServer::exec_nick_cmd;
		m["QUIT"]    = &IRCServer::exec_quit_cmd;
		m["PRIVMSG"] = &IRCServer::exec_privmsg_cmd;
		m["NOTICE"]  = &IRCServer::exec_notice_cmd;
	}

	// call command function
	void	IRCServer::exec_cmd(const cmds_map& m, Message& msg)
	{
		cmds_map::const_iterator	it = m.find(msg.getCommand());

		if (it != m.end())
			(this->*it->second)(msg);
	}

	// Execution helpers
	Message		IRCServer::parse(const std::string& packet, IRCClient& sender)
	{
		return (this->parser.parseMessage(packet, sender));
	}

	// Commands

	// PASS <password>
	// set a connection password
	void	IRCServer::exec_pass_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg);
		else if (msg.getSender().isRegistered())
			err_alreadyregistered(msg);
		else
			msg.getSender().setPassword(msg.getParam(0));
	}

	// NICK helper: Special characters listed in the RFC grammar
	static bool	is_special(char c)
	{
		return (
			(c == '[') || (c == ']') || (c == '\\') || (c == '`')
			|| (c == '_') || (c == '^') || (c == '{') || (c == '}') || (c == '|')
			);
	}

	// NICK helper: Check whether a nickname format is valid
	// A nickname is composed of 1 to 9 characters which can be digits, letters or special characters.
	// It musn't start by a digit
	static bool	nick_is_valid(const std::string& newnick)
	{
		if (newnick.size() < 10)
		{
			std::string::const_iterator	it = newnick.begin();

			if (isalpha(*it) || is_special(*it))
			{
				++it;
				while (it != newnick.end())
				{
					if (!isalnum(*it) && !is_special(*it) && (*it != '-'))
						return (false);
					++it;
				}
				return (true);
			}
		}
		return (false);
	}

	// NICK <nickname>
	// Change a user nickname
	void	IRCServer::exec_nick_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_nonicknamegiven(msg);
		else
		{
			if (!nick_is_valid(msg.getParam(0)))
				err_erroneusnickname(msg);
			// else if nickname already exists
			//    err_nicknameinuse(msg);
			else
			{
				msg.getSender().setNick(msg.getParam(0));
				std::cout << "new nick is: " << msg.getSender().getNick() << '\n'; // debug
			}
		}
	}

	// QUIT [<message>]
	// A client session is terminated with a quit message
	void	IRCServer::exec_quit_cmd(Message& msg)
	{
		if (!msg.getParams().empty())
		{
			// msg.setRecipient(users in channel);
			msg.setResponse(
				build_prefix( build_full_client_id( msg.getSender() ) )
				+ " NOTICE " + "chan tmp" // TODO: not sure about notice and need target
				+ msg.getSender().getNick() + " has quit IRC (" + msg.getParam(0) + ")"
				);
			msg.appendSeparator();
		}

		// TODO: The server acknowledges this by sending an ERROR message to the client
	}

	// NOTICE <msgtarget> :<message>
	// Send messages to a user or a channel
	// The server musn't reply to NOTICE message
	void	IRCServer::exec_notice_cmd(Message& msg)
	{
		// sorry for this pseudo code ugliness

		// if (msg.getParam(0)[0] != '#')
		// 	msg.setRecipient(msg.getParam(0) to client);
		// else
		// 	add everyone from channel
		// if (channel doesnt exist)
		// 	create channel;
		msg.setResponse(build_prefix( build_full_client_id( msg.getSender() ) ) + " PRIVMSG ");
		for (std::size_t i = 0; i < msg.getParams().size(); i++)
			msg.getResponse().append(msg.getParams()[i]);
		msg.appendSeparator();
	}

	// PRIVMSG <msgtarget> :<message>
	// Send messages to a user or a channel
	void	IRCServer::exec_privmsg_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_norecipient(msg);
		else if (msg.getParams().size() < 2)
			err_notexttosend(msg);
		// else if (msg.getParam(0) doesn't exist)
		//	err_nosuchnick(msg, msg.getParam(0));
		// else if (!msg.getSender().isOnChan(msg.getParam(0)))
		// 	err_cannotsendtochan(msg);
		// else
		// 	exec_notice_cmd(msg);
	}
}
