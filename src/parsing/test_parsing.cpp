/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 12:25:08 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/04 13:52:12 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <iostream>
#include <string>
#include "Parser.hpp"
#include "Message.hpp"

// Test parsing use cases

static int	cmp_msg(const ft_irc::Message& msg, const ft_irc::Message& expected)
{
	int	ret = 1;

	if (msg.getCommand() != expected.getCommand())
	{
		std::cout << "   Message command:  " << msg.getCommand()
				  << "\n-> Expected command: " << expected.getCommand() << std::endl;
		ret = 0;
	}
	if (msg.getResponse() != expected.getResponse())
	{
		std::cout << "   Message response:  " << msg.getResponse()
				  << "-> Expected response: " << expected.getResponse() << std::endl;
		ret = 0;
	}
	if (msg.getParams() != expected.getParams())
	{
		std::cout << "   Message params:  ";
		for (std::size_t i = 0; i < msg.getParams().size(); i++)
			std::cout << "'" <<  msg.getParam(i) << "' ";

		std::cout << "\n-> Expected params: ";
		for (std::size_t i = 0; i < expected.getParams().size(); i++)
			std::cout << "'" <<  expected.getParam(i) << "' ";
		std::cout << std::endl;
		ret = 0;
	}
	return (ret);
}

int	test_parsing()
{
	ft_irc::Parser		parser;
	ft_irc::IRCClient	dummy_client;
	ft_irc::Message		msg(dummy_client);
	ft_irc::Message		expected(dummy_client);

	dummy_client.setNick("nick");
	dummy_client.setUsername("username");

	{
		msg = parser.parseMessage("foo", dummy_client);
		expected.setCommand("foo");
		expected.setResponse(":irc.42.fr 421 foo :Unknown command\r\n");
		assert(cmp_msg(msg, expected));
	}

	{
		msg = parser.parseMessage("QUIT", dummy_client);
		expected.setCommand("QUIT");
		assert(cmp_msg(msg, expected));
	}

	{
		msg = parser.parseMessage("NICK foo", dummy_client);
		expected.setCommand("NICK");
		expected.setParam("foo");
		assert(cmp_msg(msg, expected));
	}

	return (0);
}
