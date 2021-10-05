/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:55:22 by root              #+#    #+#             */
/*   Updated: 2021/10/05 12:54:34 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_CLIENT_HPP
# define IRC_CLIENT_HPP

# define MAX_COMMAND_SIZE 512
# define CRLF "\n"
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include <string>
# include <sys/time.h>
# include <stdexcept>
# include <poll.h>
# include <iostream>

namespace ft_irc
{
	class Client
	{
	private:
		// attributes
		std::string			nick;
		std::string			realname;
		std::string			username;
		std::string			mode;
		std::string			joined_channels;
		std::string			password;
		struct sockaddr_in	address;			//IPv4 address		
		socklen_t 			address_size;		//IPv4 address size
		std::string			address_str;		//IPv4 address as string
		struct timeval		timeout;			//timeout for select()
		int					socket_fd;			//socket file descriptor
		bool				connected;		//is the client connected to the server?
		std::string			in_buffer;			//buffer for incoming data
		std::string			out_buffer;		//buffer for outgoing data
		const size_t		max_cmd_length;	//max length of a command

	public:
		// constructor
				Client(struct sockaddr_in address=(struct sockaddr_in){0,0,{0},{0}},
					   std::string nick="",
					   std::string realname="",
					   std::string username="",
					   std::string password="");

		// copy constructor
				Client(const Client& other);
		// assignment operator
		Client&	operator=(const Client& other);
		// destructor
				~Client();

		// getters
		std::string			getNick() const;
		std::string			getRealName() const;
		std::string			getUsername() const;
		std::string			getJoinedChannels() const;
		std::string			getPassword() const;
		std::string			getIpAddressStr() const;
		struct sockaddr_in&	getAddress();
		socklen_t&			getAddressSize();
		int					getSocketFd() const;

		// setters
		void				setNick(const std::string& nick);
		void				setRealName(const std::string& realname);
		void				setUsername(const std::string& username);
		void				setJoinedChannels(const std::string& joined_channels);
		void				setPassword(const std::string& password);
		void				setSocketFd(int socket_fd);

		// helpers
		bool				isRegistered() const;
		bool				isConnected() const;
		int					awaitConnection(int socket_fd);
		bool				hasNewEvents();
		bool				hasUnprocessedCommands();
		std::string			popUnprocessedCommand();
		int					updateInBuffer();
		int					updateOutBuffer();

		//add response to the output buffer
		void				sendCommand(std::string cmd);

		//operator==
		friend bool			operator==(const Client& lhs, const Client& rhs);
	};
} // namespace ft_irc

#endif // !IRC_CLIENT
