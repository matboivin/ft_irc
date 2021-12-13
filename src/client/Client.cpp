/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:56:54 by root              #+#    #+#             */
/*   Updated: 2021/12/13 14:38:00 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <list>
#include <string>
#include "ft_irc.hpp"

namespace ft_irc
{
	/* Constructor */
	Client::Client(struct sockaddr_in address,
				   std::string nick,
				   std::string realname,
				   std::string username,
				   std::string hostname)
	: _nick(nick),
	  _realname(realname),
	  _username(username),
	  _hostname(hostname),
	  _mode("i"),
	  _in_buffer(),
	  _out_buffer(),
	  _max_cmd_length(512),
	  _allowed(false),
	  _alive(true),
	  _registered(false),
	  _pinged(false),
	  _address(address),
	  _address_size(sizeof(address)),
	  _address_str(inet_ntoa(address.sin_addr)),
	  _timeout(),
	  _keep_alive(),
	  _last_event_time(),
	  _socket_fd(-1),
	  _joined_channels()
	  
	{
		this->_timeout.tv_sec = 0;
		this->_timeout.tv_usec = 50;
		this->_keep_alive.tv_sec = 30;
		this->_keep_alive.tv_usec = 0;

		if (gettimeofday(&this->_last_event_time, NULL))
			throw std::runtime_error("gettimeofday() failed");
	}

	/* Constructor */
	Client::Client(std::string nick,
				   std::string realname,
				   std::string username,
				   std::string hostname)
	: _nick(nick),
	  _realname(realname),
	  _username(username),
	  _hostname(hostname),
	  _mode("i"),
	  _in_buffer(),
	  _out_buffer(),
	  _max_cmd_length(512),
	  _allowed(false),
	  _alive(true),
	  _registered(false),
	  _pinged(false),
	  _address(),
	  _address_size(),
	  _address_str(),
	  _timeout(),
	  _keep_alive(),
	  _last_event_time(),
	  _socket_fd(-1),
	  _joined_channels()
	{
		this->_timeout.tv_sec = 0;
		this->_timeout.tv_usec = 50;
		this->_keep_alive.tv_sec = 30;
		this->_keep_alive.tv_usec = 0;

		if (gettimeofday(&this->_last_event_time, NULL))
			throw std::runtime_error("gettimeofday() failed");
	}

	/* Copy constructor */
	Client::Client(const Client& other)
	: _nick(other._nick),
	  _realname(other._realname),
	  _username(other._username),
	  _hostname(other._hostname),
	  _mode(other._mode),
	  _in_buffer(other._in_buffer),
	  _out_buffer(other._out_buffer),
	  _max_cmd_length(other._max_cmd_length),
	  _allowed(other._allowed),
	  _alive(other._alive),
	  _registered(other._registered),
	  _pinged(other._pinged),
	  _address(other._address),
	  _address_size(other._address_size),
	  _address_str(other._address_str),
	  _timeout(other._timeout),
	  _keep_alive(other._keep_alive),
	  _last_event_time(other._last_event_time),
	  _socket_fd(other._socket_fd),
	  _joined_channels(other._joined_channels)
	{
	}

	/* Copy assignment operator */
	Client&	Client::operator=(const Client& other)
	{
		if (this != &other)
		{
			this->_nick = other._nick;
			this->_realname = other._realname;
			this->_username = other._username;
			this->_hostname = other._hostname;
			this->_mode = other._mode;
			this->_in_buffer = other._in_buffer;
			this->_out_buffer = other._out_buffer;
			this->_allowed = other._allowed;
			this->_alive = other._alive;
			this->_registered = other._registered;
			this->_pinged = other._pinged;
			this->_address = other._address;
			this->_address_str = other._address_str;
			this->_address_size = other._address_size;
			this->_timeout = other._timeout;
			this->_keep_alive = other._keep_alive;
			this->_last_event_time = other._last_event_time;
			this->_socket_fd = other._socket_fd ;
			this->_joined_channels = other._joined_channels;
		}
		return (*this);
	}

	/* Destructor */
	Client::~Client()
	{
	}

	/* Getters ****************************************************************** */

	std::string	Client::getNick() const
	{
		if (this->_nick.empty())
			return ("*");
		return (this->_nick);
	}

	std::string	Client::getRealName() const
	{
		if (this->_realname.empty())
			return ("*");
		return (this->_realname);
	}

	std::string	Client::getUsername() const
	{
		if (this->_username.empty())
			return ("*");
		return (this->_username);
	}

	std::string	Client::getHostname() const
	{
		// return IP address if no host
		if (this->_hostname.empty())
			return (this->_address_str);
		return (this->_hostname);
	}

	std::string	Client::getMode() const
	{
		return (this->_mode);
	}

	struct sockaddr_in&	Client::getAddress()
	{
		return (this->_address);
	}

	std::string	Client::getIpAddressStr() const
	{
		return (this->_address_str);
	}

	socklen_t&	Client::getAddressSize()
	{
		return (this->_address_size);
	}

	int	Client::getSocketFd() const
	{
		return (this->_socket_fd);
	}

	const Client::t_channels&	Client::getJoinedChannels() const
	{
		return (this->_joined_channels);
	}

	struct timeval&	Client::getLastEventTime()
	{
		return (this->_last_event_time);
	}

	Client::t_clients	Client::getAllContacts()
	{
		t_clients	contacts;

		for (t_channels::iterator it = this->_joined_channels.begin();
			 it != this->_joined_channels.end();
			 ++it)
		{
			contacts.insert(
				contacts.end(),
				(*it)->getClients().begin(), (*it)->getClients().end()
				);
		}
		return (removeDuplicates(contacts, this));
	}

	/* Setters ****************************************************************** */

	void	Client::setNick(const std::string& nick)
	{
		this->_nick = nick;
	}

	void	Client::setRealName(const std::string& realname)
	{
		this->_realname = realname;
	}

	void	Client::setHostname(const std::string& hostname)
	{
		this->_hostname = hostname;
	}

	void	Client::setUsername(const std::string& username)
	{
		this->_username = username;
	}

	void	Client::setSocketFd(int socket_fd)
	{
		this->_socket_fd = socket_fd;
	}

	void	Client::setJoinedChannels(const t_channels& joined_channels)
	{
		this->_joined_channels = joined_channels;
	}

	void	Client::setAllowed(bool allowed)
	{
		this->_allowed = allowed;
	}

	void	Client::setAlive(bool alive)
	{
		this->_alive = alive;
	}

	void	Client::setRegistered(bool registered)
	{
		this->_registered = registered;
	}

	void	Client::setPinged(bool pinged)
	{
		this->_pinged = pinged;
	}

	/* Helpers ****************************************************************** */

	bool	Client::isConnected() const
	{
		return (this->_socket_fd != -1);
	}

	bool	Client::isAllowed() const
	{
		return (this->_allowed);
	}

	bool	Client::isAlive() const
	{
		return (this->_alive);
	}

	bool	Client::isRegistered() const
	{
		return (this->_registered);
	}

	bool	Client::isTimeouted() const
	{
		struct timeval	now;

		gettimeofday(&now, NULL);
		return ((now.tv_sec - this->_last_event_time.tv_sec) > this->_keep_alive.tv_sec);
	}

	bool	Client::isOper() const
	{
		return (this->_mode.find("o") != std::string::npos);
	}

	bool	Client::isChanOp(Channel& channel)
	{
		return (channel.hasChanOp(*this));
	}

	bool	Client::isPinged() const
	{
		return (this->_pinged);
	}

	bool	Client::isInvisible() const
	{
		return (this->_mode.find('i') != std::string::npos);
	}

	bool	Client::hasNick() const
	{
		return (!this->_nick.empty());
	}

	bool	Client::hasUser() const
	{
		return (!this->_username.empty() && !this->_realname.empty());
	}

	/* Connection handling ****************************************************** */

	int	Client::awaitConnection(int socket_fd)
	{
		this->_socket_fd = accept(socket_fd,
								  (struct sockaddr *)&this->_address,
								  &this->_address_size
								  );

		if (this->_socket_fd == -1)
		{
			std::cerr << "Accept failed" << std::endl;
			return (-1);
		}

		setNonblocking(this->_socket_fd);
		this->_address_str = inet_ntoa(this->_address.sin_addr);
		return (this->_socket_fd);
	}

	/* poll */
	bool	Client::hasNewEvents()
	{
		int				ret;
		struct pollfd	poll_fd;

		poll_fd.fd = this->_socket_fd;
		poll_fd.events = POLLIN;
		ret = poll(&poll_fd, 1, this->_timeout.tv_usec);

		if (ret == -1)
		{
			std::cerr << __FILE__ ":" <<__LINE__ << " : " << "FATAL: poll()" << std::endl;
			throw std::runtime_error("poll() failed");
		}
		return (ret > 0);
	}

	/* Resets timeout and pinged */
	void Client::updateLastEventTime()
	{
		if (gettimeofday(&this->_last_event_time, NULL))
			throw std::runtime_error("gettimeofday() failed");
		this->_pinged = false;
	}

	/* Buffer operations ******************************************************** */

	bool	Client::hasUnprocessedCommands()
	{
		//if \r\n is in the in_buffer, return true
		return (this->_in_buffer.find("\n") != std::string::npos);
	}

	std::string	Client::popUnprocessedCommand()
	{
		std::string				endofline = CRLF;
		std::string::size_type	found = this->_in_buffer.find(CRLF);

		if (found == std::string::npos)
		{
			// found = this->_in_buffer.find("\n");
			endofline = "\n";
		}
		std::string	cmd = this->_in_buffer.substr(0, this->_in_buffer.find(endofline) + endofline.size());

		Logger	logger(DEBUG);

		logger.log(LOG_LEVEL_DEBUG, "Received: " + cmd);
		this->_in_buffer.erase(0, this->_in_buffer.find(endofline) + endofline.size());
		return (cmd);
	}

	/* Reads 512 bytes from the socket if there is data to read */
	int	Client::updateInBuffer()
	{
		char					bytes_buffer[MAX_COMMAND_SIZE];
		int						ret;
		int						poll_ret;
		std::string::size_type	found;
		struct pollfd			poll_fd;

		poll_fd.fd = this->_socket_fd;
		poll_fd.events = POLLIN;

		if (this->getSocketFd() < 0)
			return (-1);
		poll_ret = poll(&poll_fd, 1, this->_timeout.tv_usec);
		if (poll_ret == -1)
		{
			std::cerr << __FILE__ ":" <<__LINE__ << " : " << "FATAL: poll()" << std::endl;
			throw std::runtime_error("poll() failed");
		}
		if (poll_ret == 0)
			return (0);
		// read
		ret = recv(this->_socket_fd, bytes_buffer, MAX_COMMAND_SIZE, 0);
		if (ret == -1)
		{
			std::cerr << __FILE__ ":" <<__LINE__ << " : " << "FATAL: recv()" << std::endl;
			throw std::runtime_error("recv() failed");
		}
		if (ret == 0)
			return (0);

		// append to in_buffer
		this->_in_buffer.append(bytes_buffer, ret);
		// if there's not \r\n in the first 512 bytes, insert a \r\n at offset 512
		if (this->_in_buffer.size() > this->_max_cmd_length)
		{
			std::string	endofline = CRLF;

			found = this->_in_buffer.find(CRLF);
			if (found == std::string::npos)
			{
				found = this->_in_buffer.find("\n");
				endofline = "\n";
			}
			if (found == std::string::npos || found > this->_max_cmd_length)
				this->_in_buffer.insert(this->_max_cmd_length, endofline);
		}
		this->updateLastEventTime();
		return (ret);
	}

	int	Client::updateOutBuffer()
	{
		int		ret;
		size_t	size = std::min(this->_out_buffer.size(), this->_max_cmd_length);

		if (!size)
			return (0);
		// write 512 bytes at most and removes them from the out_buffer
		ret = send(this->_socket_fd, this->_out_buffer.c_str(), size, 0);
		if (ret == -1)
		{
			std::cerr << __FILE__ ":" <<__LINE__ << " : " << "FATAL: poll()" << std::endl;
			throw std::runtime_error("send() failed");
		}
		this->_out_buffer.erase(0, ret);
		return (ret);
	}

	void	Client::sendCommand(std::string cmd)
	{
		this->_out_buffer += cmd;
	}

	/* Channel operations ******************************************************* */

	void	Client::joinChannel(Channel& channel)
	{
		channel.addClient(*this);
		this->_joined_channels.push_back(&channel);
	}

	void	Client::partChannel(Channel& channel)
	{
		channel.removeClient(*this);
		this->_joined_channels.remove(&channel);
	}

	/* The client quits all channels they joined */
	void	Client::quitAllChannels()
	{
		for (t_channels::iterator it = this->_joined_channels.begin();
			 it != this->_joined_channels.end();
			 ++it)
		{
			(*it)->removeClient(*this);
		}
		this->_joined_channels.clear();
	}

	/* Mode operations ********************************************************** */

	/* Adds the mode passed as parameter to the client mode string */
	int	Client::addMode(char mode_char)
	{
		std::string	valid_modes = "io";

		if (valid_modes.find(mode_char) != std::string::npos)
		{
			if ((mode_char == 'o') && !this->isOper()) // not enough rights, do nothing
				return (1);

			if (this->_mode.find(mode_char) == std::string::npos)
			{
				this->_mode += mode_char;
				return (ERR_SUCCESS);
			}
			return (1);
		}
		return (ERR_UNKNOWNMODE);
	}

	/* Removes the mode passed as parameter from the client mode string */
	int	Client::removeMode(char mode_char)
	{
		std::string	valid_modes = "io";

		if (valid_modes.find(mode_char) != std::string::npos)
		{
			if (this->_mode.find(mode_char) != std::string::npos)
			{
				this->_mode.erase(this->_mode.find(mode_char), 1);
				return (ERR_SUCCESS);
			}
			return (1);
		}
		return (ERR_UNKNOWNMODE);
	}

	/* ************************************************************************** */

	/* friend operator == */
	bool	operator==(const Client& lhs, const Client& rhs)
	{
		return ((lhs._socket_fd == rhs._socket_fd && lhs._nick == rhs._nick));
	}
} // namespace ft_irc
