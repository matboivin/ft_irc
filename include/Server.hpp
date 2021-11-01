/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:37:43 by root              #+#    #+#             */
/*   Updated: 2021/11/01 15:49:40 by mboivin          ###   ########.fr       */
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
			typedef void (Server::*cmd_fun)(Message& msg);
			typedef std::map<std::string, cmd_fun>	t_cmds;

			Server(CLIParser& CLI_parser, int backlog_max=5);
			Server(const Server& other);
			Server&		operator=(const Server& other);
			~Server();

			/* getters and setters */
			std::string						getHostname() const;
			std::string						getBindAddress() const;
			std::string						getPort() const;
			std::string						getPassword() const;
			t_cmds							getCommands() const;

			std::list<Client>::iterator		getClient(const std::string& nick);
			std::list<Channel>::iterator	getChannel(const std::string& chan_name);

			// void							setBindAddress(const std::string& bind_address);
			// void							setPort(const std::string& port);
			// void							setPassword(const std::string& password);

			/* main loop */
			int								run();

			/* Commands */
			void							exec_pass_cmd(Message& msg);
			void							exec_nick_cmd(Message& msg);
			void							exec_quit_cmd(Message& msg);
			void							exec_oper_cmd(Message& msg);
			void							exec_notice_cmd(Message& msg);
			void							exec_privmsg_cmd(Message& msg);
			void							exec_join_cmd(Message& msg);
			void							exec_part_cmd(Message& msg);

		private:
			/* Structure describing an Internet socket address. */
			struct sockaddr_in				_address;

			/* Socket descriptor. */
			int								_sockfd;
			int								_backlog_max;
			Config							_config; // holds all config
			Parser							_parser;
			t_cmds							_commands;
			std::list<Client>				_clients;
			std::list<Channel>				_channels;
			std::string						_creation_date;
			std::string						_version;
			

			bool							_createSocket();
			int								_sockGetLine(int sockfd, std::string& line);
			int								_sockGetLine(int sockfd, std::string& line, std::size_t max_bytes);
			bool							_awaitNewConnection();
			bool							_hasPendingConnections();
			bool							_processClients();
			int								_disconnectClient(Client& client);

			bool							_parse(Message& msg, const std::string& cmd);

			/* commands execution */
			void							_init_commands_map();
			int								_executeCommand(Message& msg);
			bool							_processClientCommand(Client& client);

			/* command response */
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

			/* cleaning */
			void							_shutdown();
		};
}

#endif
