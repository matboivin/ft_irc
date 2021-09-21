/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 16:56:54 by root              #+#    #+#             */
/*   Updated: 2021/09/20 17:03:20 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

namespace ft_irc
{
	IRCClient::IRCClient(struct sockaddr_in address,
	std::string nick, std::string user_agent, std::string password)
	{
		this->nick = nick;
		this->user_agent = user_agent;
		this->password = password;
		this->address = address;
		this->address_str = inet_ntoa(address.sin_addr);
		this->address_size = sizeof(address);
	}
		//copy constructor
	IRCClient::IRCClient(const IRCClient &other)
	{
		this->nick = other.nick;
		this->user_agent = other.user_agent;
		this->joined_channels = other.joined_channels;
		this->password = other.password;
		this->address_str = other.address_str;
		this->address = other.address;
		this->address_size = other.address_size;
	}
	//assignment operator
	IRCClient &IRCClient::operator=(const IRCClient &other)
	{
		this->nick = other.nick;
		this->user_agent = other.user_agent;
		this->joined_channels = other.joined_channels;
		this->password = other.password;
		this->address_str = other.address_str;
		this->address = other.address;
		this->address_size = other.address_size;
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
	std::string IRCClient::getUserAgent() const
	{
		return this->user_agent;
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
	void IRCClient::setUserAgent(std::string user_agent)
	{
		this->user_agent = user_agent;
	}
	void IRCClient::setJoinedChannels(std::string joined_channels)
	{
		this->joined_channels = joined_channels;
	}
	void IRCClient::setPassword(std::string password)
	{
		this->password = password;
	}
}
