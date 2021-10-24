/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 12:25:08 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/19 17:25:39 by mboivin          ###   ########.fr       */
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
		for (std::vector<std::string>::const_iterator it = msg.getParams().begin();
			 it != msg.getParams().end();
			 ++it)
		{
			std::cout << "'" << *it << "' ";
		}

		std::cout << "\n-> Expected params: ";
		for (std::vector<std::string>::const_iterator it = expected.getParams().begin();
			 it != expected.getParams().end();
			 ++it)
		{
			std::cout << "'" << *it << "' ";
		}
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

		parser.parseMessage(msg, "123");
		assert(cmp_msg(msg, expected));
	}

	// Unknown command
	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "foo");
		expected.setCommand("foo");
		expected.setResponse(":irc.42.fr 421 foo :Unknown command\r\n");
		assert(cmp_msg(msg, expected));
	}

	// Uppercase command if valid
	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "PArt");
		expected.setCommand("PART");
		expected.setResponse(":nick!username@0.0.0.0 PART\r\n");
		assert(cmp_msg(msg, expected));
	}

	// Valid command
	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "QUIT");
		expected.setCommand("QUIT");
		expected.setResponse(":nick!username@0.0.0.0 QUIT\r\n");
		assert(cmp_msg(msg, expected));
	}

	// Simple parameter checking
	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "JOIN  ");
		expected.setCommand("JOIN");
		expected.setResponse(":nick!username@0.0.0.0 JOIN\r\n");
		assert(cmp_msg(msg, expected));

		parser.parseMessage(msg, "JOIN #general");
		expected.setParam("#general");
		expected.setResponse(":nick!username@0.0.0.0 JOIN #general\r\n");
		assert(cmp_msg(msg, expected));
	}

	// Trailing parameter can contain spaces and colons
	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "QUIT :Gone to have lunch at: MyFavPlace");
		expected.setCommand("QUIT");
		expected.setParam(":Gone to have lunch at: MyFavPlace");
		expected.setResponse(":nick!username@0.0.0.0 QUIT :Gone to have lunch at: MyFavPlace\r\n");
		assert(cmp_msg(msg, expected));
	}

	// Trailing parameter must start with a colon
	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "QUIT :Gone to have lunch");
		expected.setCommand("QUIT");
		expected.setParam(":Gone to have lunch");
		expected.setResponse(":nick!username@0.0.0.0 QUIT :Gone to have lunch\r\n");
		assert(cmp_msg(msg, expected));
	}

	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "TOPIC #test :another topic");
		expected.setCommand("TOPIC");
		expected.setParam("#test");
		expected.setParam(":another topic");
		assert(cmp_msg(msg, expected));
	}

	// Many middle parameters
	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "KICK #Finnish John :Speaking English");
		expected.setCommand("KICK");
		expected.setParam("#Finnish");
		expected.setParam("John");
		expected.setParam(":Speaking English");
		expected.setResponse(":nick!username@0.0.0.0 KICK #Finnish John :Speaking English\r\n");
		assert(cmp_msg(msg, expected));
	}

	// List of middle parameters
	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "JOIN #foo,#bar");
		expected.setCommand("JOIN");
		expected.setParam("#foo");
		expected.setParam("#bar");
		expected.setResponse(":nick!username@0.0.0.0 JOIN #foo,#bar\r\n");
		assert(cmp_msg(msg, expected));
	}

	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "JOIN #foo, #bar"); // need checking with irrsi
		expected.setCommand("JOIN");
		expected.setParam("#foo");
		expected.setParam("#bar");
		expected.setResponse(":nick!username@0.0.0.0 JOIN #foo, #bar\r\n");
		assert(cmp_msg(msg, expected));
	}

	{
		ft_irc::Message	expected(dummy_client);

		parser.parseMessage(msg, "QUIT :#foo,#bar");
		expected.setCommand("QUIT");
		expected.setParam(":#foo,#bar");
		expected.setResponse(":nick!username@0.0.0.0 QUIT :#foo,#bar\r\n");
		assert(cmp_msg(msg, expected));
	}

	return (0);
}
