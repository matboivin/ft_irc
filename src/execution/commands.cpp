/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 17:56:38 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/03 18:29:41 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>
#include "Message.hpp"
#include "client.hpp"
#include "commands.hpp"

namespace ft_irc
{
	// PASS: set a connection password
	void	exec_pass_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_needmoreparams(msg);
		else if (msg.getSender().isRegistered())
			err_alreadyregistered(msg);
		else
			msg.getSender().setPassword(msg.getParam(0));
	}

	// NICK helper: Special characters listed in the RFC grammar
	static bool	is_special(char c)
	{
		return (
			(c == '[') || (c == ']') || (c == '\\') || (c == '`')
			|| (c == '_') || (c == '^') || (c == '{') || (c == '}') || (c == '|')
			);
	}

	// NICK helper: Check whether a nickname format is valid
	// A nickname is composed of 1 to 9 characters which can be digits, letters or special characters.
	// It musn't start by a digit
	static bool	nick_is_valid(const std::string& newnick)
	{
		if (newnick.size() < 10)
		{
			std::string::const_iterator	it = newnick.begin();

			if (isalpha(*it) || is_special(*it))
			{
				++it;
				while (it != newnick.end())
				{
					if (!isalnum(*it) && !is_special(*it) && (*it != '-'))
						return (false);
					++it;
				}
				return (true);
			}
		}
		return (false);
	}

	// NICK: Change a user nickname
	void	exec_nick_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_nonicknamegiven(msg);
		else
		{
			if (!nick_is_valid(msg.getParam(0)))
				err_erroneusnickname(msg);
			// else if nickname already exists
			//    err_nicknameinuse(msg);
			else
			{
				msg.getSender().setNick(msg.getParam(0));
				std::cout << "new nick is: " << msg.getSender().getNick() << '\n'; // debug
			}
		}
	}

	// Init the map containing the commands
	void	init_commands_map(cmds_map& m)
	{
		m["PASS"] = &exec_pass_cmd;
		m["NICK"] = &exec_nick_cmd;
	}

	// call command function
	void	exec_cmd(const cmds_map& m, Message& msg)
	{
		cmds_map::const_iterator	it = m.find(msg.getCommand());

		if (it != m.end())
			(*it->second)(msg);
	}
} // namespace ft_irc