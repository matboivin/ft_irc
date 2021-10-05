/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 12:25:08 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/05 14:52:05 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <iostream>
#include <string>
#include "Client.hpp"
#include "Message.hpp"
#include "Parser.hpp"

// Compare Messages and display only values that differe

static int	cmp_msg(const ft_irc::Message& msg, const ft_irc::Message& expected)
{
	int	ret = 1;

	if (msg.getCommand() != expected.getCommand())
	{
		std::cout << "   Message command:  " << msg.getCommand()
				  << "\n-> Expected command: " << expected.getCommand() << std::endl;
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
	if (msg.getResponse() != expected.getResponse())
	{
		std::cout << "   Message response:  " << msg.getResponse()
				  << "-> Expected response: " << expected.getResponse() << std::endl;
		ret = 0;
	}
	return (ret);
}

// Test parsing use cases

int	test_parsing()
{
	ft_irc::Parser	parser;
	ft_irc::Client	dummy_client;
	ft_irc::Message	msg(dummy_client);

	// Set dummy values to client
	dummy_client.setNick("nick");
	dummy_client.setUsername("username");

	// Wrong command format is silently ignored
	{
		ft_irc::Message	expected(dummy_client);

		msg = parser.parseMessage(dummy_client, "123");
		assert(cmp_msg(msg, expected));
	}

	// Unknown command
	{
		ft_irc::Message	expected(dummy_client);

		msg = parser.parseMessage(dummy_client, "foo");
		expected.setCommand("foo");
		expected.setResponse(":irc.42.fr 421 foo :Unknown command\r\n");
		assert(cmp_msg(msg, expected));
	}

	// Uppercase command if valid
	{
		ft_irc::Message	expected(dummy_client);

		msg = parser.parseMessage(dummy_client, "PArt");
		expected.setCommand("PART");
		assert(cmp_msg(msg, expected));
	}

	// Valid command
	{
		ft_irc::Message	expected(dummy_client);

		msg = parser.parseMessage(dummy_client, "QUIT");
		expected.setCommand("QUIT");
		assert(cmp_msg(msg, expected));
	}

	// Trailing parameter can contain spaces and colons
	{
		ft_irc::Message	expected(dummy_client);

		msg = parser.parseMessage(dummy_client, "PRIVMSG :long_trailing :foo : foo");
		expected.setCommand("PRIVMSG");
		expected.setParam(":long_trailing :foo : foo");
		assert(cmp_msg(msg, expected));
	}

	// Trailing parameter must start with a colon
	{
		ft_irc::Message	expected(dummy_client);

		msg = parser.parseMessage(dummy_client, "PRIVMSG :trailing");
		expected.setCommand("PRIVMSG");
		expected.setParam(":trailing");
		assert(cmp_msg(msg, expected));
	}

	{
		ft_irc::Message	expected(dummy_client);

		msg = parser.parseMessage(dummy_client, "PRIVMSG middle trailing");
		expected.setCommand("PRIVMSG");
		expected.setParam("middle");
		assert(cmp_msg(msg, expected));
	}

	// Parameters checking
	{
		ft_irc::Message	expected(dummy_client);

		msg = parser.parseMessage(dummy_client, "JOIN  ");
		expected.setCommand("JOIN");
		assert(cmp_msg(msg, expected));

		msg = parser.parseMessage(dummy_client, "JOIN #general");
		expected.setParam("#general");
		assert(cmp_msg(msg, expected));
	}

	return (0);
}
