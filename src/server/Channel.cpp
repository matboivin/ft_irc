/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:58:53 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/27 17:20:49 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <list>
#include <iostream>
#include <string>
#include "Client.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "server_operations.hpp"

namespace ft_irc
{
	/* Default constructor */
	Channel::Channel()
	: _name(), _topic(), _mode(), _clients(), _chan_ops()
	{
	}

	/* Name constructor */
	Channel::Channel(const std::string& name)
	: _name(name), _topic(), _mode(), _clients(), _chan_ops()
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
			_name = other.getName();
			_topic = other.getTopic();
			_mode = other.getMode();
			_clients = other.getClients();
			_chan_ops = other.getChanOps();
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
		this->_topic = topic;
		if (this->_topic[0] == ':')
			this->_topic.erase(0, 1);
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
		this->_chan_ops= chan_ops;
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
		this->_clients.push_back(&client);
	}

	/* Removes a client from the channel */
	void	Channel::removeClient(Client& client)
	{
		this->_clients.remove(&client);
	}

	/* Manage clients in channel ************************************************ */

	/* Finds a channel operator using a nickname */
	Channel::t_clients::iterator	Channel::findChanOp(Client& chan_op)
	{
		t_clients::iterator	it;

		it = std::find(this->_chan_ops.begin(), this->_chan_ops.end(), &chan_op);
		return (it);
	}

	/* Checks whether the given client is channel operator */
	bool	Channel::hasChanOp(Client& chan_op)
	{
		return (this->findChanOp(chan_op) != this->_chan_ops.end());
	}

	/* Makes a client operator of the channel */
	void	Channel::addChanOp(Client& chan_op)
	{
		this->_chan_ops.push_back(&chan_op);
	}

	/* Removes the operator privileges to a client */
	void	Channel::removeChanOp(Client& chan_op)
	{
		this->_chan_ops.remove(&chan_op);
	}

	/* Manage topic ************************************************************* */

	void	Channel::changeTopic(const std::string& topic, Message& msg)
	{
		this->setTopic(topic);

		msg.setRecipients(this->getClients());
		msg.setResponse(
			build_prefix( build_full_client_id(msg.getSender()) )
			+ " TOPIC " + this->_name + ' ' + topic
			);
		msg.appendSeparator();
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
} // !namespace ft_irc
