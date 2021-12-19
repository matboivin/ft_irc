/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:55:22 by root              #+#    #+#             */
/*   Updated: 2021/12/19 21:32:42 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

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

typedef struct pollfd pollfd_t;
namespace ft_irc
{
	class Channel;

	/*
	 * IRC Client
	 * Attribute of the Server
	 */
	class Client
	{
	public:
		/* Aliases */
		typedef std::list<Channel*>	t_channels;
		typedef std::list<Client*>	t_clients;

		std::string					_in_buffer;		/* buffer for incoming data */
		std::string					_out_buffer;	/* buffer for outgoing data */
		size_t						pollfd_index;	/* index of the pollfd in the pollfd array */

		/* Default constructor */
							Client(std::string nick="",
								   std::string realname="",
								   std::string username="",
								   std::string hostname=""
								   );

							Client(struct sockaddr_in address,
								   std::string nick="",
								   std::string realname="",
								   std::string username="",
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
		std::string			getHostname() const;
		std::string			getMode() const;
		bool				isAlive() const;
		bool				enteredPass() const;
		bool				enteredNick() const;
		bool				enteredUser() const;
		bool				isRegistered() const;
		bool				isPinged() const;
		std::string			getIpAddressStr() const;
		struct sockaddr_in&	getAddress();
		socklen_t&			getAddressSize();
		const t_channels&	getJoinedChannels() const;
		struct timeval&		getLastEventTime();
		t_clients			getAllContacts();
		const std::string&	getKickReason() const;

		/* Setters */
		void				setNick(const std::string& nick);
		void				setRealName(const std::string& realname);
		void				setUsername(const std::string& username);
		void				setHostname(const std::string& hostname);
		void				setJoinedChannels(const t_channels& joined_channels);
		void				setAlive(bool alive);
		void				setEnteredPass(bool enteredPass);
		void				setEnteredNick(bool enteredNick);
		void				setEnteredUser(bool enteredUser);
		void				setRegistered(bool registered);
		void				setPinged(bool pinged);
		void				setAddressStr(const std::string& address);

		/* Helpers */
		bool				isTimeouted() const;
		bool				isOper() const;
		bool				isChanOp(Channel& channel);
		bool				isInvisible() const;
		void				kick(const std::string& reason = "");

		/* Connection handling */
		void				updateLastEventTime();	/* resets timeout and pinged */

		/* Buffer operations */
		bool				hasUnprocessedCommands();
		std::string			popUnprocessedCommand();
		int					addToInBuffer(const std::string& command);
		int					addToOutBuffer(const std::string& command);
		/*811 Adds response to the output buffer */
		void				sendCommand(std::string cmd);

		/* Channel operations */
		void				joinChannel(Channel& channel);
		void				partChannel(Channel& channel);
		void				quitAllChannels();

		/* Mode operations */
		int					addMode(char mode_char);
		int					removeMode(char mode_char);

		/* Comparison operators */
		friend bool			operator==(const Client& lhs, const Client& rhs);

	private:
		/* Attributes */
		std::string			_nick;
		std::string			_realname;
		std::string			_username;
		std::string			_hostname;
		std::string			_mode;
		const size_t		_max_cmd_length;	/* max length of a command */
		bool				_alive;
		bool				_enteredPass;
		bool				_enteredNick;
		bool				_enteredUser;
		bool				_registered;		/* provided PASS,NICK,USER */
		bool				_pinged;			/* has the client been pinged? */
		struct sockaddr_in	_address;			/* IPv4 address */
		socklen_t			_address_size;		/* IPv4 address size */
		std::string			_address_str;		/* IPv4 address as string */
		struct timeval		_timeout;			/* timeout for poll() */
		struct timeval		_keep_alive;		/* keep_alive length */
		struct timeval		_last_event_time;	/* time since last network event */
		t_channels			_joined_channels;	/* channels the client is in */
		std::string			_kick_reason;		/* kick message */
	};
}

#endif // !CLIENT_HPP
