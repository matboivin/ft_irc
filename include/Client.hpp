/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:55:22 by root              #+#    #+#             */
/*   Updated: 2021/10/21 16:19:20 by smaccary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

# include <fcntl.h>
# include <iostream>
# include <list>
# include <poll.h>
# include <string>
# include <stdexcept>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_COMMAND_SIZE 512
# define CRLF "\n"

namespace ft_irc
{
	class Channel;

	class Client
	{
		public:
			Client(struct sockaddr_in address=(struct sockaddr_in){0,0,{0},{0}},
				   std::string nick="",
				   std::string realname="",
				   std::string username="",
				   std::string password="");

			Client(const Client& other);
			Client&	operator=(const Client& other);
			~Client();

			/* getters and setters */
			std::string					getNick() const;
			std::string					getRealName() const;
			std::string					getUsername() const;
			std::string					getPassword() const;
			std::string					getIpAddressStr() const;
			struct sockaddr_in&			getAddress();
			socklen_t&					getAddressSize();
			int							getSocketFd() const;
			const std::list<Channel*>&	getJoinedChannels() const;

			void						setNick(const std::string& nick);
			void						setRealName(const std::string& realname);
			void						setUsername(const std::string& username);
			void						setPassword(const std::string& password);
			void						setSocketFd(int socket_fd);
			void						setJoinedChannels(const std::list<Channel*>& joined_channels);
			void						joinChannel(Channel& channel);
			void						partChannel(Channel& channel);
			void						setAlive(bool alive);
			void				setConnected(bool connected);

			/* helpers */
			bool						isRegistered() const;
			bool						isConnected() const;
			int							awaitConnection(int socket_fd);
			bool						hasNewEvents();
			bool						hasUnprocessedCommands();
			std::string					popUnprocessedCommand();
			int							updateInBuffer();
			int							updateOutBuffer();
			bool						isAlive() const;

			/* add response to the output buffer */
			void						sendCommand(std::string cmd);
			
			/* debug */
			void						displayJoinedChannels();

			friend bool					operator==(const Client& lhs, const Client& rhs);

		private:
			std::string					_nick;
			std::string					_realname;
			std::string					_username;
			std::string					_mode;
			std::string					_password;
			struct sockaddr_in			_address;			/* IPv4 address	*/	
			socklen_t					_address_size;		/* IPv4 address size */
			std::string					_address_str;		/* IPv4 address as string */
			struct timeval				_timeout;			/* timeout for select() */
			int							_socket_fd;			/* socket file descriptor */
			bool						_connected;			/* is the client connected to the server? */
			std::string					_in_buffer;			/* buffer for incoming data */
			std::string					_out_buffer;		/* buffer for outgoing data */
			const size_t				_max_cmd_length;	/* max length of a command */
			std::list<Channel*>			_joined_channels;
			bool						_alive;
			struct timeval				_keep_alive;		/* keep_alive lenght */
	};
}

#endif
