/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:58:53 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/05 11:59:48 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cstdlib>
#include <list>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include "client.hpp"
#include "Message.hpp"
#include "Channel.hpp"

namespace ft_irc
{
	// default constructor
	Channel::Channel()
	: _name(), _topic(), _mode(), _clients()
	{
	}

	// name constructor
	Channel::Channel(const std::string& name)
	: _name(name), _topic(), _mode(), _clients()
	{
	}

	// copy constructor
	Channel::Channel(const Channel& other)
	: _name(other._name), _topic(other._topic), _mode(other._mode), _clients(other._clients)
	{
	}

	// assignment operator
	Channel&	Channel::operator=(const Channel& other)
	{
		if (this != &other)
		{
			_name = other.getName();
			_topic = other.getTopic();
			_mode = other.getMode();
			_clients = other.getClients();
		}
		return (*this);
	}

	// destructor
	Channel::~Channel() {}

	// getters
	std::string	Channel::getName() const
	{
		return (this->_name);
	}

	std::string	Channel::getTopic() const
	{
		return (this->_topic);
	}

	std::string	Channel::getMode() const
	{
		return (this->_mode);
	}

	std::list<IRCClient>	Channel::getClients() const
	{
		return (this->_clients);
	}

	// setters

	void	Channel::setName(const std::string& name)
	{
		this->_name = name;
	}

	void	Channel::setTopic(const std::string& topic)
	{
		this->_topic = topic;
	}

	void	Channel::setMode(const std::string& mode)
	{
		this->_mode = mode;
	}

	void	Channel::setClients(const std::list<IRCClient>& clients)
	{
		this->_clients = clients;
	}

	// manage clients in channel

	// Find a client using a nickname
	std::list<IRCClient>::iterator	Channel::findClient(IRCClient& client)
	{
		std::list<IRCClient>::iterator	it;

		it = std::find(this->_clients.begin(), this->_clients.end(), client);
		return (it);
	}

	// Check whether the given client is in the channel
	bool	Channel::hasClient(IRCClient& client)
	{
		return (this->findClient(client) != this->_clients.end());
	}

	// Add a client to the channel
	void	Channel::addClient(IRCClient& client)
	{
		this->_clients.push_back(client);
	}

	void	Channel::removeClient(IRCClient& client)
	{
		this->_clients.remove(client);
	}

	// send a message to all clients in channel
	void	Channel::broadcastMessage(const Message& msg)
	{
		for (std::list<IRCClient>::const_iterator	it = this->_clients.begin();
			it != this->_clients.end();
			++it)
		{
			//std::cout << "Sending: '" << msg.getResponse() << "' to " << it->getIpAddressStr() << std::endl;
			if (send(it->getSocketFd(), msg.getResponse().c_str(), msg.getResponse().size(), 0) < 0)
			{
				throw std::runtime_error("send() failed");
			}
		}
	}

	// debug
	void	Channel::displayClients()
	{
		std::cout << "Users in channel #" << this->getName() << ":\n";

		for (std::list<IRCClient>::iterator	it = this->_clients.begin();
			 it != this->_clients.end();
			 ++it)
		{
			std::cout << "- " << it->getNick() << '\n';
		}
	}
} // !namespace ft_irc