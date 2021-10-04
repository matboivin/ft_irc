/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 18:58:53 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/30 19:02:24 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Channel.hpp"

namespace ft_irc
{
	// default constructor
	Channel::Channel()
	: _name(), _topic(), _mode()
	{
	}

	// name constructor
	Channel::Channel(const std::string& name)
	: _name(name), _topic(), _mode()
	{
	}

	// copy constructor
	Channel::Channel(const Channel& other)
	: _name(other._name), _topic(other._topic), _mode(other._mode)
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
} // !namespace ft_irc