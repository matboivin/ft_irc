/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:37:43 by root              #+#    #+#             */
/*   Updated: 2021/12/19 19:45:48 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <list>
# include <map>
# include <string>
# include <ctime>
# include "ft_irc.hpp"
# include "Logger.hpp"
# define USER_LEN 32
# define CHAN_NB_MAX 10
# define USERS_IN_CHAN_MAX 20
# define USERS_MAX 200

namespace ft_irc
{
	class CLIParser;
	class Config;
	class Message;
	class Channel;
	class Client;
	class Parser;
	class Logger;

	/*
	 * IRC Server
	 */
	class Server
	{
	public:
		/* Aliases */
		typedef void (Server::*cmd_fun)(Message& msg);
		typedef std::map<std::string, cmd_fun>	t_cmds;
		typedef std::list<Client>				t_clients;
		typedef std::list<Channel>				t_channels;
		typedef std::vector<struct pollfd>		t_pollfds;

		/* Constructor */
								Server(CLIParser& CLI_parser, int backlog_max=5);

		/* Copy constructor */
								Server(const Server& other);

		/* Copy assignment operator */
		Server&					operator=(const Server& other);

		/* Destructor */
								~Server();

		/* Getters */
		std::string				getHostname() const;
		std::string				getBindAddress() const;
		std::string				getPort() const;
		std::string				getPassword() const;
		std::string				getCreationDate() const;
		std::string				getVersion() const;
		std::string				getDescription() const;
		t_cmds					getCommands() const;
		const t_clients&		getClients() const;
		t_clients::iterator		getClient(const std::string& nick);
		t_channels::iterator	getChannel(const std::string& chan_name);
		bool					isAlive() const;

		/* Main loop */
		int						run();

	private:
		/* Attributes */
		struct sockaddr_in			_address; /* Structure describing an Internet socket address. */
		int							_sockfd; /* Socket descriptor. */
		int							_backlog_max;
		std::string					_creation_date;
		std::string					_version;
		std::string					_description;
		Config						_config; /* Holds the config */
		Parser						_parser;
		t_cmds						_commands;
		t_clients					_clients;
		t_channels					_channels;
		Logger						_logger;
		bool						_alive;
		t_pollfds					_poll_fds;
		size_t						_max_clients;

		/* Cleaning */
		void					_shutdown();

		/* Logging */
		void					_log(int level, const std::string& message) const;

		/* Connections handling */
		bool					_createSocket();
		int						_sockGetLine(int sockfd, std::string& line);
		int						_sockGetLine(int sockfd, std::string& line, std::size_t max_bytes);
		bool					_awaitNewConnection();
		bool					_hasPendingConnections();
		bool					_processClients();
		int						_disconnectClient(Client& client);
		void					_makeWelcomeMsg(Client& client);
		int						_registerClient(Message& msg, Client& client);
		int						_pingClient(Client& client);
		bool					_pollServ();

		/* Parsing */
		bool					_parse(Message& msg, const std::string& cmd);
		Parser::t_params		_splitListOfParams(const std::string& params);

		/* Commands execution */
		void					_init_commands_map();
		int						_executeCommand(Message& msg, Client &client);
		bool					_processClientCommand(Client& client);

		/* Command response */
		void					_sendResponse(Message& msg);

		/* Channel operations */
		Channel&				_addChannel(const std::string& name, Client& creator);
		void					_removeChannel(t_channels::iterator channel);
		bool					_userOnChannel(Client& client, Channel& channel);
		bool					_userOnChannel(Client& client, const std::string& chan_name);
		void					_addUserToChannel(Client& client, Channel& channel);
		void					_removeUserFromChannel(Client& client, t_channels::iterator channel,
													   const std::string& comment = "");
		void					_removeUserFromAllChannels(Client& client,
														   const std::string& comment = "");

		/* Oper operations */
		bool					_canGiveOperPriv(const std::string& name, const std::string& password);

		/* Commands */
		void					_execDieCmd(Message& msg);
		void					_execInviteCmd(Message& msg);
		void					_execJoinCmd(Message& msg);
		void					_execKickCmd(Message& msg);
		void					_execKillCmd(Message& msg);
		void					_execListCmd(Message& msg);
		void					_execModeCmd(Message& msg);
		void					_execMotdCmd(Message& msg);
		void					_execNamesCmd(Message& msg);
		void					_execNickCmd(Message& msg);
		void					_execNoticeCmd(Message& msg);
		void					_execOperCmd(Message& msg);
		void					_execPartCmd(Message& msg);
		void					_execPassCmd(Message& msg);
		void					_execPingCmd(Message& msg);
		void					_execPongCmd(Message& msg);
		void					_execPrivmsgCmd(Message& msg);
		void					_execQuitCmd(Message& msg);
		void					_execQuitTimeoutCmd(Client& client);
		void					_execTopicCmd(Message& msg);
		void					_execUserCmd(Message& msg);
		void					_execWhoCmd(Message& msg);
		void					_execWhoisCmd(Message& msg);

		/* Command helpers */
		void					_addWhoisToMsg(Message& msg, const Client& client);
		void					_setUserMode(Message& msg, Client& target);
		void					_setChannelMode(Message& msg, Client& client, Channel& channel);
		void					_setUserModeInChan(Message& msg, Client& client, Channel& channel);
		void					_kickClient(Message& msg,
											const std::string& chan_name, const std::string& nick,
											const std::string& comment);
		void					_killClient(Message& msg, Client& target,
											const std::string& reason = "<no reason supplied>");
		void					_partClient(Message& msg,
											const std::string& chan_name,
											const std::string& comment = "");
		void					_addClientFd(Client &client);
		void					_removeClientFd(Client &client);
		int						updateClientInBuffer(Client &client, struct pollfd &client_pollfd);
	};
} // !namespace ft_irc

#endif // !SERVER_HPP
