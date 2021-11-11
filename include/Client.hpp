/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:55:22 by root              #+#    #+#             */
/*   Updated: 2021/11/11 20:30:42 by mboivin          ###   ########.fr       */
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

namespace ft_irc
{
	class Channel;

	class Client
	{
	public:
		/* Aliases */
		typedef std::list<Channel*>	t_channels;
		typedef std::list<Client*>	t_clients;

		/* Default constructor */
							Client(std::string nick="",
							std::string realname="",
							std::string username="",
							std::string password="",
							std::string hostname=""
							);

							Client(struct sockaddr_in address,
								   std::string nick="",
								   std::string realname="",
								   std::string username="",
								   std::string password="",
								   std::string hostname=""
								   );

		/* Copy constructor */
							Client(const Client& other);

		/* Copy assignment operator */
		Client&				operator=(const Client& other);

		/* Destructor */
							~Client();

		/* Getters */
		std::string			getNick() const;
		std::string			getRealName() const;
		std::string			getUsername() const;
		std::string			getPassword() const;
		std::string			getHostname() const;
		std::string			getIpAddressStr() const;
		struct sockaddr_in&	getAddress();
		socklen_t&			getAddressSize();
		int					getSocketFd() const;
		const t_channels&	getJoinedChannels() const;
		struct timeval&		getLastEventTime();
		t_clients			getAllContacts();
		/* Setters */
		void				setNick(const std::string& nick);
		void				setRealName(const std::string& realname);
		void				setUsername(const std::string& username);
		void				setPassword(const std::string& password);
		void				setHostname(const std::string& hostname);
		void				setSocketFd(int socket_fd);
		void				setJoinedChannels(const t_channels& joined_channels);
		void				setConnected(bool connected);
		void				setAlive(bool alive);
		void				setRegistered(bool registered);
		void				setPinged(bool pinged);

		/* Helpers */
		bool				isConnected() const;
		bool				isAlive() const;
		bool				isRegistered() const;
		bool				isTimeouted() const;
		bool				isOper() const;
		bool				isChanOp(Channel& channel);
		bool				isPinged() const;
		bool				isInvisible() const; // not implemented yet

		/* Connection handling */
		int					awaitConnection(int socket_fd);
		bool				hasNewEvents();
		void				updateLastEventTime();	/* resets timeout and pinged */

		/* Buffer operations */
		bool				hasUnprocessedCommands();
		std::string			popUnprocessedCommand();
		int					updateInBuffer();
		int					updateOutBuffer();
		/* Adds response to the output buffer */
		void				sendCommand(std::string cmd);

		/* Channel operations */
		void				joinChannel(Channel& channel);
		void				partChannel(Channel& channel);
		void				partAllChannels();

		/* Mode operations */
		int					addMode(char mode_char);
		int					removeMode(char mode_char);

		friend bool			operator==(const Client& lhs, const Client& rhs);

		/* debug */
		void				displayJoinedChannels();

	private:
		std::string			_nick;
		std::string			_realname;
		std::string			_hostname;
		std::string			_username;
		std::string			_mode;
		std::string			_password;
		struct sockaddr_in	_address;			/* IPv4 address	*/	
		socklen_t			_address_size;		/* IPv4 address size */
		std::string			_address_str;		/* IPv4 address as string */
		struct timeval		_timeout;			/* timeout for select() */
		int					_socket_fd;			/* socket file descriptor */
		bool				_connected;			/* is the client connected to the server? */
		std::string			_in_buffer;			/* buffer for incoming data */
		std::string			_out_buffer;		/* buffer for outgoing data */
		const size_t		_max_cmd_length;	/* max length of a command */
		t_channels			_joined_channels;
		bool				_alive;
		struct timeval		_keep_alive;		/* keep_alive lenght */
		struct timeval		_last_event_time;	/* time since last network event */
		bool				_registered;		/* is the client registered? */	
		bool				_pinged;			/* has the client been pinged? */
	};
}

#endif
