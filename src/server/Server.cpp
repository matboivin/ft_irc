/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:39:18 by root              #+#    #+#             */
/*   Updated: 2021/11/06 17:43:54 by root             ###   ########.fr       */
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
	/* Constructor */
	Server::Server(CLIParser& CLI_parser, int backlog_max)
	: _sockfd(-1),
	  _backlog_max(backlog_max),
	  _config(CLI_parser.getBindAddress(), CLI_parser.getPort(), CLI_parser.getPassword()),
	  _parser(),
	  _commands(),
	  _clients(),
	  _channels(),
	  _logger(DEBUG)
	{
		// create a new address struct
		this->_address.sin_family = AF_INET;
		this->_address.sin_port = htons(atoi(getPort().c_str()));
		this->_address.sin_addr.s_addr = inet_addr(getBindAddress().c_str());

		//init creation date string
		time_t now = time(0);
		_creation_date = ctime(&now);
		_version = "42.42";
		_description = "The answer to life the universe and everything";

		// init map of commands
		_init_commands_map();
	}

	/* Copy constructor */
	Server::Server(const Server& other)
	: _sockfd(other._sockfd),
	  _backlog_max(other._backlog_max),
	  _creation_date(other._creation_date),
	  _version(other._version),
	  _description(other._description),
	  _config(other._config),
	  _parser(other._parser),
	  _commands(other._commands),
	  _clients(other._clients),
	  _channels(other._channels),
	  _logger(other._logger)
	{
	}

	/* Copy assignment operator */
	Server&	Server::operator=(const Server& other)
	{
		if (this != &other)
		{
			this->_sockfd = other._sockfd;
			this->_backlog_max = other._backlog_max;
			this->_creation_date = other._creation_date;
			this->_version = other._version;
			this->_description = other._description;
			this->_config = other._config;
			this->_parser = other._parser;
			this->_commands = other._commands;
			this->_clients = other._clients;
			this->_channels = other._channels;
			this->_logger = other._logger;
		}
		return (*this);
	}

	/* Destructor */
	Server::~Server()
	{
		this->_shutdown();
	}

	/* Getters ****************************************************************** */

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

	std::string	Server::getCreationDate() const
	{
		return (this->_creation_date);
	}

	std::string	Server::getVersion() const
	{
		return (this->_version);
	}

	std::string	Server::getDescription() const
	{
		return (this->_description);
	}

	Server::t_cmds	Server::getCommands() const
	{
		return (this->_commands);
	}

	Server::t_clients::iterator	Server::getClient(const std::string& nick)
	{
		t_clients::iterator	it = this->_clients.begin();

		while (it != this->_clients.end())
		{
			if (it->getNick() == nick)
				break ;
			++it;
		}
		return (it);
	}

	void	Server::_log(int level, const std::string& msg) const
	{
		_logger.log(level, msg);
	}

	Server::t_channels::iterator	Server::getChannel(const std::string& chan_name)
	{
		if (!channel_is_valid(chan_name))
			return (this->_channels.end());

		t_channels::iterator	it = this->_channels.begin();

		while (it != this->_channels.end())
		{
			if (it->getName() == chan_name)
				break ;
			++it;
		}
		return (it);
	}

	/* Main loop **************************************************************** */

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

	/* Shutting down server ***************************************************** */

	void	Server::_shutdown()
	{
		t_clients::iterator	it = this->_clients.begin();

		std::cout << "Shutting down server" << std::endl;
		while (it != this->_clients.end())
		{
			this->_disconnectClient(*it);
			it = this->_clients.erase(it);
		}
	}

	/* Connections handling ***************************************************** */

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

	/* Accepts a new connection */
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

	/* Checks if there are pending connections. (poll) */
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

					Message	welcome_msg(client);

					_make_welcome_msg(welcome_msg);
					_sendResponse(welcome_msg);
					client.setRegistered(true);
				}
			}
			if (!msg.getRecipients().empty())
				_sendResponse(msg); // send response to recipient(s)
			client.updateLastEventTime();
			return (true);
		}
		return (false);
	}

	/* Process all clients */
	bool	Server::_processClients()
	{
		for (t_clients::iterator it = this->_clients.begin();
			 it != this->_clients.end();)
		{
			if (it->isAlive() == false)
			{
				this->_disconnectClient(*it);
				it = this->_clients.erase(it);
				continue ;
			}
			if (it->updateOutBuffer())
			{
				continue ;
			}
			_processClientCommand(*it);
			it->updateInBuffer();
			if (it->isTimeouted() == true)
			{
				if (it->isPinged() == false)
				{
					this->_ping_client(*it);
				}
				else
				{
					//send them a timeout message
					Message	timeout_msg(*it);

					timeout_msg.setRecipient(*it);
					timeout_msg.setResponse("ERROR :Ping timeout: 30 seconds" CRLF);
					_sendResponse(timeout_msg);
					this->_disconnectClient(*it);
				}
			}
			++it;
		}
		return (true);
	}

	int	Server::_disconnectClient(Client& client)
	{
		t_clients::iterator	it = std::find(this->_clients.begin(), this->_clients.end(), client);

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

	int	Server::_ping_client(Client& client)
	{
		Message	msg(client);
		msg.setRecipient(client);
		msg.setCommand("PING");
		msg.setResponse("PING " + this->getHostname() + " :" + this->getHostname() + CRLF);
		this->_sendResponse(msg);
		/* reset timeout and mark the client as pinged */
		client.updateLastEventTime();
		client.setPinged(true);
		return (0);
	}

	/* Parsing ************************************************************** */

	bool	Server::_parse(Message& msg, const std::string& cmd)
	{
		return (this->_parser.parseMessage(msg, cmd));
	}

	/* Commands execution *************************************************** */

	/* Inits the map containing the commands */
	void	Server::_init_commands_map()
	{
		// this->_commands["INVITE"]	= &Server::exec_invite_cmd;
		this->_commands["JOIN"]		= &Server::exec_join_cmd;
		// this->_commands["KICK"]	= &Server::exec_kick_cmd;
		// this->_commands["KILL"]	= &Server::exec_kill_cmd;
		// this->_commands["LIST"]	= &Server::exec_list_cmd;
		this->_commands["MODE"]	= &Server::exec_mode_cmd;
		// this->_commands["NAMES"]	= &Server::exec_names_cmd;
		this->_commands["NICK"]		= &Server::exec_nick_cmd;
		this->_commands["NOTICE"]	= &Server::exec_notice_cmd;
		this->_commands["OPER"]		= &Server::exec_oper_cmd;
		this->_commands["PART"]		= &Server::exec_part_cmd;
		this->_commands["PASS"]		= &Server::exec_pass_cmd;
		this->_commands["PING"]		= &Server::exec_ping_cmd;
		this->_commands["PONG"]		= &Server::exec_pong_cmd;
		this->_commands["PRIVMSG"]	= &Server::exec_privmsg_cmd;
		this->_commands["QUIT"]		= &Server::exec_quit_cmd;
		this->_commands["TOPIC"]	= &Server::exec_topic_cmd;
		this->_commands["USER"]		= &Server::exec_user_cmd;
		this->_commands["WHO"]		= &Server::exec_who_cmd;
		this->_commands["WHOIS"]	= &Server::exec_whois_cmd;
		// debug
		this->_commands["TEST"]		= &Server::exec_test_cmd;
	}

	/* Execute a command */
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

	/* Command response ********************************************************* */

	/* Sets response recipient(s) (channels or clients) */
	void	Server::_setResponseRecipients(Message& msg)
	{
		std::cout << __LINE__ << std::endl;
		t_clients::iterator	dst = getClient(msg.getParams().at(0));

		if (dst != this->_clients.end())
		{
			msg.setRecipient(*dst);
			return ;
		}

		if (msg.getParams().empty())
		{
			return ;
		}
		t_channels::iterator	channel = getChannel(msg.getParams().at(0));

		if (channel != this->_channels.end())
			msg.setRecipients(channel->getClients());
	}

	/* Sends response */
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
		//	std::cout << "Sending: '" << logOutput << "' to " << (*dst)->getIpAddressStr() << std::endl;
			_log(0, "Sending: '" + logOutput + "' to " + (*dst)->getIpAddressStr());	
			if (send((*dst)->getSocketFd(), msg.getResponse().c_str(), msg.getResponse().size(), 0) < 0)
			{
				throw std::runtime_error("send() failed");
			}
		}
	}

	/* Sends a nice welcome message */
	void	Server::_make_welcome_msg(Message& msg)
	{
		rpl_welcome(msg);
		rpl_yourhost(msg, this->_version);
	}

	/* Channel operations ******************************************************* */

	/* Adds a new channel to the server's list */
	Channel&	Server::_addChannel(const std::string& name)
	{
		this->_channels.push_back(Channel(name));
		return (this->_channels.back());
	}

	/* Removes a channel from the server's list */
	void	Server::_removeChannel(t_channels::iterator channel)
	{
		this->_channels.erase(channel);
	}

	/* Checks whether a client is in a specific channel */
	bool	Server::_userOnChannel(Client& client, Channel& channel)
	{
		return (channel.hasClient(client)); 
	}

	bool	Server::_userOnChannel(Client& client, const std::string& chan_name)
	{
		t_channels::iterator	channel = getChannel(chan_name);

		if (channel != this->_channels.end())
			return (channel->hasClient(client));
		return (false);
	}

	/* Adds a user to a channel */
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

	/* Removes user from channel */
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

	/* Removes a user from all joined channels */
	void	Server::_removeUserFromAllChannels(Client& client, Message& msg)
	{
		msg.setRecipients(client.getAllContacts());
		client.partAllChannels();

		client.displayJoinedChannels(); // debug
		std::cout << std::endl;
	}

	/* Oper operations ********************************************************** */

	bool	Server::_giveOperPriv(const std::string& name, const std::string& password)
	{
		return (this->_config.operBlockIsValid(name, password));
	}

	/* Commands ***************************************************************** */

	//void	Server::exec_invite_cmd(Message& msg);

	/*
	 * JOIN <channels>
	 * JOIN 0: leave all channels
	 */
	void	Server::exec_join_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg, true);
		else if (msg.getParams().at(0) == "0") // JOIN 0
			_removeUserFromAllChannels(msg.getSender(), msg);
		else
		{
			for (std::vector<std::string>::const_iterator chan_name = msg.getParams().begin();
				 chan_name != msg.getParams().end();
				 ++chan_name)
			{
				t_channels::iterator	channel = getChannel(*chan_name);

				if (!channel_is_valid(*chan_name))
				{
					err_nosuchchannel(msg, *chan_name);
					return ;
				}
				if (channel == this->_channels.end())
					_addUserToChannel(msg.getSender(), _addChannel(*chan_name));
				else
					_addUserToChannel(msg.getSender(), *channel);
			}
		}
	}

	//void	Server::exec_kick_cmd(Message& msg);

	//void	Server::exec_kill_cmd(Message& msg);

	//void	Server::exec_list_cmd(Message& msg);

	//void	Server::exec_names_cmd(Message& msg);

	/*
	 * NICK <nickname>
	 * Change a user nickname
	 */
	void	Server::exec_nick_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_nonicknamegiven(msg, true);
		else
		{
			std::string	new_nick = msg.getParams().at(0);

			if (!nick_is_valid(new_nick))
				err_erroneusnickname(msg, true);
			else if (getClient(new_nick) != this->_clients.end())
				err_nicknameinuse(msg, true);
			else
				msg.getSender().setNick(new_nick);
		}
	}

	/*
	 * NOTICE <msgtarget> :<message>
	 * Send messages to a user or a channel
	 * The server musn't reply to NOTICE message
	 */
	void	Server::exec_notice_cmd(Message& msg)
	{
		if (msg.getParams().size() < 2) // params are mandatory
			msg.clearRecipients();
		else
			_setResponseRecipients(msg);
	}

	/*
	 * OPER <username> <password>
	 * Authenticates a user as an IRC operator if the username/password combination exists
	 * for that server
	 */
	void	Server::exec_oper_cmd(Message& msg)
	{
		if (msg.getParams().size() < 2)
			err_needmoreparams(msg, true);
		else if (!msg.getSender().isOper())
		{
			if (_giveOperPriv(msg.getParams().at(0), msg.getParams().at(1)))
			{
				msg.getSender().addMode("o");

				Message	rpl_msg(msg.getSender()); // tmp

				rpl_youreoper(rpl_msg);
				_sendResponse(rpl_msg);
				exec_mode_cmd(msg);
			}
			else
				err_passwdmismatch(msg, true);
		}
	}

	void	Server::exec_mode_cmd(Message& msg)
	{
		if (msg.getParams().size() < 2)
			err_needmoreparams(msg);
		else
		{
			std::string	target = msg.getParams().at(0);
			std::string	mode_str = msg.getParams().at(1);

			if (target == "0")
				err_nosuchnick(msg, target);
			else if (target == "*")
				err_needmoreparams(msg);
			else if (target[0] == '#')
			{
				t_channels::iterator	channel = getChannel(target);

				if (channel == this->_channels.end())
					err_nosuchchannel(msg, target);
				else
					channel->setMode(mode_str);
			}
			else
			{
				t_clients::iterator	client = getClient(target);

				if (client == this->_clients.end())
					err_nosuchnick(msg, target);
				else
					_setUserMode(*client, mode_str, msg);
			}
		}	
	}

	int Server::_setUserMode(Client &client, const std::string& mode_str, Message& msg)
	{
		std::string	mode_str_copy = mode_str;
		int			error_code = 0;

		if (mode_str_copy.empty())
			return (1);
		if (mode_str_copy[0] == '+')
			mode_str_copy.erase(0, 1);
		for (std::string::iterator mode_char = mode_str_copy.begin();
			 mode_char != mode_str_copy.end();
			 ++mode_char)
		{
			if (*mode_char == '-')
			{
				error_code = client.removeMode(*mode_char);
			}
			else
			{
				error_code = client.addMode(*mode_char);
			}
			if (error_code)
			{
				//Append error message
				msg.appendResponse(error_msg[error_code]);
			}
		}
		return (0);
	}

	int	Client::addMode(char mode_char)
	{
		std::string valid_modes = "iswo";

		if (valid_modes.find(mode_char) != std::string::npos)
		{
			this->_mode += mode_char;
			return (ERR_SUCCESS);
		}
		return (ERR_UNKNOWNMODE);
	}

	int	Client::removeMode(char mode_char)
	{
		std::string valid_modes = "iswo";

		if (valid_modes.find(mode_char) != std::string::npos)
		{
			this->_mode.erase(this->_mode.find(mode_char), 1);
			return (ERR_SUCCESS);
		}
		return (ERR_UNKNOWNMODE);
	}

	/*
	 * PART <channels> [<message>]
	 * The client quits the channel(s)
	 */
	void	Server::exec_part_cmd(Message& msg)
	{
		if (msg.getParams().empty())
		{
			err_needmoreparams(msg, true);
			return ;
		}

		for (std::vector<std::string>::const_iterator chan_name = msg.getParams().begin();
			 chan_name != msg.getParams().end();
			 ++chan_name)
		{
			if ( ((*chan_name)[0] == ':') && (++chan_name == msg.getParams().end()) )
				break ;

			t_channels::iterator	channel = getChannel(*chan_name);

			if (channel == this->_channels.end())
				err_nosuchchannel(msg, *chan_name, true);
			else if (!_userOnChannel(msg.getSender(), *channel))
				err_notonchannel(msg, *chan_name, true);
			else
			{
				_removeUserFromChannel(msg.getSender(), *channel);
				if (channel->isEmpty())
					_removeChannel(channel);
			}
		}
	}

	/*
	 * PASS <password>
	 * set a connection password
	 */
	void	Server::exec_pass_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg, true);
		else if (msg.getSender().isRegistered())
			err_alreadyregistered(msg, true);
		else
			msg.getSender().setPassword(msg.getParams().at(0));
	}

	/*
	 * PING <server1>
	 */
	void	Server::exec_ping_cmd(Message& msg)
	{
		std::string	origin;

		if (msg.getParams().empty())
			err_needmoreparams(msg, error_msg[ERR_NOORIGIN]);
		else
		{
			origin = msg.getParams().front();

			msg.setRecipient(msg.getSender());
			msg.setResponse(
				build_prefix(msg.getServHostname())
			+ " PONG " + msg.getSender().getNick() + msg.getServHostname()
			+ " :" + origin + CRLF
			);
		}
	}

	/*
	 * PONG <server1>
	 */
	void	Server::exec_pong_cmd(Message& msg)
	{
		std::string	origin;

		if (msg.getParams().empty())
			err_needmoreparams(msg, error_msg[ERR_NOORIGIN]);
		else
		{
			origin = msg.getParams().front();

			msg.setRecipient(msg.getSender());
			msg.setResponse("");
		}
	}

	/*
	 * PRIVMSG <msgtarget> :<message>
	 * Send messages to a user or a channel
	 */
	void	Server::exec_privmsg_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_norecipient(msg, true);
		else if (msg.getParams().size() < 2)
			err_notexttosend(msg, true);
		else
		{
			std::string	msgtarget = msg.getParams().at(0);

			if (channel_is_valid(msgtarget) && !_userOnChannel(msg.getSender(), msgtarget))
				err_cannotsendtochan(msg, true);
			else if (!channel_is_valid(msgtarget) && getClient(msgtarget) == this->_clients.end() )
				err_nosuchnick(msg, msgtarget, true);
			else
				_setResponseRecipients(msg);
		}
	}

	/*
	 * QUIT [<message>]
	 * A client session is terminated with a quit message
	 */
	void	Server::exec_quit_cmd(Message& msg)
	{
		msg.setRecipients(msg.getSender().getAllContacts());
		// TODO: if no message, set default message?
		msg.getSender().setAlive(false);
		// TODO: The server acknowledges this by sending an ERROR message to the client
	}

	/*
	 * TOPIC <channel> [ <topic> ]
	 * Changes or displays the topic of a channel
	 */
	void	Server::exec_topic_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg, true);
		else
		{
			std::string				chan_name = msg.getParams().at(0);
			t_channels::iterator	channel = getChannel(chan_name);

			if (channel == this->_channels.end())
				err_nosuchchannel(msg, chan_name, true);
			else if (!_userOnChannel(msg.getSender(), *channel))
				err_notonchannel(msg, chan_name, true);
			else if (msg.getParams().size() > 1 && !channel_is_valid(msg.getParams().at(1)))
				channel->changeTopic(msg.getParams().at(1), msg);
			else if (channel->getTopic().empty())
				rpl_notopic(msg, channel->getName(), true);
			else
				rpl_topic(msg, *channel, true);
		}
	}

	/*
	 * USER <user> <mode> <unused> <realname>
	 */
	void	Server::exec_user_cmd(Message& msg)
	{
		std::string	username;
		std::string	realname;
		std::string	hostname;
		Client&		client = msg.getSender();
		//copy list of parameters
		std::vector<std::string>	params = msg.getParams();
	
		if (msg.getParams().size() < 4)
			err_needmoreparams(msg, true);
		else
		{
			client.setUsername(params.at(0));
			client.setRealName(params.at(1));
			client.setHostname(params.at(2));
			msg.setResponse("");
			msg.setRecipient(client);
		}
	}

	/*
	 * WHO [ "o" ]
	 */
	void	Server::exec_who_cmd(Message& msg)
	{
		bool			oper_only = false;
		std::string		to_match = "0"; //match all by default
		size_t			count = 0; // limit of 25

		if (msg.getParams().empty() == false)
			to_match = msg.getParams().front();
		else if (msg.getParams().size() == 2 && msg.getParams().back() == "o")
			oper_only = true;
		else if (msg.getParams().size() > 2)
		{
			err_syntaxerror(msg, msg.getCommand());
			return ;
		}
		std::string response = "";
		for (t_clients::iterator it = this->_clients.begin();
			 it != this->_clients.end();
			 ++it)
		{
			if (oper_only && !it->isOper())
				continue;
			if (match_nick(to_match, it->getNick()))
			{
				std::cout << "WHO matched " << it->getNick() << std::endl;
				response += ":" + this->getHostname() + " 352 " + msg.getSender().getNick() + " * ";
				response += it->getNick();
				response += " " + it->getIpAddressStr();
				response += " " + this->getHostname();
				response += " " + it->getNick();
				response += " H :0 " + it->getNick() + CRLF;
			}
			count++;
			if (count == 25)
			{
				//:public-irc.w3.org NOTICE mynick :WHO list limit (25) reached!
				response += ":" + this->getHostname() + " NOTICE "
				+ msg.getSender().getNick() + " :WHO list limit (25) reached!"
				+ CRLF;
			}
		}
		//END OF WHO COMMAND
		response += ":" + this->getHostname() + " 315 " +
		msg.getSender().getNick() + " " + to_match + " :End of /WHO list."
		+ CRLF;
		msg.setResponse(response);
		msg.setRecipient(msg.getSender());
	}

	/*
	 * WHOIS [target]
	 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.5.2
	*/
	void	Server::exec_whois_cmd(Message& msg)
	{
		std::string	to_match = "0";
		if (msg.getParams().empty() == false)
			to_match = msg.getParams().front();
		else if (msg.getParams().size() > 1)
		{
			err_syntaxerror(msg, msg.getCommand());
			return ;
		}
		std::string	response = "";
		for (t_clients::iterator it = this->_clients.begin();
			 it != this->_clients.end();
			 ++it)
		{
			if (match_nick(to_match, it->getNick()))
			{
				response += ":" + this->getHostname() + " 311 " + msg.getSender().getNick() + " ";
				response += it->getNick();
				response += " " + it->getUsername();
				response += " " + it->getHostname();
				response += " * :";
				response += it->getRealName();
				response += CRLF;
				response += ":" + this->getHostname() + " 312 " + msg.getSender().getNick() + " ";
				response += this->getHostname();
				response += " :";
				response += this->_description;
				response += CRLF;
				if (it->isOper())
				{
					response += ":" + this->getHostname() + " 313 " + msg.getSender().getNick() + " ";
					response += it->getNick();
					response += " :is an IRC operator" CRLF;
				}
			}
		}
		response += ":" + this->getHostname() + " 318 " + msg.getSender().getNick() +
		" " + to_match + " :End of /WHOIS list." + CRLF;
		msg.setResponse(response);
		msg.setRecipient(msg.getSender());
	}

	/* ************************************************************************** */

	// debug
	void	Server::exec_test_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg, true);
		for (std::vector<std::string>::const_iterator it = msg.getParams().begin();
			 it != msg.getParams().end();
			 ++it)
		{
			std::cout << *it << std::endl;
		}
	}

	int	Server::_sendList(Client& client)
	{
		for (t_clients::iterator it = this->_clients.begin();
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
}
