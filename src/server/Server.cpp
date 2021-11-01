/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:39:18 by root              #+#    #+#             */
/*   Updated: 2021/11/01 16:00:34 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <map>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "CLIParser.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "numeric_replies.hpp"
#include "Server.hpp"
#include "server_operations.hpp"

int	setNonblocking(int fd);

namespace ft_irc
{
	// constructor
	Server::Server(CLIParser& CLI_parser, int backlog_max)
	: _sockfd(-1), _backlog_max(backlog_max),
	  _config(CLI_parser.getBindAddress(), CLI_parser.getPort(), CLI_parser.getPassword()),
	  _parser(), _commands(),
	  _clients(), _channels()
	{
		// create a new address struct
		this->_address.sin_family = AF_INET;
		this->_address.sin_port = htons(atoi(getPort().c_str()));
		this->_address.sin_addr.s_addr = inet_addr(getBindAddress().c_str());

		// init map of commands
		_init_commands_map();
		//init creation date string
		time_t now = time(0);
		_creation_date = ctime(&now);
		_version = "42.42";
	}

	// copy constructor
	Server::Server(const Server& other)
	: _sockfd(other._sockfd), _backlog_max(other._backlog_max),
	  _config(other._config),
	  _parser(other._parser), _commands(other._commands),
	  _clients(other._clients), _channels(other._channels),
	  _creation_date(other._creation_date), _version(other._version)
	{
	}

	// assignment operator
	Server&	Server::operator=(const Server& other)
	{
		if (this != &other)
		{
			this->_sockfd = other._sockfd;
			this->_backlog_max = other._backlog_max;
			this->_config = other._config;
			this->_parser = other._parser;
			this->_commands = other.getCommands();
			this->_clients = other._clients;
			this->_channels = other._channels;
			this->_creation_date = other._creation_date;
			this->_version = other._version;
		}
		return (*this);
	}

	// destructor
	Server::~Server()
	{
		this->_shutdown();
	}

	// getters

	std::string	Server::getHostname() const
	{
		return (this->_config.getHostname());
	}

	std::string	Server::getBindAddress() const
	{
		return (this->_config.getBindAddress());
	}

	std::string	Server::getPort() const
	{
		return (this->_config.getPort());
	}

	std::string	Server::getPassword() const
	{
		return (this->_config.getPassword());
	}

	Server::t_cmds	Server::getCommands() const
	{
		return (this->_commands);
	}

	std::list<Client>::iterator	Server::getClient(const std::string& nick)
	{
		std::list<Client>::iterator	it = this->_clients.begin();

		while (it != this->_clients.end())
		{
			if (it->getNick() == nick)
				break ;
			++it;
		}
		return (it);
	}

	std::list<Channel>::iterator	Server::getChannel(const std::string& chan_name)
	{
		if (!channel_is_valid(chan_name))
			return (this->_channels.end());

		std::list<Channel>::iterator	it = this->_channels.begin();

		while (it != this->_channels.end())
		{
			if (it->getName() == chan_name)
				break ;
			++it;
		}
		return (it);
	}

	// main loop
	int	Server::run()
	{
		if (_createSocket() == false)
		{
			return (-1);
		}

		//accept incoming connections
		while (true)
		{
			if (_hasPendingConnections() == true)
				_awaitNewConnection();
			_processClients();
		}
	}

	void	Server::_make_welcome_msg(Message& msg)
	{
		msg.setResponse(
			build_prefix(msg.getServHostname())
		+ " 001 " + msg.getSender().getNick() + " :Welcome to the Internet Relay Network "
		+ msg.getSender().getNick() + CRLF
		+ build_prefix(msg.getServHostname())
		+ " 002 " + msg.getSender().getNick() + " :Your host is " + msg.getServHostname() +
		", running version " + this->_version + CRLF);
	}

	//Function to create a socket.
	//create a new listening tcp s	ocket and bind it to the given address and port
	//https://www.geeksforgeeks.org/socket-programming-cc/

	//create a new socket
	//AF_INET: IPv4
	//SOCK_STREAM: TCP
	//IPPROTO_TCP: TCP protocol
	bool	Server::_createSocket()
	{
		//Create a socket.
		this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_sockfd < 0)
		{
			std::cerr << "Error: Could not create socket." << std::endl;
			return (false);
		}
		setNonblocking(this->_sockfd);

		//Set socket options.
		int	optval = 1;
		if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		{
			std::cerr << "Error: Could not set socket options." << std::endl;
			return (false);
		}
		//Bind the socket to the address.
		if (bind(this->_sockfd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		{
			std::cerr << "Error: Could not bind socket." << std::endl;
			return (false);
		}
		//Listen for connections.
		if (listen(this->_sockfd, this->_backlog_max) < 0)
		{
			std::cerr << "Error: Could not listen on socket." << std::endl;
			return (false);
		}
		return (true);
	}

	// accepts a new connection
	bool	Server::_awaitNewConnection()
	{
		Client	new_client;

		//accept a new connection
		new_client.awaitConnection(this->_sockfd);
		if (new_client.getSocketFd() < 0)
			return (false);
		//log the clients IP address
		std::cout << "Client " << new_client.getIpAddressStr()
				  << " connected" << std::endl;

		this->_clients.push_back(new_client);
		return (true);
	}

	// Check if there are pending connections. (poll)
	bool	Server::_hasPendingConnections()
	{
		struct pollfd	poll_fd = {this->_sockfd, POLLIN, 0};
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
	
	bool	Server::_processClientCommand(Client& client)
	{
		if (client.hasUnprocessedCommands() == true)
		{
			Message	msg(client);

			if (_parse(msg, client.popUnprocessedCommand()) == true) // parse the message
			{
				_executeCommand(msg); // execute the command
				//if the client has just registered, send him a nice welcome message :D
				if (client.isRegistered() == false && !client.getNick().empty() &&
					!client.getUsername().empty() && !client.getHostname().empty())
				{
					std::cout << "Client " << client.getNick() << "@" << client.getIpAddressStr()
							  << " has just registered" << std::endl;
					Message	welcome_msg(msg);
					_make_welcome_msg(welcome_msg);
					_sendResponse(welcome_msg);
					client.setRegistered(true);
				}
				else
				{
					std::cout << "Is registered: " << client.isRegistered() << std::endl
							  << "Nick: " << client.getNick() << std::endl
							  << "Username: " << client.getUsername() << std::endl
							  << "Hostname: " << client.getHostname() << std::endl;
				}
			}
			_sendResponse(msg); // send response to recipient(s)
			client.updateLastEventTime();
			return (true);
		}
		return (false);
	}

	// process all clients
	bool	Server::_processClients()
	{
		for (std::list<Client>::iterator it = this->_clients.begin();
			 it != this->_clients.end();)
		{
			if (it->isAlive() == false)
			{
				this->_disconnectClient(*it);
				it = this->_clients.erase(it);
				continue;
			}
			if (it->updateOutBuffer())
			{
				continue;
			}
			_processClientCommand(*it);
			it->updateInBuffer();
			if (it->isTimeouted() == true)
			{
				std::cout << "Client " << it->getIpAddressStr()
						  << " timed out" << std::endl;
				this->_disconnectClient(*it);
			}
			it++;
		}
		return (true);
	}

	int	Server::_disconnectClient(Client& client)
	{
		std::list<Client>::iterator	it = std::find(this->_clients.begin(), this->_clients.end(), client);

		if (it->getSocketFd() > 0)
		{
			close(it->getSocketFd());
			it->setSocketFd(-1);
		}
		it->setAlive(false);
		it->setConnected(false);
		std::cout << "Client " << it->getIpAddressStr()
			<< " disconnected" << std::endl;
		return (0);
	}

	// Call Parser method to process a message
	bool	Server::_parse(Message& msg, const std::string& cmd)
	{
		return (this->_parser.parseMessage(msg, cmd));
	}

	// Init the map containing the commands
	void	Server::_init_commands_map()
	{
		this->_commands["PASS"]    = &Server::exec_pass_cmd;
		this->_commands["NICK"]    = &Server::exec_nick_cmd;
		this->_commands["QUIT"]    = &Server::exec_quit_cmd;
		this->_commands["OPER"]    = &Server::exec_oper_cmd;
		this->_commands["NOTICE"]  = &Server::exec_notice_cmd;
		this->_commands["PRIVMSG"] = &Server::exec_privmsg_cmd;
		this->_commands["JOIN"]    = &Server::exec_join_cmd;
		this->_commands["PART"]    = &Server::exec_part_cmd;
	}

	// Command execution
	int	Server::_executeCommand(Message& msg)
	{
		t_cmds::const_iterator	it = this->_commands.find(msg.getCommand());

		if (it != this->_commands.end())
		{
			std::cout << "Executing command \"" << msg.getCommand() << "\"" << std::endl;
			(this->*it->second)(msg);
		}
		return (0);
	}

	// Set response dst (channels or clients)
	void	Server::_setResponseRecipients(Message& msg)
	{
		std::list<Client>::iterator	dst = getClient(msg.getParams().at(0));

		if (dst != this->_clients.end())
		{
			msg.setRecipient(*dst);
			return ;
		}

		if (msg.getParams().empty())
		{
			return ;
		}

		std::list<Channel>::iterator	channel = getChannel(msg.getParams().at(0));

		if (channel != this->_channels.end())
			msg.setRecipients(channel->getClients());
	}

	// send response
	void	Server::_sendResponse(Message& msg)
	{
		if (msg.getRecipients().empty())
			return ;

		std::list<Client*>	recipients = msg.getRecipients();
		std::string			logOutput;

		for (std::list<Client*>::const_iterator	dst = recipients.begin();
			 dst != recipients.end();
			 ++dst)
		{
			logOutput = msg.getResponse();
			size_t	pos = logOutput.find(CRLF);

			if (pos != std::string::npos)
				logOutput.replace(pos, 2, CRLF_PRINTABLE); 
			std::cout << "Sending: '" << logOutput << "' to " << (*dst)->getIpAddressStr() << std::endl;
			if (send((*dst)->getSocketFd(), msg.getResponse().c_str(), msg.getResponse().size(), 0) < 0)
			{
				throw std::runtime_error("send() failed");
			}
		}
	}

	// Channel operations

	// Add a new channel to the server's list
	Channel&	Server::_addChannel(const std::string& name)
	{
		this->_channels.push_back(Channel(name));
		return (this->_channels.back());
	}

	// Remove a channel from the server's list
	void	Server::_removeChannel(std::list<Channel>::iterator channel)
	{
		this->_channels.erase(channel);
	}

	// Check whether a client is in a specific channel
	bool	Server::_userOnChannel(Client& client, Channel& channel)
	{
		return (channel.hasClient(client)); 
	}

	bool	Server::_userOnChannel(Client& client, const std::string& chan_name)
	{
		std::list<Channel>::iterator	channel = getChannel(chan_name);

		if (channel != this->_channels.end())
			return (channel->hasClient(client));
		return (false);
	}

	// Add a user to a channel (ex: JOIN command)
	void	Server::_addUserToChannel(Client& client, Channel& channel)
	{
		if (!_userOnChannel(client, channel))
		{
			std::cout << "Add " << client.getNick() << " to channel "
					  << channel.getName() << std::endl;
			client.joinChannel(channel);

			channel.displayClients(); // debug
			std::cout << std::endl;
		}
	}

	// Remove user from channel
	void	Server::_removeUserFromChannel(Client& client, Channel& channel)
	{
		if (_userOnChannel(client, channel))
		{
			std::cout << "Remove " << client.getNick() << " from channel "
					  << channel.getName() << std::endl;
			client.partChannel(channel);

			channel.displayClients(); // debug
			std::cout << std::endl;
		}
	}

	void	Server::_removeUserFromAllChannels(Client& client, Message& msg)
	{
		msg.setRecipients(client.getAllContacts());
		client.partAllChannels();

		client.displayJoinedChannels(); // debug
		std::cout << std::endl;
	}

	// oper operations
	bool	Server::_giveOperPriv(const std::string& name, const std::string& password)
	{
		return (this->_config.operBlockIsValid(name, password));
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
			msg.getSender().setPassword(msg.getParams().at(0));
	}

	// NICK <nickname>
	// Change a user nickname
	void	Server::exec_nick_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_nonicknamegiven(msg);
		else
		{
			if (!nick_is_valid(msg.getParams().at(0)))
				err_erroneusnickname(msg);
			else if (getClient(msg.getParams().at(0)) != this->_clients.end())
				err_nicknameinuse(msg);
			else
				msg.getSender().setNick(msg.getParams().at(0));
		}
	}

	// QUIT [<message>]
	// A client session is terminated with a quit message
	void	Server::exec_quit_cmd(Message& msg)
	{
		msg.setRecipients(msg.getSender().getAllContacts());
		msg.getSender().setAlive(false);
		// TODO: The server acknowledges this by sending an ERROR message to the client
	}

	// OPER <username> <password>
	// Authenticates a user as an IRC operator if the username/password combination exists
	// for that server
	void	Server::exec_oper_cmd(Message& msg)
	{
		if (msg.getParams().size() < 2)
		{
			err_needmoreparams(msg);
			return ;
		}
		if (!msg.getSender().isOper())
		{
			if (_giveOperPriv(msg.getParams().at(0), msg.getParams().at(1)))
			{
				msg.getSender().addMode("o");

				Message	rpl_msg(msg.getSender()); // tmp
				rpl_youreoper(rpl_msg);
				_sendResponse(rpl_msg);
				//exec_mode_cmd(msg);
			}
			else
				err_passwdmismatch(msg);
		}
	}

	// NOTICE <msgtarget> :<message>
	// Send messages to a user or a channel
	// The server musn't reply to NOTICE message
	void	Server::exec_notice_cmd(Message& msg)
	{
		if (msg.getParams().size() < 2) // params are mandatory
			msg.clearRecipients();
		else
			_setResponseRecipients(msg);
	}

	// PRIVMSG <msgtarget> :<message>
	// Send messages to a user or a channel
	void	Server::exec_privmsg_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_norecipient(msg);
		else if (msg.getParams().size() < 2)
			err_notexttosend(msg);
		else if (channel_is_valid(msg.getParams().at(0)) && !_userOnChannel( msg.getSender(), msg.getParams().at(0) ))
			err_cannotsendtochan(msg);
		else if (!channel_is_valid(msg.getParams().at(0)) && getClient( msg.getParams().at(0) ) == this->_clients.end() )
			err_nosuchnick(msg, msg.getParams().at(0));
		else
			_setResponseRecipients(msg);
	}

	// JOIN <channels>
	// JOIN 0 -> leave all channels
	void	Server::exec_join_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg);
		else if (msg.getParams().at(0) == "0") // JOIN 0
			_removeUserFromAllChannels(msg.getSender(), msg);
		else
		{
			for (std::vector<std::string>::const_iterator param = msg.getParams().begin();
				 param != msg.getParams().end();
				 ++param)
			{
				std::list<Channel>::iterator	channel = getChannel(*param);

				if (!channel_is_valid(*param))
				{
					err_nosuchchannel(msg, *param);
					return ;
				}
				if (channel == this->_channels.end())
					_addUserToChannel(msg.getSender(), _addChannel(*param));
				else
					_addUserToChannel(msg.getSender(), *channel);
			}
		}
	}

	// PART <channels> [<message>]
	void	Server::exec_part_cmd(Message& msg)
	{
		if (msg.getParams().empty())
		{
			err_needmoreparams(msg);
			return ;
		}

		for (std::vector<std::string>::const_iterator param = msg.getParams().begin();
			 param != msg.getParams().end();
			 ++param)
		{
			if ( ((*param)[0] == ':') && (++param == msg.getParams().end()) )
				break ;

			std::list<Channel>::iterator	channel = getChannel(*param);

			if (channel == this->_channels.end())
				err_nosuchchannel(msg, *param);
			else if (!_userOnChannel(msg.getSender(), *channel))
				err_notonchannel(msg, *param);
			else
			{
				_removeUserFromChannel(msg.getSender(), *channel);
				if (channel->isEmpty())
					_removeChannel(channel);
			}
		}
	}

	// debug
	int	Server::_sendList(Client& client)
	{
		for (std::list<Client>::iterator it = this->_clients.begin();
			 it != this->_clients.end();
			 ++it)
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

	int	Server::_sendError(Client& client, const std::string& error)
	{
		std::string response = ":" + getHostname() + " 451 ";
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

	void	Server::_shutdown()
	{
		std::list<Client>::iterator it = this->_clients.begin();

		std::cout << "Shutting down server" << std::endl;
		while (it != this->_clients.end())
		{
			this->_disconnectClient(*it);
			it = this->_clients.erase(it);
		}
	}
}
