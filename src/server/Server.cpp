/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:39:18 by root              #+#    #+#             */
/*   Updated: 2021/12/01 00:28:08 by mboivin          ###   ########.fr       */
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
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "ft_irc.hpp"

namespace ft_irc
{
	/* Constructor */
	Server::Server(CLIParser& CLI_parser, int backlog_max)
	: _address(),
	  _sockfd(-1),
	  _backlog_max(backlog_max),
	  _creation_date(),
	  _version("42.42"),
	  _description("The answer to life the universe and everything"),
	  _config(CLI_parser.getBindAddress(), CLI_parser.getPort(), CLI_parser.getPassword()),
	  _parser(),
	  _commands(),
	  _clients(),
	  _channels(),
	  _logger(DEBUG)
	{
		_log(LOG_LEVEL_INFO, "Server constructor");

		// create a new address struct
		this->_alive=false;
		this->_address.sin_family = AF_INET;
		this->_address.sin_port = htons(atoi(getPort().c_str()));
		this->_address.sin_addr.s_addr = inet_addr(getBindAddress().c_str());

		//init creation date string
		time_t	now = time(0);
		this->_creation_date = ctime(&now);

		// init map of commands
		_init_commands_map();

		_log(LOG_LEVEL_INFO, "Server constructed");
	}

	/* Copy constructor */
	Server::Server(const Server& other)
	: _address(other._address),
	  _sockfd(other._sockfd),
	  _backlog_max(other._backlog_max),
	  _creation_date(other._creation_date),
	  _version(other._version),
	  _description(other._description),
	  _config(other._config),
	  _parser(other._parser),
	  _commands(other._commands),
	  _clients(other._clients),
	  _channels(other._channels),
	  _logger(other._logger),
	  _alive(other._alive)
	{
		_log(LOG_LEVEL_INFO, "Server copy constructor");
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
			this->_alive = other._alive;
		}
		return (*this);
	}

	/* Destructor */
	Server::~Server()
	{
		_log(LOG_LEVEL_FATAL, "Server destructor.");
		_shutdown();
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

	const Server::t_clients&	Server::getClients() const
	{
		return (this->_clients);
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

	/* Checkers ***************************************************************** */

	bool	Server::isAlive() const
	{
		return (this->_alive);
	}

	/* Main loop **************************************************************** */

	int	Server::run()
	{
		if (_createSocket() == false)
			return (-1);

		_log(LOG_LEVEL_INFO, "Server listening on localhost:" + this->getPort());
		this->_alive = true;

		while (_alive)
		{
			if (_hasPendingConnections() == true)
				_awaitNewConnection();
			_processClients();
		}
		_log(LOG_LEVEL_FATAL, "Server has died");
		return (-1);
	}

	/* Shutting down server ***************************************************** */

	void	Server::_shutdown()
	{
		t_clients::iterator	it = this->_clients.begin();

		_log(LOG_LEVEL_FATAL, "Shutting down server");
		this->_alive = false;

		while (it != this->_clients.end())
		{
			_disconnectClient(*it);
			_log(LOG_LEVEL_INFO,
				"Client " + it->getNick() + "@" + it->getIpAddressStr() + " disconnected");
			it = this->_clients.erase(it);
		}
	}

	/* Logging ****************************************************************** */

	void	Server::_log(int level, const std::string& msg) const
	{
		_logger.log(level, msg);
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
			_log(LOG_LEVEL_FATAL, "Error: Could not create socket.");
			return (false);
		}
		setNonblocking(this->_sockfd);

		//Set socket options.
		int	optval = 1;

		if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		{
			_log(LOG_LEVEL_FATAL, "Error: Could not set socket options.");
			return (false);
		}
		//Bind the socket to the address.
		if (bind(this->_sockfd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
		{
			_log(LOG_LEVEL_FATAL, "Error: Could not bind socket.");
			return (false);
		}
		//Listen for connections.
		if (listen(this->_sockfd, this->_backlog_max) < 0)
		{
			_log(LOG_LEVEL_FATAL, "Error: Could not listen on socket.");
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
		_log(LOG_LEVEL_INFO, "Client " + new_client.getIpAddressStr() + " connected");

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
			_log(LOG_LEVEL_ERROR, "Error: Could not poll socket.");
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
			Message	msg(client, getHostname());

			if (_parse(msg, client.popUnprocessedCommand()) == true) // parse the message
			{
				_executeCommand(msg); // execute the command
				//if the client has just registered, send him a nice welcome message :D
				if (client.isRegistered() == false && !client.getNick().empty() &&
					!client.getUsername().empty() && !client.getHostname().empty())
				{
					_log(LOG_LEVEL_INFO,
						"Client " + client.getNick() + "@" + client.getIpAddressStr()
						+ " has just registered");
					_make_welcome_msg(client);
				}
			}
			// if (!msg.getRecipients().empty() && !msg.getResponse().empty())
			// 	_sendResponse(msg); // send response to recipient(s)
			// else
			// {
			// 	_log(LOG_LEVEL_ERROR,
			// 		"Message \"" + msg.getResponse() + "\" from " +
			// 		msg.getSender().getNick() + "@" +
			// 		msg.getSender().getIpAddressStr() + " has no recipients.");
			// 	return (false);
			// }
			client.updateLastEventTime();
			return (true);
		}
		return (false);
	}

	/* Process all clients */
	bool	Server::_processClients()
	{
		for (t_clients::iterator it = this->_clients.begin();
			 it != this->_clients.end();
			 ++it)
		{
			if (it->isAlive() == false)
			{
				_disconnectClient(*it);
				_log(LOG_LEVEL_INFO, "Client " + it->getNick() +
				"@" + it->getIpAddressStr() + " disconnected");
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
					_disconnectClient(*it);
				}
			}
		}
		return (true);
	}

	int	Server::_disconnectClient(Client& client)
	{
		t_clients::iterator	it = std::find(this->_clients.begin(), this->_clients.end(), client);

		if (it == this->_clients.end())
		{
			_log(LOG_LEVEL_ERROR, "Client " + client.getNick() + client.getIpAddressStr()
			+ " is not in the client list!");
			return (-1);
		}
		if (it->getSocketFd() > 0)
		{
			//send them a goodbye message
			Message	goodbye_msg(*it);
			
			goodbye_msg.setRecipient(*it);
			goodbye_msg.setResponse("ERROR :Closing Link: " + it->getIpAddressStr() + CRLF);
			_sendResponse(goodbye_msg);
			close(it->getSocketFd());
			it->setSocketFd(-1);
		}
		it->setAlive(false);
		it->setConnected(false);
		return (0);
	}

	int	Server::_ping_client(Client& client)
	{
		Message	msg(client);

		msg.setRecipient(client);
		msg.setCommand("PING");
		msg.setResponse("PING " + getHostname() + " :" + getHostname() + CRLF);
		_sendResponse(msg);
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

	Parser::t_params	Server::_splitListOfParams(const std::string& params)
	{
		return (this->_parser.splitListOfParams(params));
	}

	/* Commands execution *************************************************** */

	/* Inits the map containing the commands */
	void	Server::_init_commands_map()
	{
		this->_commands["INVITE"]	= &Server::_execInviteCmd;
		this->_commands["JOIN"]		= &Server::_execJoinCmd;
		this->_commands["KICK"]		= &Server::_execKickCmd;
		this->_commands["KILL"]		= &Server::_execKillCmd;
		this->_commands["LIST"]		= &Server::_execListCmd;
		this->_commands["MODE"]		= &Server::_execModeCmd;
		this->_commands["NAMES"]	= &Server::_execNamesCmd;
		this->_commands["NICK"]		= &Server::_execNickCmd;
		this->_commands["NOTICE"]	= &Server::_execNoticeCmd;
		this->_commands["OPER"]		= &Server::_execOperCmd;
		this->_commands["PART"]		= &Server::_execPartCmd;
		this->_commands["PASS"]		= &Server::_execPassCmd;
		this->_commands["PING"]		= &Server::_execPingCmd;
		this->_commands["PONG"]		= &Server::_execPongCmd;
		this->_commands["PRIVMSG"]	= &Server::_execPrivmsgCmd;
		this->_commands["QUIT"]		= &Server::_execQuitCmd;
		this->_commands["TOPIC"]	= &Server::_execTopicCmd;
		this->_commands["USER"]		= &Server::_execUserCmd;
		this->_commands["WHO"]		= &Server::_execWhoCmd;
		this->_commands["WHOIS"]	= &Server::_execWhoisCmd;
	}

	/* Execute a command */
	int	Server::_executeCommand(Message& msg)
	{
		t_cmds::const_iterator	it = this->_commands.find(msg.getCommand());

		if (it != this->_commands.end())
		{
			_log(LOG_LEVEL_DEBUG, "Executing command \"" + msg.getCommand() + "\"");
			(this->*it->second)(msg);
		}
		else
			_sendResponse(msg); // send error 421 Unknown command
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
			return ;

		t_channels::iterator	channel = getChannel(msg.getParams().at(0));

		if (channel != this->_channels.end())
		{
			Channel::t_clients	recipients = channel->getClients();

			recipients.remove(&msg.getSender());
			msg.setRecipients(recipients);
		}
	}

	/* Sends response */
	void	Server::_sendResponse(Message& msg)
	{
		if (msg.getRecipients().empty())
			return ;

		Message::t_clients	recipients = msg.getRecipients();
		std::string			logOutput;

		for (Message::t_clients::const_iterator	dst = recipients.begin();
			 dst != recipients.end();
			 ++dst)
		{
			logOutput = msg.getResponse();
			size_t	pos = logOutput.find(CRLF);

			if (pos != std::string::npos)
				logOutput.replace(pos, 2, CRLF_PRINTABLE);
			_log(LOG_LEVEL_DEBUG, "Sending: '" + logOutput + "' to " + (*dst)->getIpAddressStr());
			if (send((*dst)->getSocketFd(), msg.getResponse().c_str(), msg.getResponse().size(), 0) < 0)
				throw std::runtime_error("send() failed");
		}
	}

	/* Sends a nice welcome message */
	void	Server::_make_welcome_msg(Client& client)
	{
		Message	welcome_msg(client, getHostname());

		rpl_welcome(welcome_msg);
		rpl_yourhost(welcome_msg, this->_version);
		rpl_created(welcome_msg, this->_creation_date);
		rpl_myinfo(welcome_msg, this->_version);
		_sendResponse(welcome_msg);
		client.setRegistered(true);
	}

	/* Channel operations ******************************************************* */

	/* Adds a new channel to the server's list */
	Channel&	Server::_addChannel(const std::string& name, Client& creator)
	{
		this->_channels.push_back(Channel(name));

		Channel	chan = this->_channels.back();
		chan.addChanOp(creator);
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
			_log(LOG_LEVEL_DEBUG, "Add " + client.getNick() + " to channel " + channel.getName());
			client.joinChannel(channel);

			Message	join_msg(client);

			join_msg.setRecipients(channel.getClients());
			join_msg.setResponse(build_prefix(build_full_client_id(client)));
			join_msg.appendResponse(" JOIN ");
			join_msg.appendResponse(channel.getName());
			join_msg.appendSeparator();
			_sendResponse(join_msg);
		}
	}

	/* Removes user from channel */
	void	Server::_removeUserFromChannel(Client& client, t_channels::iterator channel,
										   const std::string& comment)
	{
		if (_userOnChannel(client, *channel))
		{
			_log(LOG_LEVEL_DEBUG, "Remove " + client.getNick() + " from channel " + channel->getName());

			client.partChannel(*channel);

			Message	part_msg(client);

			part_msg.setRecipients(channel->getClients());
			part_msg.setResponse(build_prefix(build_full_client_id(client)));
			part_msg.appendResponse(" PART ");
			part_msg.appendResponse(channel->getName());
			if (!comment.empty())
			{
				part_msg.appendResponse(" ");
				part_msg.appendResponse(comment);
			}
			part_msg.appendSeparator();
			_sendResponse(part_msg);

			if (channel->isEmpty())
				_removeChannel(channel);
		}
	}

	/* Removes a user from all joined channels */
	void	Server::_removeUserFromAllChannels(Client& client, const std::string& comment)
	{
		Client::t_channels	joined_channels = client.getJoinedChannels();

		for (Client::t_channels::const_iterator it = joined_channels.begin();
			 it != joined_channels.end();
			 ++it)
		{
			_removeUserFromChannel(client, getChannel((*it)->getName()), comment);
		}
	}

	/* Oper operations ********************************************************** */

	bool	Server::_userCanBeOper(const std::string& name)
	{
		return (this->_config.operUserIsValid(name));
	}

	bool	Server::_canGiveOperPriv(const std::string& name, const std::string& password)
	{
		return (this->_config.operBlockIsValid(name, password));
	}

	/* Commands ***************************************************************** */

	/*
	 * INVITE <nickname> <channel>
	 * Invite a user to a channel
	 */
	void	Server::_execInviteCmd(Message& msg)
	{
		if (msg.getParams().size() != 2)
			err_needmoreparams(msg);
		else if (getClient(msg.getParams().at(0)) == this->_clients.end())
			err_nosuchnick(msg, msg.getParams().at(0));
		else if (channel_is_valid(msg.getParams().at(1)))
		{
			std::string				guest = msg.getParams().at(0);
			t_clients::iterator		guest_user = getClient(guest);
			std::string				chan_name = msg.getParams().at(1);
			t_channels::iterator	channel = getChannel(chan_name);
			bool					chan_exists = (channel != this->_channels.end());

			if (chan_exists && !msg.getSender().isChanOp(*channel))
				err_chanoprivsneeded(msg, chan_name);
			else if (chan_exists && !_userOnChannel(msg.getSender(), *channel))
				err_notonchannel(msg, chan_name);
			else if (chan_exists && _userOnChannel(*guest_user, *channel))
				err_useronchannel(msg, guest, chan_name);
			else
			{
				Message	invite_msg(msg.getSender(), getHostname());

				rpl_inviting(invite_msg, chan_name, guest);
				_sendResponse(invite_msg);
			}
		}
		_sendResponse(msg);
	}

	/*
	 * JOIN <channels>
	 * JOIN 0: leave all channels
	 */
	void	Server::_execJoinCmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg, true);
		else if (msg.getParams().at(0) == "0")
			_removeUserFromAllChannels(msg.getSender());
		else
		{
			Message	names_msg(msg.getSender(), msg.getServHostname());

			for (std::vector<std::string>::const_iterator chan_name = msg.getParams().begin();
				 chan_name != msg.getParams().end();
				 ++chan_name)
			{
				if (!channel_is_valid(*chan_name))
					return ;

				t_channels::iterator	channel = getChannel(*chan_name);

				if (channel == this->_channels.end())
				{
					_addUserToChannel(msg.getSender(), _addChannel(*chan_name, msg.getSender()));
					channel = getChannel(*chan_name);
				}
				else
					_addUserToChannel(msg.getSender(), *channel);
				rpl_topic(msg, *channel);
				_sendResponse(msg);
				names_msg.clearParams();
				names_msg.setParam(channel->getName());
				_execNamesCmd(names_msg);
				_sendResponse(names_msg);
			}
		}
	}

	/*
	 * KICK <channel> <client> [<comment>]
	 * Forcibly removes <client> from <channel>
	 *
	 * The user must be a channel operator.
	 */
	void	Server::_kickClient(Message& msg,
								const std::string& chan_name, const std::string& nick,
								const std::string& comment
								)
	{
		t_channels::iterator	channel = getChannel(chan_name);
		t_clients::iterator		user = getClient(nick);

		if (channel == this->_channels.end())
			err_nosuchchannel(msg, chan_name);
		else if (!msg.getSender().isChanOp(*channel))
			err_chanoprivsneeded(msg, chan_name);
		else if (!_userOnChannel(msg.getSender(), *channel))
			err_notonchannel(msg, chan_name);
		else if ((user != this->_clients.end()) && !_userOnChannel(*user, *channel))
			err_usernotinchannel(msg, nick, chan_name);
		else if (user == this->_clients.end())
			err_nosuchnick(msg, nick);
		else
		{
			msg.setRecipients(channel->getClients());
			msg.setResponse(build_prefix(build_full_client_id( msg.getSender())));
			msg.appendResponse(" KICK ");
			msg.appendResponse(chan_name);
			msg.appendResponse(" ");
			msg.appendResponse(nick);
			if (!comment.empty())
			{
				msg.appendResponse(" ");
				msg.appendResponse(comment);
			}
			msg.appendSeparator();
		}
		_sendResponse(msg);
	}

	/*
	 * KICK <channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>]
	 *
	 * The server MUST NOT send KICK messages with multiple channels or
	 * users to clients.  This is necessarily to maintain backward
	 * compatibility with old client software.
	 */
	void	Server::_execKickCmd(Message& msg)
	{
		if (msg.getParams().size() < 2)
			err_needmoreparams(msg);
		else
		{
			std::string	comment = "";
			Parser::t_params	chan_names = _splitListOfParams(msg.getParams().at(0));
			Parser::t_params	nicknames = _splitListOfParams(msg.getParams().at(1));

			if (msg.getParams().size() == 3)
				comment = msg.getParams().at(2);

			std::size_t	len = chan_names.size();

			for (std::size_t i = 0; i < len; ++i)
				_kickClient(msg, chan_names[i], nicknames[i], comment);
			// Handle different number of channels and nicknames
			// if (len != nicknames.size())
		}
	}

	/*
	 * KILL <client> <comment>
	 * Cause a client-server connection to be closed by the server.
	 *
	 * The user must be an IRC operator.
	 * The comment given is the actual reason for the KILL.
	 */
	void	Server::_execKillCmd(Message& msg)
	{
		if (msg.getParams().size() < 2)
			err_needmoreparams(msg, true);
		else if (!msg.getSender().isOper())
			err_noprivileges(msg, true);
		else if (msg.getParams().at(0) == getHostname())
			err_cantkillserver(msg, true);
		else
		{
			std::string			nick = msg.getParams().at(0);
			t_clients::iterator	user = getClient(nick);

			if (user == this->_clients.end())
				err_nosuchnick(msg, nick, true);
			else
				user->setAlive(false); // will be clean by server loop
		}
		_sendResponse(msg);
	}

	/*
	 * LIST [<channel>]
	 *
	 * If <channel> is specified, only lists information about that channel.
	 * Otherwise, lists all channels and their topics.
	 */

	void	Server::_execListCmd(Message& msg)
	{
		bool	matchAll = msg.getParams().size() == 0;

		msg.clearResponse();
		msg.setRecipient(msg.getSender());

		for (t_channels::iterator channels_it = this->_channels.begin();
			 channels_it != this->_channels.end();
			 ++channels_it)
		{
			if (matchAll || is_string_in_msg_params(msg, channels_it->getName()))
				rpl_list(msg, *channels_it);
		}
		rpl_listend(msg);
		_sendResponse(msg);
	}

	/* Helper for MODE */
	int	Server::_setUserMode(Client &client, const std::string& mode_str, Message& msg)
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

	/*
	 * MODE <nickname> <flags>
	 * MODE <channel> <flags>
	 * Set a user or a channel mode.
	 */
	void	Server::_execModeCmd(Message& msg)
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
					err_usersdontmatch(msg);
				else
					_setUserMode(*client, mode_str, msg);
			}
		}
		_sendResponse(msg);
	}

	//https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.5
	/*
	 * NAMES <channel> *( "," <channel> )
	 *
	 *	By using the NAMES command, a user can list all nicknames that are
	 *	visible to them on any channel that they can see.  Channel names
	 *	which they can see are those which aren't private (+p) or secret (+s)
	 *	or those which they are actually on.  The <channel> parameter
	 *	specifies which channel(s) to return information about if valid.
	 *	There is no error reply for bad channel names.

	 *	If no <channel> parameter is given, a list of all channels and their
	 *	occupants is returned.  At the end of this list, a list of users who
	 *	are visible but either not on any channel or not on a visible channel
	 *	are listed as being on `channel' "*".
	 */
	void	Server::_execNamesCmd(Message& msg)
	{
		bool				matchAll = msg.getParams().size() == 0;
		std::stringstream	params_size;

		msg.clearResponse();
		msg.setRecipient(msg.getSender());
		params_size << msg.getParams().size(); // convert size_t to string
		_logger(LOG_LEVEL_DEBUG, params_size.str());

		for (t_channels::iterator channels_it = this->_channels.begin();
			 channels_it != this->_channels.end();
			 ++channels_it)
		{
			if (matchAll || is_string_in_msg_params(msg, channels_it->getName()))
				rpl_namreply(msg, *channels_it);
			rpl_endofnames(msg, channels_it->getName());
		}
		if (matchAll)
		{
			rpl_namreply(msg, getClients());
			rpl_endofnames(msg, "*");
		}
		_sendResponse(msg);
	}

	/*
	 * NICK <nickname>
	 * Change a user nickname
	 */
	void	Server::_execNickCmd(Message& msg)
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
			{
				if (new_nick.size() > 16)
					new_nick.resize(16);

				msg.setRecipient(msg.getSender());
				msg.addRecipients(msg.getSender().getAllContacts());
				msg.setResponse(build_prefix(build_full_client_id(msg.getSender())));
				msg.appendResponse(" NICK ");
				msg.appendResponse(new_nick);
				msg.appendSeparator();
				msg.getSender().setNick(new_nick);
			}
		}
		_sendResponse(msg);
	}

	/*
	 * NOTICE <msgtarget> :<message>
	 * Send messages to a user or a channel
	 * The server musn't reply to NOTICE message
	 */
	void	Server::_execNoticeCmd(Message& msg)
	{
		if (msg.getParams().size() < 2) // params are mandatory
			return ;
		_setResponseRecipients(msg);
		_sendResponse(msg);
	}

	/*
	 * OPER <username> <password>
	 * Authenticates a user as an IRC operator if the username/password combination exists
	 * for that server
	 */
	void	Server::_execOperCmd(Message& msg)
	{
		if (msg.getParams().size() < 2)
			err_needmoreparams(msg, true);
		else if (!msg.getSender().isOper())
		{
			if (!_userCanBeOper(msg.getParams().at(0)))
				err_nooperhost(msg, true);
			if (!_canGiveOperPriv(msg.getParams().at(0), msg.getParams().at(1)))
				err_passwdmismatch(msg, true);
			else
			{
				std::string	nick = msg.getSender().getNick();

				Message	rpl_msg(msg);

				msg.setRecipient(msg.getSender());
				msg.setCommand("MODE");
				msg.clearParams();
				msg.setParam(nick);
				msg.setParam("+o");
				msg.setResponse(build_prefix(getHostname()));
				msg.appendResponse(" MODE ");
				msg.appendResponse(nick);
				msg.appendResponse(" +o");
				msg.appendSeparator();
				_execModeCmd(msg);
				if (msg.getSender().isOper() == true)
				{
					rpl_youreoper(rpl_msg);
					_sendResponse(rpl_msg);
				}
			}
		}
		_sendResponse(msg);
	}

	/*
	 * PART <channel> [<message>]
	 * The client quits the channel
	 */
	void	Server::_partClient(Message& msg,
								const std::string& chan_name, const std::string& comment
								)
	{
		t_channels::iterator	channel = getChannel(chan_name);

		if (channel == this->_channels.end())
			err_nosuchchannel(msg, chan_name, true);
		else if (!_userOnChannel(msg.getSender(), *channel))
			err_notonchannel(msg, chan_name, true);
		else
		{
			_removeUserFromChannel(msg.getSender(), channel, comment);
			return ;
		}
		_sendResponse(msg);
	}

	/*
	 * PART <channels> [<message>]
	 * The client quits the channel(s)
	 */
	void	Server::_execPartCmd(Message& msg)
	{
		if (msg.getParams().empty())
		{
			err_needmoreparams(msg, true);
			_sendResponse(msg);
		}
		else
		{
			std::string			comment = "";
			Parser::t_params	chan_names = _splitListOfParams(msg.getParams().at(0));

			if (msg.getParams().size() == 2)
				comment = msg.getParams().at(1);

			std::size_t	len = chan_names.size();

			for (std::size_t i = 0; i < len; ++i)
				_partClient(msg, chan_names[i], comment);
		}
	}

	/*
	 * PASS <password>
	 * set a connection password
	 */
	void	Server::_execPassCmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg, true);
		else if (msg.getSender().isRegistered())
			err_alreadyregistered(msg, true);
		else
			msg.getSender().setPassword(msg.getParams().at(0));
			// TODO
		_sendResponse(msg);
	}

	/*
	 * PING <server1>
	 */
	void	Server::_execPingCmd(Message& msg)
	{
		std::string	origin;

		if (msg.getParams().empty())
			err_noorigin(msg, true);
		else
		{
			origin = msg.getParams().front();

			msg.setRecipient(msg.getSender());
			msg.setResponse(build_prefix(getHostname()));
			msg.appendResponse(" PONG ");
			msg.appendResponse(getHostname());
			msg.appendResponse(" :");
			msg.appendResponse(origin);
			msg.appendResponse(CRLF);
		}
		_sendResponse(msg);
	}

	/*
	 * PONG <server1>
	 */
	void	Server::_execPongCmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_noorigin(msg, true);
		else
		{
			msg.setRecipient(msg.getSender());
			msg.clearResponse();
		}
		_sendResponse(msg);
	}

	/*
	 * PRIVMSG <msgtarget> :<message>
	 * Send messages to a user or a channel
	 */
	void	Server::_execPrivmsgCmd(Message& msg)
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
		_sendResponse(msg);
	}

	/*
	 * QUIT [<message>]
	 * A client session is terminated with a quit message
	 */
	void	Server::_execQuitCmd(Message& msg)
	{
		Message	quit_msg(msg.getSender(), getHostname());

		// The server acknowledges by sending an ERROR message to the client
		quit_msg.setResponse("ERROR :Quit: leaving");
		quit_msg.appendSeparator();

		msg.setRecipients(msg.getSender().getAllContacts());
		msg.setResponse(build_prefix(build_full_client_id(msg.getSender())));
		msg.appendResponse(" QUIT :Quit: ");
		if (!msg.getParams().empty())
			msg.appendResponse(msg.getParams().at(0));
		else
			msg.appendResponse("leaving");
		msg.appendSeparator();
		_sendResponse(msg);
		_sendResponse(quit_msg);

		msg.getSender().quitAllChannels();
		_disconnectClient(msg.getSender());
	}

	/*
	 * TOPIC <channel> [ <topic> ]
	 * Changes or displays the topic of a channel
	 */
	void	Server::_execTopicCmd(Message& msg)
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
		_sendResponse(msg);
	}

	/*
	 * USER <user> <mode> <unused> <realname>
	 */
	void	Server::_execUserCmd(Message& msg)
	{
		Client&						client = msg.getSender();
		std::vector<std::string>	params = msg.getParams(); //copy list of parameters
	
		if (msg.getParams().size() < 4)
			err_needmoreparams(msg, true);
		else
		{
			client.setUsername(params.at(0));
			client.setRealName(params.at(1));
			client.setHostname(params.at(2));
			msg.clearResponse();
			msg.setRecipient(client);
		}
		_sendResponse(msg);
	}

	/*
	 * WHO [ "o" ]
	 */
	void	Server::_execWhoCmd(Message& msg)
	{
		bool		oper_only = false;
		std::string	to_match = "0";     //match all by default
		size_t		count = 0;          // limit of 25

		if (msg.getParams().empty() == false)
			to_match = msg.getParams().front();
		else if (msg.getParams().size() == 2 && msg.getParams().back() == "o")
			oper_only = true;
		else if (msg.getParams().size() > 2)
		{
			err_syntaxerror(msg, msg.getCommand());
			_sendResponse(msg);
			return ;
		}

		for (t_clients::iterator it = this->_clients.begin();
			 it != this->_clients.end();
			 ++it)
		{
			if (oper_only && !it->isOper())
				continue ;
			if (match_nick(to_match, it->getNick()))
			{
				_log(LOG_LEVEL_DEBUG, "WHO matched " + it->getNick());
				rpl_whoreply(msg, msg.getSender().getNick(), *it);
			}
			++count;
			if (count == 25)
			{
				//:public-irc.w3.org NOTICE mynick :WHO list limit (25) reached!
				msg.setRecipient(msg.getSender());
				msg.setResponse(build_prefix(getHostname()));
				msg.appendResponse(" NOTICE ");
				msg.appendResponse(msg.getSender().getNick());
				msg.appendResponse(" :WHO list limit (25) reached!");
				msg.appendResponse(CRLF);
			}
		}
		//END OF WHO COMMAND
		rpl_endofwho(msg, msg.getSender().getNick(), to_match);
		_sendResponse(msg);
	}

	void	Server::_addWhoisToMsg(Message& msg, const Client& client)
	{
		// :mynick 311 mynick nickname user hostname * :realname
		rpl_whoisuser(msg, client, false);
		rpl_whoisserver(msg, this->_description, false);

		if (client.isOper())
			rpl_whoisoperator(msg, client, false);
		_sendResponse(msg);
	}

	/*
	 * WHOIS [target]
	 * https://datatracker.ietf.org/doc/html/rfc1459#section-4.5.2
	*/
	void	Server::_execWhoisCmd(Message& msg)
	{
		Parser::t_params::const_iterator	paramsIt;
		std::string							to_match = "0"; //match all by default
		bool								matchAll = false;

		msg.setResponse("");
		matchAll = msg.getParams().empty() || msg.getParams().front() == "0";

		for (t_clients::iterator it = this->_clients.begin();
			 it != this->_clients.end();
			 ++it)
		{
			if (matchAll)
			{
				this->_addWhoisToMsg(msg, *it);
			}
			else
			{
				paramsIt = msg.getParams().begin();
				//iterate over params
				for (; paramsIt != msg.getParams().end(); ++paramsIt)
				{
					//_logger.log(0, "WHOIS: " + *paramsIt  + ":" + it->getNick());
					if (match_nick(*paramsIt, it->getNick()))
					{
						// _logger.log(0, "WHOIS matched " + it->getNick());
						this->_addWhoisToMsg(msg, *it);
						break ;
					}
				}
			}
		}
		rpl_endofwhois(msg, msg.getSender().getNick(), to_match);
		_sendResponse(msg);
	}
}
