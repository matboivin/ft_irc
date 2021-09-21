/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:28:44 by mboivin           #+#    #+#             */
/*   Updated: 2021/09/21 15:31:20 by mboivin          ###   ########.fr       */
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
	IRCParser::IRCParser()
	{
	}

	// copy constructor
	IRCParser::IRCParser(const IRCParser& other)
	{
		static_cast<void>(other); // tmp
	}

	// assignment operator
	IRCParser&	IRCParser::operator=(const IRCParser& other)
	{
		static_cast<void>(other); // tmp

		return (*this);
	}

	// destructor
	IRCParser::~IRCParser()
	{
	}

	// main parsing function
	// not sure about how to pass the client arg
	void	IRCParser::parse(const std::string& packet, IRCClient& sender)
	{
		Message	msg;

		static_cast<void>(packet);
		msg.setPrefix(sender.getNick() + "!" + "tmp" + "@" + sender.getIpAddressStr()); // tmp
		std::cout << msg.getPrefix() << std::endl; // tmp
	}

	// dummy tmp function that would pass msg to appropriate worker
	void	IRCParser::nextStep(Message& msg)
	{
		msg.appendSeparator();
		// pass the message
	}
}