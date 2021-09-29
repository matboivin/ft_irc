/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:56:54 by root              #+#    #+#             */
/*   Updated: 2021/09/29 17:05:27 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

namespace ft_irc
{
	IRCClient::IRCClient(struct sockaddr_in address,
	std::string nick, std::string realname, std::string password)
	{
		this->nick = nick;
		this->realname = realname;
		this->password = password;
		this->address = address;
		this->address_str = inet_ntoa(address.sin_addr);
		this->address_size = sizeof(address);
		this->socket_fd = -1;
		this->connected = false;
		this->timeout = (struct timeval){.tv_sec = 0, .tv_usec = 50};
		this->max_cmd_length = 512;
	}
		//copy constructor
	IRCClient::IRCClient(const IRCClient &other)
	{
		this->nick = other.nick;
		this->realname = other.realname;
		this->joined_channels = other.joined_channels;
		this->password = other.password;
		this->address_str = other.address_str;
		this->address = other.address;
		this->address_size = other.address_size;
		this->socket_fd = other.socket_fd;
		this->connected = other.connected;
		this->timeout = other.timeout;
		this->max_cmd_length = other.max_cmd_length;
	}
	//assignment operator
	IRCClient &IRCClient::operator=(const IRCClient &other)
	{
		this->nick = other.nick;
		this->realname = other.realname;
		this->joined_channels = other.joined_channels;
		this->password = other.password;
		this->address_str = other.address_str;
		this->address = other.address;
		this->address_size = other.address_size;
		this->socket_fd = other.socket_fd;
		this->connected = other.connected;
		this->timeout = other.timeout;
		this->max_cmd_length = other.max_cmd_length;
		return *this;
	}
	//destructor
	IRCClient::~IRCClient()
	{
	}
	//IRCClient IRCClient::getters
	std::string IRCClient::getNick() const
	{
		return this->nick;
	}
	std::string IRCClient::getRealName() const
	{
		return this->realname;
	}
	std::string IRCClient::getJoinedChannels() const
	{
		return this->joined_channels;
	}
	std::string IRCClient::getPassword() const
	{
		return this->password;
	}
	std::string IRCClient::getIpAddressStr() const
	{
		return this->address_str;
	}
	struct sockaddr_in &IRCClient::getAddress()
	{
		return this->address;
	}
	//IRCClient::getAdressSize
	socklen_t &IRCClient::getAddressSize()
	{
		return this->address_size;
	}
	//IRCClient IRCClient::setters
	void IRCClient::setNick(std::string nick)
	{
		this->nick = nick;
	}
	void IRCClient::setRealName(std::string realname)
	{
		this->realname = realname;
	}
	void IRCClient::setJoinedChannels(std::string joined_channels)
	{
		this->joined_channels = joined_channels;
	}
	void IRCClient::setPassword(std::string password)
	{
		this->password = password;
	}
	
	void IRCClient::setSocketFd(int socket_fd)
	{
		this->socket_fd = socket_fd;
	}

	bool IRCClient::isRegistered() const
	{
		return (!(this->nick.empty() || this->realname.empty()));
	}
	//get socket fd
	int IRCClient::getSocketFd() const
	{
		return this->socket_fd;
	}
	//isConnected
	bool IRCClient::isConnected() const
	{
		return (this->socket_fd != -1);
	}

	int	IRCClient::awaitConnection(int socket_fd)
	{
		this->socket_fd = accept(socket_fd, (struct sockaddr *)&this->address,
								 &this->address_size);
		this->address_str = inet_ntoa(this->address.sin_addr);
		return this->socket_fd;
	}
	//poll
	bool			IRCClient::hasNewEvents()
	{
		struct pollfd poll_fd = {.fd = this->socket_fd, .events = POLLIN};
		int ret = poll(&poll_fd, 1, this->timeout.tv_usec);
		if (ret == -1)
			throw std::runtime_error("poll() failed");
		return (ret > 0);
	}
	bool				IRCClient::hasUnprocessedCommands()
	{
		//if \r\n is in the buffer, return true
		return (this->buffer.find(CRLF) != std::string::npos);
	}
	
	std::string			IRCClient::popUnprocessedCommand()
	{
		std::string cmd = this->buffer.substr(0, this->buffer.find(CRLF));
		
		this->buffer.erase(0, this->buffer.find(CRLF) + 2);
		return cmd;
	}

	//reads 512 bytes from the socket if there is data to read
	int					IRCClient::updateBuffer()
	{
		char					buffer[MAX_COMMAND_SIZE];
		int						ret;
		struct pollfd			poll_fd = {.fd = this->socket_fd, .events = POLLIN};
		int						poll_ret = poll(&poll_fd, 1, this->timeout.tv_usec);
		std::string::size_type	found;

		if (poll_ret == -1)
			throw std::runtime_error("poll() failed");
		if (poll_ret == 0)
			return 0;
		//read
		ret = recv(this->socket_fd, buffer, this->max_cmd_length, 0);
		if (ret == -1)
			throw std::runtime_error("recv() failed");
		if (ret == 0)
			return 0;
		//append to buffer
		this->buffer.append(buffer, ret);
		//if there's not \r\n in the first 512 bytes, insert a \r\n at offset 512
		if (this->buffer.size() > this->max_cmd_length)
		{
			found = this->buffer.find(CRLF);
			if (found == std::string::npos || found > this->max_cmd_length)
				this->buffer.insert(this->max_cmd_length, CRLF);
		}
		return ret;
	}
	//friend operator ==
	bool				operator==(const IRCClient &lhs, const IRCClient &rhs)
	{
		return (lhs.socket_fd == rhs.socket_fd && lhs.nick == rhs.nick);
	}
}
