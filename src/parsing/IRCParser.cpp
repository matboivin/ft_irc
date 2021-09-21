/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 16:33:34 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "IRCParser.hpp"
#include "client.hpp"
#include "Message.hpp"

namespace ft_irc
{
	// Parser for IRC protocol messages

	// default constructor
	IRCParser::IRCParser() : _packet_it() {}

	// copy constructor
	IRCParser::IRCParser(const IRCParser& other) : _packet_it(other._packet_it)
	{
		static_cast<void>(other); // tmp
	}

	// assignment operator
	IRCParser&	IRCParser::operator=(const IRCParser& other)
	{
		if (this != &other)
			_packet_it = other.getPacketIt();

		return (*this);
	}

	// destructor
	IRCParser::~IRCParser() {}

	// getters
	IRCParser::str_const_it	IRCParser::getPacketIt() const
	{
		return (this->_packet_it);
	}

	// setters
	void	IRCParser::setPacketIt(IRCParser::str_const_it packet_it)
	{
		this->_packet_it = packet_it;
	}

	// parsing

	bool	IRCParser::_parseSeparator()
	{
		std::string	s = &(*this->getPacketIt());

		return (s == CRLF);
	}

	// main parsing function
	// not sure about how to pass the client arg
	// packet looks like: <command> <params> <crlf>
	void	IRCParser::parseMessage(const std::string& packet, IRCClient& sender)
	{
		Message	msg;

		static_cast<void>(sender);
		this->setPacketIt(packet.begin());

		//msg.setPrefix(sender.getNick() + "!" + "tmp" + "@" + sender.getIpAddressStr());
	}

	// 3 types of messages -> 3 types of handling them:
	//  - respond directly to sender
	//  - forward message to recipient(s)
	//  - broadcast (e.g., joining a channel)

	// dummy tmp function that would pass msg to appropriate worker
	void	IRCParser::nextStep(Message& msg)
	{
		msg.appendSeparator();
		// pass the message
	}
}