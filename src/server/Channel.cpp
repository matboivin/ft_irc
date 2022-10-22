/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:58:53 by mboivin           #+#    #+#             */
/*   Updated: 2022/10/22 22:02:38 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <list>
#include <iostream>
#include <string>
#include "ft_irc.hpp"

namespace ft_irc
{
	/* Default constructor */
	Channel::Channel()
	: _name(), _topic(), _mode("nt"), _clients(), _chan_ops()
	{
	}

	/* Name constructor */
	Channel::Channel(const std::string& name)
	: _name(name), _topic(), _mode("nt"), _clients(), _chan_ops()
	{
	}

	/* Copy constructor */
	Channel::Channel(const Channel& other)
	: _name(other._name),
	  _topic(other._topic),
	  _mode(other._mode),
	  _clients(other._clients),
	  _chan_ops(other._chan_ops)
	{
	}

	/* Copy assignment operator */
	Channel&	Channel::operator=(const Channel& other)
	{
		if (this != &other)
		{
			_name = other._name;
			_topic = other._topic;
			_mode = other._mode;
			_clients = other._clients;
			_chan_ops = other._chan_ops;
		}
		return (*this);
	}

	/* Destructor */
	Channel::~Channel() {}

	/* Getters ****************************************************************** */

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

	const Channel::t_clients&	Channel::getClients() const
	{
		return (this->_clients);
	}

	const Channel::t_clients&	Channel::getChanOps() const
	{
		return (this->_chan_ops);
	}

	/* Setters ****************************************************************** */

	void	Channel::setName(const std::string& name)
	{
		this->_name = name;
	}

	void	Channel::setTopic(const std::string& topic)
	{
		std::size_t	pos;

		if (topic.at(0) == ':')
			pos = 1;
		else
			pos = 0;
		this->_topic = topic.substr(pos);
	}

	void	Channel::setMode(const std::string& mode)
	{
		this->_mode = mode;
	}

	void	Channel::setClients(const t_clients& clients)
	{
		this->_clients = clients;
	}

	void	Channel::setChanOps(const t_clients& chan_ops)
	{
		this->_chan_ops = chan_ops;
	}

	/* Helpers ****************************************************************** */

	bool	Channel::isEmpty() const
	{
		return (this->_clients.empty());
	}

	/* Manage clients in channel ************************************************ */

	/* Finds a client using a nickname */
	Channel::t_clients::iterator	Channel::findClient(Client& client)
	{
		t_clients::iterator	it;

		it = std::find(this->_clients.begin(), this->_clients.end(), &client);
		return (it);
	}

	/* Checks whether the given client is in the channel */
	bool	Channel::hasClient(Client& client)
	{
		return (this->findClient(client) != this->_clients.end());
	}

	/* Adds a client to the channel */
	void	Channel::addClient(Client& client)
	{
		if (!this->hasClient(client))
			this->_clients.push_back(&client);
	}

	/* Removes a client from the channel */
	void	Channel::removeClient(Client& client)
	{
		if (this->hasClient(client))
		{
			if (this->hasChanOp(client))
				this->removeChanOp(client);

			this->_clients.remove(&client);
		}
	}

	/* Manage clients in channel ************************************************ */

	/* Finds a channel operator using a nickname */
	Channel::t_clients::iterator	Channel::findChanOp(const Client& chan_op)
	{
		t_clients::iterator	it;

		it = std::find(this->_chan_ops.begin(), this->_chan_ops.end(), &chan_op);
		return (it);
	}

	/* Checks whether the given client is channel operator */
	bool	Channel::hasChanOp(const Client& chan_op)
	{
		return (this->findChanOp(chan_op) != this->_chan_ops.end());
	}

	/* Makes a client operator of the channel */
	void	Channel::addChanOp(Client& chan_op)
	{
		if (!this->hasChanOp(chan_op))
			this->_chan_ops.push_back(&chan_op);
	}

	/* Removes the operator privileges to a client */
	void	Channel::removeChanOp(Client& chan_op)
	{
		if (this->hasChanOp(chan_op))
			this->_chan_ops.remove(&chan_op);
	}

	/* Manage topic ************************************************************* */

	void	Channel::changeTopic(const std::string& topic, Message& msg)
	{
		this->setTopic(topic);

		msg.setRecipients(this->getClients());
		msg.setResponse(buildPrefix(buildFullClientId(msg.getSender())));
		msg.appendResponse(" TOPIC ");
		msg.appendResponse(this->_name);
		msg.appendResponse(" :");
		msg.appendResponse(this->_topic);
		msg.appendSeparator();
	}

	/* Mode operations ********************************************************** */

	/* Check whether the given mode is set */
	bool	Channel::hasMode(const char& mode_char) const
	{
		return (this->_mode.find(mode_char) != std::string::npos);
	}

	/* Adds the mode passed as parameter to the client mode string */
	int	Channel::addMode(const char& mode_char)
	{
		const std::string	modes = "nto";

		if (modes.find(mode_char) != std::string::npos)
		{
			if (this->_mode.find(mode_char) == std::string::npos)
				this->_mode += mode_char;
			return (ERR_SUCCESS);
		}
		return (ERR_UNKNOWNMODE);
	}

	/* Removes the mode passed as parameter from the client mode string */
	int	Channel::removeMode(const char& mode_char)
	{
		const std::string	modes = "nto";

		if (modes.find(mode_char) != std::string::npos)
		{
			if (this->_mode.find(mode_char) != std::string::npos)
				this->_mode.erase(this->_mode.find(mode_char), 1);
			return (ERR_SUCCESS);
		}
		return (ERR_UNKNOWNMODE);
	}

	/* ************************************************************************** */

	// debug
	void	Channel::displayClients()
	{
		std::cout << "Users in channel " << this->getName() << ":\n";

		for (t_clients::iterator	it = this->_clients.begin();
			 it != this->_clients.end();
			 ++it)
		{
			std::cout << "- " << (*it)->getNick() << '\n';
		}
	}

	void	Channel::displayChanOps()
	{
		std::cout << "ChanOps in channel " << this->getName() << ":\n";

		for (t_clients::iterator	it = this->_chan_ops.begin();
			 it != this->_chan_ops.end();
			 ++it)
		{
			std::cout << "- " << (*it)->getNick() << '\n';
		}
	}
} // !namespace ft_irc
