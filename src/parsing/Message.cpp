/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:48:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/30 16:45:38 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "Message.hpp"
#include "IRCParser.hpp"

namespace ft_irc
{
	// default constructor
	Message::Message()
	: _sender(), _type(undefined), _serv_hostname("irc.42.fr"), _response(), _command(), _params()
	{
	}

	// server hostname constructor
	Message::Message(const std::string& serv_hostname)
	: _sender(), _type(undefined), _serv_hostname(serv_hostname), _response(), _command(), _params()
	{
	}

	// copy constructor
	Message::Message(const Message& other)
	: _sender(other._sender), _type(other._type), _serv_hostname(other._serv_hostname),
	  _response(other._response), _command(other._command), _params(other._params)
	{
	}

	// assignment operator
	Message&	Message::operator=(const Message& other)
	{
		if (this != &other)
		{
			_sender = other.getSender();
			_type = other.getType();
			_serv_hostname = other.getServHostname();
			_response = other.getResponse();
			_command = other.getCommand();
			_params = other.getParams();
		}
		return (*this);
	}

	// destructor
	Message::~Message() {}

	// getters
	IRCClient	Message::getSender() const
	{
		return (this->_sender);
	}

	reply_type	Message::getType() const
	{
		return (this->_type);
	}

	std::string	Message::getServHostname() const
	{
		return (this->_serv_hostname);
	}

	std::string	Message::getResponse() const
	{
		return (this->_response);
	}

	std::string	Message::getCommand() const
	{
		return (this->_command);
	}

	Message::str_vec	Message::getParams() const
	{
		return (this->_params);
	}

	// setters

	void	Message::setSender(const IRCClient& sender)
	{
		this->_sender = sender;
	}

	void	Message::setType(reply_type type)
	{
		this->_type = type;
	}

	void	Message::setServHostname(const std::string& serv_hostname)
	{
		this->_serv_hostname = serv_hostname;
	}

	void	Message::setResponse(const std::string& response)
	{
		this->_response = response;
	}

	void	Message::setCommand(const std::string& command)
	{
		this->_command = command;
	}

	void	Message::setParam(const std::string& param)
	{
		if (!param.empty() && (this->_params.size() < MSG_MAX_PARAMS))
			this->_params.push_back(param);
	}

	// end message with CRLF
	void	Message::appendSeparator()
	{
		this->_response.append("\r\n");
	}

	// debug
	void	Message::displayMessage() const
	{
		std::cout << "command: " << getCommand() << '\n'
				  << "params:  ";
		
		for (std::size_t i = 0; i < this->_params.size(); i++)
			std::cout << "'" << this->_params[i] << "' ";
		std::cout << std::endl;
	}

	void	Message::displayResponse() const
	{
		std::cout << "Response: '" << getResponse() << "'" << std::endl;
	}
} // !namespace ft_irc