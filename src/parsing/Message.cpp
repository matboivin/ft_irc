/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 18:48:18 by mboivin           #+#    #+#             */
/*   Updated: 2021/11/09 16:31:39 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "Client.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include <algorithm>

namespace ft_irc
{
	/* Default constructor */
	Message::Message(Client& sender)
	: _sender(sender),
	  _serv_hostname("irc.42.fr"),
	  _response(),
	  _command(),
	  _params(),
	  _recipients()
	{
	}

	/* Server hostname constructor */
	Message::Message(Client& sender, const std::string& serv_hostname)
	: _sender(sender),
	  _serv_hostname(serv_hostname),
	  _response(),
	  _command(),
	  _params(),
	  _recipients()
	{
	}

	/* Copy constructor */
	Message::Message(const Message& other)
	: _sender(other._sender),
	  _serv_hostname(other._serv_hostname),
	  _response(other._response),
	  _command(other._command),
	  _params(other._params),
	  _recipients(other._recipients)
	{
	}

	/* Copy assignment operator */
	Message&	Message::operator=(const Message& other)
	{
		if (this != &other)
		{
			_sender = other.getSender();
			_serv_hostname = other.getServHostname();
			_response = other.getResponse();
			_command = other.getCommand();
			_params = other.getParams();
			_recipients = other.getRecipients();
		}
		return (*this);
	}

	/* Destructor */
	Message::~Message() {}

	/* Getters ****************************************************************** */

	Client&	Message::getSender() const
	{
		return (this->_sender);
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

	const Message::t_params&	Message::getParams() const
	{
		return (this->_params);
	}

	const Message::t_clients&	Message::getRecipients() const
	{
		return (this->_recipients);
	}

	/* Setters ****************************************************************** */

	void	Message::setSender(const Client& sender)
	{
		this->_sender = sender;
	}

	void	Message::setServHostname(const std::string& serv_hostname)
	{
		this->_serv_hostname = serv_hostname;
	}

	void	Message::setResponse(const std::string& response)
	{
		this->_response = response;
	}

	void	Message::appendResponse(const std::string& response)
	{
		this->_response += response;
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

	void	Message::setRecipients(const t_clients& recipients)
	{
		this->_recipients = recipients;
	}

	void	Message::setRecipient(Client& recipient)
	{
		t_clients::const_iterator	found;

		/* Checks if the recipient is already in the list */
		found = std::find(this->_recipients.begin(), this->_recipients.end(), &recipient);

		if (found == this->_recipients.end())
			this->_recipients.push_back(&recipient);
	}

	/* Helpers ****************************************************************** */

	/* Ends message with CRLF */
	void	Message::appendSeparator()
	{
		this->_response += "\r\n";
	}

	/* Remove params */
	void	Message::clearParams()
	{
		this->_params.clear();
	}

	/* Remove response */
	void	Message::clearResponse()
	{
		this->_response.clear();
	}

	/* Clients operations ******************************************************* */

	/*
	 * Remove all recipients
	 * (useful when a message is not correct)
	 */
	void	Message::clearRecipients()
	{
		this->_recipients.clear();
	}

	/* Append a range of recipients */
	void	Message::addRecipients(const t_clients& recipients)
	{
		this->_recipients.insert(this->_recipients.end(), recipients.begin(), recipients.end());
	}

	/* ************************************************************************** */

	// debug
	void	Message::displayMessage() const
	{
		std::cout << "command:  " << getCommand() << '\n'
				  << "params:   ";

		for (t_params::const_iterator it = this->_params.begin();
			 it != this->_params.end();
			 ++it)
		{
			std::cout << "'" << *it << "' ";
		}
		std::cout << "\nresponse: " << this->_response << std::endl;
	}
} // !namespace ft_irc
