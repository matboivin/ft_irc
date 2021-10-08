/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:56:54 by root              #+#    #+#             */
/*   Updated: 2021/10/08 14:44:31 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <list>
#include <string>
#include "Channel.hpp"
#include "Client.hpp"

int	setNonblocking(int fd);

namespace ft_irc
{
	// constructor
	Client::Client(struct sockaddr_in address,
				   std::string nick,
				   std::string realname,
				   std::string username,
				   std::string password)
	: _nick(nick), _realname(realname), _username(username),
	  _mode(),
	  _password(password),
	  _address(address),
	  _address_size(sizeof(address)), _address_str(inet_ntoa(address.sin_addr)),
	  _timeout(), _socket_fd(-1),
	  _connected(false),
	  _in_buffer(), _out_buffer(),
	  _max_cmd_length(512),
	  _joined_channels()
	{
		this->_timeout = (struct timeval){.tv_sec = 0, .tv_usec = 50};
	}

	// copy constructor
	Client::Client(const Client& other)
	: _nick(other._nick), _realname(other._realname), _username(other._username),
	  _mode(other._mode),
	  _password(other._password), _address(other._address),
	  _address_size(other._address_size), _address_str(other._address_str),
	  _timeout(other._timeout), _socket_fd(other._socket_fd),
	  _connected(other._connected),
	  _in_buffer(other._in_buffer), _out_buffer(other._out_buffer),
	  _max_cmd_length(other._max_cmd_length),
	  _joined_channels(other._joined_channels)
	{
	}

	// assignment operator
	Client &Client::operator=(const Client& other)
	{
		if (this != &other)
		{
			this->_nick = other.getNick();
			this->_realname = other.getRealName();
			this->_username = other.getUsername();
			this->_password = other.getPassword();
			this->_address = other._address;
			this->_address_str = other._address_str;
			this->_address_size = other._address_size;
			this->_timeout = other._timeout;
			this->_socket_fd = other.getSocketFd();
			this->_connected = other._connected;
			this->_joined_channels = other.getJoinedChannels();
		}
		return (*this);
	}

	//destructor
	Client::~Client()
	{
	}

	// getters

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

	std::list<Channel*>	Client::getJoinedChannels() const
	{
		return (this->_joined_channels);
	}

	// setters

	void	Client::setNick(const std::string& nick)
	{
		this->_nick = nick;
	}

	void	Client::setRealName(const std::string& realname)
	{
		this->_realname = realname;
	}

	void	Client::setUsername(const std::string& username)
	{
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

	void	Client::setJoinedChannels(std::list<Channel*> joined_channels)
	{
		this->_joined_channels = joined_channels;
	}

	void	Client::joinChannel(Channel& channel)
	{
		this->_joined_channels.push_back(&channel);
	}

	// helpers

	bool	Client::isRegistered() const
	{
		return (!(this->_nick.empty() || this->_realname.empty()));
	}

	bool	Client::isConnected() const
	{
		return (this->_socket_fd != -1);
	}

	int	Client::awaitConnection(int socket_fd)
	{
		this->_socket_fd = accept(socket_fd, (struct sockaddr *)&this->_address,
								 &this->_address_size);

		if (this->_socket_fd == -1)
			throw std::runtime_error("accept() failed");

		setNonblocking(this->_socket_fd);
		this->_address_str = inet_ntoa(this->_address.sin_addr);
		return (this->_socket_fd);
	}

	//poll
	bool	Client::hasNewEvents()
	{
		struct pollfd	poll_fd = {.fd = this->_socket_fd, .events = POLLIN};
		int				ret = poll(&poll_fd, 1, this->_timeout.tv_usec);

		if (ret == -1)
			throw std::runtime_error("poll() failed");
		return (ret > 0);
	}

	bool	Client::hasUnprocessedCommands()
	{
		//if \r\n is in the in_buffer, return true
		return (this->_in_buffer.find(CRLF) != std::string::npos);
	}

	std::string	Client::popUnprocessedCommand()
	{
		std::string	cmd = this->_in_buffer.substr(0, this->_in_buffer.find(CRLF));
		
		this->_in_buffer.erase(0, this->_in_buffer.find(CRLF) + sizeof(CRLF) - 1);
		return (cmd);
	}

	//reads 512 bytes from the socket if there is data to read
	int	Client::updateInBuffer()
	{
		char					bytes_buffer[MAX_COMMAND_SIZE];
		int						ret;
		struct pollfd			poll_fd = {.fd = this->_socket_fd, .events = POLLIN};
		int						poll_ret = poll(&poll_fd, 1, this->_timeout.tv_usec);
		std::string::size_type	found;

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

	//friend operator ==
	bool	operator==(const Client& lhs, const Client& rhs)
	{
		return ((lhs._socket_fd == rhs._socket_fd && lhs._nick == rhs._nick));
	}
} // namespace ft_irc
