/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:58:53 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/04 16:02:05 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cstdlib>
#include <list>
#include <iostream>
#include <string>
#include "client.hpp"
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
	std::list<IRCClient>::iterator	Channel::findClient(const std::string& nick)
	{
		std::list<IRCClient>::iterator	it = this->_clients.begin();

		while (it != this->_clients.end())
		{
			if (it->getNick() == nick)
				break ;
			++it;
		}
		return (it);
	}

	// Check whether the given client is in the channel
	bool	Channel::hasClient(const std::string& nick)
	{
		std::list<IRCClient>::iterator	it;

		it = this->findClient(nick);
		return (it != this->_clients.end());
	}

	// Add a client to the channel
	int	Channel::addClient(IRCClient& client)
	{
		if (!this->hasClient(client.getNick()))
		{
			this->_clients.push_back(client);
			return (EXIT_SUCCESS);
		}
		return (EXIT_FAILURE);
	}

	int	Channel::removeClient(const std::string& nick)
	{
		if (this->hasClient(nick))
		{
			this->_clients.remove(*findClient(nick));
			return (EXIT_SUCCESS);
		}
		return (EXIT_FAILURE);
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