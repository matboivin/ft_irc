/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboivin <mboivin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 17:56:38 by mboivin           #+#    #+#             */
/*   Updated: 2021/10/03 19:27:55 by mboivin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>
#include "Message.hpp"
#include "client.hpp"
#include "numeric_replies.hpp"
#include "commands.hpp"

namespace ft_irc
{
	// PASS <password>
	// set a connection password
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

	// NICK <nickname>
	// Change a user nickname
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

	// QUIT [<message>]
	// A client session is terminated with a quit message
	void	exec_quit_cmd(Message& msg)
	{
		if (!msg.getParams().empty())
		{
			// msg.setRecipient(users in channel);
			msg.setResponse(
				build_prefix( build_full_client_id( msg.getSender() ) )
				+ " NOTICE " + "chan tmp" // TODO: not sure about notice and need target
				+ msg.getSender().getNick() + " has quit IRC (" + msg.getParam(0) + ")"
				);
			msg.appendSeparator();
		}

		// TODO: The server acknowledges this by sending an ERROR message to the client
	}

	// PRIVMSG <msgtarget> :<message>
	// Send messages to a user or a channel
	void	exec_privmsg_cmd(Message& msg)
	{
		if (msg.getParams().empty())
			err_norecipient(msg);
		else if (msg.getParams().size() < 2)
			err_notexttosend(msg);

		// sorry for this pseudo code ugliness

		// else if (msg.getParam(0) doesn't exist)
		//	err_nosuchnick(msg, msg.getParam(0));
		// else if (!msg.getSender().isOnChan(msg.getParam(0)))
		// 	err_cannotsendtochan(msg);
		// else
		// {
		// 	if (msg.getParam(0)[0] != '#')
		// 		msg.setRecipient(msg.getParam(0));
		// 	else
		// 		add everyone from channel
		// 	if (channel doesnt exist)
		// 		create channel;
		// 	msg.setResponse(build_prefix( build_full_client_id( msg.getSender() ) ) + " PRIVMSG ");
		// 	for (std::size_t i = 0; i < msg.getParams().size(); i++)
		// 		msg.getResponse().append(msg.getParams()[i]);
		// 	msg.appendSeparator();
		// }
	}

	// Init the map containing the commands
	void	init_commands_map(cmds_map& m)
	{
		m["PASS"] = &exec_pass_cmd;
		m["NICK"] = &exec_nick_cmd;
		m["QUIT"] = &exec_quit_cmd;
		m["PRIVMSG"] = &exec_privmsg_cmd;
	}

	// call command function
	void	exec_cmd(const cmds_map& m, Message& msg)
	{
		cmds_map::const_iterator	it = m.find(msg.getCommand());

		if (it != m.end())
			(*it->second)(msg);
	}
} // namespace ft_irc