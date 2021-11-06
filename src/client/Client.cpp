/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:56:54 by root              #+#    #+#             */
/*   Updated: 2021/11/06 15:34:33 by root             ###   ########.fr       */
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
	: _nick(nick), _realname(realname), _hostname(hostname), _username(username),
	  _mode(),
	  _password(password),
	  _address(address),
	  _address_size(sizeof(address)),
	  _address_str(inet_ntoa(address.sin_addr)),
	  _timeout(),
	  _socket_fd(-1),
	  _connected(false),
	  _in_buffer(),
	  _out_buffer(),
	  _max_cmd_length(512),
	  _joined_channels(),
	  _alive(true),
	  _registered(false)
	{
		this->_timeout = (struct timeval){.tv_sec = 0, .tv_usec = 50};
		this->_keep_alive = (struct timeval){.tv_sec = 30, .tv_usec = 0};

		if (gettimeofday(&this->_last_event_time, NULL))
			throw std::runtime_error("gettimeofday() failed");
	}

	/* Copy constructor */
	Client::Client(const Client& other)
	: _nick(other._nick), _realname(other._realname), _hostname(other._hostname),
	  _username(other._username),
	  _mode(other._mode),
	  _password(other._password),
	  _address(other._address),
	  _address_size(other._address_size),
	  _address_str(other._address_str),
	  _timeout(other._timeout),
	  _socket_fd(other._socket_fd),
	  _connected(other._connected),
	  _in_buffer(other._in_buffer),
	  _out_buffer(other._out_buffer),
	  _max_cmd_length(other._max_cmd_length),
	  _joined_channels(other._joined_channels),
	  _alive(other._alive),
	  _keep_alive(other._keep_alive),
	  _last_event_time(other._last_event_time),
	  _registered(other._registered)
	{
	}

	/* Copy assignment operator */
	Client&	Client::operator=(const Client& other)
	{
		if (this != &other)
		{
			this->_nick = other.getNick();
			this->_realname = other.getRealName();
			this->_username = other.getUsername();
			this->_password = other.getPassword();
			this->_hostname = other.getHostname();
			this->_address = other._address;
			this->_address_str = other._address_str;
			this->_address_size = other._address_size;
			this->_timeout = other._timeout;
			this->_socket_fd = other.getSocketFd();
			this->_connected = other._connected;
			this->_joined_channels = other.getJoinedChannels();
			this->_alive = other._alive;
			this->_keep_alive = other._keep_alive;
			this->_last_event_time = other._last_event_time;
			this->_registered = other._registered;
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
			throw std::runtime_error("accept() failed");

		setNonblocking(this->_socket_fd);
		this->_address_str = inet_ntoa(this->_address.sin_addr);
		return (this->_socket_fd);
	}

	/* poll */
	bool	Client::hasNewEvents()
	{
		struct pollfd	poll_fd = {.fd = this->_socket_fd, .events = POLLIN};
		int				ret = poll(&poll_fd, 1, this->_timeout.tv_usec);

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

		Logger logger(0);
		logger.log(0, "Received: " + cmd);
		this->_in_buffer.erase(0, this->_in_buffer.find(CRLF) + sizeof(CRLF) - 1);
		return (cmd);
	}

	/* Reads 512 bytes from the socket if there is data to read */
	int	Client::updateInBuffer()
	{
		char					bytes_buffer[MAX_COMMAND_SIZE];
		int						ret;
		struct pollfd			poll_fd = {.fd = this->_socket_fd, .events = POLLIN};
		int						poll_ret;
		std::string::size_type	found;

		if (this->getSocketFd() < 0)
			return (-1);
		poll_ret = poll(&poll_fd, 1, this->_timeout.tv_usec);
		if (poll_ret == -1)
			throw std::runtime_error("poll() failed");
		if (poll_ret == 0)
			return (0);
		//read
		ret = recv(this->_socket_fd, bytes_buffer, MAX_COMMAND_SIZE, 0);
		if (ret == -1)
			throw std::runtime_error("recv() failed");
		if (ret == 0)
			return (0);
		//std::cerr.write(bytes_buffer, ret);
		//append to in_buffer
		this->_in_buffer.append(bytes_buffer, ret);
		//if there's not \r\n in the first 512 bytes, insert a \r\n at offset 512
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
		size_t	size;

		//write 512 bytes at most and removes them from the out_buffer
		size = std::min(this->_out_buffer.size(), this->_max_cmd_length);
		if (!size)
			return (0);
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

	/* The client quits all channels they joined */
	void	Client::partAllChannels()
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
	void	Client::addMode(const std::string& mode)
	{
		if (this->_mode.find(mode) == std::string::npos)
			this->_mode.append(mode);
	}

	/* Removes the mode passed as parameter from the client mode string */
	void	Client::removeMode(const std::string& mode)
	{
		size_t	pos = this->_mode.find(mode);

		if (pos != std::string::npos)
			this->_mode.erase(pos, 1);
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

		for (t_channels::iterator	it = this->_joined_channels.begin();
			 it != this->_joined_channels.end();
			 ++it)
		{
			std::cout << "- " << (*it)->getName() << '\n';
		}
	}
} // namespace ft_irc
