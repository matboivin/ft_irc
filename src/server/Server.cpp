/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:39:18 by root              #+#    #+#             */
/*   Updated: 2021/10/05 14:03:37 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "Server.hpp"
#include "server_operations.hpp"

int	setNonblocking(int fd);

namespace ft_irc
{
	// constructor
	Server::Server(std::string bind_address,
				   std::string port,
				   std::string password,
				   std::string hostname,
				   int backlog_max)
	: hostname(hostname),
	  bind_address(bind_address), port(port),
	  password(password), address(),
	  sockfd(-1), backlog_max(backlog_max),
	  parser(), commands(),
	  clients(), channels()
	{
		// create a new address struct
		address.sin_family = AF_INET;
		address.sin_port = htons(atoi(this->port.c_str()));
		address.sin_addr.s_addr = inet_addr(this->bind_address.c_str());

		// init map of commands
		this->init_commands_map(commands);
	}

	// copy constructor
	Server::Server(const Server& other)
	: hostname(other.hostname),
	  bind_address(other.bind_address), port(other.port),
	  password(other.password), address(other.address),
	  sockfd(other.sockfd), backlog_max(other.backlog_max),
	  parser(other.parser), commands(other.commands),
	  clients(other.clients), channels(other.channels)
	{
	}

	// assignment operator
	Server &Server::operator=(const Server& other)
	{
		if (this != &other)
		{
			this->hostname = other.hostname;
			this->bind_address = other.bind_address;
			this->port = other.port;
			this->password = other.password;
			this->sockfd = other.sockfd;
			this->backlog_max = other.backlog_max;
			this->parser = other.parser;
			this->commands = other.commands;
			this->clients = other.clients;
			this->channels = other.channels;
		}
		return (*this);
	}

	// destructor
	Server::~Server()
	{
	}

	// getters

	std::string	Server::getBindAddress() const
	{
		return (this->bind_address);
	}

	std::string	Server::getPort() const
	{
		return (this->port);
	}

	std::string	Server::getPassword() const
	{
		return (this->password);
	}

	Server::cmds_map	Server::getCommands() const
	{
		return (this->commands);
	}

	// setters

	void	Server::setBindAddress(const std::string& bind_address)
	{
		this->bind_address = bind_address;
	}

	void	Server::setPort(const std::string& port)
	{
		this->port = port;
	}

	void	Server::setPassword(const std::string& password)
	{
		this->password = password;
	}

	// main loop
	int	Server::run()
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

	//Function to create a socket.
	//create a new listening tcp s	ocket and bind it to the given address and port
	//https://www.geeksforgeeks.org/socket-programming-cc/

	//create a new socket
	//AF_INET: IPv4
	//SOCK_STREAM: TCP
	//IPPROTO_TCP: TCP protocol
	bool	Server::createSocket()
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
		int	optval = 1;
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

	// read char by char
	int	Server::sockGetLine(int sockfd, std::string& line)
	{
	
		char	c;

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

	// read char by char
	int	Server::sockGetLine(int sockfd, std::string& line, std::size_t max_bytes)
	{
		char	c;

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

	// accepts a new connection
	bool	Server::awaitNewConnection()
	{
		Client new_client;

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

	// Check if there are pending connections. (poll)
	bool	Server::hasPendingConnections()
	{
		struct pollfd	poll_fd = {this->sockfd, POLLIN, 0};
		int				poll_result = poll(&poll_fd, 1, 0);

		if (poll_result < 0)
		{
			std::cerr << "Error: Could not poll socket." << std::endl;
			return (false);
		}
		if (poll_result == 0)
			return (false);
		return (true);
	}

	// process all clients
	bool	Server::processClients()
	{
		for (std::list<Client>::iterator it = this->clients.begin();
			 it != this->clients.end();
			 ++it)
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
					std::vector<Client>	recipients = msg.getRecipients();

					for (std::vector<Client>::const_iterator	it = recipients.begin();
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

	int	Server::disconnectClient(Client& client)
	{
		std::list<Client>::iterator	it = std::find(this->clients.begin(), this->clients.end(), client);

		//log the closing of the connection
		std::cout << "Closing connection to " << it->getIpAddressStr() << std::endl
				  << "---------------------------------------------------------"
				  << std::endl;

		close(it->getSocketFd());
		it = this->clients.erase(it);
		return (0);
	}

	int	Server::executeCommand(Message& msg, Client& client)
	{
		if (msg.getCommand() == "QUIT")
		{
			this->disconnectClient(client);
		}
		exec_cmd(commands, msg);
		return (0);
	}

	int	Server::sendList(Client& client)
	{
		for (std::list<Client>::iterator it = this->clients.begin();
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

	int	Server::sendError(Client& client, const std::string& error)
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
	void	Server::init_commands_map(cmds_map& m)
	{
		m["PASS"]    = &Server::exec_pass_cmd;
		m["NICK"]    = &Server::exec_nick_cmd;
		m["QUIT"]    = &Server::exec_quit_cmd;
		m["PRIVMSG"] = &Server::exec_privmsg_cmd;
		m["NOTICE"]  = &Server::exec_notice_cmd;
	}

	// call command function
	void	Server::exec_cmd(const cmds_map& m, Message& msg)
	{
		cmds_map::const_iterator	it = m.find(msg.getCommand());

		if (it != m.end())
			(this->*it->second)(msg);
	}

	// Execution helpers
	Message		Server::parse(const std::string& packet, Client& sender)
	{
		return (this->parser.parseMessage(packet, sender));
	}

	// Channel operations

	std::list<Channel>::iterator	Server::getChannel(const std::string& chan_name)
	{
		std::list<Channel>::iterator	it = this->channels.begin();

		while (it != this->channels.end())
		{
			if (it->getName() == chan_name)
				break ;
			++it;
		}
		return (it);
	}

	void	Server::addChannel(const std::string& name)
	{
		this->channels.push_back(Channel(name));
	}

	// Check whether a client is in a specific channel
	bool	Server::userInChannel(Client& client, const std::string& chan_name)
	{
		std::list<Channel>::iterator	it = this->getChannel(chan_name);

		if (it != this->channels.end())
			return (it->hasClient(client));

		return (false);
	}

	// Add a user to a channel (ex: JOIN command)
	void	Server::addUserToChannel(Client& client, const std::string& chan_name)
	{
		if (!this->userInChannel(client, chan_name))
			this->getChannel(chan_name)->addClient(client);
	}

	// Remove user from channel
	void	Server::removeUserFromChannel(Client& client, const std::string& chan_name)
	{
		if (this->userInChannel(client, chan_name))
			this->getChannel(chan_name)->removeClient(client);
	}

	// Commands

	// PASS <password>
	// set a connection password
	void	Server::exec_pass_cmd(Message& msg)
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
	void	Server::exec_nick_cmd(Message& msg)
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
	void	Server::exec_quit_cmd(Message& msg)
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
	void	Server::exec_notice_cmd(Message& msg)
	{
		// sorry for this pseudo code ugliness

		// if (msg.getParam(0) is not a channel)
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
	void	Server::exec_privmsg_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_norecipient(msg);
		else if (msg.getParams().size() < 2)
			err_notexttosend(msg);
		// else if (msg.getParam(0) doesn't exist)
		//	err_nosuchnick(msg, msg.getParam(0));
		// else if (!msg.getSender() is not in channel)
		// 	err_cannotsendtochan(msg);
		// else
		// 	exec_notice_cmd(msg);
	}
}
