/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:56:54 by root              #+#    #+#             */
/*   Updated: 2021/11/28 16:32:59 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <list>
#include <string>
#include "Channel.hpp"
#include "Client.hpp"
#include "server_operations.hpp"
#include "ft_irc.hpp"

int	setNonblocking(int fd);

namespace ft_irc
{
	/* Constructor */
	Client::Client(struct sockaddr_in address,
				   std::string nick,
				   std::string realname,
				   std::string username,
				   std::string password,
				   std::string hostname)
	: _nick(nick),
	  _realname(realname),
	  _hostname(hostname),
	  _username(username),
	  _mode(),
	  _password(password),
	  _in_buffer(),
	  _out_buffer(),
	  _max_cmd_length(512),
	  _connected(false),
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
				   std::string password,
				   std::string hostname)
	: _nick(nick),
	  _realname(realname),
	  _hostname(hostname),
	  _username(username),
	  _mode(),
	  _password(password),
	  _in_buffer(),
	  _out_buffer(),
	  _max_cmd_length(512),
	  _connected(false),
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
	  _hostname(other._hostname),
	  _username(other._username),
	  _mode(other._mode),
	  _password(other._password),
	  _in_buffer(other._in_buffer),
	  _out_buffer(other._out_buffer),
	  _max_cmd_length(other._max_cmd_length),
	  _connected(other._connected),
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
			this->_hostname = other._hostname;
			this->_username = other._username;
			this->_in_buffer = other._in_buffer;
			this->_out_buffer = other._out_buffer;
			this->_connected = other._connected;
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
		return (this->_nick);
	}

	std::string	Client::getRealName() const
	{
		return (this->_realname);
	}

	std::string	Client::getUsername() const
	{
		return (this->_username);
	}

	std::string	Client::getPassword() const
	{
		return (this->_password);
	}

	std::string	Client::getHostname() const
	{
		return (this->_hostname);
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
		std::cout << "Nick: " << nick << std::endl;
		this->_nick = nick;
	}

	void	Client::setRealName(const std::string& realname)
	{
		std::cout << "Real Name: " << realname << std::endl;
		this->_realname = realname;
	}

	void	Client::setHostname(const std::string& hostname)
	{
		std::cout << "Hostname: " << hostname << std::endl;
		this->_hostname = hostname;
	}

	void	Client::setUsername(const std::string& username)
	{
		std::cout << "Username: " << username << std::endl;
		this->_username = username;
	}

	void	Client::setPassword(const std::string& password)
	{
		this->_password = password;
	}
	
	void	Client::setSocketFd(int socket_fd)
	{
		this->_socket_fd = socket_fd;
	}

	void	Client::setJoinedChannels(const t_channels& joined_channels)
	{
		this->_joined_channels = joined_channels;
	}

	void	Client::setConnected(bool connected)
	{
		this->_connected = connected;
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
		return (this->_alive);
	}

	bool	Client::isAlive() const
	{
		return (this->_socket_fd != -1);
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

	//placeholder
	bool	Client::isOper() const
	{
		return (this->_mode.find("o") != std::string::npos);
	}

	bool	Client::isChanOp(Channel& channel)
	{
		return (channel.hasChanOp(*this));
	}

	//isPinged()
	bool	Client::isPinged() const
	{
		return (this->_pinged);
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
		struct pollfd	poll_fd;
		poll_fd.fd = this->_socket_fd;
		poll_fd.events = POLLIN;

		int	ret = poll(&poll_fd, 1, this->_timeout.tv_usec);

		if (ret == -1)
			throw std::runtime_error("poll() failed");
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
		return (this->_in_buffer.find(CRLF) != std::string::npos);
	}

	std::string	Client::popUnprocessedCommand()
	{
		std::string	cmd = this->_in_buffer.substr(0, this->_in_buffer.find(CRLF));

		Logger	logger(DEBUG);

		logger.log(LOG_LEVEL_DEBUG, "Received: " + cmd);
		this->_in_buffer.erase(0, this->_in_buffer.find(CRLF) + sizeof(CRLF) - 1);
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
			throw std::runtime_error("poll() failed");
		if (poll_ret == 0)
			return (0);
		// read
		ret = recv(this->_socket_fd, bytes_buffer, MAX_COMMAND_SIZE, 0);
		if (ret == -1)
			throw std::runtime_error("recv() failed");
		if (ret == 0)
			return (0);

		// append to in_buffer
		this->_in_buffer.append(bytes_buffer, ret);
		// if there's not \r\n in the first 512 bytes, insert a \r\n at offset 512
		if (this->_in_buffer.size() > this->_max_cmd_length)
		{
			found = this->_in_buffer.find(CRLF);
			if (found == std::string::npos || found > this->_max_cmd_length)
				this->_in_buffer.insert(this->_max_cmd_length, CRLF);
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
			throw std::runtime_error("send() failed");
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

	/* Mode operations ********************************************************** */

	/* Adds the mode passed as parameter to the client mode string */
	int	Client::addMode(char mode_char)
	{
		std::string	valid_modes = "iswo";

		if (valid_modes.find(mode_char) != std::string::npos)
		{
			this->_mode += mode_char;
			return (ERR_SUCCESS);
		}
		return (ERR_UNKNOWNMODE);
	}

	/* Removes the mode passed as parameter from the client mode string */
	int	Client::removeMode(char mode_char)
	{
		std::string	valid_modes = "iswo";

		if (valid_modes.find(mode_char) != std::string::npos)
		{
			this->_mode.erase(this->_mode.find(mode_char), 1);
			return (ERR_SUCCESS);
		}
		return (ERR_UNKNOWNMODE);
	}

	/* ************************************************************************** */

	/* friend operator == */
	bool	operator==(const Client& lhs, const Client& rhs)
	{
		return ((lhs._socket_fd == rhs._socket_fd && lhs._nick == rhs._nick));
	}

	// debug
	void	Client::displayJoinedChannels()
	{
		if (this->_joined_channels.empty())
		{
			std::cout << this->getNick() << " parted all channels.\n";
			return ;
		}

		std::cout << this->getNick() << " joined channels:\n";

		for (t_channels::iterator it = this->_joined_channels.begin();
			 it != this->_joined_channels.end();
			 ++it)
		{
			std::cout << "- " << (*it)->getName() << '\n';
		}
	}
} // namespace ft_irc
