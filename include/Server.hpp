/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:37:43 by root              #+#    #+#             */
/*   Updated: 2021/10/09 12:33:16 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <list>
# include <map>
# include <string>

namespace ft_irc
{
	class Message;
	class Channel;
	class Client;
	class Parser;

	class Server
	{
	public:
		// aliases
		typedef void (Server::*cmd_fun)(Message& msg); // pointer to command function
		typedef std::map<std::string, cmd_fun>	cmds_map; // commands functions

	private:
		// attributes
		std::string			_hostname;
		std::string			_bind_address;
		std::string			_port;
		std::string			_password;
		//Structure describing an Internet socket address.
		struct sockaddr_in	_address;
		//Socket descriptor.
		int					_sockfd;
		int					_backlog_max;
		Parser				_parser;
		cmds_map			_commands;
		std::list<Client>	_clients;
		std::list<Channel>	_channels;

	public:
		// constructor
					Server(std::string bind_address="0.0.0.0",
						   std::string port="6697",
						   std::string password="",
						   std::string hostname="irc.42.fr",
						   int backlog_max=5);
		// copy constructor
					Server(const Server& other);
		// assignment operator
		Server&		operator=(const Server& other);
		// destructor
					~Server();

		// getters
		std::string	getBindAddress() const;
		std::string	getPort() const;
		std::string	getPassword() const;
		cmds_map	getCommands() const;

		std::list<Client>::iterator		getClient(const std::string& nick);
		std::list<Channel>::iterator	getChannel(const std::string& chan_name);

		// setters
		void		setBindAddress(const std::string& bind_address);
		void		setPort(const std::string& port);
		void		setPassword(const std::string& password);

		// main loop
		int			run();

		// Commands
		void		exec_pass_cmd(Message& msg);
		void		exec_nick_cmd(Message& msg);
		void		exec_quit_cmd(Message& msg);
		void		exec_notice_cmd(Message& msg);
		void		exec_privmsg_cmd(Message& msg);
		void		exec_join_cmd(Message& msg);
		void		exec_part_cmd(Message& msg);


	private:
		//Function to create a socket.
		//create a new listening tcp s	ocket and bind it to the given address and port
		//https://www.geeksforgeeks.org/socket-programming-cc/
		bool		_createSocket();
		int			_sockGetLine(int sockfd, std::string& line);
		int			_sockGetLine(int sockfd, std::string& line, std::size_t max_bytes);
		//awaitConnection
		bool		_awaitNewConnection();
		bool		_hasPendingConnections();
		bool		_processClients();
		int			_disconnectClient(Client& client);

		// parsing
		Message		_parse(Client& sender, const std::string& cmd);

		// commands execution
		void		_init_commands_map();
		int			_executeCommand(Message& msg);
		// command response
		void		_configResponse(Message& msg);
		void		_sendResponse(Message& msg);

		// Channel operations
		Channel&	_addChannel(const std::string& name);
		void		_removeChannel(std::list<Channel>::iterator channel);
		bool		_userOnChannel(Client& client, Channel& channel);
		bool		_userOnChannel(Client& client, const std::string& chan_name);
		void		_addUserToChannel(Client& client, Channel& channel);
		void		_removeUserFromChannel(Client& client, Channel& channel);
		void		_removeUserFromAllChannels(Client& client);

		// debug
		int			_sendList(Client& client);
		int			_sendError(Client& client, const std::string& error);
	};
}

#endif // !SERVER_HPP
