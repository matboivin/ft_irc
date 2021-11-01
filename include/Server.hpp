/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:37:43 by root              #+#    #+#             */
/*   Updated: 2021/11/01 19:03:42 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <list>
# include <map>
# include <string>
# include <ctime>

namespace ft_irc
{
	class CLIParser;
	class Config;
	class Message;
	class Channel;
	class Client;
	class Parser;

	class Server
	{
		public:
			/* Aliases */
			typedef void (Server::*cmd_fun)(Message& msg);
			typedef std::map<std::string, cmd_fun>	t_cmds;

			Server(CLIParser& CLI_parser, int backlog_max=5);
			Server(const Server& other);
			Server&		operator=(const Server& other);
			~Server();

			/* Getters and setters */
			std::string						getHostname() const;
			std::string						getBindAddress() const;
			std::string						getPort() const;
			std::string						getPassword() const;
			std::string						getCreationDate() const;
			std::string						getVersion() const;
			std::string						getDescription() const;
			t_cmds							getCommands() const;
			std::list<Client>::iterator		getClient(const std::string& nick);
			std::list<Channel>::iterator	getChannel(const std::string& chan_name);

			/* Main loop */
			int								run();

			/* Commands */
			void							exec_join_cmd(Message& msg);
			void							exec_nick_cmd(Message& msg);
			void							exec_notice_cmd(Message& msg);
			void							exec_oper_cmd(Message& msg);
			void							exec_part_cmd(Message& msg);
			void							exec_pass_cmd(Message& msg);
			void							exec_ping_cmd(Message& msg);
			void							exec_pong_cmd(Message& msg);
			void							exec_privmsg_cmd(Message& msg);
			void							exec_quit_cmd(Message& msg);
			void							exec_topic_cmd(Message& msg);
			void							exec_user_cmd(Message& msg);
			void							exec_who_cmd(Message& msg);
			void							exec_whois_cmd(Message& msg);
			void							exec_test_cmd(Message& msg); // debug

		private:
			struct sockaddr_in				_address; /* Structure describing an Internet socket address. */
			int								_sockfd; /* Socket descriptor. */
			int								_backlog_max;
			std::string						_creation_date;
			std::string						_version;
			std::string						_description;
			Config							_config; /* Holds all config */
			Parser							_parser;
			t_cmds							_commands;
			std::list<Client>				_clients;
			std::list<Channel>				_channels;

			/* Cleaning */
			void							_shutdown();

			/* Connections handling */
			bool							_createSocket();
			int								_sockGetLine(int sockfd, std::string& line);
			int								_sockGetLine(int sockfd, std::string& line, std::size_t max_bytes);
			bool							_awaitNewConnection();
			bool							_hasPendingConnections();
			bool							_processClients();
			int								_disconnectClient(Client& client);
			int								_ping_client(Client& client);

			/* Parsing */
			bool							_parse(Message& msg, const std::string& cmd);

			/* Commands execution */
			void							_init_commands_map();
			int								_executeCommand(Message& msg);
			bool							_processClientCommand(Client& client);

			/* Command response */
			void							_setResponseRecipients(Message& msg);
			void							_sendResponse(Message& msg);
			void							_make_welcome_msg(Message& msg);

			/* Channel operations */
			Channel&						_addChannel(const std::string& name);
			void							_removeChannel(std::list<Channel>::iterator channel);
			bool							_userOnChannel(Client& client, Channel& channel);
			bool							_userOnChannel(Client& client, const std::string& chan_name);
			void							_addUserToChannel(Client& client, Channel& channel);
			void							_removeUserFromChannel(Client& client, Channel& channel);
			void							_removeUserFromAllChannels(Client& client);
			void							_removeUserFromAllChannels(Client& client, Message& msg);

			/* Oper operations */
			bool							_giveOperPriv(const std::string& name, const std::string& password);

			/* debug */
			int								_sendList(Client& client);
			int								_sendError(Client& client, const std::string& error);
		};
}

#endif
