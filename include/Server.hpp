/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:37:43 by root              #+#    #+#             */
/*   Updated: 2021/11/26 16:35:39 by mboivin          ###   ########.fr       */
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

namespace ft_irc
{
	class CLIParser;
	class Config;
	class Message;
	class Channel;
	class Client;
	class Parser;
	class Logger;

	class Server
	{
	public:
		/* Aliases */
		typedef void (Server::*cmd_fun)(Message& msg);
		typedef std::map<std::string, cmd_fun>	t_cmds;
		typedef std::list<Client>				t_clients;
		typedef std::list<Channel>				t_channels;

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
		t_clients::iterator		getClient(const std::string& nick);
		t_channels::iterator	getChannel(const std::string& chan_name);

		/* Main loop */
		int						run();

		/* Commands */
		void					exec_invite_cmd(Message& msg);
		void					exec_join_cmd(Message& msg);
		void					_kickClient(Message& msg,
											const std::string& chan_name, const std::string& nick,
											const std::string& comment = ""
											);
		void					exec_kick_cmd(Message& msg);
		void					exec_kill_cmd(Message& msg);
		//void					exec_list_cmd(Message& msg);
		void					exec_mode_cmd(Message& msg);
		void					exec_names_cmd(Message& msg);
		void					exec_nick_cmd(Message& msg);
		void					exec_notice_cmd(Message& msg);
		void					exec_oper_cmd(Message& msg);
		void					_partClient(Message& msg, const std::string& chan_name,
											const std::string& comment = ""
											);
		void					exec_part_cmd(Message& msg);
		void					exec_pass_cmd(Message& msg);
		void					exec_ping_cmd(Message& msg);
		void					exec_pong_cmd(Message& msg);
		void					exec_privmsg_cmd(Message& msg);
		void					exec_quit_cmd(Message& msg);
		void					exec_topic_cmd(Message& msg);
		void					exec_user_cmd(Message& msg);
		void					exec_who_cmd(Message& msg);
		void					exec_whois_cmd(Message& msg);

	private:
		struct sockaddr_in		_address; /* Structure describing an Internet socket address. */
		int						_sockfd; /* Socket descriptor. */
		int						_backlog_max;
		std::string				_creation_date;
		std::string				_version;
		std::string				_description;
		Config					_config; /* Holds all config */
		Parser					_parser;
		t_cmds					_commands;
		t_clients				_clients;
		t_channels				_channels;
		Logger					_logger;

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
		int						_ping_client(Client& client);

		/* Parsing */
		bool					_parse(Message& msg, const std::string& cmd);
		Parser::t_params		_splitListOfParams(const std::string& params);

		/* Commands execution */
		void					_init_commands_map();
		int						_executeCommand(Message& msg);
		bool					_processClientCommand(Client& client);

		/* Command response */
		void					_setResponseRecipients(Message& msg);
		void					_sendResponse(Message& msg);
		void					_make_welcome_msg(Client& client);

		/* Channel operations */
		Channel&				_addChannel(const std::string& name, Client& creator);
		void					_removeChannel(t_channels::iterator channel);
		bool					_userOnChannel(Client& client, Channel& channel);
		bool					_userOnChannel(Client& client, const std::string& chan_name);
		void					_addUserToChannel(Client& client, Channel& channel);
		void					_removeUserFromChannel(Client& client, Channel& channel,
													   const std::string& comment = ""
													   );
		void					_removeUserFromAllChannels(Client& client,
														   const std::string& comment = ""
														   );
		int						_setUserMode(Client& client, const std::string& mode, Message& msg);

		/* Oper operations */
		bool					_canGiveOperPriv(const std::string& name, const std::string& password);

		/* Command helpers */
		void					_addWhoisToMsg(Message& msg, const Client& client);
	};
}

#endif
