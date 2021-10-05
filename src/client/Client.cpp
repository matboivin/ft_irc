/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:56:54 by root              #+#    #+#             */
/*   Updated: 2021/10/05 12:57:09 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
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
	: nick(nick), realname(realname), username(username),
	  mode(),
	  joined_channels(),
	  password(password),
	  address(address),
	  address_size(sizeof(address)), address_str(inet_ntoa(address.sin_addr)),
	  timeout(), socket_fd(-1),
	  connected(false),
	  in_buffer(), out_buffer(),
	  max_cmd_length(512)
	{
		this->timeout = (struct timeval){.tv_sec = 0, .tv_usec = 50};
	}

	// copy constructor
	Client::Client(const Client& other)
	: nick(other.nick), realname(other.realname), username(other.username),
	  mode(other.mode),
	  joined_channels(other.joined_channels),
	  password(other.password), address(other.address),
	  address_size(other.address_size), address_str(other.address_str),
	  timeout(other.timeout), socket_fd(other.socket_fd),
	  connected(other.connected),
	  in_buffer(other.in_buffer), out_buffer(other.out_buffer),
	  max_cmd_length(other.max_cmd_length)
	{
	}

	// assignment operator
	Client &Client::operator=(const Client& other)
	{
		if (this != &other)
		{
			this->nick = other.getNick();
			this->realname = other.getRealName();
			this->username = other.getUsername();
			this->joined_channels = other.getJoinedChannels();
			this->password = other.getPassword();
			this->address = other.address;
			this->address_str = other.address_str;
			this->address_size = other.address_size;
			this->timeout = other.timeout;
			this->socket_fd = other.getSocketFd();
			this->connected = other.connected;
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
		return (this->nick);
	}

	std::string	Client::getRealName() const
	{
		return (this->realname);
	}

	std::string	Client::getUsername() const
	{
		return (this->username);
	}

	std::string	Client::getJoinedChannels() const
	{
		return (this->joined_channels);
	}

	std::string	Client::getPassword() const
	{
		return (this->password);
	}

	struct sockaddr_in&	Client::getAddress()
	{
		return (this->address);
	}

	std::string	Client::getIpAddressStr() const
	{
		return (this->address_str);
	}

	socklen_t&	Client::getAddressSize()
	{
		return (this->address_size);
	}

	int	Client::getSocketFd() const
	{
		return (this->socket_fd);
	}

	// setters

	void	Client::setNick(const std::string& nick)
	{
		this->nick = nick;
	}

	void	Client::setRealName(const std::string& realname)
	{
		this->realname = realname;
	}

	void	Client::setUsername(const std::string& username)
	{
		this->username = username;
	}

	void	Client::setJoinedChannels(const std::string& joined_channels)
	{
		this->joined_channels = joined_channels;
	}

	void	Client::setPassword(const std::string& password)
	{
		this->password = password;
	}
	
	void	Client::setSocketFd(int socket_fd)
	{
		this->socket_fd = socket_fd;
	}

	// helpers

	bool	Client::isRegistered() const
	{
		return (!(this->nick.empty() || this->realname.empty()));
	}

	bool	Client::isConnected() const
	{
		return (this->socket_fd != -1);
	}

	int	Client::awaitConnection(int socket_fd)
	{
		this->socket_fd = accept(socket_fd, (struct sockaddr *)&this->address,
								 &this->address_size);

		if (this->socket_fd == -1)
			throw std::runtime_error("accept() failed");

		setNonblocking(this->socket_fd);
		this->address_str = inet_ntoa(this->address.sin_addr);
		return (this->socket_fd);
	}

	//poll
	bool	Client::hasNewEvents()
	{
		struct pollfd	poll_fd = {.fd = this->socket_fd, .events = POLLIN};
		int				ret = poll(&poll_fd, 1, this->timeout.tv_usec);

		if (ret == -1)
			throw std::runtime_error("poll() failed");
		return (ret > 0);
	}

	bool	Client::hasUnprocessedCommands()
	{
		//if \r\n is in the in_buffer, return true
		return (this->in_buffer.find(CRLF) != std::string::npos);
	}

	std::string	Client::popUnprocessedCommand()
	{
		std::string	cmd = this->in_buffer.substr(0, this->in_buffer.find(CRLF));
		
		this->in_buffer.erase(0, this->in_buffer.find(CRLF) + sizeof(CRLF) - 1);
		return (cmd);
	}

	//reads 512 bytes from the socket if there is data to read
	int	Client::updateInBuffer()
	{
		char					bytes_buffer[MAX_COMMAND_SIZE];
		int						ret;
		struct pollfd			poll_fd = {.fd = this->socket_fd, .events = POLLIN};
		int						poll_ret = poll(&poll_fd, 1, this->timeout.tv_usec);
		std::string::size_type	found;

		if (poll_ret == -1)
			throw std::runtime_error("poll() failed");
		if (poll_ret == 0)
			return (0);
		//read
		ret = recv(this->socket_fd, bytes_buffer, MAX_COMMAND_SIZE, 0);
		if (ret == -1)
			throw std::runtime_error("recv() failed");
		if (ret == 0)
			return (0);
		//std::cerr.write(bytes_buffer, ret);
		//append to in_buffer
		this->in_buffer.append(bytes_buffer, ret);
		//if there's not \r\n in the first 512 bytes, insert a \r\n at offset 512
		if (this->in_buffer.size() > this->max_cmd_length)
		{
			found = this->in_buffer.find(CRLF);
			if (found == std::string::npos || found > this->max_cmd_length)
				this->in_buffer.insert(this->max_cmd_length, CRLF);
		}
		return (ret);
	}

	int	Client::updateOutBuffer()
	{
		int		ret;
		size_t	size;		
		
		//write 512 bytes at most and removes them from the out_buffer
		size = std::min(this->out_buffer.size(), this->max_cmd_length);
		if (!size)
			return (0);
		ret = send(this->socket_fd, this->out_buffer.c_str(), size, 0);
		if (ret == -1)
			throw std::runtime_error("send() failed");
		this->out_buffer.erase(0, ret);
		return (ret);
	}

	void	Client::sendCommand(std::string cmd)
	{
		this->out_buffer += cmd;
	}

	//friend operator ==
	bool	operator==(const Client& lhs, const Client& rhs)
	{
		return ((lhs.socket_fd == rhs.socket_fd && lhs.nick == rhs.nick));
	}
} // namespace ft_irc
